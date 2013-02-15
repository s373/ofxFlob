/*
 *  TBlob.cpp
 *  example-helloflob
 *
 *  Created by andré sier on 20130215.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */


#include "TBlob.h"
#include "ofxFlob.h"
#include "ImageBlobs.h"



TBlob::TBlob (const ABlob * b) {
	
	TBlob::TBlob();	
	
	id = ImageBlobs::idnumbers++;
	pixelcount = b->pixelcount;	
	boxminx = b->boxminx;
	boxminy = b->boxminy;
	boxmaxx = b->boxmaxx;
	boxmaxy = b->boxmaxy;
	boxcenterx = b->boxcenterx;
	boxcentery = b->boxcentery;
	boxdimx = b->boxdimx;
	boxdimy = b->boxdimy;
	pboxcenterx = boxcenterx;
	pboxcentery = boxcentery;
	//		ivelx = b->ivelx;
	//		ively = b->ively;
	
	cx = b->cx;
	cy = b->cy;
	bx = b->bx;
	by = b->by;
	dimx = b->dimx;
	dimy = b->dimy;
	
	armleftx = b->armleftx;
	armlefty = b->armlefty;
	armrightx = b->armrightx;
	armrighty = b->armrighty;
	headx = b->headx;
	heady = b->heady;
	bottomx = b->bottomx;
	bottomy = b->bottomy;
	footleftx = b->footleftx;
	footlefty = b->footlefty;
	footrightx = b->footrightx;
	footrighty = b->footrighty;
	
	
	// TBlob specific
	
	newblob=true;
	birthtime=ofGetSystemTime();//System.currentTimeMillis();
	lifetime = ofxFlob::TBlobLifeTime;
	presencetime = 1;
	linked=false;
	
	pcx = cx;
	pcy = cy;
	prevelx=prevely=velx=vely=0.0f;
	maxdist2 = ofxFlob::TBlobMaxDistSquared;//2555f;//1000;//~31px//100;
	calcrad();
	
}
