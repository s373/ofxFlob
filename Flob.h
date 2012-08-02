/*
 *  Flob.h
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#include "ImageBlobs.h"
//#include "ABlob.h"
//#include "trackedBlob.h"



class ofxFlob {
public:
	ImageBlobs					imageblobs;
//	ofxCvColorImage				videoimg;
	ofImage				videoimgsmall;
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
	
	static int					trackedBlobLifeTime;
	
	string						version;
	
	
	
	
	
	//// constructores
	~ofxFlob();
	ofxFlob ();

	void setup(int srcW, int srcH, int dstW, int dstH);
	
	float getPresencef(){
		return (float) presence / numPixels;
	}
	
	
	/// main funcs
	ofImage & binarize (unsigned char * pix, int width, int height);
	void setBackground (ofImage& img);
	void easeBackground (ofImage& img);
	vector<ABlob*>*  calc( ofImage & bin ); 
	
	bool getAnyFeatureActive() {
		return (trackfeatures[0] || trackfeatures[1]
				|| trackfeatures[2] || trackfeatures[3] || trackfeatures[4]);
	
	}

};