/*
 *  Flob.h
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 s373.net/x. All rights reserved.
 *
 */


/**
 * Flob
 * Fast multi-blob detector and simple skeleton tracker using flood-fill algorithms.
 * http://s373.net/code/flob
 *
 * Copyright (C) 2008-2013 Andre Sier http://s373.net
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 * 
 * @author      Andre Sier 
 * @modified    20130208
 * @version     0.2.2y (22)
 * @url			http://s373.net/code/flob
 */




#pragma once

#include "ofMain.h"
#include "ImageBlobs.h"
typedef ofxFlob Flob;
class ofxFlob {
public:
	ImageBlobs		imageblobs;
	ofImage			videoimggray;
	ofImage			*videotex;
	ofImage			videotexmotion;
	ofImage			videotexbin;
	ofImage			videoteximgmotion;	
	ofImage			backgroundPixels;
//	ofFloatImage
		
	int							numPixels;	
	int							videoresw, videoresh;
	float						worldwidth, worldheight;
	int							presence;
	float						presencef;
	int							videotexmode, pvideotexmode;
	bool						videotexchange;
	
	int							videothresh, videofade;
	float						videothreshf, videofadef;
	bool						mirrorX, mirrorY;
	int							blur;
	bool						trackfeatures[5];
	
	bool floatmode;
	float floatsmooth;//=0.555f;
	vector<float> backgroundLuma;
	vector<float> currentLuma;
	
	
	enum { STATIC_DIFFERENCE = 0, CONTINUOUS_DIFFERENCE, CONTINUOUS_EASE_DIFFERENCE }; //om
	enum { RED = 0, GREEN, BLUE, LUMA601, LUMA709, LUMAUSER } ;//colormode
	enum { ABSDIF = 0, LESSER, GREATER };//thresholdmode
	
	int							om;
	int							colormode, thresholdmode;
	
	float						lumausercoefs[3];
	
	float						continuous_ease;
	
	int							trackedBlobLifeTime,
								trackedBlobMaxDistSquared;
	
	string						version;
	
	
	bool					bdoclear;
	
	
	//// 
	
	~ofxFlob();
	ofxFlob ();

	void setup(int srcW, int srcH, float dstW, float dstH);
	
	/// 
	
	ofImage & binarize (unsigned char * pix, int width, int height);
	
	vector<ABlob*>*  calc( ofImage & bin ); 
	
	float getPresencef(){
		return presencef;
	}
	
	
	void setBackground (ofImage& imggray);
	void easeBackground (ofImage& imggray);
	void clearBackground(){
		bdoclear = true;
	}

	bool getAnyFeatureActive() {
		return (trackfeatures[0] || trackfeatures[1]
				|| trackfeatures[2] || trackfeatures[3] || trackfeatures[4]);	
	}
	
	void draw(){
		videotex->draw(0,0,ofGetWidth(), ofGetHeight());	
	}

	
	
	ofxFlob* setThresh(const float t){
		videothreshf = t;
		videothresh = (int)t;
		return this;
	}
	ofxFlob* setFade(const float f){
		videofadef = f;
		videofade = (int)f;
		return this;
	}
	ofxFlob* setMirror(const bool mx, const bool my){
		mirrorX = mx; mirrorY = my;
		return this;
	}
	ofxFlob* setFloatmode(const bool fm){
		floatmode = fm;
		return this;
	}
	ofxFlob* setFloatsmooth(const float s){
		floatsmooth = s;
		return this;
	}
	ofxFlob* setOm(const int o){
		if(o>1) om = CONTINUOUS_EASE_DIFFERENCE;
		else if(o>0) om = CONTINUOUS_DIFFERENCE;
		else om = STATIC_DIFFERENCE;		
		return this;
	}
	ofxFlob* setColormode(const int cm){
		colormode = cm;
		return this;
	}
	ofxFlob* setThresholdmode(const int tm){
		thresholdmode = tm;
		return this;
	}
	ofxFlob* setLumaUserCoefs(const float *coefs){
		for(int i=0; i<3; i++) lumausercoefs[i] = coefs[i];
		return this;
	}
	ofxFlob* setMinNumPixels(const float min){
		imageblobs.ninpix = min;
		return this;
	}
	ofxFlob* setMaxNumPixels(const float max){
		imageblobs.maxpix = max;
		return this;
	}
	ofxFlob* setContinuousEase(const float e){
		continuous_ease = e;
		return this;
	}

	
	const float getThresh()		{ return videothreshf ; }
	const float getFade()		{ return videofadef ; }
	const bool getMirrorX()		{ return mirrorX; }
	const bool getMirrorY()		{ return mirrorY; }
	const bool getFloatmode()	{ return floatmode; }
	const bool getFloatsmooth()	{ return floatsmooth; }
	const int getOm()			{ return om; }
	const int getColormode()	{ return colormode; }
	const int getThresholdmode(){ return thresholdmode; }
	const float * getLumaUserCoefs() { return lumausercoefs; }
	const int getMinNumPixels()	{ return imageblobs.ninpix; }
	const int getMaxNumPixels()	{ return imageblobs.maxpix; }
	const float getContinuousEase() { return continuous_ease; }

	
	
	
};