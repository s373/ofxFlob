/*
 *  baseBlob.h
 *  example-helloflob
 *
 *  Created by andré sier on 20130208.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
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

class baseBlob {
public:
	baseBlob(){
		id=0;
		pixelcount=0;
		boxminx= boxminy= boxmaxx= boxmaxy=0;
		boxcenterx=boxcentery=0;
	}
	virtual ~baseBlob(){}; 
	//polymorphy should include a base virtual destructor
	
	int id;
	int pixelcount;
	int boxminx, boxminy, boxmaxx, boxmaxy;
	int boxcenterx, boxcentery;
	
	
};