//
//  fakGrabbedMovie.h
//  fakBreakupVideo
//
//  Created by Jakob on 2/14/13.
//
//

#ifndef __fakBreakupVideo__fakGrabbedMovie__
#define __fakBreakupVideo__fakGrabbedMovie__

#include "ofMain.h"
#include "fakGrabbedMovieStill.h"
#include "ofxFontStash.h"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"

#include <string>
#include <iostream>
//#include "ZenLib/Ztring.h"
//#include "MediaInfo/MediaInfo.h" //Staticly-loaded library (.lib or .a or .so)
//#define MediaInfoNameSpace MediaInfoLib;
#include "MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
#define MediaInfoNameSpace MediaInfoDLL;

//using namespace ZenLib;
using namespace MediaInfoNameSpace;
using namespace boost;

#define REsizeFactor 10.0
#define TimeToWaitForMovie 500

#define FAK_ORANGECOLOR ofColor(238, 71, 0, 255)
#define FAK_DARKORANGECOLOR ofColor(99, 30, 0, 255)
#define FAK_DARKDARKORANGECOLOR ofColor(75, 21, 0, 255)
#define FAK_MIDDLEDARKORANGECOLOR ofColor(170, 50, 0, 255)
#define FAK_LIGHTERMIDDLEDARKORANGECOLOR ofColor(185, 55, 0, 255)

#define FAK_GREENCOLOR ofColor(117, 130, 16, 255)
#define FAK_LIGHTGRAY ofColor(205, 205, 205, 255)
#define FAK_MIDDLEGRAY ofColor(195, 195, 195, 255)
#define FAK_TRANSPARENT ofColor(0,0,0,0)

#define FAK_ORANGE1 ofColor(255, 80, 6, 255)
#define FAK_ORANGE2 ofColor(255, 183, 153, 255)
#define FAK_ORANGE3 ofColor(255, 147, 101, 255)
#define FAK_ORANGE4 ofColor(255, 168, 131, 255)
#define FAK_ORANGE5 ofColor(255, 211, 193, 255)

#define FAK_WHITE ofColor(255, 255, 255, 255)
#define FAK_BLACK ofColor(0, 0, 0, 255)
#define FAK_SHADOW ofColor(0, 0, 0, 130)
#define FAK_GRAY ofColor(59, 59, 59, 255)

class fakGrabbedMovie : public ofThread {
    
public:

    fakGrabbedMovie(){
        gmTotalFrames = 0;
        gmSetupFinished = FALSE;
        gmCalcResizeSwitch = FALSE; // makes texture divide by REsizeFactor - but allocation is done only at the beginning and not later on!!!
        vfFramesToTimeSwitch = TRUE;
        gmScrubMovie = FALSE;
        gmRollOver = FALSE;
        gmNumberOfStills = 1;
        gmHasNoFrames = FALSE;
        gmCurrAllocating = false;
    }
    
    // functions
    
    void setup(string vfMovieName, int _numberOfStills, int _gmThumbWidth, int _gmThumbHeight, bool _showPlaceHolder){
        
        gmThumbWidth = _gmThumbWidth;
        gmThumbHeight = _gmThumbHeight;
        gmMouseEventsEnabled = false;
        
        string shaderProgram = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        \
        void main (void){\
        vec2 pos = gl_TexCoord[0].st;\
        \
        vec3 src = texture2DRect(tex0, pos).rgb;\
        float mask = texture2DRect(maskTex, pos).r;\
        \
        gl_FragColor = vec4( src , mask);\
        }";
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
        shader.linkProgram();
        
        setInPointImage.loadImage("MoviePrint_SetInPoint_v001_00000.png");
        setOutPointImage.loadImage("MoviePrint_SetOutPoint_v001_00000.png");
        frameForwardImage.loadImage("MoviePrint_FrameForward_v001_00000.png");
        frameForward2Image.loadImage("MoviePrint_FrameForward_v001_00001.png");
        frameForward3Image.loadImage("MoviePrint_FrameForward_v001_00002.png");
        frameBackwardImage.loadImage("MoviePrint_FrameBackward_v001_00000.png");
        frameBackward2Image.loadImage("MoviePrint_FrameBackward_v001_00001.png");
        frameBackward3Image.loadImage("MoviePrint_FrameBackward_v001_00002.png");
        corruptImage.loadImage("MoviePrint_Corrupt_v001_00000.jpg");
        emptyImage.loadImage("MoviePrint_Corrupt_00000.jpg");
        updatingStill.loadImage("MoviePrint_StillUpdating_v001_00000.png");
        headerImage.loadImage("MoviePrint_Layout_Header_v001_00000.png");
        
        setNumberOfStills(_numberOfStills);
        
        gmFontStashUbuntu.setup("Ubuntu-Light.ttf", 1.03);
        gmFontStashFranchise.setup("Franchise-Bold.ttf", 0.9);
        gmFontStashHelveticaLight.setup("HelveticaNeueLTCom-Lt.ttf");
        gmFontStashHelveticaMedium.setup("HelveticaNeueLTCom-Md.ttf");
        
      
        franchiseFontRightSize = 0;
        stringMargin = 2;

        gmSetTitleInfo = TRUE; //create new title size und umbruch
        
        loadNewMovieToBeGrabbed(vfMovieName, gmNumberOfStills, _showPlaceHolder, false);
        
        gmSetupFinished = TRUE;
        gmShowFramesUI = TRUE;
        
    }
    
    bool loadNewMovieToBeGrabbed(string vfMovieName, int _numberOfStills, bool _showPlaceHolder, bool _addListener){
        
        setNumberOfStills(_numberOfStills);
        stop(FALSE);
        
        isMovieLoaded = FALSE;
        
        if (!_showPlaceHolder) {
            ofLog(OF_LOG_VERBOSE, "_____________________________________ start loadMovie function");
            gmMovie.loadMovie(vfMovieName);
            ofLog(OF_LOG_VERBOSE, "_____________________________________ end loadMovie function");
            
            if (gmMovie.isLoaded()) {
                isMovieLoaded = TRUE;
                if (gmMovie.getTotalNumFrames() < 2) { //check if movie has only one frame, if so than calculate totalframes and later use setPosition instead of setFrame
                    gmHasNoFrames = TRUE;
                    gmFrameRate = 25;
                    gmTotalFrames = gmMovie.getDuration() * gmFrameRate;
                } else {
                    gmHasNoFrames = FALSE;
                    gmTotalFrames = gmMovie.getTotalNumFrames();
                    gmFrameRate = ceil(gmTotalFrames/gmMovie.getDuration());
                }
                gmImageRatio = gmMovie.getWidth()/gmMovie.getHeight();
                gmPixelRatio = gmMovie.getPixelFormat();
                ofLog(OF_LOG_VERBOSE, "ImageRatio:" + ofToString(gmImageRatio) + " PixelRatio:" + ofToString(gmPixelRatio)  + " Framerate:" + ofToString(gmFrameRate) + " totalFrames:" + ofToString(gmTotalFrames) + " getDuration:" + ofToString(gmMovie.getDuration()));
                
            } else {
                gmTotalFrames = gmNumberOfStills;
                ofLog(OF_LOG_VERBOSE, "gmTotalFrames set manually");
                
            }
            
        }
        allocateNewNumberOfStills(gmNumberOfStills, gmThumbWidth, gmThumbHeight, _showPlaceHolder, _addListener);
        
        updatingStill.resize(gmThumbWidth, gmThumbHeight);
        
        getMovieInformation(vfMovieName);

        gmSetTitleInfo = TRUE;
        
        return isMovieLoaded;
    }
    
    void loadNewMovieToBeScrubbed(string vfMovieName){
        
            ofLog(OF_LOG_VERBOSE, "_____________________________________ start loadMovie function SCRUB");
            gmMovieScrub.loadMovie(vfMovieName);
            ofLog(OF_LOG_VERBOSE, "_____________________________________ end loadMovie function SCRUB");
        
    }

    string ReplaceString(string subject, const string& search, const string& replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return subject;
    }
    
    void getMovieInformation(string _vfMovieName){

        gmMIFilePathOhne = getMoviePathName();
        gmMIFilePath = "FilePath: " + gmMIFilePath;
        
        
        //Information about MediaInfo
        MediaInfo MI;
        MI.Open(__T(_vfMovieName));
        
        MI.Option(__T("Inform"), __T("General;Name : %FileName%.%FileExtension%\\r\\n:::Format : %Format%\\r\\n:::Format/String : %Format/String%\\r\\n:::FileSize : %FileSize/String%\\r\\n:::Duration : %Duration/String1%\\r\\n:::\nVideo;FrameCount : %FrameCount%\\r\\n:::Size : %Width%x%Height%\\r\\n:::DisplayAspectRatio : %DisplayAspectRatio/String%\\r\\n:::FrameRate : %FrameRate/String%"));
        
        gmInfCol1 = MI.Inform();
        gmInfCol1 = ReplaceString(gmInfCol1, ":::", "\n");

        gmMIFileName = MI.Get(Stream_General, 0, __T("FileName"), Info_Text).c_str();
        gmMIFileExtension = MI.Get(Stream_General, 0, __T("FileExtension"), Info_Text).c_str();
        gmMIFormat = MI.Get(Stream_General, 0, __T("Format"), Info_Text).c_str();
        gmMIFormatString = MI.Get(Stream_General, 0, __T("Format/String"), Info_Text).c_str();
        gmMIFileSizeString = MI.Get(Stream_General, 0, __T("FileSize/String"), Info_Text).c_str();
        gmMIDurationString1 = MI.Get(Stream_General, 0, __T("Duration/String1"), Info_Text).c_str();
        
        gmMIFrameCount = MI.Get(Stream_Video, 0, __T("FrameCount"), Info_Text).c_str();
        gmMIWidth = MI.Get(Stream_Video, 0, __T("Width"), Info_Text).c_str();
        gmMIHeight = MI.Get(Stream_Video, 0, __T("Height"), Info_Text).c_str();
        gmMIWidth = gmMIWidth + "x" + gmMIHeight;
        gmMIDisplayAspectRatioString = MI.Get(Stream_Video, 0, __T("DisplayAspectRatio/String"), Info_Text).c_str();
        gmMIFrameRateString = MI.Get(Stream_Video, 0, __T("FrameRate/String"), Info_Text).c_str();
        gmMIVFormat = MI.Get(Stream_Video, 0, __T("Format"), Info_Text).c_str();
        gmMIFormatInfo = MI.Get(Stream_Video, 0, __T("Format/Info"), Info_Text).c_str();
        gmMIBitRate = MI.Get(Stream_Video, 0, __T("BitRate"), Info_Text).c_str();
        gmMIPixelAspectRatio = MI.Get(Stream_Video, 0, __T("PixelAspectRatio"), Info_Text).c_str();
        gmMIDisplayAspectRatio = MI.Get(Stream_Video, 0, __T("DisplayAspectRatio"), Info_Text).c_str();
        gmMIFrameRate_ModeString = MI.Get(Stream_Video, 0, __T("FrameRate_Mode/String"), Info_Text).c_str();
        gmMIColorSpace = MI.Get(Stream_Video, 0, __T("ColorSpace"), Info_Text).c_str();
        gmMIChromaSubsampling = MI.Get(Stream_Video, 0, __T("ChromaSubsampling"), Info_Text).c_str();
        gmMIBitDepthString = MI.Get(Stream_Video, 0, __T("BitDepth/String"), Info_Text).c_str();
        gmMIInterlacementString = MI.Get(Stream_Video, 0, __T("Interlacement/String"), Info_Text).c_str();

        gmMIAFormat = MI.Get(Stream_Audio, 0, __T("Format"), Info_Text).c_str();
        gmMIAChannelsString = MI.Get(Stream_Audio, 0, __T("Channel(s)/String"), Info_Text).c_str();
        gmMIASamplingRate = MI.Get(Stream_Audio, 0, __T("SamplingRate/String"), Info_Text).c_str();

        gmMIFileNameClean = gmMIFileName + "." + gmMIFileExtension;

        MI.Option(__T("Inform"), __T("Video;Video\\r\\n:::Format : %Format%\\r\\n:::Format/Info : %Format/Info%\\r\\n:::BitRate : %BitRate%\\r\\n:::PixelAspectRatio : %PixelAspectRatio%\\r\\n:::DisplayAspectRatio : %DisplayAspectRatio%\\r\\n:::FrameRate_Mode/String : %FrameRate_Mode/String%\\r\\n:::ColorSpace : %ColorSpace%\\r\\n:::ChromaSubsampling : %ChromaSubsampling%\\r\\n:::BitDepth/String : %BitDepth/String%\\r\\n:::Interlacement/String : %Interlacement/String%"));
        
        gmInfCol2 = MI.Inform().c_str();
        gmInfCol2 = ReplaceString(gmInfCol2, ":::", "\n");
        
        MI.Option(__T("Inform"), __T("Audio;Audio\\r\\n:::Format : %Format%\\r\\n:::Format/Info : %Format/Info%\\r\\n:::BitRate : %BitRate%\\r\\n:::Channel(s)/String : %Channel(s)/String%\\r\\n:::ChannelPositons : %ChannelPositons%\\r\\n:::SamplingRate/String : %SamplingRate/String%"));
        
        gmInfCol3 = MI.Inform().c_str();
        gmInfCol3 = ReplaceString(gmInfCol3, ":::", "\n");

        MI.Close();

    }
    
    void allocateNewNumberOfStills(int _numberOfStills, int _gmThumbWidth, int _gmThumbHeight, bool _drawPlaceHolder, bool _addListener){
        if (isMovieLoaded) {
            gmCurrAllocating = true;
            gmThumbWidth = _gmThumbWidth;
            gmThumbHeight = _gmThumbHeight;
            
            stop(TRUE);
            while (isThreadRunning()) {
                ofLog(OF_LOG_VERBOSE, "allocateNewNumberOfStills is waiting for thread to stop");
            }
            
//          unregister All Mouse Events of the Stills (old gmNumberOfStills)
            disableMouseEvents();
            
            setNumberOfStills(_numberOfStills);
            grabbedStill.clear();
            grabbedStill.resize(_numberOfStills);

            if (_addListener) {
                enableMouseEvents();
            }
            for(int i=0; i<gmNumberOfStills; i++)
            {
                if (_addListener) {
                    ofAddListener(grabbedStill[i].gsClickedInside, this, &fakGrabbedMovie::scrubMovie);
                    ofAddListener(grabbedStill[i].gsMovedInside, this, &fakGrabbedMovie::rollOverMovie);
                }
                grabbedStill[i].gsID = i;
                grabbedStill[i].gsX = 0;
                grabbedStill[i].gsY = 0;
                grabbedStill[i].gsDrawWidth = 160;
                grabbedStill[i].gsDrawHeight = 90;
                grabbedStill[i].gsUpperLimitY = gmUpperLimitY;
                grabbedStill[i].gsLowerLimitY = gmLowerLimitY;
                grabbedStill[i].gsLeftLimitX = gmLeftLimitX;
                grabbedStill[i].gsRightLimitX = gmRightLimitX;
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
            
            // creating maskFbo - for rounding stills
            maskFbo.allocate(grabbedStill[0].gsWidth,grabbedStill[0].gsHeight);
            ofPushStyle();
            maskFbo.begin();
            ofEnableAlphaBlending();
            ofClear(0,0,0,255);
            ofClear(0, 0, 0, 0);
            ofSetColor(255, 255, 255, 255);
            ofRectRounded(0, 0, grabbedStill[0].gsWidth, grabbedStill[0].gsHeight, grabbedStill[0].gsWidth/64);
            maskFbo.end();
            ofPopStyle();
     
            gmSetTitleInfo = TRUE; //create new title size und umbruch

            ofLog(OF_LOG_VERBOSE, "Allocations worked");
        } else if (_drawPlaceHolder){
            setNumberOfStills(_numberOfStills);
            grabbedStill.clear();
            grabbedStill.resize(_numberOfStills);
        }
        gmCurrAllocating = false;
    }
    
    string StringToUpper(string strToConvert){
        std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);
        return strToConvert;
    }

    string getMoviePathName(){
        gmMIFilePath = ofToString(gmMovie.getMoviePath());
        vector<string> tempVectorString = ofSplitString(gmMIFilePath, "/");
        tempVectorString.pop_back();
        return ofJoinString(tempVectorString, "/") + "/";
    }
    
    int CountNewlines(string s){
        int len = s.length();
        int c = 0;
        for (int i=0; i < len;  i++)
        {
            if (s[i] == '\n') c++;
        }
        return c;
    }

     void scrubMovie(int & i){
        if (isMovieLoaded) {
            gmScrubID = i;
//            ofLog(OF_LOG_VERBOSE, "rollOverMovieID" + ofToString(gmRollOverMovieID) + "gmRollOverMovieButtonID" + ofToString(gmRollOverMovieButtonID));
            if (gmRollOverMovieButtonID == 0) {
                gmScrubMovie = TRUE;
            }
        }
    }
    
    void rollOverMovie(ofVec2f & i){
        if (isMovieLoaded) {
            gmRollOverMovieID = i.x;
            gmRollOverMovieButtonID = i.y;
            gmRollOver = TRUE;
//            ofLog(OF_LOG_VERBOSE, "rollOverMovieID" + ofToString(gmRollOverMovieID) + "gmRollOverMovieButtonID" + ofToString(gmRollOverMovieButtonID));
        }
    }
    
    void setAllLimitsUpper(int _upperLimit){
        ofLog(OF_LOG_VERBOSE, "igmNumberOfStills" + ofToString(gmNumberOfStills));
        ofLog(OF_LOG_VERBOSE, "grabbedStill" + ofToString(grabbedStill.size()));
        gmUpperLimitY = _upperLimit;
        for (int i=0; i<grabbedStill.size(); i++) {
            grabbedStill[i].gsUpperLimitY = gmUpperLimitY;
        }
    }
    
    void setAllLimitsLower(int _LowerLimit){
        gmLowerLimitY = _LowerLimit;
        for (int i=0; i<grabbedStill.size(); i++) {
            grabbedStill[i].gsLowerLimitY = gmLowerLimitY;
        }
    }
    
    void setAllLimitsLeft(int _leftLimit){
        gmLeftLimitX = _leftLimit;
        for (int i=0; i<grabbedStill.size(); i++) {
            grabbedStill[i].gsLeftLimitX = gmLeftLimitX;
        }
    }
    
    void setAllLimitsRight(int _rightLimit){
        gmRightLimitX = _rightLimit;
        for (int i=0; i<grabbedStill.size(); i++) {
            grabbedStill[i].gsRightLimitX = gmRightLimitX;
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
    
    int numberGrabbed(){
        gmNumberGrabbedCounter = 0;
        if (isMovieLoaded) {
            for(int i=0; i<gmNumberOfStills; i++)
            {
                if(!grabbedStill[i].gsToBeGrabbed){
                    gmNumberGrabbedCounter++;
                }
            }
        }
        return gmNumberGrabbedCounter;
    }
    
    float percLoaded(){
        int gmNumberLoaded = numberLoaded();
        return (float)gmNumberLoaded/(gmNumberOfStills-1);
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
    
    void disableMouseEvents(){
        if (isMovieLoaded) {
            gmMouseEventsEnabled = false;
            for (int i=0; i<gmNumberOfStills; i++) {
                grabbedStill[i].unregisterMouseEvents();
            }
        }
    }
    
    bool getMouseEventsEnabled(){
        return gmMouseEventsEnabled;
    }
    
    void enableMouseEvents(){
        if (isMovieLoaded) {
            gmMouseEventsEnabled = true;
            for (int i=0; i<gmNumberOfStills; i++) {
                grabbedStill[i].registerMouseEvents();
            }
        }
    }
    
    void update(){
        if (isMovieLoaded) {
            gmMovie.update();
        }
    }
    
    void updateAllFrameNumbers(int gridTimeArray[]){
        if (isMovieLoaded) {
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
    
    void setNumberOfStills(int _numberOfStills){
        gmNumberOfStills = _numberOfStills;
    }
    
    void grabToImage(int i, int f){
        
        if (isMovieLoaded) {
            
            if (f < 0) {
                f = 0;
            }
            if (f > gmTotalFrames-1) {
                f = gmTotalFrames-1;
            }

            if (gmHasNoFrames) { // movies die "keine frames haben" benoetigen setPosition, deshalb sind auch meist die ersten paar frames "kaputt"
                if (f < 5) {
                    f = 5;
                }
                gmMovie.setPosition((float)(f-2)/(float)(gmTotalFrames-1)); //setPosition Movies brauchen das setzen des frames davor und dann nextFrame
                gmMovie.nextFrame();
                gmMovie.nextFrame();
                if (gmThreadCounter < 2) { // der erste frame muss ein wenig warten, bis das movie bereit ist
                    ofSleepMillis(TimeToWaitForMovie);
                }
                ofLog(OF_LOG_VERBOSE, "setPosition: " + ofToString(gmMovie.getPosition()) + " f: " + ofToString(f) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));
                
            } else {
                if (f==0) {
                    gmMovie.setFrame(0);
                    if (gmThreadCounter < 2) { // der erste frame muss ein wenig warten, bis das movie bereit ist
                        ofSleepMillis(TimeToWaitForMovie);
                    }
                } else {
                    gmMovie.setFrame(f);
                    if (gmThreadCounter < 2) { // der erste frame muss ein wenig warten, bis das movie bereit ist
                        ofSleepMillis(TimeToWaitForMovie);
                    }
                }
                ofLog(OF_LOG_VERBOSE, "setFrame: " + ofToString(gmMovie.getCurrentFrame())+ " f: " + ofToString(f) + " getCurrentFrame: " + ofToString(gmMovie.getCurrentFrame()));
            }
            ofLog(OF_LOG_VERBOSE, "gsImage Size: " + ofToString(grabbedStill[i].gsImage.width)+ " x " + ofToString(grabbedStill[i].gsImage.height));
            ofLog(OF_LOG_VERBOSE, "gmMovie Size: " + ofToString(gmMovie.getWidth())+ " x " + ofToString(gmMovie.getHeight()));
            if (grabbedStill[i].gsImage.isAllocated() && !gmCurrAllocating) {
                grabbedStill[i].gsImage.setFromPixels(gmMovie.getPixelsRef());
                grabbedStill[i].gsToBeGrabbed = FALSE;
            } else {
                ofLog(OF_LOG_VERBOSE, "CRASH AVOIDED grabbedStill[i].gsImage.isAllocated() FALSE _______________________________");
            }
        }
    }
    
    void drawStill(int i, float _x, float _y, float _w, float _h, float _alpha, bool _superKeyPressed, bool _shiftKeyPressed, bool _drawPlaceHolder){
        
        if (isMovieLoaded) {
            
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255);
            
            grabbedStill[i].gsX = _x;
            grabbedStill[i].gsY = _y;
            grabbedStill[i].gsDrawWidth = _w;
            grabbedStill[i].gsDrawHeight = _h;
            grabbedStill[i].gsResizeFactor = gmMovie.getWidth()/_w;
            
            if (grabbedStill[i].gsToBeUpdated) { // load textures in proper size
                if (!grabbedStill[i].gsToBeGrabbed) {
                    if (gmCalcResizeSwitch) {
                        grabbedStill[i].gsImage.resize(grabbedStill[i].gsWidth, grabbedStill[i].gsHeight);
                    }
                    grabbedStill[i].gsTexture.loadData(grabbedStill[i].gsImage);
                    grabbedStill[i].gsToBeUpdated = FALSE;
                }
            }
            
            shader.begin(); // draw still with rounded corners
            shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
            
            ofSetColor(255, 255, 255, 255);
            grabbedStill[i].gsTexture.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight);
            
            shader.end();

            // draw selection
            ofPushStyle();
            ofSetColor(255, 255, 255, 30);
            if (grabbedStill[i].gsRollOver) {
                int tempSelectionWidth = 2;
                ofRect(grabbedStill[i].gsX - tempSelectionWidth/2, grabbedStill[i].gsY - tempSelectionWidth/2, grabbedStill[i].gsDrawWidth + tempSelectionWidth, grabbedStill[i].gsDrawHeight + tempSelectionWidth);
            }
            ofPopStyle();
            
            // draw update image
            if (grabbedStill[i].gsToBeGrabbed) {
                ofPushMatrix();
                ofPushStyle();
                ofSetColor(0, 0, 0, 130);
                ofRect(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight);
                ofSetColor(255, 255, 255, 200);
                updatingStill.drawSubsection(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight,updatingStill.width/2 - grabbedStill[i].gsDrawWidth/2, updatingStill.height/2 - grabbedStill[i].gsDrawHeight/2);
//                }
                ofPopStyle();
                ofPopMatrix();
            }

            if (gmShowFramesUI) { // drawing UI
                drawStillUI(i, grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight, _alpha);
            }
            
            // drawing overlay graphics
                if (grabbedStill[i].gsRollOver) {
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver3) {
                        ofSetColor(255);
                    }
                    setInPointImage.draw(grabbedStill[i].gsX, grabbedStill[i].gsY + grabbedStill[i].gsDrawHeight - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);                    
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver4) {
                        ofSetColor(255);
                    }
                    setOutPointImage.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY + grabbedStill[i].gsDrawHeight - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver1) {
                        ofSetColor(255);
                    }
                    if (_superKeyPressed) {
                        frameBackward3Image.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else if(_shiftKeyPressed){
                        frameBackward2Image.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else {
                        frameBackwardImage.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    }
                    
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver2) {
                        ofSetColor(255);
                    }
                    if (_superKeyPressed) {
                        frameForward3Image.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else if(_shiftKeyPressed){
                        frameForward2Image.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else {
                        frameForwardImage.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    }

                }
            ofPopStyle();
            ofSetColor(255);
        } else if (_drawPlaceHolder){
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255);
            
//            grabbedStill[i].gsX = _x;
//            grabbedStill[i].gsY = _y;
//            grabbedStill[i].gsDrawWidth = _w;
//            grabbedStill[i].gsDrawHeight = _h;
//            grabbedStill[i].gsResizeFactor = gmMovie.getWidth()/_w;
            
//            shader.begin(); // draw still with rounded corners
//            shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
//            grabbedStill[i].gsTexture.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight);
//            shader.end();
            

            ofPushMatrix();
            ofPushStyle();
            ofSetColor(FAK_MIDDLEGRAY);
//            updatingStill.drawSubsection(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight,updatingStill.width/2 - grabbedStill[i].gsDrawWidth/2, updatingStill.height/2 - grabbedStill[i].gsDrawHeight/2);
            
            ofRect(_x, _y, _w, _h);
            
            ofPopStyle();
            ofPopMatrix();

            ofPopStyle();
            ofSetColor(255);        }
    }

    void printStill(int i, float _x, float _y, float _w, float _h, bool _drawPlaceHolder){
        
        if (_drawPlaceHolder){
            
            ofPushStyle();
            ofSetColor(FAK_MIDDLEGRAY);
            
            ofRect(_x, _y, _w, _h);
            
            ofPopStyle();
            
        } else if (isMovieLoaded) {
            
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255);
            
            grabbedStill[i].gsDrawWidth = _w;
            grabbedStill[i].gsDrawHeight = _h;
            grabbedStill[i].gsResizeFactor = gmMovie.getWidth()/_w;
            
            if (grabbedStill[i].gsToBeUpdated) { // load textures in proper size
                if (!grabbedStill[i].gsToBeGrabbed) {
                    if (gmCalcResizeSwitch) {
                        grabbedStill[i].gsImage.resize(grabbedStill[i].gsWidth, grabbedStill[i].gsHeight);
                    }
                    grabbedStill[i].gsTexture.loadData(grabbedStill[i].gsImage);
                    grabbedStill[i].gsToBeUpdated = FALSE;
                }
            }
            
            shader.begin(); // draw still with rounded corners
            shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
            grabbedStill[i].gsTexture.draw(_x, _y, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight);
            shader.end();
            
            if (gmShowFramesUI) { // drawing UI
                drawStillUI(i, _x, _y, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight, 1.0);
            }
            
            ofPopStyle();
            ofSetColor(255);
        }
    }
    
    void drawGridOfStills(float _x, float _y, int _gridColumns, float _gridMargin, float _scrollAmount, float _scaleFactor, float _alpha, bool _isBeingPrinted, bool _isActive, bool _superKeyPressed, bool _shiftKeyPressed, bool _drawPlaceHolder){

        // draw all frames
        ofPushStyle();
        ofPushMatrix();
        ofEnableAlphaBlending();
        ofSetColor(FAK_ORANGECOLOR); // draw title rect
       
        ofSetColor(255, 255, 255, 255); // draw stills
        for(int i=0; i<gmNumberOfStills; i++)
        {
            float tempX = (_x + (gmThumbWidth+_gridMargin)*(i%_gridColumns)) * _scaleFactor;
            float tempY = (_y + (gmThumbHeight+_gridMargin)*(i/_gridColumns)) * _scaleFactor;
            drawStill(i, tempX, tempY, gmThumbWidth * _scaleFactor, gmThumbHeight * _scaleFactor, 1, _superKeyPressed, _shiftKeyPressed, _drawPlaceHolder);
        }
        
        if (_isBeingPrinted) {
            gmSetTitleInfo = TRUE; //create new title size und umbruch
        }
        
        ofPopMatrix();
        ofPopStyle();
        
    }
    
    void drawMoviePrint(float _x, float _y, int _gridColumns, int _gridRows, float _gridMargin, float _scaleFactor, float _alpha, bool _drawPlaceHolder, float _printHeaderHeight, bool _printDisplayVideoAudioInfo, bool _drawPreview){

        ofPushStyle();
        ofPushMatrix();

        if (_printDisplayVideoAudioInfo) { // draw info header
            ofPushStyle();
            ofPushMatrix();
            ofEnableAlphaBlending();
            ofSetColor(FAK_GRAY);
            ofRect(_x * _scaleFactor, _y * _scaleFactor, (_gridMargin + (gmThumbWidth+_gridMargin) * _gridColumns) * _scaleFactor, _printHeaderHeight * _scaleFactor);
            for(int i=0; i<_gridColumns; i++)
            {
                switch (i%5) {
                    case 0:
                        ofSetColor(FAK_ORANGE1);
                        break;
                    case 1:
                        ofSetColor(FAK_ORANGE2);
                        break;
                    case 2:
                        ofSetColor(FAK_ORANGE3);
                        break;
                    case 3:
                        ofSetColor(FAK_ORANGE4);
                        break;
                    case 4:
                        ofSetColor(FAK_ORANGE5);
                        break;
                    default:
                        ofSetColor(255, 255, 255, 255);
                        break;
                }
                // draw orange stripes
                ofRect((_x + _gridMargin + (gmThumbWidth+_gridMargin) * i) * _scaleFactor, (_y + _printHeaderHeight*0.7) * _scaleFactor, gmThumbWidth * _scaleFactor, _printHeaderHeight* 0.15 * _scaleFactor);
            }
            
            if (_drawPreview) { // draw Info fake for preview
                ofSetColor(255, 255, 255, 255);
                ofRect(((_x + _gridMargin) * _scaleFactor), ((_y +_printHeaderHeight*0.3) * _scaleFactor), (gmThumbWidth/4.0 - gmThumbWidth/40.0) * _scaleFactor, _printHeaderHeight*0.3 * _scaleFactor);
                ofRect(((_x + _gridMargin + gmThumbWidth/4.0) * _scaleFactor), ((_y + _printHeaderHeight*0.45) * _scaleFactor), ((gmThumbWidth/4)*3) * _scaleFactor, _printHeaderHeight*0.15 * _scaleFactor);
            } else {
                // draw Info text
                float tempFontHeightBig = 20;
                float tempFontHeightSmall = 10;
                float tempFontScale = _scaleFactor;

                // get Width of Type
                float tempWidthOfName = gmFontStashFranchise.getBBox("name", tempFontHeightBig * _scaleFactor, 0, 0).getWidth();
                float tempWidthOfPathName = gmFontStashHelveticaMedium.getBBox(ofToString(gmMovie.getMoviePath()), tempFontHeightSmall * _scaleFactor, 0, 0).getWidth();
                
                // when PathName width bigger then display width then downscale the PathName
                if ((((gmThumbWidth+_gridMargin) * _gridColumns - _gridMargin) * _scaleFactor + tempWidthOfName) <= tempWidthOfPathName) {
                    tempFontScale = tempFontScale * (((gmThumbWidth+_gridMargin) * _gridColumns - _gridMargin) * _scaleFactor + tempWidthOfName)/tempWidthOfPathName*0.75;
                }
                float tempWidthOfPath = gmFontStashHelveticaLight.getBBox(ofToString(gmMIFilePathOhne), tempFontHeightSmall * tempFontScale, 0, 0).getWidth();

                ofSetColor(255, 255, 255, 255);
                gmFontStashFranchise.draw("name",20 * _scaleFactor, (int)((_x + _gridMargin) * _scaleFactor), (int)((_y + _printHeaderHeight*0.6) * _scaleFactor));
                gmFontStashHelveticaLight.draw(ofToString(gmMIFilePathOhne), tempFontHeightSmall * tempFontScale, (int)((_x + _gridMargin) * _scaleFactor + tempWidthOfName + tempWidthOfName*0.1), (int)((_y + _printHeaderHeight*0.6) * _scaleFactor));
                gmFontStashHelveticaMedium.draw(ofToString(gmMIFileNameClean), tempFontHeightSmall * tempFontScale, (int)((_x + _gridMargin) * _scaleFactor + tempWidthOfName + tempWidthOfName*0.1 + tempWidthOfPath), (int)((_y + _printHeaderHeight*0.6) * _scaleFactor));
            }

            ofPopMatrix();
            ofPopStyle();
            
            ofTranslate(0, (_printHeaderHeight) * _scaleFactor);
        }
        
        // draw all frames
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, 255);
        int tempNumberOfThumbsToDisplay;
        tempNumberOfThumbsToDisplay = _gridColumns * _gridRows;

        for(int i=0; i<tempNumberOfThumbsToDisplay; i++)
        {
            float tempX = (_x + _gridMargin + (gmThumbWidth+_gridMargin)*(i%_gridColumns)) * _scaleFactor;
            float tempY = (_y + _gridMargin + (gmThumbHeight+_gridMargin)*(i/_gridColumns)) * _scaleFactor;
            if (((_gridColumns * _gridRows) > gmNumberOfStills) || !isMovieLoaded) {
                printStill(i, tempX, tempY, gmThumbWidth * _scaleFactor, gmThumbHeight * _scaleFactor, true);
            } else {
                printStill(i, tempX, tempY, gmThumbWidth * _scaleFactor, gmThumbHeight * _scaleFactor, _drawPlaceHolder);
            }
        }
        ofPopMatrix();
        ofPopStyle();
    }

    void drawStillUI(int i, float x, float y, float w, float h, float _alpha){
        
        if (isMovieLoaded) {
            
            float tempFontSize = ofMap(w, 0.0, 1000.0, 0.0, 45.0); // groessen mapping der still UI
            string dummyString;
            
            if (vfFramesToTimeSwitch) {
                dummyString = framesToTime(grabbedStill[i].gsFrameNumber);
            } else {
                dummyString = "#" + ofToString(grabbedStill[i].gsFrameNumber);
            }
            
            ofPushStyle();
            ofEnableAlphaBlending();
            
            ofRectangle rect = gmFontStashUbuntu.getBBox(dummyString, tempFontSize, 0, 0);
            if (grabbedStill[i].gsManipulated) {
                ofSetColor(FAK_ORANGECOLOR, 200*_alpha);
            } else {
                ofSetColor(0,0,0,200*_alpha);
            }
            ofRectRounded(x, y, rect.width + rect.width*0.03, rect.height + rect.height*0.3, rect.width*0.03);
            if (grabbedStill[i].gsToBeUpdated) {
                ofSetColor(100, 255 * _alpha);
            } else {
                ofSetColor(255, 255 * _alpha);
            }
            gmFontStashUbuntu.drawMultiLine(dummyString, tempFontSize, x + rect.width*0.015, y+rect.height + rect.height*0.15);
            
            ofPopStyle();
            
        }
    }

    // Thread funcions
    
    void threadedFunction(){
        
        if (gmSetupFinished && isMovieLoaded) { // only start when setup is finished and movie is loaded
            lock();
            do {
                for (int i = 0; i<gmNumberOfStills; i++) {
                    if (grabbedStill[i].gsToBeGrabbed) {
                        gmThreadCounter++;
                        grabToImage(i, grabbedStill[i].gsFrameNumber);
                    }
                }
            } while (!allGrabbed());
            unlock();
        }
        stop(TRUE);
                ofLog(OF_LOG_VERBOSE, "Closing Thread Function-----------------------------------------------" );

    }
    
    void start(){
        
        startThread(true, false);   // blocking, verbose
        gmThreadCounter = 0;
        
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
    
    vector<fakGrabbedMovieStill> grabbedStill;
    
    bool devTurnOffMovieSwitch = FALSE;
    bool isMovieLoaded = FALSE;
    
    string gmMovieName;
    int gmTotalFrames;
    float gmImageRatio;
    float gmPixelRatio;
    int gmFrameRate;
    bool gmSetupFinished;
    int gmNumberLoadedCounter;
    int gmNumberGrabbedCounter;
    bool gmCalcResizeSwitch;
    bool vfFramesToTimeSwitch;
    bool gmScrubMovie;
    bool gmRollOver;
    int gmRollOverMovieID;
    int gmRollOverMovieButtonID;
    int gmScrubID;
    int gmNumberOfStills;
    bool gmHasNoFrames;
    int gmThreadCounter;
    int gmUpperLimitY;
    int gmLowerLimitY;
    int gmLeftLimitX;
    int gmRightLimitX;
    int gmThumbWidth;
    int gmThumbHeight;
    bool gmShowFramesUI;
    bool gmMouseEventsEnabled;
    bool gmCurrAllocating;
    
    ofImage setInPointImage;
    ofImage setOutPointImage;
    ofImage frameForwardImage;
    ofImage frameForward2Image;
    ofImage frameForward3Image;
    ofImage frameBackwardImage;
    ofImage frameBackward2Image;
    ofImage frameBackward3Image;
    ofImage corruptImage;
    ofImage updatingStill;
    ofImage headerImage;
    ofImage emptyImage;
    
    ofxFontStash gmFontStashHelveticaLight;
    ofxFontStash gmFontStashHelveticaMedium;
    ofxFontStash gmFontStashUbuntu;
    ofxFontStash gmFontStashFranchise;
    int tempFontSize[24] = {6, 10, 14, 18, 22, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 80, 92, 108, 128, 256, 300};

    int franchiseFontRightSize;
    int stringMargin;
    ofFilePath gmLoadedFilePath;
    bool gmSetTitleInfo;
    string gmInfCollected;
    string gmInfCol1, gmInfCol2, gmInfCol3;
    string gmMIFileName, gmMIFileExtension, gmMIFormat, gmMIFormatString, gmMIFileSizeString, gmMIDurationString1, gmMIFrameCount, gmMIWidth, gmMIHeight, gmMIDisplayAspectRatioString, gmMIFrameRateString;
    string gmMIVFormat, gmMIFormatInfo, gmMIBitRate, gmMIPixelAspectRatio, gmMIDisplayAspectRatio, gmMIFrameRate_ModeString, gmMIColorSpace, gmMIChromaSubsampling, gmMIBitDepthString, gmMIInterlacementString;
    string gmMIAFormat, gmMIAChannelsString, gmMIASamplingRate, gmMIFilePath;
    
    string gmMIFileNameClean, gmMIFilePathOhne;

    // used for rounded corner mask
    ofShader shader;
    ofFbo maskFbo;
    
};

#endif /* defined(__fakBreakupVideo__fakGrabbedMovie__) */
