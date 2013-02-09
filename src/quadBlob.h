/*
 *  quadBlob.h
 *  example-helloflob
 *
 *  Created by andré sier on 20130208.
 *  Copyright 2013 s373.net/x. All rights reserved.
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

/**
 * use a blob like an ar quad
 *
 */
#pragma once

#include "baseBlob.h" 

class quadBlob : public baseBlob {
public:
	 float cx, cy;
	 float quad[8];// 4*2
	
	quadBlob() {
	}
	
	quadBlob(const quadBlob * b) {
		quad[0] = b->quad[0];
		quad[1] = b->quad[1];
		quad[2] = b->quad[2];
		quad[3] = b->quad[3];
		quad[4] = b->quad[4];
		quad[5] = b->quad[5];
		quad[6] = b->quad[6];
		quad[7] = b->quad[7];
		cx = b->cx;
		cy = b->cy;
		id = b->id;
		pixelcount = b->pixelcount;
		boxminx = b->boxminx;
		boxminy = b->boxminy;
		boxmaxx = b->boxmaxx;
		boxmaxy = b->boxmaxy;
		boxcenterx = b->boxcenterx;
		boxcentery = b->boxcentery;
	}
	
};
