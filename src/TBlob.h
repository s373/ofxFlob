/*
 *  trackedBlob.h
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
 */
#pragma once

#include "ofMain.h" 
#include "ABlob.h"

class ImageBlobs; class ofxFlob;
class TBlob : public ABlob {
public:
	 bool newblob;
	 unsigned long birthtime;
	 unsigned long lifetime;	
	 unsigned int presencetime;
	
	 bool linked;
	
	 unsigned int pboxcenterx,pboxcentery;
	 float pcx,pcy;
	
	 float velx,vely;
	 float prevelx,prevely;
	 float maxdist2;
	 float rad,rad2;
	
	
	// constructores
	
	~TBlob(){}
	TBlob(){
		ABlob::ABlob();
		newblob=linked=false;
		birthtime=lifetime=presencetime=pcx=pcy=velx=vely=prevelx=prevely=rad=rad2=0;
		maxdist2=2555.f;
	}
	
	TBlob (const ABlob * b);
	
	void calcrad(){
		rad = (boxdimx<boxdimy)?boxdimx/2.f:boxdimy/2.f;
		rad2 = rad*rad;
	}
	
	
};