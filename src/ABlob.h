/*
 *  ABlob.h
 *  ofxflob test
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

#include "baseBlob.h" 


class ABlob : public baseBlob {
public:
	
	 int boxdimx,boxdimy;
	 int pboxcenterx,pboxcentery;
	 // normalized to desired output coords vals
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