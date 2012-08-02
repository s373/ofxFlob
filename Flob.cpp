/*
 *  Flob.cpp
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Flob.h"

///
ofxFlob::~ofxFlob() {}

ofxFlob::ofxFlob() {
//	videoresw = videoresh = 128; worldwidth = worldheight = 700;
//	setup();
}


void ofxFlob::setup(int srcW, int srcH, int dstW, int dstH){
	videoresw = srcW; videoresh = srcH;
	worldwidth = dstW; worldheight = dstH;

	colormode = GREEN;
	
	for(int i=0; i<5;i++) trackfeatures[i] = false;
	videoimg.allocate(videoresw, videoresh, OF_IMAGE_COLOR);
	videotexbin.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);
	videotexmotion.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);
	videoteximgmotion.allocate(videoresw, videoresh, OF_IMAGE_COLOR);
	videotex.allocate(videoresw, videoresh, OF_IMAGE_COLOR_ALPHA);
	numPixels = videoresw * videoresh * 1;
	backgroundPixels.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);
	
	imageblobs.setup(this);
	
}



ofImage & ofxFlob::binarize (unsigned char * pix){

	videoimg.setFromPixels(pix, videoresw, videoresh, OF_IMAGE_COLOR);
	videoimg.update();

	// skiping mirror
	
	int currentVal = 0, backgroundVal = 0, diffVal = 0;
	// float currentValf = 0, backgroundValf = 0, diffValf = 0;
	float diffValf = 0;
	
	float fease = 0.17f;
	
	
	presence = 0;
	
	unsigned char * videoimgpix = videoimg.getPixels();
	unsigned char * backgroundpix = backgroundPixels.getPixels();
	unsigned char * videotexbinpix = videotexbin.getPixels();
		
	
	if (om == STATIC_DIFFERENCE) {
	
		
		for (int i = 0; i < numPixels; i++) {
			int currColor = videoimgpix[i*3+1];
			int bkgdColor = backgroundpix[i];
			
			// currentValf = backgroundPixelsF[i];
			// backgroundValf = backgroundPixelsF[i];
			
			currentVal = currColor;
			backgroundVal = bkgdColor;
			
//			switch (colormode) {
//				case RED:
//					currentVal = (currColor) & 0xFF;
//					backgroundVal = (bkgdColor) & 0xFF;
//					break;
//				case GREEN:
//					currentVal = (currColor >> 8) & 0xFF;
//					backgroundVal = (bkgdColor >> 8) & 0xFF;
//					break;
//				default:
//				case BLUE:
//					currentVal = (currColor >> 16) & 0xFF;
//					backgroundVal = (bkgdColor >> 16) & 0xFF;
//					break;
////				case LUMA601:
////					float pixval3 = (float) (0.299f * ((currColor) & 0xFF)
////											+ 0.587 * ((currColor >> 8) & 0xFF) + 0.114 * ((currColor >> 16) & 0xFF)) + 0.5f; // CCIR
////					float bgval3 = (float) (0.299f * ((bkgdColor) & 0xFF)
////										   + 0.587 * ((bkgdColor >> 8) & 0xFF) + 0.114 * ((bkgdColor >> 16) & 0xFF)) + 0.5f; // CCIR
////					currentVal = (int) pixval3;
////					backgroundVal = (int) bgval3;
////					break;
////				case LUMA709:
////					float pixval1 = (float) (0.2126f * ((currColor) & 0xFF)
////											 + 0.7152 * ((currColor >> 8) & 0xFF) + 0.0722 * ((currColor >> 16) & 0xFF)) + 0.5f; // CCIR
////					float bgval1 = (float) (0.2126f * ((bkgdColor) & 0xFF)
////											+ 0.7152 * ((bkgdColor >> 8) & 0xFF) + 0.0722 * ((bkgdColor >> 16) & 0xFF)) + 0.5f; // CCIR
////					// 709
////					currentVal = (int) pixval1;
////					backgroundVal = (int) bgval1;
////					break;
//				case LUMAUSER:
//					float pixval2 = (lumausercoefs[0] * ((currColor) & 0xFF)
//									 + lumausercoefs[1] * ((currColor >> 8) & 0xFF) + lumausercoefs[2]
//									 * ((currColor >> 16) & 0xFF)) + 0.5f; // CCIR
//					// 709
//					float bgval2 = (lumausercoefs[0] * ((bkgdColor) & 0xFF)
//									+ lumausercoefs[1] * ((bkgdColor >> 8) & 0xFF) + lumausercoefs[2]
//									* ((bkgdColor >> 16) & 0xFF)) + 0.5f; // CCIR
//					// 709
//					currentVal = (int) pixval2;
//					backgroundVal = (int) bgval2;
//					break;
//					
//			}
			
//			backgroundPixelsF[i] += (backgroundVal - backgroundPixelsF[i])
//			* fease;
//			currentPixelsF[i] += (currentVal - currentPixelsF[i]) * fease;
			
			diffVal = ABS(currentVal - backgroundVal);
//			diffValf = ABS(currentPixelsF[i] - backgroundPixelsF[i]);
			
//			differencePixelsF[i] = diffValf;
			
			int binarize = 0;
			// if (diffVal > videothresh) {
			if (diffValf > videothresh) {
				presence += 1;
				binarize = 255;
			}
			
			videotexbinpix[i] = binarize;
//			(binarize << 24) | (binarize << 16)
//			| (binarize << 8) | binarize;
		}
		videoimg.update();
		videotexbin.update();
		
		return videotexbin;
		
	} else if (om >= CONTINUOUS_DIFFERENCE) {
		
		for (int i = 0; i < numPixels; i++) {
			int currColor = videoimgpix[i*3+1];
			int bkgdColor = backgroundpix[i];
			
			// currentValf = backgroundPixelsF[i];
			// backgroundValf = backgroundPixelsF[i];
			
			currentVal = currColor;
			backgroundVal = bkgdColor;
			
//			
//			switch (colormode) {
//				case RED:
//					currentVal = (currColor) & 0xFF;
//					backgroundVal = (bkgdColor) & 0xFF;
//					break;
//				case GREEN:
//					currentVal = (currColor >> 8) & 0xFF;
//					backgroundVal = (bkgdColor >> 8) & 0xFF;
//					break;
//				default:
//				case BLUE:
//					currentVal = (currColor >> 16) & 0xFF;
//					backgroundVal = (bkgdColor >> 16) & 0xFF;
//					break;
////				case LUMA601:
////					float pixval = (float) (0.299f * ((currColor) & 0xFF)
////											+ 0.587 * ((currColor >> 8) & 0xFF) + 0.114 * ((currColor >> 16) & 0xFF)) + 0.5f; // CCIR
////					float bgval = (float) (0.299f * ((bkgdColor) & 0xFF)
////										   + 0.587 * ((bkgdColor >> 8) & 0xFF) + 0.114 * ((bkgdColor >> 16) & 0xFF)) + 0.5f; // CCIR
////					// 601
////					currentVal = (int) pixval;
////					backgroundVal = (int) bgval;
////					break;
////				case LUMA709:
////					float pixval1 = (float) (0.2126f * ((currColor) & 0xFF)
////											 + 0.7152 * ((currColor >> 8) & 0xFF) + 0.0722 * ((currColor >> 16) & 0xFF)) + 0.5f; // CCIR
////					float bgval1 = (float) (0.2126f * ((bkgdColor) & 0xFF)
////											+ 0.7152 * ((bkgdColor >> 8) & 0xFF) + 0.0722 * ((bkgdColor >> 16) & 0xFF)) + 0.5f; // CCIR
////					// 709
////					currentVal = (int) pixval1;
////					backgroundVal = (int) bgval1;
////					break;
//				case LUMAUSER:
//					float pixval2 = (lumausercoefs[0] * ((currColor) & 0xFF)
//									 + lumausercoefs[1] * ((currColor >> 8) & 0xFF) + lumausercoefs[2]
//									 * ((currColor >> 16) & 0xFF)) + 0.5f; // CCIR
//					// 709
//					float bgval2 = (lumausercoefs[0] * ((bkgdColor) & 0xFF)
//									+ lumausercoefs[1] * ((bkgdColor >> 8) & 0xFF) + lumausercoefs[2]
//									* ((bkgdColor >> 16) & 0xFF)) + 0.5f; // CCIR
//					// 709
//					currentVal = (int) pixval2;
//					backgroundVal = (int) bgval2;
//					break;
//			}
//			
////			backgroundPixelsF[i] += (backgroundVal - backgroundPixelsF[i])
////			* fease;
////			currentPixelsF[i] += (currentVal - currentPixelsF[i]) * fease;
			
			diffVal = ABS(currentVal - backgroundVal);
//			diffValf = ABS(currentPixelsF[i] - backgroundPixelsF[i]);
//			differencePixelsF[i] = diffValf;
			
			int binarize = 0;
			if (diffVal > videothresh) {
				presence += 1;
				binarize = 255;
			}
			
			videotexbinpix[i] = binarize;
//			(binarize << 24) | (binarize << 16)
//			| (binarize << 8) | binarize;
		}
		
		
		videotexbin.update();
		
		// now update motion img and use that as base for tracking
		
//		videotexmotion.loadPixels();
		unsigned char * videotexmotionpix = videotexmotion.getPixels();
		
		for (int i = 0; i < numPixels; i++) {
			
//			int currColor = videoimgpix[i*3+1];
//			int bkgdColor = backgroundpix[i];

			float valf = videotexmotionpix[i] ;//ABS(currColor - bkgdColor);//videotexbinpix[i]; //differencePixelsF[i];
			// int value = (videotexmotion.pixels[i] >> 8) & 0xff;
			valf -= videofade; // minus fade
			valf += videotexbinpix[i]; // (videotexbinpix[i] >> 8) & 0xff; // + binary
			valf = valf < 0 ? 0 : valf > 255 ? 255 : valf;
			int value = (int)(valf+0.5f);
			videotexmotionpix[i] = value;
			
		}
		
		videoimg.update();
		videotexmotion.update();
		
		// learn background as the frame that just passed
		if (om == 1)
			setBackground(videoimg);
		if (om == 2)
			easeBackground(videoimg);
		
		return videotexmotion;
	}
	
	return videotexbin; // / just for return
}





void ofxFlob::setBackground (ofImage& img){
	unsigned char * bgpix = backgroundPixels.getPixels();
	unsigned char * imgpix = img.getPixels();
	for(int i=0; i<numPixels; i++){
		bgpix[i] = imgpix[i*3+1];
	}
	backgroundPixels.update();
	
}
void ofxFlob::easeBackground (ofImage& img){
	unsigned char * bgpix = backgroundPixels.getPixels();
	unsigned char * imgpix = img.getPixels();
	for(int i=0; i<numPixels; i++){
		bgpix[i] += ( imgpix[i*3+1] - bgpix[i] ) * 0.05f;
	}
	backgroundPixels.update();
	
}


vector<ABlob*>* ofxFlob::calc (ofImage& bin){
	imageblobs.calc(bin);
	return &imageblobs.theblobs;
}

