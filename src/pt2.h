/*
 *  pt2.h
 *  ofxflob test
 *
 *  Created by andre sier on 29/3/10.
 *  Copyright 2010 s373.net/x. All rights reserved.
 *
 */


#pragma once

#include "ofMain.h" 


class pt2 {
public:
	int x,y;
	
	pt2(){ x=0; y=0; }
	pt2(int _x, int _y) { x = _x; y = _y; }
	void operator=(const pt2 &o){ x = o.x; y = o.y; }
};