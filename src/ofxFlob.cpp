/*
 *  Flob.cpp
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 s373.net/x. All rights reserved.
 *
 */

#include "ofxFlob.h"

unsigned int	ofxFlob::TBlobLifeTime = 10;
unsigned int	ofxFlob::TBlobMaxDistSquared = 2555.f;


ofxFlob::~ofxFlob() {}



ofxFlob::ofxFlob() {		
	numPixels = 0;	
	videoresw = videoresh = 128;
	presence = 0;
	videotexmode = pvideotexmode = 0;
	videotexchange = false;	
	videothreshf = videothresh = 25;
	videofadef = videofade = 12;
	mirrorX = false, mirrorY = false;
	worldwidth = worldheight = 700;
	blur = 0;
	om = CONTINUOUS_DIFFERENCE;
	colormode = GREEN;	
	thresholdmode = ABSDIF;
	continuous_ease = 0.10f;	

	floatmode = true;
	floatsmooth = 0.555f;
	bdoclear = true;
	TBlobDoSorting = false;
	clampGray = false;
	nearGray = 0;
	farGray = 255;
}




void ofxFlob::setup(const int srcW,const int srcH, const float dstW, const float dstH ){
	videoresw = srcW; videoresh = srcH;
	worldwidth = dstW; worldheight = dstH;
	
	for(int i=0; i<5;i++) trackfeatures[i] = true;
	videoimggray.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);	
	videotexbin.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);
	videotexmotion.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);
	videotex = &videoimggray;
	numPixels = videoresw * videoresh * 1;
	backgroundPixels.allocate(videoresw, videoresh, OF_IMAGE_GRAYSCALE);
	backgroundLuma.assign(numPixels,0.0f);
	currentLuma.assign(numPixels,0.0f);
	imageblobs.setup(this);	
}



ofxFlob* ofxFlob::setTBlobLifeTime(const float e){
	ofxFlob::TBlobLifeTime = e;
	return this;
}
ofxFlob* ofxFlob::setTBlobMaxDistSquared(const float e){
	ofxFlob::TBlobMaxDistSquared = e;
	return this;
}






ofImage & ofxFlob::binarize (unsigned char * pix, int width, int height){

	unsigned char * videoimggraypix = videoimggray.getPixels();
	unsigned char * backgroundpix = backgroundPixels.getPixels();
	unsigned char * videotexbinpix = videotexbin.getPixels();

	ofImage src;
	if(width!=videoresw||height!=videoresh){
		src.setFromPixels(pix, width, height, OF_IMAGE_COLOR);
		src.resize(videoresw, videoresh);
		pix = src.getPixels();
	}

	
	// colormode pass	rgb->luma
	for(int i=0; i<numPixels;i++){
		int pixval = 0;		
		int loc = i*3;
		switch (colormode) {
			case RED: pixval = pix[loc]; break;
			case GREEN: pixval = pix[loc+1]; break;
			case BLUE: pixval = pix[loc+2]; break;
			case LUMA601:
				pixval = (0.299f * pix[loc] + 0.587f * pix[loc+1] + 0.114f * pix[loc+2] +0.5f);
				break;
			case LUMA709:
				pixval = (0.2126f * pix[loc] + 0.7152f * pix[loc+1] + 0.0722f * pix[loc+2] +0.5f);
				break;
			case LUMAUSER:
				pixval = (lumausercoefs[0] * pix[loc] + lumausercoefs[1] * pix[loc+1] + lumausercoefs[2] * pix[loc+2] +0.5f);
				break;				
		}
		
		videoimggraypix[i] = pixval;
	}
		
	
	
//	videoimggray.update();
	

	
	
	// pixelMirror implementation as
	
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
		delete[] image;
		
	} else if (mirrorX && !mirrorY) {
		
		int* scanline = new int[videoresw]; // one hscanline
		for (int j = 0; j < videoresh; j++) {
			for (int i = 0; i < videoresw; i++) {
				scanline[(videoresw - i - 1)] = videoimggraypix[j * videoresw + i];
				videoimggraypix[j * videoresw + i] = scanline[i];
			}
		}
		
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
		
		delete[] scanline;
	}
	
	videoimggray.update();

	
	// core binarize
	int currentVal = 0, backgroundVal = 0, diffVal = 0;
	float currentValf = 0, backgroundValf = 0, diffValf = 0;
		
	presence = 0;
	
	
	for(int i=0; i<numPixels;i++){
		 currentVal = videoimggraypix[i];
		 backgroundVal = backgroundpix[i];
		
		if(clampGray){
			currentVal = currentVal < nearGray ? nearGray : currentVal > farGray ? farGray : currentVal; 
			backgroundVal = backgroundVal < nearGray ? nearGray : backgroundVal > farGray ? farGray : backgroundVal; 			
		}
		
		int binarize = 0;
		
		if(floatmode){
			if(ABS(currentLuma[i]-currentVal)>1e-5) currentLuma[i] += ((float)currentVal-currentLuma[i])*floatsmooth;
			if(ABS(backgroundLuma[i]-backgroundVal)>1e-5) backgroundLuma[i] += ((float)backgroundVal-backgroundLuma[i])*floatsmooth;
			currentValf = currentLuma[i];
			backgroundValf = backgroundLuma[i];
			
			switch(thresholdmode){
				case ABSDIF: 
					diffValf = ABS(currentValf-backgroundValf); 
					if (diffValf >= videothreshf) {
						presence += 1;
						binarize = 255;
					}
					break;
				case LESSER: 
					diffValf = currentValf-backgroundValf; 
					if (diffValf <= videothreshf) {
						presence += 1;
						binarize = 255;
					}
					break;
				case GREATER: 
					diffValf = currentValf-backgroundValf; 
					if (diffValf >= videothreshf) {
						presence += 1;
						binarize = 255;
					}
					break;
			}
		} else {
			
			switch(thresholdmode){
				case ABSDIF: 
					diffVal = ABS(currentVal-backgroundVal); 
					if (diffVal >= videothresh) {
						presence += 1;
						binarize = 255;
					}
					break;
				case LESSER: 
					diffVal = currentVal-backgroundVal; 
					if (diffVal <= videothresh) {
						presence += 1;
						binarize = 255;
					}
					break;
				case GREATER: 
					diffVal = currentVal-backgroundVal; 
					if (diffVal >= videothresh) {
						presence += 1;
						binarize = 255;
					}
					break;
			}
			
		}
		
		// encode pix to binary img
		videotexbinpix[i] = binarize;
		//(binarize << 24) | (binarize << 16)	| (binarize << 8) | binarize;							
	}
	
	// 
	videotexbin.update();	
	if(presence>0){
		presencef = (float)presence / numPixels;
	} else {
		presencef = 0.0f;
	}
	
	
	
	
	if (om > STATIC_DIFFERENCE) {
		// now update motion img and use that as base for tracking
		
		unsigned char * videotexmotionpix = videotexmotion.getPixels();
		
		for (int i = 0; i < numPixels; i++) {
			if(floatmode){
				float valf = videotexmotionpix[i];
				// int value = (videotexmotion.pixels[i] >> 8) & 0xff;
				valf -= videofadef; // minus fade
				valf += videotexbinpix[i]; // (videotexbinpix[i] >> 8) & 0xff; // + binary
				valf = valf < 0 ? 0 : valf > 255 ? 255 : valf;
				int value = (int)(valf+0.5f);
				videotexmotionpix[i] = value;
			} else {
				int value = videotexmotionpix[i];
				value -= videofade; // minus fade
				value += videotexbinpix[i]; // + binary
				value = value < 0 ? 0 : value > 255 ? 255 : value;
				videotexmotionpix[i] = value;				
			}
			
		}
		
		videotexmotion.update();
		videotex = &videotexmotion;
		
		// learn background as the frame that just passed
		if (om == 1) 
			setBackground(videoimggray);
		else 
			easeBackground(videoimggray);
		
		return videotexmotion;
	}
	
	if(bdoclear){
		setBackground(videoimggray);
		bdoclear = false;
	}
	videotex = &videotexbin;
	return videotexbin;
	
	
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
		if(ABS(bgpix[i]-imgpix[i])>1e-5)
			bgpix[i] += (unsigned char)(((float)imgpix[i] - (float)bgpix[i] ) * continuous_ease);
	}
	backgroundPixels.update();
	
}


vector<ABlob*>* ofxFlob::calc (ofImage& bin){
	imageblobs.calc(bin);
	return &imageblobs.theblobs;
}

vector<TBlob*>*  ofxFlob::calcsimple( ofImage & bin ){
	imageblobs.calc(bin);
	return imageblobs.calcsimple();

}

vector<TBlob*>*  ofxFlob::track( ofImage & bin ){
	imageblobs.calc(bin);
	imageblobs.dotracking();
	return &imageblobs.TBlobs;
}

vector<TBlob*>*  ofxFlob::tracksimple( ofImage & bin ){
	imageblobs.calc(bin);
	return imageblobs.tracksimple();

}

