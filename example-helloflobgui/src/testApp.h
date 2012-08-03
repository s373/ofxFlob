#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFlob.h"
#include "OPanel.h"

class testApp : public ofBaseApp{

	public:

		testApp();
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);

	
	
	ofVideoGrabber	vidGrabber;
	ofxFlob flob;
	OPanel panel;
	
	vector<ABlob*> *blobs;
	
};

#endif
