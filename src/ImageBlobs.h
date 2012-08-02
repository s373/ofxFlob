/*
 *  ImageBlobs.h
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 s373.net/x. All rights reserved.
 *
 */

#pragma once


#include "ofMain.h" 
#include "ABlob.h"
#include "trackedBlob.h"
#include "pt2.h"

class ofxFlob; // fwddecl

class ImageBlobs {
public:		
	int					idnumbers;
	int					numblobs, prevnumblobs;
	int					trackednumblobs, prevtrackednumblobs;
	int					lifetime;
	int					ninpix;
	int					maxpix;
	vector<bool>		imagemap;
	bool				imagemaplit;
	int					w,h;
	float				wr,hr, w2, h2;
	float				wcoordsx, wcoordsy;
	int					worldw, worldh;
	int					numpix;
	float				lowpass;
	float				trackingmindist;
	
	vector				<ABlob *>				theblobs;
//	vector				<ABlob *>				prevblobs;
//	vector				<trackedBlob>		trackedblobs;
//	vector				<trackedBlob>		prevtrackedblobs;
//	vector				<trackedBlob>		tbsimplelist;
	vector				<pt2 *>				thecoords;

	ofxFlob				*tflob;
	
	
	ImageBlobs(){}
	~ImageBlobs(){}
	
	void setup( ofxFlob *flob );
	
	void calcdims(int w, int h, int ww, int wh);	
	void calc(ofImage & image);
	void copy_blobs_to_previousblobs();
	
	ABlob *	  calc_feature_arms(ABlob * b);
	ABlob *	  calc_feature_head(ABlob * b);
	ABlob *	  calc_feature_bottom(ABlob * b);
	ABlob *	  calc_feature_feet(ABlob * b);
	bool testimagemap(int x, int y) {
		return imagemap[y * w + x];
		
	}
	
};