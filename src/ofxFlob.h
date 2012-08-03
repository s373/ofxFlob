/*
 *  Flob.h
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 s373.net/x. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ImageBlobs.h"

class ofxFlob {
public:
	ImageBlobs					imageblobs;
	ofImage				videoimggray;
	ofImage						*videotex;
//	ofxCvGrayscaleImage			videotexmotion;
//	ofxCvGrayscaleImage			videotexbin;
//	ofxCvColorImage				videoteximgmotion;	
//	ofxCvGrayscaleImage			backgroundPixels;
	ofImage			videotexmotion;
	ofImage			videotexbin;
	ofImage			videoteximgmotion;	
	ofImage			backgroundPixels;
//	ofFloatImage
	
	int							numPixels;
	
	int							videoresw, videoresh;
	int							presence;
	int							videotexmode, pvideotexmode;
	bool						videotexchange;
	
	int							videothresh;
	int							videofade;
	bool						mirrorX, mirrorY;
	int							worldwidth, worldheight;
	bool						coordsmode;
	int							blur;
	bool						trackfeatures[5];
	int							om;
	enum { STATIC_DIFFERENCE = 0, CONTINUOUS_DIFFERENCE = 1 };
	int							colormode;
	enum { RED = 0, GREEN, BLUE, LUMA601, LUMA709, LUMAUSER } ;
	float						lumausercoefs[3];
	int							blobpixmin, blobpixmax;
	
	float						bglowpass;
	
	int							trackedBlobLifeTime;
	
	string						version;
	
	
	
	
	
	//// 
	
	~ofxFlob();
	ofxFlob ();

	void setup(int srcW, int srcH, int dstW, int dstH);
	
	float getPresencef(){
		return (float) presence / numPixels;
	}
	
	
	/// 
	
	ofImage & binarize (unsigned char * pix, int width, int height);
	
	vector<ABlob*>*  calc( ofImage & bin ); 
	
	void setBackground (ofImage& img);
	void easeBackground (ofImage& img);

	bool getAnyFeatureActive() {
		return (trackfeatures[0] || trackfeatures[1]
				|| trackfeatures[2] || trackfeatures[3] || trackfeatures[4]);	
	}
	
	void draw(){
		videotex->draw(0,0,ofGetWidth(), ofGetHeight());	
	}

};