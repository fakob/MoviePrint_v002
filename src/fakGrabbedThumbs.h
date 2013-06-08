//
//  fakGrabbedThumbs.h
//  fakBreakupVideo
//
//  Created by Jakob on 3/12/13.
//
//

#ifndef fakBreakupVideo_fakGrabbedThumbs_h
#define fakBreakupVideo_fakGrabbedThumbs_h

#include "ofMain.h"
#include "fakGrabbedStill.h"

#include <iostream>

#define REsizeFactor 10.0

class fakGrabbedThumbs : public ofThread {
    
public:
    
    fakGrabbedThumbs(){
        
        gmTotalFrames = 0;
        gmSetupFinished = FALSE;
        gmCalcResizeSwitch = FALSE;
        vfFramesToTimeSwitch = TRUE;
        gmScrubMovie = FALSE;
        gmNumberOfStills = 1;
        gmNumberOfThumbs = 1;
    }
    
    // functions
    
    void setNumberOfStills(int _numberOfStills){
        gmNumberOfStills = _numberOfStills;
    }
    
    void setNumberOfScrubs(int _numberOfScrubs){
        gmNumberOfThumbs = _numberOfScrubs;
    }
    
    void setup(string vfMovieName, int _numberOfStills){
        
        setNumberOfStills(_numberOfStills);
        gmTypo8.loadFont("verdana.ttf", 8, true, true);
        gmTypo8.setLineHeight(10.0f);
        
        loadNewMovie(vfMovieName, gmNumberOfStills);
        
        gmSetupFinished = TRUE;
        
    }
    
    void createGrid(int _numberOfStills){
        if (isMovieLoaded) {
            stop(TRUE);
            while (isThreadRunning()) {
                cout << "createGrid is waiting for thread to stop" << endl;
            }
            // unregister All Mouse Events of the Stills (old gmNumberOfStills)
            for(int i=0; i<gmNumberOfStills; i++)
            {
                grabbedStill[i].unregisterMouseEvents();
            }
            
            setNumberOfStills(_numberOfStills);
            grabbedStill.clear();
            grabbedStill.resize(_numberOfStills);
            
            for(int i=0; i<gmNumberOfStills; i++)
            {
                //                cout << "allocations are about to begin" << endl;
                grabbedStill[i].registerMouseEvents();
                ofAddListener(grabbedStill[i].gsClickedInside, this, &fakGrabbedThumbs::scrubMovie);
                grabbedStill[i].gsID = i;
                grabbedStill[i].gsX = 0;
                grabbedStill[i].gsY = 0;
                grabbedStill[i].gsDrawWidth = 160;
                grabbedStill[i].gsDrawHeight = 90;
                if (gmCalcResizeSwitch) {
                    grabbedStill[i].gsResizeFactor = REsizeFactor;
                } else {
                    grabbedStill[i].gsResizeFactor = 1.0;
                }
                grabbedStill[i].gsWidth = gmMovie.getWidth()/grabbedStill[i].gsResizeFactor;
                grabbedStill[i].gsHeight = gmMovie.getHeight()/grabbedStill[i].gsResizeFactor;
                grabbedStill[i].gsImage.setUseTexture(FALSE);
                grabbedStill[i].gsImage.allocate(gmMovie.getWidth(), gmMovie.getHeight(), OF_IMAGE_COLOR);
                grabbedStill[i].gsTexture.allocate(grabbedStill[i].gsWidth, grabbedStill[i].gsHeight,GL_RGB);
                grabbedStill[i].gsToBeUpdated = TRUE;
            }
            cout << "Allocations worked" << endl;
        }
    }
        
    bool loadNewMovie(string vfMovieName, int _numberOfStills){
        
        setNumberOfStills(_numberOfStills);
        //        gmMovie.loadMovie(vfMovieName);
        isMovieLoaded = FALSE;
        gmMovie.loadMovie(vfMovieName);
        if (gmMovie.isLoaded()) {
            isMovieLoaded = TRUE;
            gmTotalFrames = gmMovie.getTotalNumFrames();
            gmFrameRate = ceil(gmTotalFrames/gmMovie.getDuration());
            gmImageRatio = gmMovie.getWidth()/gmMovie.getHeight();
            gmPixelRatio = gmMovie.getPixelFormat();
            cout << "ImageRatio:" + ofToString(gmImageRatio) + " PixelRatio:" + ofToString(gmPixelRatio)  + " Framerate:" + ofToString(gmFrameRate) << endl;
            
            gmMovieScrub.loadMovie(vfMovieName);
            gmMovieThumbs.loadMovie(vfMovieName);
            
        } else {
            gmTotalFrames = 49;
        }
        createGrid(gmNumberOfStills);
        
        return isMovieLoaded;
    }
    
    void scrubMovie(int & i){
        if (isMovieLoaded) {
            gmScrubID = i;
            gmScrubMovie = TRUE;
        }
    }
        
    int numberLoaded(){
        gmNumberLoadedCounter = 0;
        if (isMovieLoaded) {
            for(int i=0; i<gmNumberOfStills; i++)
            {
                if(!grabbedStill[i].gsToBeUpdated){
                    gmNumberLoadedCounter++;
                }
                
            }
        }
        return gmNumberLoadedCounter;
    }
    
    float percLoaded(){
        gmNumberLoadedCounter = 0;
        if (isMovieLoaded) {
            
            for(int i=0; i<gmNumberOfStills; i++)
            {
                if(!grabbedStill[i].gsToBeUpdated){
                    gmNumberLoadedCounter++;
                }
                
            }
        }
        return (float)gmNumberLoadedCounter/(gmNumberOfStills-1);
    }
    
    bool allGrabbed(){
        int allGrabbed = 0;
        if (isMovieLoaded) {
            
            for(int i=0; i<gmNumberOfStills; i++)
            {
                if(!grabbedStill[i].gsToBeGrabbed){
                    allGrabbed++;
                }
                
            }
        }
        if (allGrabbed == gmNumberOfStills){
            return TRUE;
        } else {
            return FALSE;
        }
        
    }
    
    void setAllToBeGrabbedAndToBeUpdated(){
        if (isMovieLoaded) {
            for (int i=0; i<gmNumberOfStills; i++) {
                grabbedStill[i].gsToBeUpdated = TRUE;
                grabbedStill[i].gsToBeGrabbed = TRUE;
                grabbedStill[i].gsManipulated = FALSE;
            }
        }
    }
    
    void update(){
        if (isMovieLoaded) {
            gmMovie.update();
        }
    }
    
    void grabToImage(int i, int f){
        
        if (isMovieLoaded) {
            
            if (f > gmTotalFrames) {
                f = gmTotalFrames;
            }
            if (f < 0) {
                f = 0;
            }
            gmMovie.setFrame(f);
            cout << ofToString(gmMovie.getCurrentFrame());
            grabbedStill[i].gsImage.setFromPixels(gmMovie.getPixelsRef());
            grabbedStill[i].gsToBeGrabbed = FALSE;
            cout << "grabToImage - i: " + ofToString(i) + " f: " + ofToString(f) << endl;
        }
    }
    
    void drawStill(int i, float x, float y){
        if (isMovieLoaded) {
            ofSetColor(255);
            
            grabbedStill[i].gsX = x;
            grabbedStill[i].gsY = y;
            
            if (grabbedStill[i].gsToBeUpdated) {
                
                ofSetColor(100);
                
                if (!grabbedStill[i].gsToBeGrabbed) {
                    grabbedStill[i].gsImage.resize(grabbedStill[i].gsWidth, grabbedStill[i].gsHeight);
                    grabbedStill[i].gsTexture.loadData(grabbedStill[i].gsImage);
                    grabbedStill[i].gsToBeUpdated = FALSE;
                }
            }
            
            grabbedStill[i].gsTexture.draw(x, y, grabbedStill[i].gsWidth, grabbedStill[i].gsHeight);
            
            ofSetColor(255);
        }
    }
    
    void drawStill(int i, float x, float y, float w, float h){
        
        if (isMovieLoaded) {
            
            ofSetColor(255);
            
            grabbedStill[i].gsX = x;
            grabbedStill[i].gsY = y;
            grabbedStill[i].gsDrawWidth = w;
            grabbedStill[i].gsDrawHeight = h;
            grabbedStill[i].gsResizeFactor = gmMovie.getWidth()/w;
            
            if (grabbedStill[i].gsToBeUpdated) {
                if (!grabbedStill[i].gsManipulated) {
                    ofSetColor(100);
                }
                if (!grabbedStill[i].gsToBeGrabbed) {
                    cout << "updateTexture: " + ofToString(i) << endl;
                    grabbedStill[i].gsTexture.loadData(grabbedStill[i].gsImage);
                    grabbedStill[i].gsToBeUpdated = FALSE;
                }
            }
            
            grabbedStill[i].gsTexture.draw(x, y, w, h);
            
            ofSetColor(255);
        }
    }
    
    void drawStillUI(int i, float x, float y, float w, float h, float a){
        if (isMovieLoaded) {
            
            string dummyString;
            
            if (vfFramesToTimeSwitch) {
                dummyString = framesToTime(grabbedStill[i].gsFrameNumber);
            } else {
                dummyString = "#" + ofToString(grabbedStill[i].gsFrameNumber);
            }
            
            ofRectangle rect = gmTypo8.getStringBoundingBox(dummyString, 0,0);
            ofEnableAlphaBlending();
            ofSetColor(0,0,0,200*a);
            if (grabbedStill[i].gsManipulated) {
                ofSetColor(255,0,0,200*a);
            }
            ofRect(x+1, y+1, rect.width+3, rect.height+3);
            ofDisableAlphaBlending();
            ofSetColor(255, 255 * a);
            
            if (grabbedStill[i].gsToBeUpdated) {
                ofSetColor(100, 255 * a);
                if (gmScrubMovie) {
                    ofSetColor(255, 255 * a);
                    dummyString += "-SCRUBBING";
                    cout << "scrubbing" << endl;
                }
            }
            
            if (!grabbedStill[i].gsToBeGrabbed) {
                gmTypo8.drawString(dummyString, x, y+10);
            }
            ofSetColor(255, 255 * a);
        }
    }
    
    void printImageToPNG(){
        if (isMovieLoaded) {
            int outputFactor = 2;
            int outputWidth = ofGetWidth() * outputFactor;
            int outputHeight = ofGetHeight() * outputFactor;
            gmFboToSave.allocate(outputWidth, outputHeight);
            gmPixToSave.allocate(outputWidth,outputHeight,OF_IMAGE_COLOR);
            gmFboToSave.begin();
            ofBackground(0, 0, 0);
            for(int i=0; i<gmNumberOfStills; i++)
            {
                if (gmCalcResizeSwitch) {
                    drawStill(i, 162*(i%7) * outputFactor, 92*(i/7) * outputFactor);
                } else {
                    drawStill(i, 162*(i%7) * outputFactor, 92*(i/7) * outputFactor, 160 * outputFactor, 90 * outputFactor);
                }
                drawStillUI(i, 162*(i%7) * outputFactor, 92*(i/7) * outputFactor, 160 * outputFactor, 90 * outputFactor, 1.0);
            }
            //        for(int i=0; i<gmNumberOfStills; i++)
            //        {
            //            if (gmCalcResizeSwitch) {
            //                drawStill(i, ((gridWidth+gridMargin)*(i%gridColumns))+(ofGetWidth()/2 - gridAreaWidth/2), ((gridHeight+gridMargin)*(i/gridColumns))+(ofGetHeight()/2 - gridAreaHeight/2));
            //            } else {
            //                drawStill(i, ((gridWidth+gridMargin)*(i%gridColumns))+(ofGetWidth()/2 - gridAreaWidth/2), ((gridHeight+gridMargin)*(i/gridColumns))+(ofGetHeight()/2 - gridAreaHeight/2), gridWidth, gridHeight);
            //            }
            //            drawStillUI(i, ((gridWidth+gridMargin)*(i%gridColumns))+(ofGetWidth()/2 - gridAreaWidth/2), ((gridHeight+gridMargin)*(i/gridColumns))+(ofGetHeight()/2 - gridAreaHeight/2), gridWidth, gridHeight);
            //        }
            
            gmFboToSave.end();
            gmFboToSave.readToPixels(gmPixToSave);
            ofSaveImage(gmPixToSave, "testimage.png", OF_IMAGE_QUALITY_BEST);
            cout << "Finished saving testimage.png" << endl;
        }
    }
    
    void updateAllFrameNumbers(int gridTimeArray[], int _numberOfStills){
        if (isMovieLoaded) {
            gmNumberOfStills = _numberOfStills;
            for (int i = 0; i<gmNumberOfStills; i++) {
                grabbedStill[i].gsFrameNumber = gridTimeArray[i];
                grabbedStill[i].gsToBeUpdated = TRUE;
                grabbedStill[i].gsToBeGrabbed = TRUE;
                grabbedStill[i].gsManipulated = FALSE;
            }
        }
    }
    
    string framesToTime(int _frames){
        
        int frames  =    _frames % gmFrameRate;
        int seconds =   (_frames / gmFrameRate) % 60;
        int minutes =  ((_frames / gmFrameRate) / 60) % 60;
        int hours   = (((_frames / gmFrameRate) / 60) / 60) % 24;
        return (ofToString(hours,2,'0') + ":" + ofToString(minutes,2,'0') + ":" + ofToString(seconds,2,'0') + ":" + ofToString(frames,2,'0'));
        
    }
    
    
    // Thumb Functions
    
    void setupThumbs(int _thumbSizeW, int _thumbSizeH, int _numberOfThumbs){
        stop(TRUE);
        while (isThreadRunning()) {
            cout << "createGrid is waiting for thread to stop" << endl;
        }
        setNumberOfScrubs(_numberOfThumbs);
        grabbedThumbs.clear();
        grabbedThumbs.resize(gmNumberOfThumbs);
        if (isMovieLoaded) {
            
            for(int i=0; i<gmNumberOfThumbs; i++)
            {
                grabbedThumbs[i].gsID = i;
                grabbedThumbs[i].gsDrawWidth = 160;
                grabbedThumbs[i].gsDrawHeight = 90;
                grabbedThumbs[i].gsWidth = _thumbSizeW;
                grabbedThumbs[i].gsHeight = _thumbSizeH;
                grabbedThumbs[i].gsImage.setUseTexture(FALSE);
                grabbedThumbs[i].gsImage.allocate(gmMovie.getWidth(), gmMovie.getHeight(), OF_IMAGE_COLOR);
                grabbedThumbs[i].gsTexture.allocate(grabbedStill[i].gsWidth, grabbedStill[i].gsHeight,GL_RGB);
                grabbedThumbs[i].gsToBeUpdated = TRUE;
            }
        }
        
        cout << "SetupScrub - Allocations worked" << endl;
    }
    
    bool allThumbsGrabbed(){
        int allThumbsGrabbed = 0;
        if (isMovieLoaded) {
            
            for(int i=0; i<gmNumberOfThumbs; i++)
            {
                if(!grabbedThumbs[i].gsToBeGrabbed){
                    allThumbsGrabbed++;
                }
                
            }
        }
        if (allThumbsGrabbed == gmNumberOfThumbs){
            return TRUE;
        } else {
            return FALSE;
        }
        
    }
    
    void setAllThumbsToBeGrabbedAndToBeUpdated(){
        for (int i=0; i<gmNumberOfThumbs; i++) {
            grabbedThumbs[i].gsToBeUpdated = TRUE;
            grabbedThumbs[i].gsToBeGrabbed = TRUE;
        }
    }
    
    void grabToImageThumb(int i, int f){
        if (isMovieLoaded) {
            
            if (f > gmTotalFrames) {
                f = gmTotalFrames;
            }
            if (f < 0) {
                f = 0;
            }
            
            gmMovieThumbs.setFrame(f+i+1);
            grabbedThumbs[i].gsImage.setFromPixels(gmMovieThumbs.getPixelsRef());
            grabbedThumbs[i].gsToBeGrabbed = FALSE;
            
        }
    }
    
    void drawThumbs(float x, float y, float w, float h){
        if (isMovieLoaded) {
            
            ofSetColor(255);
            
            for (int i = 0; i < gmNumberOfThumbs; i++) {
                grabbedThumbs[i].gsX = x;
                grabbedThumbs[i].gsY = y;
                grabbedThumbs[i].gsDrawWidth = w;
                grabbedThumbs[i].gsDrawHeight = h;
                
                if (grabbedThumbs[i].gsToBeUpdated) {
                    
                    //                    ofSetColor(100);
                    
                    if (!grabbedThumbs[i].gsToBeGrabbed) {
                        cout << "updateTexture: " + ofToString(i) << endl;
                        grabbedThumbs[i].gsTexture.loadData(grabbedThumbs[i].gsImage);
                        grabbedThumbs[i].gsToBeUpdated = FALSE;
                    }
                }
                grabbedThumbs[i].gsTexture.draw(i * grabbedThumbs[i].gsDrawWidth, grabbedThumbs[i].gsY, w, h);
            }
            
            ofSetColor(255);
        }
    }
    
    // Thread Functions
    
    void threadedFunction(){
        
        if (gmSetupFinished && isMovieLoaded) { // only start when setup is finished and movie is loaded
            cout << "threaded Function - Try to lock" << endl;
            lock();
            cout << "threaded Function - Locked" << endl;
            //            if (!gmScrubMovie) { // updating stills
            while (!allGrabbed()) {
                
                // FIX first still was always wrong - probably due to quicktime doing first setFrame wrong - now it is grabbed twice
                grabToImage(0, grabbedStill[0].gsFrameNumber);
                grabbedStill[0].gsToBeGrabbed = TRUE;
                
                for (int i = 0; i<gmNumberOfStills; i++) {
                    if (grabbedStill[i].gsToBeGrabbed) {
                        grabToImage(i, grabbedStill[i].gsFrameNumber);
                    }
                }
                cout << "hanging in allGrabbed while loop" << endl;
            }
            //            } else { // updating thumbs
            while (!allThumbsGrabbed()) {
                
                // FIX first still was always wrong - probably due to quicktime doing first setFrame wrong - now it is grabbed twice
                grabToImageThumb(0, grabbedThumbs[0].gsFrameNumber);
                grabbedThumbs[0].gsToBeGrabbed = TRUE;
                
                for (int i = 0; i<gmNumberOfThumbs; i++) {
                    if (grabbedThumbs[i].gsToBeGrabbed) {
                        grabToImageThumb(i, grabbedThumbs[i].gsFrameNumber);
                    }
                }
                cout << "hanging in allThumbsGrabbed while loop" << endl;
            }
            //            }
            unlock();
        }
        stop(TRUE);
    }
    
    void start(){
        
        startThread(true, false);   // blocking, verbose
        
    }
    
    void stop(bool rightNow){
        
        if (!rightNow) {
            waitForThread();
        }
        stopThread();
        
    }
    
    // Properties
    
    ofVideoPlayer gmMovie;
    ofVideoPlayer gmMovieScrub;
    ofVideoPlayer gmMovieThumbs;
    
    vector<fakGrabbedStill> grabbedStill;
    
    vector<fakGrabbedStill> grabbedThumbs;
    
    int gmTotalFrames;
    float gmImageRatio;
    float gmPixelRatio;
    int gmFrameRate;
    bool gmSetupFinished;
    int gmNumberLoadedCounter;
    bool gmCalcResizeSwitch;
    bool vfFramesToTimeSwitch;
    bool gmScrubMovie;
    int gmScrubID;
    int gmNumberOfStills;
    int gmNumberOfThumbs;
    
    ofFbo gmFboToSave;
    ofPixels gmPixToSave;
    
    ofTrueTypeFont gmTypo8;
    
    bool devTurnOffMovieSwitch = FALSE;
    bool isMovieLoaded = FALSE;
    
    
};

#endif
