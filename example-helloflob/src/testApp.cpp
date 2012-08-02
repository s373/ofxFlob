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
	flob .setup(128,128, ofGetWidth(), ofGetHeight());

	cout << "ofxflob "  << flob.worldwidth << " : " << flob.worldheight << "\n ";
	cout << flob.videoresw << " : " << flob.videoresh << "\n ";
	

	
	ofPoint loc(20, 20);
	ofPoint dim(220, 25);
	panel.setup("ofxflob",loc, dim);		
		
	panel.addSlider("flobdim", 128, 10, 512)->setClamp(true);
	panel.addSlider("thresh", 55, 0, 255)->setClamp(true);
	panel.addSlider("fade", 50, 0, 255)->setClamp(true);
	panel.addSlider("presence", 0, 0, 1)->setClamp(true);
	panel.addSlider("fps", 0, 0, 1000)->setClamp(true);
	panel.addSlider("blobs", 0, 0, 20);
	panel.addSlider("bglowpass", 0.0021, 0.0000000001, 0.1)->setClamp(true);

	panel.dim.set(20,20);
	panel.addRadioButton("om", 2);
	panel.addButton("mirrorX", 1);
	panel.addButton("mirrorY", 0);
	

}

//--------------------------------------------------------------
void testApp::update(){
	panel.update();
	
	if(panel.isValsNew("flobdim")){
		int dim = (int)panel.get("flobdim");
		flob .setup(dim,dim, ofGetWidth(), ofGetHeight());
	}
	
	
	flob.videothresh = panel.get("thresh");
	flob.videofade = panel.get("fade");
	flob.om = panel.get("om");
	flob.mirrorX = panel.get("mirrorX");
	flob.mirrorY = panel.get("mirrorY");
	flob.bglowpass = panel.get("bglowpass");
	
	vidGrabber.grabFrame();
	if(vidGrabber.isFrameNew()){
		blobs = flob.calc(	
						  flob.binarize(	vidGrabber.getPixels(), 
										(int)vidGrabber.getWidth(), 
										(int)vidGrabber.getHeight() ) );		
	}
	
	panel.set("presence", flob.getPresencef() );
	panel.set("fps", ofGetFrameRate() );
	if(blobs!=NULL)
		panel.set("blobs", blobs->size());
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
			ofRect(ab.cx, ab.cy, 15,15);
			
			ofLine(ab.cx, ab.cy, ab.armleftx, ab.armlefty);
			ofLine(ab.cx, ab.cy, ab.armrightx, ab.armrighty);
			ofLine(ab.cx, ab.cy, ab.headx, ab.heady);
			ofLine(ab.cx, ab.cy, ab.footleftx, ab.footlefty);
			ofLine(ab.cx, ab.cy, ab.footrightx, ab.footrighty);
			ofLine(ab.cx, ab.cy, ab.bottomx, ab.bottomy);
			
				
		}
	}
	
	panel.draw();
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
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
void testApp::resized(int w, int h){

}

