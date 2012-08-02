/*
 *  CV.cpp
 *  balls-opencvExample
 *
 *  Created by andre sier on 24/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ComputerVision.h"


//////////////////////////////////////////////////////////
//construtores
ComputerVision::ComputerVision(){
	
}

//////////////////////////////////////////////////////////
ComputerVision::~ComputerVision(){
	if(camera!=NULL)
		delete camera;
}


//////////////////////////////////////////////////////////

//funções de arranque



void ComputerVision::init(){
	fadeAmnt    = 10.0f;
	threshAmnt  = 10;
	bPauseVideo = false;
	frameCount  = 0;
	om = 1;
	//	bUseCamera  = true;
	
	DrawOm = 1;
	
	bLearnBackground = true;
	mirrorX = true;
	//		mirrorX = false;
	
	w = imgcolorfull.width;
	h = imgcolorfull.height;
	
	cvwidth = imgcolor.width;
	cvheight = imgcolor.height;
	
	bRoi = true;	
	roix = 0;
	roiy = 0;
	roiw = w;
	roih = h;
	
}


void ComputerVision::setMirrorX(bool m){
	mirrorX = m;
}

void ComputerVision::loadVideo(string path){
	
	//	printf("loading %s",path);
	
	bUseCamera = false;
	video.loadMovie(path);
	video.play();
	
	imgcolor.allocate(video.width, video.height);
	imggray.allocate(video.width, video.height);
	imgthresh.allocate(video.width, video.height);
	imgprev.allocate(video.width, video.height);
	imggraymotion.allocate(video.width, video.height);			
	init();
}


//////////////////////////////////////////////////////////
void ComputerVision::setupCamera(int which, int width, int height){
	
	bUseCamera = true;
	camera = new ofVideoGrabber(); //alocate the pointer
	
	camera->setDeviceID(which);
	camera->initGrabber(width, height);
	camera->listDevices();
	camera->setDeviceID(which);//again
	
	imgcolorfull.allocate(camera->width, camera->height);
	imgcolor.allocate(camera->width, camera->height);
	imggray.allocate(camera->width, camera->height);
	imgthresh.allocate(camera->width, camera->height);
	imgprev.allocate(camera->width, camera->height);
	imggraymotion.allocate(camera->width, camera->height);
	init();	
}

// new
//////////////////////////////////////////////////////////
void ComputerVision::setupCamera(int which, int _camwidth, int _camheight, int _cvwidth, int _cvheight ){
	bUseCamera = true; /// stupid
	camera = new ofVideoGrabber(); //alocate the pointer
	
	camera->setDeviceID(which);
	camera->initGrabber(_camwidth, _camheight);
	camera->listDevices();
	camera->setDeviceID(which);//again
	
	imgcolorfull.allocate(_camwidth, _camheight);
	
	imgcolor.allocate(_cvwidth, _cvheight);
	imggray.allocate(_cvwidth, _cvheight);
	imgthresh.allocate(_cvwidth, _cvheight);
	imgprev.allocate(_cvwidth, _cvheight);
	imggraymotion.allocate(_cvwidth, _cvheight);
	
	
	printf("cam : %ld %ld   cv : %ld %ld", imgcolorfull.width,imgcolorfull.height, imgcolor.width, imgcolor.height );
	
	//should go also to init
	//	camwidth, camheight, cvwidth, cvheight;
	
	
	init();	
}




void ComputerVision::setRoiWH(int _w, int _h){	
	roiw = _w;
	roih = _h;		
	printf("cv roi : %ld %ld %ld %ld \n",roix,roiy,roiw,roih);
}
void ComputerVision::setRoiXY(int _x, int _y){	
	roix = _x;
	roiy = _y;		
	printf("cv roi : %ld %ld %ld %ld \n",roix,roiy,roiw,roih);
}


//////////////////////////////////////////////////////////
void ComputerVision::update(){		
	
	if(bPauseVideo ) return;
	
	if(bUseCamera){
		camera->grabFrame();
		
		// previous frame
		if(om>0||bLearnBackground)
			imgprev = imggray;	
		if(om==0&&bLearnBackground)
			bLearnBackground=false;
		
		//		imgcolor.setFromPixels(camera->getPixels(), camera->width, camera->height);
		
		imgcolorfull.setFromPixels(camera->getPixels(), camera->width, camera->height);
		imgcolorfull.setROI(roix,roiy,roiw, roih);
		
		//	ofxCvColorImage cropedimage;
		//		cropedimage.allocate(imgcolorfull.getWidth(), imgcolorfull.getHeight());
		//		cropedimage = imgcolorfull;
		//		cropedimage.resize(cvwidth, cvheight);
		
		//		imgcolorfull.resize(cvwidth, cvheight);
		
		if(imgcolor.width!=imgcolorfull.width)
			imgcolor.scaleIntoMe(imgcolorfull, CV_INTER_LINEAR);// = cropedimage;//imgcolorfull;
		else			
			imgcolor = imgcolorfull;
//			imgcolor.setFromPixels(imgcolorfull.getPixels(), imgcolorfull.width, imgcolorfull.height);
		//imgcolor = imgcolorfull;
			
		if(	mirrorX )
			imgcolor.mirror(false,true);
		
		
		//crashes
		//	//aplying roi
		//		ofxCvColorImage camimg;
		//		camimg.setFromPixels(camera->getPixels(),camwidth,camheight);
		//		camimg.setROI(roix,roiy,roiw, roih);
		//		camimg.resize(cvwidth, cvheight);
		//		imgcolor.setFromPixels(camimg.getPixels(), cvwidth,cvheight);
		
	}else{		
		video.idleMovie();
		if(video.isFrameNew()){
			if(om>0||bLearnBackground)
				imgprev = imggray;		
			if(om==0&&bLearnBackground)
				bLearnBackground=false;
			frameCount++;
		}else{
			return;
		}		
		imgcolor.setFromPixels(video.getPixels(), video.width, video.height);
	}
	
	//	imgcolor.setROI(roix,roiy,roiw, roih);
	//	imgcolor.resize(w, h);
	
	//convert the color image to grayscale
	imggray = imgcolor;
	//	//mirror gray image x axis
	//	if(	mirrorX )
	//		imggray.mirror(false,true);
	
	//do absolute diff between the prev frame and current frame
	//all pixels that are different will show up as non-black
	imgthresh.absDiff(imggray, imgprev);	
	//threshold to binary value
	imgthresh.threshold(threshAmnt);
	
	// fade imggraymotion and add tresh
	imggraymotion -= fadeAmnt; 
	imggraymotion += imgthresh;
	
	// encontrar blobs nos contours
	//	contourFinder.findContours(imggraymotion, 10, (340*240)/3, MAX_BLOBS, false);	// 20blobs & no find holes
	//	contourFinder.findContours(imggraymotion, 10, w*h/15, MAX_BLOBS, false);	// 20blobs & no find holes
	contourFinder.findContours(imggraymotion, 10, w*h/3, MAX_BLOBS, false);	// 20blobs & no find holes
	
}

/*
 void ComputerVision::draw(float x, float y ,float z){
 
 }
 */
//////////////////////////////////////////////////////////
void ComputerVision::draw(float x, float y, float z, float al){
	switch (DrawOm) {
		case 0:
			break;
			
		case 1:
			int cx,cy;
			
			glPushMatrix();
			glTranslatef(-imggray.width,-imggray.height,z);
			glColor4f(1.f,1.f,1.f,al);
			//ofSetColor(0xFFFFFF);
			//			imgcolor.mirror(false,true);
			imgcolor.draw(x,y); 
			imggray.draw(x+imgcolor.width+5, y);
			imgthresh.draw(x, y + imggray.height);
			//imggray.draw(x, y + imggray.height + imgthresh.height);				
			imggraymotion.draw(x + imggray.width +5 ,  y + imggray.height);			
			
			// we could draw the whole contour finder
			cx = x + imggray.width +5;//360;
			cy =  y + imggray.height;//0;//40;
			//				contourFinder.draw(cx,cy);
			
			glColor4f(0.f,.2f,1.f,al);
			// e tb desenhar os centroids
			for(int i=0; i < contourFinder.nBlobs; i++) {
				ofRect(contourFinder.blobs[i].centroid.x+cx,contourFinder.blobs[i].centroid.y+cy, 10,10);
			}
			
			//				// e tb desenhar os centroids duas vezes!!
			//				for(int i=0; i < contourFinder.nBlobs; i++) {
			//					ofRect(contourFinder.blobs[i].centroid.x+cx,contourFinder.blobs[i].centroid.y+cy+imggray.height, 10,10);
			//				}
			
			glPopMatrix();
			
			
			break;						
			
		case 2:
			
			glColor4f(1.f,1.f,1.f,0.8f);
			imggraymotion.draw(0,0,w,h); //setable during setup
			
			break;

			
		case 3:
			
			glColor4f(1.f,1.f,1.f,0.8f);
			imgcolorfull.draw(0,0,w,h); //setable during setup
			
			break;
			
			
		case 4:
			
			glColor4f(1.f,1.f,1.f,0.2f);//0.8f);
			imggraymotion.draw(0,0,w,h); //setable during setup
			
			glColor4f(1.f,1.f,1.f,0.05f);
			imggray.draw(w-imggray.width, h-imggray.height);
			imggraymotion.draw(w-imggray.width, h-imggray.height*2);
			
			break;
	}
}



void ComputerVision::draw(int width, int height, float al){
	glColor4f(1.f,1.f,1.f,al);//2f);//0.8f);
	imgcolor.draw(0,0,width,height); //setable during setup

}

//////////////////////////////////////////////////////////
void ComputerVision::refreshvdig(){
	// delete it
	camera->close();
	delete camera;
	// make a new camera
	camera = new ofVideoGrabber();
	camera->setDeviceID(0);
	camera->initGrabber(320,240);
}


//////////////////////////////////////////////////////////
void ComputerVision::setFade(float amt){
	fadeAmnt = amt;
}

//////////////////////////////////////////////////////////
void ComputerVision::settings(){
	camera->videoSettings();
}


//////////////////////////////////////////////////////////
void ComputerVision::setThresh(float thresh){
	threshAmnt = thresh;
}

//////////////////////////////////////////////////////////
void ComputerVision::setDrawOm(int om){
	DrawOm = om;
}
//////////////////////////////////////////////////////////