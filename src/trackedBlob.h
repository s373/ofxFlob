/*
 *  trackedBlob.h
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 s373.net/x. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h" 
#include "ABlob.h"


class trackedBlob : public ABlob {
public:
	 bool newblob;
	 long birthtime;
	 long lifetime;	//=ofxFlob.trackedBlobLifeTime;//5;//60;//5*60;//300frames
	 int presencetime;
	
	 bool linked;	//=false;
	
	 int pposx,pposy;
	 float pcx,pcy;
	
	 float velx,vely;
	 float prevelx,prevely;
	 float maxdist2;//=2555f;//1000;//~31px//100;
	 float rad,rad2;
	
	
	// constructores
	
	~trackedBlob(){}
	trackedBlob();
	
	trackedBlob (trackedBlob & b) {
	
		newblob = b.newblob;
		birthtime = b.birthtime;
		presencetime = b.presencetime;
		
		pposx= b.pposx; pposy=b.pposy;
		pcx = b.pcx; pcy = b.pcy;
		
		velx = b.velx; vely = b.vely;
		prevelx = b.prevelx; prevely = b.prevely;
		maxdist2=b.maxdist2;
		rad=b.rad;
		rad2=b.rad2;
		linked=b.linked;
		
		armleftx = b.armleftx;
		armlefty = b.armlefty;
		armrightx = b.armrightx;
		armrighty = b.armrighty;
		headx = b.headx;
		heady = b.heady;
		bottomx = b.bottomx;
		bottomy = b.bottomy;
		footleftx = b.footleftx;
		footlefty = b.footlefty;
		footrightx = b.footrightx;
		footrighty = b.footrighty;
	
	}
	
	
	trackedBlob ( ABlob &a , trackedBlob & b) {
		//super(a);
		id = b.id;
		pixelcount = b.pixelcount;
		boxminx = b.boxminx;    boxminy = b.boxminy;    boxmaxx = b.boxmaxx;    boxmaxy = b.boxmaxy;
		boxcenterx = b.boxcenterx;    boxcentery = b.boxcentery;
		boxdimx = b.boxdimx;    boxdimy = b.boxdimy;
		pboxcenterx = b.pboxcenterx;    pboxcentery = b.pboxcentery;
		cx = b.cx; cy = b.cy;
		//   pcx = b.pcx; pcy = b.pcy;
		dimx = b.dimx; dimy = b.dimy;
		
		//this(b);
		
		newblob = b.newblob;
		birthtime = b.birthtime;
		presencetime = b.presencetime;
		
		pposx= b.pposx; pposy=b.pposy;
		pcx = b.pcx; pcy = b.pcy;
		
		velx = b.velx; vely = b.vely;
		prevelx = b.prevelx; prevely = b.prevely;
		maxdist2=b.maxdist2;
		rad=b.rad;
		rad2=b.rad2;
		linked=b.linked;
		
		
		armleftx = a.armleftx;
		armlefty = a.armlefty;
		armrightx = a.armrightx;
		armrighty = a.armrighty;
		headx = a.headx;
		heady = a.heady;
		bottomx = a.bottomx;
		bottomy = a.bottomy;
		footleftx = a.footleftx;
		footlefty = a.footlefty;
		footrightx = a.footrightx;
		footrighty = a.footrighty;
		
		
	}
	
	
	trackedBlob ( ABlob &b) {
		//super(b);
		//ABlob::ABlob(b);		
		id = b.id;
		pixelcount = b.pixelcount;
		boxminx = b.boxminx;    boxminy = b.boxminy;    boxmaxx = b.boxmaxx;    boxmaxy = b.boxmaxy;
		boxcenterx = b.boxcenterx;    boxcentery = b.boxcentery;
		boxdimx = b.boxdimx;    boxdimy = b.boxdimy;
		pboxcenterx = b.pboxcenterx;    pboxcentery = b.pboxcentery;
		cx = b.cx; cy = b.cy;
		
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
		
		calcrad();
		
		
	}
	
	
	
	
	
	
	
	void calcrad(){
		rad = (boxdimx<boxdimy)?boxdimx/2.0f:boxdimy/2.0f;
		rad2 = rad*rad;
	}

	
	
};