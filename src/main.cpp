#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

    ofAppGLFWWindow window;
    
	ofSetupOpenGL(1320,660, OF_WINDOW);			// <-------- setup the GL context

    window.setWindowTitle("MoviePrint");
//    window.hideCursor();
//    window.setWindowPosition(1200, 300);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
