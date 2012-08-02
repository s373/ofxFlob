/*
 *  ImageBlobs.cpp
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageBlobs.h"
#include "Flob.h"


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
	lowpass = 0.05f;
	trackingmindist = 500; // ~22pix (squared)
	
	theblobs.clear();
//	prevblobs.clear();
//	trackedblobs.clear();
//	prevtrackedblobs.clear();
//	tbsimplelist.clear();
	thecoords.clear();
	
	tflob = flob;
	
	calcdims(tflob->videoresw, tflob->videoresh, tflob->worldwidth,
			 tflob->worldheight);
	
}


void ImageBlobs::calcdims(int w, int h, int ww, int wh) {
	this->w = w;
	this->h = h;
	// w2 = w / 2;
	// h2 = h / 2;
	
	wr = 1.0f / w;
	hr = 1.0f / h;
	numpix = w * h;
	worldw = ww;
	worldh = wh;
	wcoordsx = worldw * wr;
	wcoordsy = worldh * hr;
	w2 = worldw / 2;
	h2 = worldh / 2;
	
	cout << "wr "<< wr << " " << " wcorrdsx " << wcoordsx << " " << wcoordsy << endl;
}



/// leaking...

void ImageBlobs::calc(ofImage & image) {
	int min0 = 1000000;
	int max0 = -100000;
	
	int pixelcount = 0;

	copy_blobs_to_previousblobs();
	
	
	thecoords.clear();
	// if (w != pimage.width) {
	// calcdims(pimage.width, pimage.height, tflob.worldwidth,
	// tflob.worldheight);
	// }
//	imagemap = new boolean[numpix];
	imagemap.assign( numpix, false );
	
//	pimage.loadPixels();
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

						delete thecoords[0];
						thecoords.erase(thecoords.begin());

						if ((p2->x >= 0) && (p2->x < imgW)
							&& (p2->y >= 0) && (p2->y < imgH)) {
							
							if (imagemap[p2->y * imgW + p2->x] == false) {
								
								int pixval2 = (pix[(int) (p2->y * imgW + p2->x)]);// & 0xFF;
								if (pixval2 > 0) {
									imagemap[p2->y * imgW + p2->x] = true;
									pixelcount++;
									
									
									
									pt2 *pb0 = new pt2(p2->x, p2->y + 1 );
									pt2 *pb1= new pt2(p2->x, p2->y - 1 );
									pt2 *pb2= new pt2(p2->x + 1, p2->y );
									pt2 *pb3= new pt2(p2->x - 1, p2->y );

									
									thecoords.push_back(pb0);
									thecoords.push_back(pb1);
									thecoords.push_back(pb2);
									thecoords.push_back(pb3);

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
						b->boxdimx = (b->boxmaxx - b->boxminx) ;
						b->boxdimy = (b->boxmaxy - b->boxminy) ;
						b->bx = b->boxminx * wcoordsx;
						b->by = b->boxminy * wcoordsy;
						b->cx = b->boxcenterx * wcoordsx;
						b->cy = b->boxcentery * wcoordsy;
						b->dimx = b->boxdimx * wcoordsx;
						b->dimy = b->boxdimy * wcoordsy;
//						
						if (tflob.getAnyFeatureActive()) {
							if (tflob.trackfeatures[0])
								b = calc_feature_head(b);
							if (tflob.trackfeatures[1]) {
								b = calc_feature_arms(b);
							}
							if (tflob.trackfeatures[2])
								b = calc_feature_feet(b);
							if (tflob.trackfeatures[3])
								b = calc_feature_bottom(b);
						}
						
//						ABlob blob (b); //= new ABlob(b);
						theblobs.push_back(b);
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

	
	while(theblobs.size()>0){
		delete theblobs[0];
		theblobs.erase( theblobs.begin() );
	}
	theblobs.clear();
	
	prevnumblobs = numblobs;
	numblobs = 0; // reset count per frame at begin
//	
//	while(prevblobs.size()>0){
//		delete prevblobs[0];
//		prevblobs.erase( prevblobs.begin() );
//	}
//
//	prevblobs.clear();// = new ArrayList<ABlob>();
//	
//	for (int i = 0; i < theblobs.size(); i++) {
//		prevblobs.push_back(theblobs[i]);
//	}
//	theblobs.clear();
//	
	
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


ABlob &	  ImageBlobs::calc_feature_arms(ABlob & b){
	//
	int bx = b.boxminx;
	int by = b.boxminy;
	int ex = b.boxmaxx;
	int ey = b.boxmaxy;
	// int ey = b.boxcentery;
	
	int cx = b.boxcenterx;
	
	int i = 0, j = 0;
	
	bool found = false;
	// armleft
	i = bx;
	for (j = by; j < ey; j++) {
		if (testimagemap(i, j)) {
			b.armleftx = i * wcoordsx;
			b.armlefty = j * wcoordsy;
			found = true;
			break;
		}
	}
	if (!found) {
		j = by;
		for (i = bx; i < cx; i++) {
			if (testimagemap(i, j)) {
				b.armleftx = i * wcoordsx;
				b.armlefty = j * wcoordsy;
				found = true;
				break;
			}
		}
		if (!found) {
			b.armleftx = b.boxcenterx * wcoordsx;
			b.armlefty = b.boxcentery * wcoordsy;
		}
	}
	
	found = false;
	// armright
	i = ex;
	for (j = by; j < ey; j++) {
		if (testimagemap(i, j)) {
			b.armrightx = i * wcoordsx;
			b.armrighty = j * wcoordsy;
			found = true;
			break;
		}
	}
	// armright try upper quad
	if (!found) {
		j = by;
		for (i = ex - 1; i > cx; i++) {
			if (testimagemap(i, j)) {
				b.armrightx = i * wcoordsx;
				b.armrighty = j * wcoordsy;
				found = true;
				break;
			}
		}
		if (!found) {
			b.armrightx = b.boxcenterx * wcoordsx;
			b.armrighty = b.boxcentery * wcoordsy;
		}
	}
	return b;
	
}



/**
 * ABlob calc_feature_head(ABlob b)<br>
 * <br>
 * calculates where the top center point is in a blob.<br>
 * 
 * @Param ABlob b
 * @return ABlob b
 * 
 */


ABlob &	  ImageBlobs::calc_feature_head(ABlob & b){
	
	// int bx = b.boxminx;
	int by = b.boxminy;
	int ex = b.boxmaxx;
	// int ey = b.boxmaxy;
	int cx = b.boxcenterx;
	int i = 0, j = 0;
	int k = cx - 1;
	// head
	j = by;
	for (i = cx; i < ex; i++) {
		
		if (testimagemap(i, j)) {
			b.headx = i * wcoordsx;
			b.heady = j * wcoordsy;
			break;
		}
		if (testimagemap(k--, j)) {
			b.headx = i * wcoordsx;
			b.heady = j * wcoordsy;
			break;
		}
	}
	
	return b;
}





/**
 * ABlob calc_feature_bottom(ABlob b)<br>
 * <br>
 * calculates where the bottom center point is in a blob.<br>
 * 
 * @Param ABlob b
 * @return ABlob b
 * 
 */



ABlob &	  ImageBlobs::calc_feature_bottom(ABlob & b){

	
	int ex = b.boxmaxx;
	int ey = b.boxmaxy;
	
	int cx = b.boxcenterx;// (bx+ex)/2;///b.boxdimx/2 + bx;
	// int cy = b.boxcentery;//(by+ey)/2;//b.boxdimy/2 + by;
	
	// cx = PApplet.constrain(cx ,0,w-1);
	// cy = PApplet.constrain(cy ,0,h-1);
	
	int i = 0, j = 0;
	// int dir=1;
	
	// bottom
	bool found = false;
	j = ey;
	for (i = cx; i < ex; i++) {
		if (testimagemap(i, j)) {
			b.bottomx = i * wcoordsx;
			b.bottomy = j * wcoordsy;
			found = true;
			break;
			
		}
	}
	
	if (!found) {
		for (i = 0; i <= cx; i++) {
			if (testimagemap(i, j)) {
				b.bottomx = i * wcoordsx;
				b.bottomy = j * wcoordsy;
				found = true;
				break;
				
			}
		}
	}
	
	if (!found) {
		b.bottomx = b.boxcenterx * wcoordsx;
		b.bottomy = ey * wcoordsy;
		
	}
	
	return b;
	
	
}






/**
 * ABlob calc_feature_feet(ABlob b)<br>
 * <br>
 * calculates where the left and right bottom points are in a blob.<br>
 * 
 * @Param ABlob b
 * @return ABlob b
 * 
 */

ABlob &	  ImageBlobs::calc_feature_feet(ABlob & b){
	// /passed to 2 feet instead of one bottom
	int bx = CLAMP(b.boxminx, 0, w - 1);
	// int by = PApplet.constrain(b.boxminy,0,h-1);
	int ex = CLAMP(b.boxmaxx, 0, w - 1);
	int ey = CLAMP(b.boxmaxy, 0, h - 1);
	
	int cx = b.boxcenterx;// (bx+ex)/2;///b.boxdimx/2 + bx;
	int cy = b.boxcentery;// (by+ey)/2;//b.boxdimy/2 + by;
	
	cx = CLAMP(cx, 0, w - 1);
	cy = CLAMP(cy, 0, h - 1);
	
	int i = 0, j = 0;
	
	// footleft
	j = ey;
	for (i = bx; i < cx; i++) {
		if (testimagemap(i, j)) {
			b.footleftx = i * wcoordsx;
			b.footlefty = j * wcoordsx;
			// System.out.print("found armleft at "+b.armleftx+" "+b.armlefty
			// );
			break;
			
		}
	}
	// footright
	j = ey;
	for (i = ex - 1; i > cx; i--) {
		if (testimagemap(i, j)) {
			b.footrightx = i * wcoordsx;
			b.footrighty = j * wcoordsy;
			break;
		}
	}
	
	return b;
	
}



