/*
 *  Flob.cpp
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 s373.net/x. All rights reserved.
 *
 */

#include "ofxFlob.h"

///
ofxFlob::~ofxFlob() {}

ofxFlob::ofxFlob() {		
	numPixels = 0;
	
	videoresw = videoresh = 128;
	presence = 0;
	videotexmode = pvideotexmode = 0;
	videotexchange = false;
	
	videothresh = 25;
	videofade = 12;
	mirrorX = false, mirrorY = false;
	worldwidth = worldheight = 700;
	blur = 0;
	om = CONTINUOUS_DIFFERENCE;
	colormode = GREEN;	
	bglowpass = 0.05;	
	trackedBlobLifeTime = 100;
	
}


void ofxFlob::setup(int srcW, int srcH, int dstW, int dstH){
	videoresw = srcW; videoresh = srcH;
	worldwidth = dstW; worldheight = dstH;

	colormode = GREEN;
	
	for(int i=0; i<5;i++) trackfeatures[i] = true;
	videoimggray.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);	
	videotexbin.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);
	videotexmotion.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);
//	videoteximgmotion.allocate(videoresw, videoresh, OF_IMAGE_COLOR);
//	videotex.allocate(videoresw, videoresh, OF_IMAGE_COLOR_ALPHA);
	videotex = &videoimggray;
	numPixels = videoresw * videoresh * 1;
	backgroundPixels.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);
	
	imageblobs.setup(this);
	
	bglowpass = 0.0121;
}



ofImage & ofxFlob::binarize (unsigned char * pix, int width, int height){

	unsigned char * videoimggraypix = videoimggray.getPixels();
	unsigned char * backgroundpix = backgroundPixels.getPixels();
	unsigned char * videotexbinpix = videotexbin.getPixels();

	
	if(width!=videoresw||height!=videoresh){
		ofImage src;
		src.setFromPixels(pix, width, height, OF_IMAGE_COLOR);
		src.resize(videoresw, videoresh);
		unsigned char * srcpix = src.getPixels();
		
		for(int i=0; i<numPixels;i++){
			videoimggraypix[i] = srcpix[i*3+1]; //g	
		}
		
	} else {
		
		for(int i=0; i<numPixels;i++){
			//		videoimggraypix[i] = videoimgpix[i*3+1]; //g	
			videoimggraypix[i] = pix[i*3+1]; //g	
		}
		
	}
	
	videoimggray.update();
	
	// 
	int currentVal = 0, backgroundVal = 0, diffVal = 0;
	// float currentValf = 0, backgroundValf = 0, diffValf = 0;
	float diffValf = 0;	
	float fease = 0.17f;	
	
	presence = 0;
	
	
	// implementing mirror
	
	if (mirrorX && mirrorY) {
		int* image = new int[numPixels]; // one image to flipx&y
		for (int i = 0; i < numPixels; i++) {
			image[i] = videoimggraypix[i];
		}
		
		for (int j = 0; j < videoresh; j++) {
			for (int i = 0; i < videoresw; i++) {
				videoimggraypix[j * videoresw + i ] = image[(videoresw - i - 1)
															 + (videoresh - j - 1) * videoresw ];
			}
		}
		videoimggray.update();
		delete[] image;
		
	} else if (mirrorX && !mirrorY) {
		
		int* scanline = new int[videoresw]; // one hscanline
		for (int j = 0; j < videoresh; j++) {
			for (int i = 0; i < videoresw; i++) {
				scanline[(videoresw - i - 1)] = videoimggraypix[j * videoresw + i];
				videoimggraypix[j * videoresw + i] = scanline[i];
			}
		}
		
		videoimggray.update();
		delete[] scanline;
		
	} else if (!mirrorX && mirrorY) {
		// working ok since 001j
		int* scanline = new int[videoresh]; // one vscanline
		
		for (int i = 0; i < videoresw; i++) {
			for (int j = 0; j < videoresh; j++) {
				scanline[(videoresh - j - 1)] = videoimggraypix[j * videoresw + i];
				videoimggraypix[j * videoresw + i] = scanline[j];
			}
		}
		
		videoimggray.update();
		delete[] scanline;
	}
	
	
	
	// core binarize
			
	
	if (om == STATIC_DIFFERENCE) {
		
		for (int i = 0; i < numPixels; i++) {
//			int currColor = videoimgpix[i*3+1];
			int currColor = videoimggraypix[i];
			int bkgdColor = backgroundpix[i];
						
			diffVal = ABS(currColor - bkgdColor);
//			diffValf = ABS(currentPixelsF[i] - backgroundPixelsF[i]);
						
			int binarize = 0;
			if (diffVal >= videothresh) {
				presence += 1;
				binarize = 255;
			}
			
			videotexbinpix[i] = binarize;
//			(binarize << 24) | (binarize << 16)
//			| (binarize << 8) | binarize;
		}

		videotexbin.update();		
		videotex = &videotexbin;
		
		return videotexbin;
		
	} else if (om >= CONTINUOUS_DIFFERENCE) {
		
		for (int i = 0; i < numPixels; i++) {
			int currColor = videoimggraypix[i];
			int bkgdColor = backgroundpix[i];
			
			// currentValf = backgroundPixelsF[i];
			// backgroundValf = backgroundPixelsF[i];
			
			currentVal = currColor;
			backgroundVal = bkgdColor;
			
			diffVal = ABS(currentVal - backgroundVal);
//			diffValf = ABS(currentPixelsF[i] - backgroundPixelsF[i]);
//			differencePixelsF[i] = diffValf;
			
			int binarize = 0;
			if (diffVal >= videothresh) {
				presence += 1;
				binarize = 255;
			}
			
			videotexbinpix[i] = binarize;
		}
				
		videotexbin.update();

		// now update motion img and use that as base for tracking
		
		unsigned char * videotexmotionpix = videotexmotion.getPixels();
		
		for (int i = 0; i < numPixels; i++) {
			
			float valf = videotexmotionpix[i];
			// int value = (videotexmotion.pixels[i] >> 8) & 0xff;
			valf -= videofade; // minus fade
			valf += videotexbinpix[i]; // (videotexbinpix[i] >> 8) & 0xff; // + binary
			valf = valf < 0 ? 0 : valf > 255 ? 255 : valf;
			int value = (int)(valf+0.5f);
			videotexmotionpix[i] = value;
			
		}
		
		videotexmotion.update();
		videotex = &videotexmotion;
		
		// learn background as the frame that just passed
		if (om == 1)
			setBackground(videoimggray);
		if (om == 2)
			easeBackground(videoimggray);
		
		return videotexmotion;
	}
	
	return videotexbin; // / just for return
}





void ofxFlob::setBackground (ofImage& img){
	unsigned char * bgpix = backgroundPixels.getPixels();
	unsigned char * imgpix = img.getPixels();
	for(int i=0; i<numPixels; i++){
		bgpix[i] = imgpix[i];
	}
	backgroundPixels.update();
	
}
void ofxFlob::easeBackground (ofImage& img){
	unsigned char * bgpix = backgroundPixels.getPixels();
	unsigned char * imgpix = img.getPixels();
	for(int i=0; i<numPixels; i++){
		bgpix[i] += ( imgpix[i] - bgpix[i] ) * bglowpass;
	}
	backgroundPixels.update();
	
}


vector<ABlob*>* ofxFlob::calc (ofImage& bin){
	imageblobs.calc(bin);
	return &imageblobs.theblobs;
}

