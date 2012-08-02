/*
 *  CV.h
 *  balls-opencvExample
 *
 *  Created by andre sier on 24/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


/*
 *  computervision.h
 *  openFrameworks
 *
 *  Created by andre sier on 10/1/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


//inspirada na do theo

#pragma once


#include "ofMain.h"
#include "ofxOpenCv.h"

#define MAX_BLOBS	20
//100

class ComputerVision{
public:
	//// imagens que preciso
	ofxCvColorImage			imgcolorfull;
	ofxCvColorImage			imgcolor;	
	ofxCvGrayscaleImage		imggray;
	ofxCvGrayscaleImage		imggraymotion;
	ofxCvGrayscaleImage		imgthresh;
	ofxCvGrayscaleImage		imgprev;
	ofxCvContourFinder		contourFinder;
	ofVideoGrabber			*camera; // now as a pointer.., preciso de a iniciar
	ofVideoPlayer			video;	
	// outras vars
	float threshAmnt, fadeAmnt;
	bool bPauseVideo;
	bool bUseCamera;
	int frameCount;	
	int DrawOm;	
	int w,h;	
	int om; // modo de operação // 1 remove bg //0 diferença contínua
	bool bLearnBackground;
	bool mirrorX;
	
	//roi stuff
	bool bRoi;
	int roix,roiy, roiw, roih;
	int camwidth, camheight, cvwidth, cvheight;
	
	//construtores
	ComputerVision();
	~ComputerVision();
	//funções
	void loadVideo(string path);
	void setupCamera(int which, int width, int height);
	void setupCamera(int which, int _camwidth, int _camheight, int _cvwidth, int _cvheight );
	void init();
	
	void setRoiWH(int _w, int _h);
	void setRoiXY(int _w, int _h);
	void setMirrorX(bool m);
	
	void update();
	//	void draw(float x, float y);
	void draw(float x, float y ,float z, float al);
	void draw(int width, int height, float al);
	void refreshvdig();	
	void setFade(float amt);
	void setThresh(float thresh);
	void setDrawOm(int om);
	void settings();
	
	//new
	
	
	void crop(ofxCvImage * orgImage, ofImage * targetImage,int x, int y,int width, int height, bool color)
	{
		unsigned char * pixels = orgImage->getPixels();
		int totalWidth = orgImage->getWidth();
		int subRegionLength = width * height;
		if(color) subRegionLength*=3; // rgb
		unsigned char subRegion[subRegionLength];
		
		int result_pix = 0;
		for (int i = y; i < y+height; i++)
		{
			for(int j = x; j < x+width; j++)
			{
				int base = (i * totalWidth) + j;
				if(color) base *= 3; // rgb
				
				subRegion[result_pix] = pixels[base];
				result_pix++;
				
				if(color)
				{
					subRegion[result_pix] = pixels[base+1];
					result_pix++;
					subRegion[result_pix] = pixels[base+2];
					result_pix++;
				}
			}
		}
		if(color)
			targetImage->setFromPixels(subRegion, width, height, OF_IMAGE_COLOR, true);
		else
			targetImage->setFromPixels(subRegion, width, height, OF_IMAGE_GRAYSCALE, false);
		
	}	 
	
	
};