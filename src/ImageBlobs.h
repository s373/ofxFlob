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
#include "baseBlob.h"
#include "ABlob.h"
#include "trackedBlob.h"
#include "quadBlob.h"
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
	float				lp1,lp2;
	float				trackingmindist;
	
	vector				<ABlob *>				theblobs;
	vector				<ABlob *>				prevblobs;
	vector				<trackedBlob *>			trackedblobs;
	vector				<trackedBlob *>			prevtrackedblobs;
	vector				<trackedBlob *>			tbsimplelist;
	vector				<quadBlob *>			quadbloblist;
	vector				<pt2 *>					thecoords;

	ofxFlob				*tflob;
	
	
	ImageBlobs();
	~ImageBlobs(){}
	
	void setup( ofxFlob *flob );
	void clear();
	
	void calcdims(const int w, const int h, const float ww, const float wh);
	void calc(ofImage & image);
	void calcQuad(ofImage & image);
	void copy_blobs_to_previousblobs();

	vector<trackedBlob*>* calcsimple();
	vector<trackedBlob*>* tracksimple();
	
	void addTrackedBlob(trackedBlob * b);
	void addNewBlob(trackedBlob * b);
	void dotracking();
	void doremoveprevblobs();
	void doaddnewtrackedblobs();
	void sorttrackedblobs();
	bool matchblobprevtrackedblobs(ABlob & ab) ;
	void compareblobsprevblobs();
	void add_tracker_match(ABlob & b, trackedBlob &prev);
	bool isCollide(int x, int y);
	
	vector<float> postcollidetrackedblobs(float x, float y, float rad);
	vector<float> postcollideblobs(float x, float y, float rad);
	
	
	ABlob *	  calc_feature_arms(ABlob * b);
	ABlob *	  calc_feature_head(ABlob * b);
	ABlob *	  calc_feature_bottom(ABlob * b);
	ABlob *	  calc_feature_feet(ABlob * b);
	quadBlob *	  calc_quad(quadBlob * b);
	inline bool testimagemap(const int x, const int y) {
		return imagemap[y * w + x];
		
	}
	
};