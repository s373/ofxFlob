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
#include "TBlob.h"
#include "quadBlob.h"
#include "pt2.h"

class ofxFlob; // fwddecl

class ImageBlobs {
public:		
	
	static unsigned int				idnumbers;
	unsigned int					numblobs, prevnumblobs;
	unsigned int					trackednumblobs, prevtrackednumblobs;
	unsigned int					lifetime;
	unsigned int					ninpix;
	unsigned int					maxpix;
	vector<bool>					imagemap;
	bool							imagemaplit;
	unsigned int					w,h;
	float							wr,hr, w2, h2;
	float							wcoordsx, wcoordsy;
	unsigned int					worldw, worldh;
	unsigned int					numpix;
	float							lp1,lp2;
	float							trackingmindist;
	
	
	vector				<ABlob *>				theblobs;
	vector				<ABlob *>				prevblobs;
	vector				<TBlob *>				TBlobs;
	vector				<TBlob *>				prevTBlobs;
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

	vector<TBlob*>* calcsimple();
	vector<TBlob*>* tracksimple();
	
	void dotracking();
	void doremoveprevblobs();
	void sortTBlobs();
	bool matchblobprevTBlobs(ABlob * ab) ;
	void compareblobsprevblobs();
	bool isCollide(int x, int y);
	
	vector<float> postcollideTBlobs(float x, float y, float rad);
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