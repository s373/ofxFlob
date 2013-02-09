#include "testApp.h"

//--------------------------------------------------------------
testApp::testApp(){

}

//--------------------------------------------------------------
void testApp::setup(){

	blobs = NULL;
	vidGrabber.setVerbose(true);
	vidGrabber.listDevices();
	vidGrabber.initGrabber(640, 480);
	
//	flob .setup((int)vidGrabber.getWidth(), (int)vidGrabber.getHeight(), ofGetWidth(), ofGetHeight());
	flob .setup(327,245, (float)ofGetWidth(), (float)ofGetHeight());

	flob.setOm(Flob::CONTINUOUS_DIFFERENCE)
	->setColormode(Flob::LUMA601)
	->setFade(25)->setThresh(12)
	->setThresholdmode(Flob::ABSDIF);
	
	cout << "ofxflob "  << flob.worldwidth << " : " << flob.worldheight << "\n ";
	cout << flob.videoresw << " : " << flob.videoresh << "\n ";
	

}

//--------------------------------------------------------------
void testApp::update(){
			
	vidGrabber.grabFrame();
	if(vidGrabber.isFrameNew()){
		blobs = flob.calc(  flob.binarize(	vidGrabber.getPixels(), 
										(int)vidGrabber.getWidth(), 
										(int)vidGrabber.getHeight() ) );		
	}
	
}


//--------------------------------------------------------------
void testApp::draw(){
	
	ofEnableAlphaBlending();
	ofBackground(0);
	
	ofSetColor(255,200);
	flob.draw();
	
	int s = 128;
//	vidGrabber.draw( ofGetWidth()-s, 0, s, s);
	flob.videoimggray.draw(ofGetWidth()-s, 0, s, s);
	
	flob.videotexbin.draw(ofGetWidth()-s, s, s, s);
	flob.videotexmotion.draw(ofGetWidth()-s, s+s, s, s);
	
	if(blobs!=NULL){
		for(int i=0; i<blobs->size();i++){
			ABlob & ab  = *(blobs->at(i));
			ofSetColor(0,0,255,100);
			ofRect(ab.bx, ab.by, ab.dimx, ab.dimy);
			ofSetColor(0,255,0,200);
			ofRect(ab.cx, ab.cy, 10,10);
							
		}
	}
	
	string info = "ofxFlob info:\n";
	info += "srcdim: " + ofToString(flob.videoresw) +" "+ofToString(flob.videoresh)+" dstdim: "+ ofToString(flob.worldwidth) +" "+ofToString(flob.worldheight)+ "\n";
	info += "<t/T> thresh: " + ofToString(flob.getThresh()) + "\n";
	info += "<f/F> fade: " + ofToString(flob.getFade()) + "\n";
	info += "<o> om: " + ofToString(flob.getOm()) + "\n";
	info += "<c> colormode: " + ofToString(flob.getColormode()) + "\n";
	info += "<b> thresholdmode: " + ofToString(flob.getThresholdmode()) + "\n";
	info += "<x> mirrorx: " + ofToString(flob.mirrorX) + "\n";
	info += "<y> mirrory: " + ofToString(flob.mirrorY) + "\n<space> to clearbg in om 0\nfps: "+ofToString(ofGetFrameRate());
	
	ofSetColor(255);
	ofDrawBitmapString(info, 10,20);
		
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	
	if(key=='t'){
		flob.setThresh(flob.getThresh()-2);
	}
	if(key=='T'){
		flob.setThresh(flob.getThresh()+2);
	}
	if(key=='f'){
		flob.setFade(flob.getFade()-2);
	}
	if(key=='F'){
		flob.setFade(flob.getFade()+2);
	}
	if(key=='o'){
		flob.om = (flob.om + 1)%3;	
	}
	if(key=='c'){		
		flob.colormode = (flob.colormode + 1)%5;	
	}
	if(key=='b'){		
		flob.thresholdmode = (flob.thresholdmode + 1)%3;	
	}
	if(key=='x'){
		flob.mirrorX ^= true;	
	}
	if(key=='y'){
		flob.mirrorY ^= true;	
	}
	if(key==' '){
		flob.clearBackground();
	}
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	if(ofGetFrameNum()>10)
	flob.setup((int)ofRandom(10,500),(int)ofRandom(10,500), (float)ofGetWidth(), (float)ofGetHeight());
}

