/*
 *  ABlob.h
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once

#include "ofMain.h" 


class ABlob {
public:
	
	 int id;
	 int pixelcount;
	 int boxminx,boxminy,boxmaxx,boxmaxy;
	 int boxcenterx,boxcentery;
	 int boxdimx,boxdimy;
	 int pboxcenterx,pboxcentery;
	//normalized values
	 float cx, cy;
	float bx, by; //beginx beginy
	//  float pcx,pcy;
	 float dimx, dimy;
	
	///features:
	 float armleftx,armlefty,
			armrightx,armrighty,
			headx,heady,
			bottomx,bottomy,
			footleftx,footlefty,
			footrightx,footrighty;
	
	ABlob(){;} 
	~ABlob(){;}
	ABlob(const ABlob & b){
		id = b.id;
		pixelcount = b.pixelcount;
		boxminx = b.boxminx;    boxminy = b.boxminy;    boxmaxx = b.boxmaxx;    boxmaxy = b.boxmaxy;
		boxcenterx = b.boxcenterx;    boxcentery = b.boxcentery;
		boxdimx = b.boxdimx;    boxdimy = b.boxdimy;
		pboxcenterx = b.pboxcenterx;    pboxcentery = b.pboxcentery;
		cx = b.cx; cy = b.cy;
		bx = b.bx; by = b.by;

		dimx = b.dimx; dimy = b.dimy;
		
		armleftx = b.armleftx;
		armlefty = b.armlefty;
		armrightx = b.armrightx;
		armrighty = b.armrighty;
		headx = b.headx;
		heady = b.heady;
		bottomx = b.bottomx;
		bottomy = b.bottomy;
		heady = b.heady;
		footleftx = b.footleftx;
		footlefty = b.footlefty;
		footrightx = b.footrightx;
		footrighty = b.footrighty;						
		
	}
	
};