/*
 *  ImageBlobs.cpp
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 s373.net/x. All rights reserved.
 *
 */

#include "ImageBlobs.h"
#include "ofxFlob.h"


unsigned int ImageBlobs::idnumbers = 0;

ImageBlobs::ImageBlobs(){
	clear();	
}

void ImageBlobs::clear(){
	while (theblobs.size()>0) {
		delete theblobs[0];
		theblobs.erase(theblobs.begin());
	}

	while (prevblobs.size()>0) {
		delete prevblobs[0];
		prevblobs.erase(prevblobs.begin());
	}

	while (TBlobs.size()>0) {
		delete TBlobs[0];
		TBlobs.erase(TBlobs.begin());
	}
	while (prevTBlobs.size()>0) {
		delete prevTBlobs[0];
		prevTBlobs.erase(prevTBlobs.begin());
	}
	while (quadbloblist.size()>0) {
		delete quadbloblist[0];
		quadbloblist.erase(quadbloblist.begin());
	}
	while (thecoords.size()>0) {
		delete thecoords[0];
		thecoords.erase(thecoords.begin());
	}
	
					   
					   					   
}

void ImageBlobs::setup(ofxFlob *flob){	
		idnumbers = 0;
		numblobs = prevnumblobs = 0;
		trackednumblobs =  prevtrackednumblobs = 0;
		lifetime = 100;
		ninpix = 100;
		maxpix = 10000;
		imagemap.clear();
		imagemaplit = false;
	
		w=h=0;
		wr=hr = 0;
		wcoordsx= wcoordsy =
		worldw= worldh =
		numpix = 0;
		lp1 = 0.05f;
		lp2 = 1.f - lp1;
		trackingmindist = 500; // ~22pix (squared)
	
		clear();
	
		tflob = flob;
	
		calcdims(tflob->videoresw, tflob->videoresh, tflob->worldwidth,
				 tflob->worldheight);
	
}


void ImageBlobs::calcdims(const int w, const int h, const float ww, const float wh) {
	this->w = w;
	this->h = h;	
	wr = 1.0f / w;
	hr = 1.0f / h;
	numpix = w * h;
	worldw = ww;
	worldh = wh;
	wcoordsx = worldw * wr;
	wcoordsy = worldh * hr;
	w2 = worldw / 2;
	h2 = worldh / 2;
}



void ImageBlobs::calc( ofImage & image) {
	int min0 = 1000000;
	int max0 = -100000;
	
	unsigned int pixelcount = 0;	

	copy_blobs_to_previousblobs();
	
//	int pblobssize = prevblobs.size();
	
	thecoords.clear();
	imagemap.assign( numpix, false );
	
	 unsigned char *pix = image.getPixels();
	
	int imgW = image.getWidth();
	int imgH = image.getHeight();
	
	
	for (int j = 0; j < imgH; j++) {
		int rowLoc = j * imgW;
		for (int i = 0; i < imgW; i++) {			
			if (((pix[rowLoc + i])) > 0) {
				if (i < min0)
					min0 = i;
				else if (i > max0)
					max0 = i;
				if (imagemap[rowLoc + i] == false) {					
					pt2 *p = new pt2();					
					ABlob *b = new ABlob();					
					p->x = i;
					p->y = j;
					thecoords.push_back(p);
					pixelcount = 0;
					b->boxminx = i;
					b->boxmaxx = i;
					b->boxminy = j;
					b->boxmaxy = j;
					
					while (thecoords.size()>0) {						
						pt2 *p2 = new pt2(thecoords[0]->x, thecoords[0]->y);
						delete thecoords[0]; //   
						thecoords.erase(thecoords.begin()); //  

						if ((p2->x >= 0) && (p2->x < imgW)
							&& (p2->y >= 0) && (p2->y < imgH)) {
							
							int bloc = p2->y * imgW + p2->x;
							
							if (imagemap[bloc] == false) {										
								int pixval2 = pix[bloc];
								if (pixval2 > 0) {
									imagemap[bloc] = true;
									pixelcount++;
									
									thecoords.push_back(new pt2(p2->x, p2->y + 1 ));
									thecoords.push_back(new pt2(p2->x, p2->y - 1 ));
									thecoords.push_back(new pt2(p2->x + 1, p2->y ));
									thecoords.push_back(new pt2(p2->x - 1, p2->y ));

									if (p2->x < b->boxminx)
										b->boxminx = p2->x;
									if (p2->x > b->boxmaxx)
										b->boxmaxx = p2->x;
									if (p2->y < b->boxminy)
										b->boxminy = p2->y;
									if (p2->y > b->boxmaxy)
										b->boxmaxy = p2->y;
								}
							}
						}
						
						delete p2;
					}

					
					if (pixelcount >= ninpix && pixelcount <= maxpix) {
						b->id = numblobs;
						b->pixelcount = pixelcount;
						b->boxcenterx = (int) ((b->boxminx + b->boxmaxx) * 0.5f) ;
						b->boxcentery = (int) ((b->boxminy + b->boxmaxy) * 0.5f) ;
						b->boxdimx = (b->boxmaxx - b->boxminx) ;
						b->boxdimy = (b->boxmaxy - b->boxminy) ;
						b->bx = b->boxminx * wcoordsx;
						b->by = b->boxminy * wcoordsy;
						b->cx = b->boxcenterx * wcoordsx;
						b->cy = b->boxcentery * wcoordsy;
						b->dimx = b->boxdimx * wcoordsx;
						b->dimy = b->boxdimy * wcoordsy;
						{// glitchy vel calcs
							if(prevnumblobs>numblobs){
								ABlob *c = prevblobs[numblobs];
								b->pboxcenterx = c->boxcenterx;
								b->pboxcentery = c->boxcentery;
								b->ivelx = (b->boxcenterx-b->pboxcenterx) ;
								b->ively = (b->boxcentery-b->pboxcentery) ;
							}
						}

						if (tflob->getAnyFeatureActive()) {
							if (tflob->trackfeatures[0])	b = calc_feature_head(b);
							if (tflob->trackfeatures[1])	b = calc_feature_arms(b);							
							if (tflob->trackfeatures[2])	b = calc_feature_feet(b);
							if (tflob->trackfeatures[3])	b = calc_feature_bottom(b);
						}
						
//						ABlob *blob = new ABlob(b);
						theblobs.push_back(b);//lob);
						numblobs++;
//						delete b;
					} else {
						delete b;
					}
					
				}				
				
			}
		}
	}
}



void ImageBlobs::calcQuad( ofImage & image) {
	int min0 = 1000000;
	int max0 = -100000;
	
	int pixelcount = 0;
	
	copy_blobs_to_previousblobs();
	
	numblobs = 0;
	quadbloblist.clear();
	thecoords.clear();
	imagemap.assign( numpix, false );
	
	unsigned char *pix = image.getPixels();
	
	int imgW = image.getWidth();
	int imgH = image.getHeight();
	
	
	for (int j = 0; j < imgH; j++) {
		int rowLoc = j * imgW;
		for (int i = 0; i < imgW; i++) {			
			if (((pix[rowLoc + i])) > 0) {
				if (i < min0)
					min0 = i;
				else if (i > max0)
					max0 = i;
				if (imagemap[rowLoc + i] == false) {					
					pt2 *p = new pt2();					
					quadBlob *b = new quadBlob();					
					p->x = i;
					p->y = j;
					thecoords.push_back(p);
					pixelcount = 0;
					b->boxminx = i;
					b->boxmaxx = i;
					b->boxminy = j;
					b->boxmaxy = j;
					
					while (thecoords.size()>0) {						
						pt2 *p2 = new pt2(thecoords[0]->x, thecoords[0]->y);
						delete thecoords[0]; //
						thecoords.erase(thecoords.begin()); //
						
						if ((p2->x >= 0) && (p2->x < imgW)
							&& (p2->y >= 0) && (p2->y < imgH)) {
							
							int bloc = p2->y * imgW + p2->x;
							
							if (imagemap[bloc] == false) {										
								int pixval2 = pix[bloc];
								if (pixval2 > 0) {
									imagemap[bloc] = true;
									pixelcount++;
									
									thecoords.push_back(new pt2(p2->x, p2->y + 1 ));
									thecoords.push_back(new pt2(p2->x, p2->y - 1 ));
									thecoords.push_back(new pt2(p2->x + 1, p2->y ));
									thecoords.push_back(new pt2(p2->x - 1, p2->y ));
									
									if (p2->x < b->boxminx)
										b->boxminx = p2->x;
									if (p2->x > b->boxmaxx)
										b->boxmaxx = p2->x;
									if (p2->y < b->boxminy)
										b->boxminy = p2->y;
									if (p2->y > b->boxmaxy)
										b->boxmaxy = p2->y;
								}
							}
						}
						
						delete p2;
					}
					
					
					if (pixelcount >= ninpix && pixelcount <= maxpix) {
						b->id = numblobs;
						b->pixelcount = pixelcount;
						b->boxcenterx = (int) ((b->boxminx + b->boxmaxx) * 0.5) ;
						b->boxcentery = (int) ((b->boxminy + b->boxmaxy) * 0.5) ;
//						b->boxdimx = (b->boxmaxx - b->boxminx) ;
//						b->boxdimy = (b->boxmaxy - b->boxminy) ;
//						b->bx = b->boxminx * wcoordsx;
//						b->by = b->boxminy * wcoordsy;
						b->cx = b->boxcenterx * wcoordsx;
						b->cy = b->boxcentery * wcoordsy;
//						b->dimx = b->boxdimx * wcoordsx;
//						b->dimy = b->boxdimy * wcoordsy;
						
						b = calc_quad(b);
						
						quadbloblist.push_back(b);
						numblobs++;
					} else {
						delete b;
					}
					
				}				
				
			}
		}
	}
}




void ImageBlobs::copy_blobs_to_previousblobs(){
	
	prevnumblobs = numblobs;
	numblobs = 0; // reset count per frame at begin
	
	if(prevblobs.size()>0){	
		while(prevblobs.size()>0){
			delete prevblobs[0];
			prevblobs.erase( prevblobs.begin() );
		}

		prevblobs.clear();
	}
	
	for (int i = 0; i < theblobs.size(); i++) {
		prevblobs.push_back(theblobs[i]);
	}
	
	
	while(theblobs.size()>0){
//		delete theblobs[0]; //copied, dont delete the ptr, deleted as previousblobs
		theblobs.erase( theblobs.begin() );// erase the reference though
	}
	theblobs.clear();
	
	
}







// /// tracking code


vector<TBlob*>* ImageBlobs:: calcsimple() {
	
	//TBlobs.clear();// = new ArrayList<TBlob>();
	while(TBlobs.size()>0){
		delete TBlobs[0];
		TBlobs.erase(TBlobs.begin());
	}
	
	
	TBlob *b1, *b2;
	ABlob *ab;
	
	for (int i = 0; i < theblobs.size(); i++) {
		ab = theblobs.at(i);
		b1 = new TBlob(ab);
		b2 = (i >= prevblobs.size()) ? NULL : new TBlob(prevblobs.at(i));
		if (b2 != NULL) {
			
			b1->id = b2->id; // b2maintains id!
			b1->presencetime = b2->presencetime + 1;
			b1->prevelx = b2->velx;
			b1->prevely = b2->vely;
			b1->pboxcenterx = b2->boxcenterx;
			b1->pboxcentery = b2->boxcentery;
			
			b1->armleftx = ab->armleftx;
			b1->armlefty = ab->armlefty;
			b1->armrightx = ab->armrightx;
			b1->armrighty = ab->armrighty;
			b1->headx = ab->headx;
			b1->heady = ab->heady;
			b1->bottomx = ab->bottomx;
			b1->bottomy = ab->bottomy;
			b1->footleftx = ab->footleftx;
			b1->footlefty = ab->footlefty;
			b1->footrightx = ab->footrightx;
			b1->footrighty = ab->footrighty;
			
		} else {
			b1->id = idnumbers++;
			b1->pboxcenterx = ab->boxcenterx;
			b1->pboxcentery = ab->boxcentery;
			b1->prevelx = 0.f;
			b1->prevely = 0.f;
			
			b1->armleftx = ab->armleftx;
			b1->armlefty = ab->armlefty;
			b1->armrightx = ab->armrightx;
			b1->armrighty = ab->armrighty;
			b1->headx = ab->headx;
			b1->heady = ab->heady;
			b1->bottomx = ab->bottomx;
			b1->bottomy = ab->bottomy;
			b1->footleftx = ab->footleftx;
			b1->footlefty = ab->footlefty;
			b1->footrightx = ab->footrightx;
			b1->footrighty = ab->footrighty;
			
		}
		
		b1->cx = ab->cx;// already *worldcoords
		b1->cy = ab->cy;
		b1->boxcenterx = ab->boxcenterx;
		b1->boxcentery = ab->boxcentery;
		
		b1->velx = lp2 * b1->prevelx + lp1 * (b1->cx - b1->pcx);
		b1->vely = lp2 * b1->prevely + lp1 * (b1->cy - b1->pcy);		
		
		
		// // old smooth
		// b1.velx = lp2 * b1.velx + lp1 * (b1.boxcenterx - b1.pboxcenterx)
		// * wr;// vx;//b.cx - b.pcx;
		// b1.vely = lp2 * b1.vely + lp1 * (b1.boxcentery - b1.pboxcentery)
		// * hr;// vy;//b.cy - b.pcy;

		b1->boxminx = ab->boxminx;
		b1->boxmaxx = ab->boxmaxx;
		b1->boxminy = ab->boxminy;
		b1->boxmaxy = ab->boxmaxy;
		b1->boxdimx = ab->boxdimx;
		b1->boxdimy = ab->boxdimy;
		
		b1->dimx = ab->dimx;
		b1->dimy = ab->dimy;
		b1->rad = (ab->boxdimx < ab->boxdimy) ? ab->boxdimx / 2.f : ab->boxdimy / 2.f;
		b1->rad2 = b1->rad * b1->rad;
		
		TBlobs.push_back(b1);
		
	}
	
	if (TBlobs.size() < 1 && idnumbers != 0) { // reset id count
		idnumbers = 0;
	}
	
	
	return &TBlobs;
	
}

/**
 * public ArrayList<TBlob> tracksimpleAL()<br>
 * <br>
 * tracksimpleAL() is a simpler tracking mechanism,<br>
 * a bit faster than track, but doesn't maintain everything<br>
 * 
 * @Param void
 * @return ArrayList<TBlob>
 * 
 */

// /// simple tracking code in flob
vector<TBlob*>* ImageBlobs:: tracksimple() {
	// tbsimplelist = new ArrayList<TBlob>();
	//prevTBlobs = new ArrayList<TBlob>();
	
	while(prevTBlobs.size()>0){
		delete prevTBlobs[0];
		prevTBlobs.erase(prevTBlobs.begin());
	}
	
		
	for (int i = 0; i < TBlobs.size(); i++) {
		prevTBlobs.push_back(TBlobs.at(i));
	}
	
	TBlobs.clear();
	
	TBlob *b1, *b2;
	ABlob *ab;
	
	for (int i = 0; i < theblobs.size(); i++) {
		ab = theblobs.at(i);
		b1 = new TBlob(ab);
		b2 = (i >= prevnumblobs) ? NULL : new TBlob(prevTBlobs.at(i));
		if (b2 != NULL) {
			b1->id = b2->id;
			b1->prevelx = b2->velx;
			b1->prevely = b2->vely;
			b1->pcx = b2->cx;
			b1->pcy = b2->cy;
		} else {
			b1->id = idnumbers++;
			b1->pcx = ab->cx;
			b1->pcy = ab->cy;
			b1->prevelx = 0.f;
			b1->prevely = 0.f;
		}
		b1->cx = ab->cx;
		b1->cy = ab->cy;

		b1->velx = lp2 * b1->prevelx + lp1 * (b1->cx - b1->pcx);
		b1->vely = lp2 * b1->prevely + lp1 * (b1->cy - b1->pcy);		
		
//		b1->velx += lp2 * b1->velx + lp1 * (b1->boxcenterx - b1->pboxcenterx) * wr;	
//		b1->vely += lp2 * b1->vely + lp1 * (b1->boxcentery - b1->pboxcentery) * hr;	

		
		b1->boxminx = ab->boxminx;
		b1->boxmaxx = ab->boxmaxx;
		b1->boxminy = ab->boxminy;
		b1->boxmaxy = ab->boxmaxy;
		b1->boxdimx = ab->boxdimx;
		b1->boxdimy = ab->boxdimy;
		
		b1->dimx = ab->dimx;
		b1->dimy = ab->dimy;
		b1->rad = (ab->boxdimx < ab->boxdimy) ? ab->boxdimx / 2.f
		: ab->boxdimy / 2.f;
		b1->rad2 = b1->rad * b1->rad;
		
		// cp feats
		b1->armleftx = ab->armleftx;
		b1->armlefty = ab->armlefty;
		b1->armrightx = ab->armrightx;
		b1->armrighty = ab->armrighty;
		b1->headx = ab->headx;
		b1->heady = ab->heady;
		b1->bottomx = ab->bottomx;
		b1->bottomy = ab->bottomy;
		b1->footleftx = ab->footleftx;
		b1->footlefty = ab->footlefty;
		b1->footrightx = ab->footrightx;
		b1->footrighty = ab->footrighty;
		
		TBlobs.push_back(b1);
		
	}
	
	
	if (TBlobs.size() < 1 && idnumbers != 0) { // reset id count
		idnumbers = 0;
	}
	
	
	
	return &TBlobs;
	
}





void ImageBlobs:: dotracking() { // memory management is buggy here, tracksimple is good
	// / copy current tracked blob to prev tracked blob and increment life
	
	prevtrackednumblobs = trackednumblobs;
	trackednumblobs = 0;
	prevTBlobs.clear();// = new ArrayList<TBlob>();
	
	
	
	for (int i = 0; i < TBlobs.size(); i++) {
		TBlob * tb = TBlobs.at(i);
		// tb.presencetime++;
		prevTBlobs.push_back(tb);
	}
	
	// new arraylist of TBlobs
	TBlobs.clear();// = new ArrayList<TBlob>();
	
	// always init tracking, unlink all blobs
	for (int i = 0; i < prevTBlobs.size(); i++) {
		prevTBlobs.at(i)->linked = false;
	}
	
	if (theblobs.size() > 0) {
		compareblobsprevblobs();
	}
	// always
	doremoveprevblobs();
	
	if(tflob->TBlobDoSorting){
		sortTBlobs();
	}
	
	if (TBlobs.size() < 1 && idnumbers != 0) { // reset id count
		idnumbers = 0;
	}
}

void ImageBlobs:: sortTBlobs() {
	
	vector<TBlob*> temp ;
	
	if (TBlobs.size() > 0) {		
		for (int i = TBlobs.size() - 1; i >= 0; i--) {
			int minid2 = (int) 2e16;
			int who = -1;
			for (int j = 0; j < TBlobs.size(); j++) {
				TBlob *tb = (TBlobs.at(i));
				if (tb->id < minid2) {
					minid2 = tb->id;
					who = j;
				}
			}
			//
			if (who > -1) {
				temp.push_back(TBlobs.at(who));// minid2));
				TBlobs.erase( TBlobs.begin() + i);
			}
		}
		
		for (int i = 0; i < temp.size(); i++) {
			
			TBlobs.push_back(temp.at(i));
//			temp.erase(temp.begin());
//			i--;
			// can leave this lingering
		}
		
	}
	
}

bool ImageBlobs:: matchblobprevTBlobs(ABlob * ab) {
	
	bool matched = false;
	float mintrackeddist = 1000000;
	int who = -1;
	float mindist = trackingmindist;// 1000;///2500;//1000; //
	// float vx0=0f;
	// float vy=0f;
	
	for (int i = (prevTBlobs.size() - 1); i >= 0; i--) {
		
		TBlob * prev = ( prevTBlobs.at(i));
		if (prev->linked)
			continue;
		
		float dx = ab->cx - prev->cx;
		float dy = ab->cy - prev->cy;
		float d2 = dx * dx + dy * dy;// ABS(dx)+ABS(dy);//dx * dx + dy * dy;
		if (d2 < mindist && d2 < mintrackeddist) {
			mintrackeddist = d2;
			who = i;
			matched = true;
		}
	}
	
	if (matched) {
		// System.out.print("matched blob "+who+ "\n");
//		cout <<"matched blob "<<who<< "\n";
		TBlob * b = ( prevTBlobs.at(who));//remove(who);
		//delete reference, not object
		prevTBlobs.erase(prevTBlobs.begin()+who);
		
		b->linked = true;
		b->newblob = false;
		b->presencetime++;
		b->prevelx = b->velx;
		b->prevely = b->vely;
		b->pcx = b->cx;
		b->pcy = b->cy;
		b->cx = ab->cx;
		b->cy = ab->cy;
		b->velx = lp2 * b->prevelx + lp1 * (b->cx - b->pcx);
		b->vely = lp2 * b->prevely + lp1 * (b->cy - b->pcy);
		
		// box
		b->boxminx = ab->boxminx;
		b->boxmaxx = ab->boxmaxx;
		b->boxminy = ab->boxminy;
		b->boxmaxy = ab->boxmaxy;
		b->boxdimx = ab->boxdimx;
		b->boxdimy = ab->boxdimy;
		b->dimx = ab->dimx;
		b->dimy = ab->dimy;
		
		b->rad = (ab->boxdimx < ab->boxdimy) ? ab->boxdimx / 2.f
		: ab->boxdimy / 2.f;
		b->rad2 = b->rad * b->rad;
		
		// cp feats
		b->armleftx = ab->armleftx;
		b->armlefty = ab->armlefty;
		b->armrightx = ab->armrightx;
		b->armrighty = ab->armrighty;
		b->headx = ab->headx;
		b->heady = ab->heady;
		b->bottomx = ab->bottomx;
		b->bottomy = ab->bottomy;
		b->footleftx = ab->footleftx;
		b->footlefty = ab->footlefty;
		b->footrightx = ab->footrightx;
		b->footrighty = ab->footrighty;
		
		TBlobs.push_back(b);
		trackednumblobs++;
		
	}
	
	return matched;
}

void ImageBlobs::compareblobsprevblobs() {
	
	for (int i = 0; i < theblobs.size(); i++) {
		
		ABlob * ab = (theblobs.at(i));
		bool matched = matchblobprevTBlobs(ab);
		
		if (!matched){
			idnumbers++;
			TBlob * tb = new TBlob(ab);
			TBlobs.push_back(tb);
			trackednumblobs++;
		}
	}
	
}

void ImageBlobs::doremoveprevblobs() {
	
	for (int i = prevTBlobs.size() - 1; i >= 0; i--) {
		
		TBlob & tb = *(prevTBlobs.at(i));
//		
//		if (tb.linked)
//			cout <<"flob: a linked blob in doremove error." << i <<" \n";
//		else {
			// check life
			if (tb.lifetime-- < 0){
				delete prevTBlobs[i];
				prevTBlobs.erase( prevTBlobs.begin()+i);
			}else {
				// inactive TBlob
				TBlob * b = prevTBlobs.at(i);
				// dont delete here
				prevTBlobs.erase( prevTBlobs.begin() + i);
				b->velx = 0.f;
				b->vely = 0.f;
				b->presencetime++;
				TBlobs.push_back(b);
				trackednumblobs++;
				
			}
		}
//	}
	
}


bool ImageBlobs:: isCollide(int x, int y) {
	
	// receives a pair, tests inside any box, if inside boxes tests inside
	// imagemap
	
	if (x >= 0 && x < w && y >= 0 && y < h) {
		for (int i = 0; i < theblobs.size(); i++) {
			ABlob & b = *(theblobs.at(i));
			if (x > b.boxminx && x < b.boxmaxx && y > b.boxminy
				&& y < b.boxmaxy) {
				// inside a box; if is true, return, else keep searching
				// blobs
				if (imagemap[y * w + x])
					return true;
			}
		}
	}
	
	return false;
	
}

vector<float> ImageBlobs:: postcollideTBlobs(float x, float y, float rad) {
	
	vector<float> dcol;// = { 0f, -1f, -1f, 0f, 0f }; // default return
	dcol.push_back(0.f);
	dcol.push_back(-1.f);
	dcol.push_back(-1.f);
	dcol.push_back(0.f);
	dcol.push_back(0.f);
	// x,y,rad are normed to scene size
	x *= w;
	y *= h;
	rad *= w;
	// receives a pair, tests inside any box, if inside boxes tests inside
	// imagemap
	
	if (x >= 0.f && x < w - 1.f && y >= 0.f && y < h - 1.f) {
		
		for (int i = 0; i < TBlobs.size(); i++) {
			// ABlob b = (ABlob) theblobs.get(i);
			TBlob & b = *(TBlobs.at(i));
			
			// 0. close point on blob
			float closex = (x < b.boxminx) ? b.boxminx
			: ((x > b.boxmaxx) ? b.boxmaxx : x);
			float closey = (y < b.boxminy) ? b.boxminy
			: ((y > b.boxmaxy) ? b.boxmaxy : y);
			
			// 1. dist blob close
			float dx0 = closex - x;
			float dy0 = closey - y;
			float d0 = dx0 * dx0 + dy0 * dy0;
			float minsdist = rad * rad + b.rad2;
			
			if (d0 < minsdist && imagemap[((int) y * w + (int) x)]) {
				// compute normalized vector from close to center
				float nvx = b.boxcenterx - closex;
				float nvy = b.boxcentery - closey;
				float d1 = ABS(nvx) + ABS(nvy);// (float)Math.sqrt(nvx*nvx+nvy*nvy);
				float nvl = d1 > 0.f ? 1.0f / d1 : 1.f;
				nvx *= nvl;
				nvy *= nvl;
				// moving the circle along this normal by a distance equal
				// to the circle radius
				// minus the distance from the closest point to the circle
				// center
				float move = rad - d1 + 0.0001f;
				
				nvx *= move;
				nvy *= move;
				
				dcol[0] = 1.f;
				dcol[1] = nvx * wr;
				dcol[2] = nvy * hr;
				dcol[3] = b.velx * wr;
				dcol[4] = b.vely * hr;
				return dcol;
				
			}
			
		}
	}
	
	return dcol;
	
}






vector<float> ImageBlobs:: postcollideblobs(float x, float y, float rad) {
	
	vector<float> dcol;// = { 0f, -1f, -1f, 0f, 0f }; // default return
	dcol.push_back(0.f);
	dcol.push_back(-1.f);
	dcol.push_back(-1.f);
	dcol.push_back(0.f);
	dcol.push_back(0.f);
	
	// x,y,rad are normed to scene size
	x *= w;
	y *= h;
	rad *= w;
	// receives a pair, tests inside any box, if inside boxes tests inside
	// imagemap
	
	if (x >= 0.f && x < w && y >= 0.f && y < h) {
		
		for (int i = 0; i < TBlobs.size(); i++) {
			// ABlob b = (ABlob) theblobs.get(i);
			TBlob & b = *(TBlobs[i]);
			
			// 0. close point on blob
			float closex = (x < b.boxminx) ? b.boxminx
			: ((x > b.boxmaxx) ? b.boxmaxx : x);
			float closey = (y < b.boxminy) ? b.boxminy
			: ((y > b.boxmaxy) ? b.boxmaxy : y);
			
			// 1. dist blob close
			float dx0 = closex - x;
			float dy0 = closey - y;
			float d0 = dx0 * dx0 + dy0 * dy0;
			float minsdist = rad * rad + b.rad2;
			if (d0 < minsdist && imagemap[(int) (y * w + x)]) {
				// compute normalized vector from close to center
				float nvx = b.boxcenterx - closex;
				float nvy = b.boxcentery - closey;
				float d1 = ABS(nvx) + ABS(nvy);// (float)Math.sqrt(nvx*nvx+nvy*nvy);
				float nvl = d1 > 0.f ? 1.0f / d1 : 1.f;
				nvx *= nvl;
				nvy *= nvl;
				// moving the circle along this normal by a distance equal
				// to the circle radius minus the distance from the closest 
				// point to the circle center
				float move = rad - d1 + 0.0001f;
				
				nvx *= move;
				nvy *= move;
				
				dcol[0] = 1.f;
				dcol[1] = nvx * wr;
				dcol[2] = nvy * hr;
				dcol[3] = b.velx * wr;
				dcol[4] = b.vely * hr;
				// break;
				return dcol;
				
			}
		}
	}
	
	return dcol;
}




/**
 * ABlob calc_feature_arms(ABlob b)<br>
 * <br>
 * calculates where the left and right arm are in a blob and store the
 * values in the blob to be accessed after tracking
 * 
 * @Param ABlob b
 * @return ABlob b
 * 
 */


ABlob *	  ImageBlobs::calc_feature_arms(ABlob * b){
	//
	int bx = b->boxminx;
	int by = b->boxminy;
	int ex = b->boxmaxx;
	int ey = b->boxmaxy;
	// int ey = b->boxcentery;
	
	int cx = b->boxcenterx;
	
	int i = 0, j = 0;
	
	bool found = false;
	// armleft
	i = bx;
	for (j = by; j < ey; j++) {
		if (testimagemap(i, j)) {
			b->armleftx = i * wcoordsx;
			b->armlefty = j * wcoordsy;
			found = true;
			break;
		}
	}
	if (!found) {
		j = by;
		for (i = bx; i < cx; i++) {
			if (testimagemap(i, j)) {
				b->armleftx = i * wcoordsx;
				b->armlefty = j * wcoordsy;
				found = true;
				break;
			}
		}
		if (!found) {
			b->armleftx = b->boxcenterx * wcoordsx;
			b->armlefty = b->boxcentery * wcoordsy;
		}
	}
	
	found = false;
	// armright
	i = ex;
	for (j = by; j < ey; j++) {
		if (testimagemap(i, j)) {
			b->armrightx = i * wcoordsx;
			b->armrighty = j * wcoordsy;
			found = true;
			break;
		}
	}
	// armright try upper quad
	if (!found) {
		j = by;
		for (i = ex - 1; i > cx; i++) {
			if (testimagemap(i, j)) {
				b->armrightx = i * wcoordsx;
				b->armrighty = j * wcoordsy;
				found = true;
				break;
			}
		}
		if (!found) {
			b->armrightx = b->boxcenterx * wcoordsx;
			b->armrighty = b->boxcentery * wcoordsy;
		}
	}
	return b;
	
}



/**
 * ABlob calc_feature_head(ABlob b)<br>
 * <br>
 * calculates where the top center point is in a blob-><br>
 * 
 * @Param ABlob b
 * @return ABlob b
 * 
 */


ABlob *	  ImageBlobs::calc_feature_head(ABlob * b){
	
	// int bx = b->boxminx;
	int by = b->boxminy;
	int ex = b->boxmaxx;
	// int ey = b->boxmaxy;
	int cx = b->boxcenterx;
	int i = 0, j = 0;
	int k = cx - 1;
	// head
	j = by;
	for (i = cx; i < ex; i++, k--) {
		
		if (testimagemap(i, j)) {
			b->headx = i * wcoordsx;
			b->heady = j * wcoordsy;
			break;
		}
		if (testimagemap(k, j)) {
			b->headx = k * wcoordsx;
			b->heady = j * wcoordsy;
			break;
		}
	}
	
	return b;
}





/**
 * ABlob calc_feature_bottom(ABlob b)<br>
 * <br>
 * calculates where the bottom center point is in a blob-><br>
 * 
 * @Param ABlob b
 * @return ABlob b
 * 
 */



ABlob *	  ImageBlobs::calc_feature_bottom(ABlob * b){
	
	
	int ex = b->boxmaxx;
	int ey = b->boxmaxy;
	
	int cx = b->boxcenterx;// (bx+ex)/2;///b->boxdimx/2 + bx;
	// int cy = b->boxcentery;//(by+ey)/2;//b->boxdimy/2 + by;
	
	// cx = PApplet.constrain(cx ,0,w-1);
	// cy = PApplet.constrain(cy ,0,h-1);
	
	int i = 0, j = 0;
	// int dir=1;
	
	// bottom
	bool found = false;
	j = ey;
	for (i = cx; i < ex; i++) {
		if (testimagemap(i, j)) {
			b->bottomx = i * wcoordsx;
			b->bottomy = j * wcoordsy;
			found = true;
			break;
			
		}
	}
	
	if (!found) {
		for (i = 0; i <= cx; i++) {
			if (testimagemap(i, j)) {
				b->bottomx = i * wcoordsx;
				b->bottomy = j * wcoordsy;
				found = true;
				break;
				
			}
		}
	}
	
	if (!found) {
		b->bottomx = b->boxcenterx * wcoordsx;
		b->bottomy = ey * wcoordsy;
		
	}
	
	return b;
	
	
}






/**
 * ABlob calc_feature_feet(ABlob b)<br>
 * <br>
 * calculates where the left and right bottom points are in a blob-><br>
 * 
 * @Param ABlob b
 * @return ABlob b
 * 
 */

ABlob *	  ImageBlobs::calc_feature_feet(ABlob * b){
	// /passed to 2 feet instead of one bottom
	int bx = CLAMP(b->boxminx, 0, w - 1);
	// int by = PApplet.constrain(b->boxminy,0,h-1);
	int ex = CLAMP(b->boxmaxx, 0, w - 1);
	int ey = CLAMP(b->boxmaxy, 0, h - 1);
	
	int cx = b->boxcenterx;// (bx+ex)/2;///b->boxdimx/2 + bx;
	int cy = b->boxcentery;// (by+ey)/2;//b->boxdimy/2 + by;
	
	cx = CLAMP(cx, 0, w - 1);
	cy = CLAMP(cy, 0, h - 1);
	
	int i = 0, j = 0;
	
	bool found = false; // new
	
	
	// footleft
	j = ey;
	for (i = bx; i < cx; i++) {
		if (testimagemap(i, j)) {
			b->footleftx = i * wcoordsx;
			b->footlefty = j * wcoordsx;
			// System.out.print("found armleft at "+b->armleftx+" "+b->armlefty
			// );
			found = true;
			break;
			
		}
	}
	
	if(!found){
		b->footleftx = b->cx;
		b->footlefty = b->boxmaxy * wcoordsy;//b->cy;
	}
	
	
	found = false;	
	// footright
	j = ey;
	for (i = ex - 1; i > cx; i--) {
		if (testimagemap(i, j)) {
			b->footrightx = i * wcoordsx;
			b->footrighty = j * wcoordsy;
			found = true;
			break;
		}
	}
	
	
	if(!found){
		b->footrightx = b->cx;
		b->footrighty = b->boxmaxy * wcoordsy;//b->cy;
	}
	
	
	return b;
	
}



quadBlob * ImageBlobs::calc_quad(quadBlob * b) {
	int bx = b->boxminx;
	int by = b->boxminy;
	int ex = b->boxmaxx;
	int ey = b->boxmaxy;
	int cx = b->boxcenterx;
	int i = 0, j = 0;
	bool found = false;
	i = bx;
	for (j = by; j < ey; j++) {
		if (testimagemap(i, j)) {
			b->quad[0] = i * wcoordsx;
			b->quad[1] = j * wcoordsy;
			found = true;
			break;
		}
	}
	// armleft try upper quad
	if (!found) {
		j = by;
		for (i = bx; i < cx; i++) {
			if (testimagemap(i, j)) {
				b->quad[0] = i * wcoordsx;
				b->quad[1] = j * wcoordsy;
				found = true;
				break;
			}
		}
		if (!found) {
			b->quad[0] = b->boxcenterx * wcoordsx;
			b->quad[1] = b->boxcentery * wcoordsy;
		}
	}
	
	found = false;
	// armright
	i = ex;
	for (j = by; j < ey; j++) {
		if (testimagemap(i, j)) {
			b->quad[2] = i * wcoordsx;
			b->quad[3] = j * wcoordsy;
			found = true;
			break;
		}
	}
	
	// armright try upper quad
	if (!found) {
		j = by;
		for (i = ex - 1; i > cx; i++) {
			if (testimagemap(i, j)) {
				b->quad[2] = i * wcoordsx;
				b->quad[3] = j * wcoordsy;
				found = true;
				break;
			}
		}
		
		if (!found) {
			b->quad[2] = b->boxcenterx * wcoordsx;
			b->quad[3] = b->boxcentery * wcoordsy;
		}
		
	}
	
	// feet
	
	bx = CLAMP(b->boxminx, 0, w - 1);
	ex = CLAMP(b->boxmaxx, 0, w - 1);
	ey = CLAMP(b->boxmaxy, 0, h - 1);
	
	cx = b->boxcenterx;
	
	cx = CLAMP(cx, 0, w - 1);
	
	// footleft
	j = ey;
	for (i = bx; i < cx; i++) {
		if (testimagemap(i, j)) {
			b->quad[4] = i * wcoordsx;
			b->quad[5] = j * wcoordsx;
			break;
			
		}
	}
	// footright
	j = ey;
	for (i = ex - 1; i > cx; i--) {
		if (testimagemap(i, j)) {
			b->quad[6] = i * wcoordsx;
			b->quad[7] = j * wcoordsy;
			break;
		}
	}
	return b;
}
// eof