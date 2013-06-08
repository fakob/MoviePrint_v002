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
#include "fakGrabbedStill.h"
#include "ofxFontStash.h"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "ofxModifierKeys.h"

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
    }
    
    // functions
    
    void setup(string vfMovieName, int _numberOfStills, int _gmGridWidth, int _gmGridHeight){
        
        gmGridWidth = _gmGridWidth;
        gmGridHeight = _gmGridHeight;

        
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
        updatingStill.loadImage("MoviePrint_StillUpdating_v001_00000.png");

        
        setNumberOfStills(_numberOfStills);
        
        gmFontStash.setup("Ubuntu-Light.ttf", 1.03);
        gmFontStashFranchise.setup("Franchise-Bold.ttf", 0.9);
        
        franchiseFontRightSize = 0;
        stringMargin = 2;

        gmSetTitleInfo = TRUE; //create new title size und umbruch
        
        loadNewMovieToBeGrabbed(vfMovieName, gmNumberOfStills);
        
        gmSetupFinished = TRUE;
        gmShowFramesUI = TRUE;
        
    }
    
    bool loadNewMovieToBeGrabbed(string vfMovieName, int _numberOfStills){
        
        setNumberOfStills(_numberOfStills);
        stop(FALSE);
        
        isMovieLoaded = FALSE;
        
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
        createGrid(gmNumberOfStills, gmGridWidth, gmGridHeight);
        
//        updatingStill.resize(gmGridWidth, gmGridHeight);
        
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
        
        //Information about MediaInfo
        MediaInfo MI;
        MI.Open(__T(_vfMovieName));
        
//        MI.Option(__T("Inform"), __T("General;General\\r\\n:::CompleteName : %CompleteName%\\r\\n:::FolderName : %FolderName%\\r\\n:::FileName : %FileName%\\r\\n:::FileExtension : %FileExtension%\\r\\n:::Format : %Format%\\r\\n:::Format/Info : %Format/Info%\\r\\n:::FileSize : %FileSize%\\r\\n:::Duration : %Duration%\\r\\n:::Duration/String3 : %Duration/String3%"));
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

        gmMIFileName = "FileName : " + gmMIFileName + "." + gmMIFileExtension;
        gmMIFormat = "Format : " + gmMIFormat;
        gmMIFormatString = "Format/String : " + gmMIFormatString;
        gmMIFileSizeString = "FileSize : " + gmMIFileSizeString;
        gmMIDurationString1 = "Duration : " + gmMIDurationString1;
        gmMIFrameCount = "FrameCount : " + gmMIFrameCount;
        gmMIWidth = "Size : " + gmMIWidth;
        gmMIDisplayAspectRatioString = "DisplayAspectRatio : " + gmMIDisplayAspectRatioString;
        gmMIFrameRateString = "FrameRate : " + gmMIFrameRateString;
        gmMIVFormat = "Codec : " + gmMIVFormat;
        gmMIFormatInfo = "Codec/Info : " + gmMIFormatInfo;
        gmMIBitRate = "BitRate : " + gmMIBitRate;
        gmMIPixelAspectRatio = "PixelAspectRatio : " + gmMIPixelAspectRatio;
        gmMIDisplayAspectRatio = "DisplayAspectRatio : " + gmMIDisplayAspectRatio;
        gmMIFrameRate_ModeString = "FrameRate_Mode : " + gmMIFrameRate_ModeString;
        gmMIColorSpace = "ColorSpace : " + gmMIColorSpace;
        gmMIChromaSubsampling = "ChromaSubsampling : " + gmMIChromaSubsampling;
        gmMIBitDepthString = "BitDepth : " + gmMIBitDepthString;
        gmMIInterlacementString = "Interlacement : " + gmMIInterlacementString;
        gmMIAFormat = "AudioCodec : " + gmMIAFormat;
        gmMIAChannelsString = "Channels : " + gmMIAChannelsString;
        gmMIASamplingRate = "SamplingRate : " + gmMIASamplingRate;
       
//        MI.Option(__T("Inform"), __T("Video;Video\\r\\n:::Format : %Format%\\r\\n:::Format/Info : %Format/Info%\\r\\n:::Format/Version : %Format/Version%\\r\\n:::Format/Profile : %Format/Profile%\\r\\n:::Format/Compression : %Format/Compression%\\r\\n:::Format/Settings : %Format/Settings%\\r\\n:::CodecID/Info : %CodecID/Info%\\r\\n:::BitRate : %BitRate%\\r\\n:::BitRate/Mode : %BitRate/Mode%\\r\\n:::Width/String : %Width/String%\\r\\n:::Height/String : %Height/String%\\r\\n:::PixelAspectRatio : %PixelAspectRatio%\\r\\n:::PixelAspectRatio/String : %PixelAspectRatio/String%\\r\\n:::DisplayAspectRatio : %DisplayAspectRatio%\\r\\n:::DisplayAspectRatio/String : %DisplayAspectRatio/String%\\r\\n:::FrameRate_Mode/String : %FrameRate_Mode/String%\\r\\n:::FrameRate/String : %FrameRate/String%\\r\\n:::FrameCount : %FrameCount%\\r\\n:::ColorSpace : %ColorSpace%\\r\\n:::ChromaSubsampling : %ChromaSubsampling%\\r\\n:::BitDepth/String : %BitDepth/String%\\r\\n:::Interlacement : %Interlacement%\\r\\n:::Interlacement/String : %Interlacement/String%\\r\\n:::Channel(s)/String : %Channel(s)/String%\\r\\n:::ChannelPositons : %ChannelPositons%\\r\\n:::SamplingRate/String : %SamplingRate/String%"));
        MI.Option(__T("Inform"), __T("Video;Video\\r\\n:::Format : %Format%\\r\\n:::Format/Info : %Format/Info%\\r\\n:::BitRate : %BitRate%\\r\\n:::PixelAspectRatio : %PixelAspectRatio%\\r\\n:::DisplayAspectRatio : %DisplayAspectRatio%\\r\\n:::FrameRate_Mode/String : %FrameRate_Mode/String%\\r\\n:::ColorSpace : %ColorSpace%\\r\\n:::ChromaSubsampling : %ChromaSubsampling%\\r\\n:::BitDepth/String : %BitDepth/String%\\r\\n:::Interlacement/String : %Interlacement/String%"));
        
        gmInfCol2 = MI.Inform().c_str();
        gmInfCol2 = ReplaceString(gmInfCol2, ":::", "\n");
        
//        MI.Option(__T("Inform"), __T("Audio;Audio\\r\\n:::Format : %Format%\\r\\n:::Format/Info : %Format/Info%\\r\\n:::Format/Version : %Format/Version%\\r\\n:::Format/Profile : %Format/Profile%\\r\\n:::Format/Compression : %Format/Compression%\\r\\n:::Format/Settings : %Format/Settings%\\r\\n:::BitRate : %BitRate%\\r\\n:::BitRate/Mode : %BitRate/Mode%\\r\\n:::Channel(s)/String : %Channel(s)/String%\\r\\n:::ChannelPositons : %ChannelPositons%\\r\\n:::SamplingRate/String : %SamplingRate/String%"));
        MI.Option(__T("Inform"), __T("Audio;Audio\\r\\n:::Format : %Format%\\r\\n:::Format/Info : %Format/Info%\\r\\n:::BitRate : %BitRate%\\r\\n:::Channel(s)/String : %Channel(s)/String%\\r\\n:::ChannelPositons : %ChannelPositons%\\r\\n:::SamplingRate/String : %SamplingRate/String%"));
        
        gmInfCol3 = MI.Inform().c_str();
        gmInfCol3 = ReplaceString(gmInfCol3, ":::", "\n");

        MI.Close();

    }
    
    void createGrid(int _numberOfStills, int _gmGridWidth, int _gmGridHeight){
        if (isMovieLoaded) {
            
            gmGridWidth = _gmGridWidth;
            gmGridHeight = _gmGridHeight;
            
            stop(TRUE);
            while (isThreadRunning()) {
                ofLog(OF_LOG_VERBOSE, "createGrid is waiting for thread to stop");
            }
            
//          unregister All Mouse Events of the Stills (old gmNumberOfStills)
            for(int i=0; i<gmNumberOfStills; i++)
            {
                grabbedStill[i].unregisterMouseEvents();
            }
            
            setNumberOfStills(_numberOfStills);
            grabbedStill.clear();
            grabbedStill.resize(_numberOfStills);
            
            for(int i=0; i<gmNumberOfStills; i++)
            {
                grabbedStill[i].registerMouseEvents();
                ofAddListener(grabbedStill[i].gsClickedInside, this, &fakGrabbedMovie::scrubMovie);
                ofAddListener(grabbedStill[i].gsMovedInside, this, &fakGrabbedMovie::rollOverMovie);
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
        }
    }
    
    string wrapStringFontStash(string _text, float _size ,int _width) {
        
        _width = stringMargin*2 + _width;
        string typeWrapped = "";
        string tempString = "";
        //        vector <string> words = ofSplitString(_text, " ");
        vector <string> words;
        boost::split(words, _text, boost::is_any_of(" \n"));
        //        ofLog(OF_LOG_VERBOSE, "words: " + ofToString(words));
        
        for(int i=0; i<words.size(); i++) {
            
            string wrd = words[i];
            
            // if we aren't on the first word, add a space
            if (i > 0) {
                tempString += " ";
            }
            tempString += wrd;
            
            float stringwidth = gmFontStash.getWidth(tempString, _size) + _size/3;
            stringwidth = ceil (stringwidth);
            //            ofLog(OF_LOG_VERBOSE, "stringwidth:" + ofToString(stringwidth) + " tempString:" + ofToString(tempString));
            
            if(stringwidth >= _width) {
                typeWrapped += "\n";
                tempString = wrd;       // make sure we're including the extra word on the next line
            } else if (i > 0) {
                // if we aren't on the first word, add a space
                typeWrapped += " ";
            }
            
            typeWrapped += wrd;
            
        }
        
        //        ofLog(OF_LOG_VERBOSE, "typeWrapped" + ofToString(typeWrapped));
        
        return typeWrapped;
        
    }

    string wrapStringFontStashFranchise(string _tempString, int _tempWidth, int _tempHeight) {
        
        int tempLineHeight = 0;
        int tempCutPoint = 0;
        float tempLineHeightFactor = 0.9;
        franchiseFontRightSize = 0;
        string tempTempString = _tempString;
        vector<int> tempTempVecIterator;
        tempTempVecIterator.clear();
//        ofLog(OF_LOG_VERBOSE, "_tempString: " + ofToString(_tempString));

        
        for (int j=0; j<sizeof(tempFontSize); j++) {
            
            for (int i=1; i<_tempString.length(); i++) {
                string tempStringIncrement = _tempString.substr(tempCutPoint,i-tempCutPoint);
                float tempStringWidth = gmFontStashFranchise.getWidth(tempStringIncrement, tempFontSize[j]);
//                ofLog(OF_LOG_VERBOSE, "tempFontSize[j]: " + ofToString(tempFontSize[j]));
//                ofLog(OF_LOG_VERBOSE, "tempStringIncrement: " + ofToString(tempStringIncrement));
//                ofLog(OF_LOG_VERBOSE, "tempStringWidth: " + ofToString(tempStringWidth));

                if ((int)tempStringWidth > (_tempWidth-(stringMargin+stringMargin+stringMargin))) {
                    tempCutPoint = i-1;
                    tempLineHeight = tempLineHeight + tempFontSize[j] * tempLineHeightFactor;
                }
//                ofLog(OF_LOG_VERBOSE, "tempLineHeight: " + ofToString(tempLineHeight));

            }

                if (tempLineHeight > (_tempHeight-(tempFontSize[j] * tempLineHeightFactor + stringMargin + stringMargin))) {
                franchiseFontRightSize = tempFontSize[j-1];
                break;
            }
            if (j == (sizeof(tempFontSize)-1)) {
                franchiseFontRightSize = tempFontSize[j];
                break;
            }
            tempLineHeight = 0;
            tempCutPoint = 0;
        }
        
        tempLineHeight = 0;
        tempCutPoint = 0;
        
//        ofLog(OF_LOG_VERBOSE, "franchiseFontRightSize: " + ofToString(franchiseFontRightSize));
        for (int i=1; i<_tempString.length(); i++) {
            string tempStringIncrement = _tempString.substr(tempCutPoint,i-tempCutPoint);
            float tempStringWidth = gmFontStashFranchise.getWidth(tempStringIncrement, franchiseFontRightSize);
//            ofLog(OF_LOG_VERBOSE, "tempStringIncrement: " + ofToString(tempStringIncrement));
//            ofLog(OF_LOG_VERBOSE, "tempStringWidth: " + ofToString(tempStringWidth));
            if ((int)tempStringWidth > (_tempWidth-(stringMargin+stringMargin+stringMargin))) {
                tempCutPoint = i-1;
                tempLineHeight = tempLineHeight + franchiseFontRightSize * tempLineHeightFactor;
                tempTempVecIterator.push_back(tempCutPoint);
            }
            if (i == (_tempString.length()-1)) {
                tempTempVecIterator.push_back(tempCutPoint);
            }
        }
        for (int k = tempTempVecIterator.size()-1; k-- > 0; ) {
            tempTempString.insert(tempTempVecIterator[k], "\n");
        }
        tempTempString = StringToUpper(tempTempString);
        tempTempString = tempTempString.substr(0,tempTempString.length()-4);
//        ofLog(OF_LOG_VERBOSE, "tempTempString: " + ofToString(tempTempString));
        
        return tempTempString;
    }
    
    string StringToUpper(string strToConvert){
        std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);
        return strToConvert;
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
    
    void drawTitle(float _x, float _y, float _width, float _height, float _alpha, bool _isBeingPrinted, bool _isActive){
        float tempStringMargin = franchiseFontRightSize/10;
        
        if (gmSetTitleInfo) {
            string pathName = gmMovie.getMoviePath();
            pathName = gmLoadedFilePath.getFileName(pathName, TRUE);
            gmMovieName = wrapStringFontStashFranchise(pathName, _width , _height);
            gmSetTitleInfo = FALSE;
        }

        gmFontStashFranchise.drawMultiLine(gmMovieName, franchiseFontRightSize, _x + tempStringMargin, _y + tempStringMargin + franchiseFontRightSize*0.8);
    }
    
    void drawInfo1(float _x, float _y, float _width, float _height, float _alpha, bool _isBeingPrinted, bool _isActive){
//        float tempFontSize = 11.0;
        float tempFontSize = ofMap(_width, 0.0, 1000.0, 0.0, 45.0);
        float tempStringMargin = tempFontSize/4;
        int distance = 0;
        gmFontStash.drawMultiLine(wrapStringFontStash(gmMIFileName, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*1);
        distance = CountNewlines(wrapStringFontStash(gmMIFileName, tempFontSize, (_width+tempStringMargin+tempStringMargin))) + 1;
        if (tempStringMargin + tempFontSize*1.03*(1+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIFormat, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(1+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(2+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIFormatString, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(2+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(3+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIFileSizeString, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(3+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(4+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIDurationString1, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(4+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(5+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIFrameCount, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(5+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(6+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIWidth, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(6+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(7+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIDisplayAspectRatioString, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(7+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(8+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIFrameRateString, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(8+distance));
        }
    }

    void drawInfo2(float _x, float _y, float _width, float _height, float _alpha, bool _isBeingPrinted, bool _isActive){
        
        float tempFontSize = ofMap(_width, 0.0, 1000.0, 0.0, 45.0);
        float tempStringMargin = tempFontSize/4;
        int distance = 1;
        gmFontStash.drawMultiLine(wrapStringFontStash(gmMIVFormat, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*1);
        if (tempStringMargin + tempFontSize*1.03*(1+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIPixelAspectRatio, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(1+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(2+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIDisplayAspectRatio, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(2+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(3+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIFrameRate_ModeString, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(3+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(4+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIColorSpace, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(4+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(5+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIChromaSubsampling, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(5+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(6+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIBitDepthString, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(6+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(7+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIInterlacementString, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(7+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(8+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIAFormat, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(8+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(9+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIAChannelsString, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(9+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(10+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(gmMIASamplingRate, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(10+distance));
        }
    }

    void drawInfo3(float _x, float _y, float _width, float _height, float _alpha, bool _isBeingPrinted, bool _isActive){
        float tempFontSize = ofMap(_width, 0.0, 1000.0, 0.0, 45.0);
        float tempStringMargin = tempFontSize/4;
        int distance = 1;
        string tempDispFrames = "Displayed Frames : " + ofToString(gmNumberOfStills);
        string tempInPoint = "InPoint : " + ofToString(grabbedStill[0].gsFrameNumber);
        string tempOutPoint = "OutPoint : " + ofToString(grabbedStill[gmNumberOfStills-1].gsFrameNumber);
        gmFontStash.drawMultiLine(wrapStringFontStash(tempDispFrames, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*1);
        if (tempStringMargin + tempFontSize*1.03*(1+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(tempInPoint, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(1+distance));
        }
        if (tempStringMargin + tempFontSize*1.03*(2+distance) < _height) {
            gmFontStash.drawMultiLine(wrapStringFontStash(tempOutPoint, tempFontSize, (_width+tempStringMargin+tempStringMargin)), tempFontSize, _x + tempStringMargin, _y + tempStringMargin + tempFontSize*1.03*(2+distance));
        }
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
            for (int i=0; i<gmNumberOfStills; i++) {
                grabbedStill[i].unregisterMouseEvents();
            }
        }
    }
    
    void enableMouseEvents(){
        if (isMovieLoaded) {
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
            if (grabbedStill[i].gsImage.isAllocated()) {
                grabbedStill[i].gsImage.setFromPixels(gmMovie.getPixelsRef());
                grabbedStill[i].gsToBeGrabbed = FALSE;
            } else {
                ofLog(OF_LOG_VERBOSE, "CRASH AVOIDED grabbedStill[i].gsImage.isAllocated() FALSE _______________________________");
            }
        }
    }
    
    void drawStill(int i, float _x, float _y, float _w, float _h, float _alpha, bool _isBeingPrinted){
        
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
            if (!_isBeingPrinted) { // draw selection
                if (grabbedStill[i].gsRollOver) {
                    int tempSelectionWidth = 2;
                    ofRect(grabbedStill[i].gsX - tempSelectionWidth/2, grabbedStill[i].gsY - tempSelectionWidth/2, grabbedStill[i].gsDrawWidth + tempSelectionWidth, grabbedStill[i].gsDrawHeight + tempSelectionWidth);
                }
            }
            
            shader.begin(); // draw still with rounded corners
            shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
            grabbedStill[i].gsTexture.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight);
            shader.end();
            
            // draw update image
            if (grabbedStill[i].gsToBeGrabbed) {
                ofPushStyle();
                ofSetColor(255, 255, 255, 200);
//                updatingStill.draw(floor(grabbedStill[i].gsX), floor(grabbedStill[i].gsY), grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight);
                ofSetRectMode(OF_RECTMODE_CENTER);
//                if (grabbedStill[i].gsDrawWidth < 160) { // doesnt work right now
//                    updatingStill.drawSubsection(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth/2, grabbedStill[i].gsY + grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight, updatingStill.width/2 - grabbedStill[i].gsDrawWidth/4, updatingStill.height/2 - grabbedStill[i].gsDrawHeight/4, updatingStill.width/2, updatingStill.height/2);
//                } else {
                    updatingStill.drawSubsection(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth/2, grabbedStill[i].gsY + grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight, updatingStill.width/2 - grabbedStill[i].gsDrawWidth/2, updatingStill.height/2 - grabbedStill[i].gsDrawHeight/2);
//                }
                ofPopStyle();
            }

            if (gmShowFramesUI) { // drawing UI
                drawStillUI(i, grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawWidth, grabbedStill[i].gsDrawHeight, _alpha);
            }
            
            if (!_isBeingPrinted) { // drawing overlay graphics
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
                    if (ofGetModifierPressed(OF_KEY_SPECIAL)) {
                        frameBackward3Image.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else if(ofGetModifierPressed(OF_KEY_SHIFT)){
                        frameBackward2Image.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else {
                        frameBackwardImage.draw(grabbedStill[i].gsX, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    }
                    
                    ofSetColor(255, 50);
                    if (grabbedStill[i].gsRollOver2) {
                        ofSetColor(255);
                    }
                    if (ofGetModifierPressed(OF_KEY_SPECIAL)) {
                        frameForward3Image.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else if(ofGetModifierPressed(OF_KEY_SHIFT)){
                        frameForward2Image.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    } else {
                        frameForwardImage.draw(grabbedStill[i].gsX + grabbedStill[i].gsDrawWidth - grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsY, grabbedStill[i].gsDrawHeight/2, grabbedStill[i].gsDrawHeight/2);
                    }

                }
            }
            ofPopStyle();
            ofSetColor(255);
        }
    }
    
    void drawGmMoviePrint(float _x, float _y, int _gridColumns, float _gridMargin, float _scrollAmount, float _scaleFactor, float _alpha, bool _isBeingPrinted, bool _isActive){

        if (_isBeingPrinted) {
            gmSetTitleInfo = TRUE; //create new title size und umbruch
        }
        
        // draw all frames
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(FAK_ORANGECOLOR); // draw title rect
        ofRectRounded((_x + (gmGridWidth+_gridMargin)*0) * _scaleFactor, _y * _scaleFactor , gmGridWidth * _scaleFactor, gmGridHeight * _scaleFactor, gmGridWidth * _scaleFactor/64);
        ofSetColor(30, 30, 30, 255); // draw info rect
        ofRectRounded((_x + (gmGridWidth+_gridMargin)*1) * _scaleFactor, _y * _scaleFactor, ((gmGridWidth * (_gridColumns-1))+(_gridMargin * (_gridColumns-2))) * _scaleFactor, gmGridHeight * _scaleFactor, gmGridWidth * _scaleFactor/64);
        ofSetColor(255); // draw title and infos
        drawTitle((_x + (gmGridWidth+_gridMargin)*0) * _scaleFactor, _y * _scaleFactor, gmGridWidth * _scaleFactor, gmGridHeight * _scaleFactor, 1, _isBeingPrinted, TRUE);
        drawInfo1((_x + (gmGridWidth+_gridMargin)*1) * _scaleFactor, _y * _scaleFactor, gmGridWidth * _scaleFactor, gmGridHeight * _scaleFactor, 1, _isBeingPrinted, TRUE);
        drawInfo2((_x + (gmGridWidth+_gridMargin)*2) * _scaleFactor, _y * _scaleFactor, gmGridWidth * _scaleFactor, gmGridHeight * _scaleFactor, 1, _isBeingPrinted, TRUE);
        drawInfo3((_x + (gmGridWidth+_gridMargin)*3) * _scaleFactor, _y * _scaleFactor, gmGridWidth * _scaleFactor, gmGridHeight * _scaleFactor, 1, _isBeingPrinted, TRUE);
        
        ofSetColor(FAK_DARKORANGECOLOR); // draw mini timeline 
        int tempTimelineHeight = 5;
        ofRect((_x + (gmGridWidth+_gridMargin)*1) * _scaleFactor, (_y + gmGridHeight - tempTimelineHeight) * _scaleFactor, ((gmGridWidth * (_gridColumns-1))+(_gridMargin * (_gridColumns-2))) * _scaleFactor, tempTimelineHeight * _scaleFactor);
        ofSetColor(FAK_ORANGECOLOR); // draw mini timeline range
        ofRect(
            ofMap(grabbedStill[0].gsFrameNumber,
                  0,
                  gmTotalFrames-1,
                  (_x + (gmGridWidth+_gridMargin)*1) * _scaleFactor,
                  ((_x + (gmGridWidth+_gridMargin)*1) + (gmGridWidth * (_gridColumns-1))+(_gridMargin * (_gridColumns-2))) * _scaleFactor),
            (_y + gmGridHeight - tempTimelineHeight) * _scaleFactor,
            ofMap(grabbedStill[gmNumberOfStills-1].gsFrameNumber - grabbedStill[0].gsFrameNumber,
                  0,
                  gmTotalFrames-1,
                  0,
                  ((gmGridWidth * (_gridColumns-1))+(_gridMargin * (_gridColumns-2))) * _scaleFactor),
            tempTimelineHeight * _scaleFactor);

        for(int i=0; i<gmNumberOfStills; i++) // draw mini timeline stills positions
        {
            if (grabbedStill[i].gsManipulated) {
                ofSetColor(200, 200, 200, 255);
            } else {
                ofSetColor(FAK_MIDDLEDARKORANGECOLOR, 255);
            }
            ofRect(
                   ofMap(grabbedStill[i].gsFrameNumber,
                         0,
                         gmTotalFrames-1,
                         (_x + (gmGridWidth+_gridMargin)*1) * _scaleFactor,
                         ((_x + (gmGridWidth+_gridMargin)*1) + (gmGridWidth * (_gridColumns-1))+(_gridMargin * (_gridColumns-2))) * _scaleFactor),
                   (_y + gmGridHeight - tempTimelineHeight) * _scaleFactor,
                   1 * _scaleFactor,
                   tempTimelineHeight * _scaleFactor);
        }
        
        ofSetColor(255, 255, 255, 255); // draw stills
        for(int i=0; i<gmNumberOfStills; i++)
        {
            float tempY = ((gmGridHeight+_gridMargin)*((i+_gridColumns)/_gridColumns) + _y );
            drawStill(i, (_x + (gmGridWidth+_gridMargin)*(i%_gridColumns)) * _scaleFactor, tempY * _scaleFactor, gmGridWidth * _scaleFactor, gmGridHeight * _scaleFactor, 1, _isBeingPrinted);
        }
        
        
        // draw the inbetween frames linien
        if (!_isBeingPrinted) {
            for(int i=0; i<gmNumberOfStills-1; i++)
            {
                if (i%_gridColumns != _gridColumns-1) {
                    float tempY = ((gmGridHeight+_gridMargin)*((i+_gridColumns)/_gridColumns) + _y );
                    ofPushStyle();
                    ofColor tempColor(FAK_DARKORANGECOLOR);
                    ofSetColor(tempColor);
                    int tempHeight = (gmGridHeight - gmGridWidth/32) * _scaleFactor;
                    int deltaFrame = abs(grabbedStill[i+1].gsFrameNumber - grabbedStill[i].gsFrameNumber);
                    //                int mappedDelta = ofMap(deltaFrame, 0, 20, 0, 3, TRUE);
                    if (deltaFrame <= 1) {
                        ofRect((_x + (gmGridWidth+_gridMargin)*(i%_gridColumns) + gmGridWidth) * _scaleFactor, (tempY + gmGridHeight/2) * _scaleFactor - _gridMargin*2, _gridMargin * _scaleFactor, _gridMargin*4);
                    } else if (deltaFrame > 1 && deltaFrame <= 5){
                        ofRect((_x + (gmGridWidth+_gridMargin)*(i%_gridColumns) + gmGridWidth + _gridMargin/2) * _scaleFactor, (tempY + gmGridWidth/64) * _scaleFactor, 1 * _scaleFactor, tempHeight);
                    } else if (deltaFrame > 5 && deltaFrame <= 10){
                        ofRect((_x + (gmGridWidth+_gridMargin)*(i%_gridColumns) + gmGridWidth + (_gridMargin/3)) * _scaleFactor, (tempY + gmGridWidth/64) * _scaleFactor, 1 * _scaleFactor, tempHeight);
                        ofRect((_x + (gmGridWidth+_gridMargin)*(i%_gridColumns) + gmGridWidth + (_gridMargin/3)*2) * _scaleFactor, (tempY + gmGridWidth/64) * _scaleFactor, 1 * _scaleFactor, tempHeight);
                    } else if (deltaFrame > 10){
                        //                    int mappedBrightness = ofMap(deltaFrame, 0, gmTotalFrames, 100, 200);
                        //                    ofSetColor(mappedBrightness);
                        //                    tempColor.setBrightness(mappedBrightness);
                        //                    ofSetColor(tempColor);
                        ofRect((_x + (gmGridWidth+_gridMargin)*(i%_gridColumns) + gmGridWidth + (_gridMargin/4)) * _scaleFactor, (tempY + gmGridWidth/64) * _scaleFactor, 1 * _scaleFactor, tempHeight);
                        ofRect((_x + (gmGridWidth+_gridMargin)*(i%_gridColumns) + gmGridWidth + (_gridMargin/4)*2) * _scaleFactor, (tempY + gmGridWidth/64) * _scaleFactor, 1 * _scaleFactor, tempHeight);
                        ofRect((_x + (gmGridWidth+_gridMargin)*(i%_gridColumns) + gmGridWidth + (_gridMargin/4)*3) * _scaleFactor, (tempY + gmGridWidth/64) * _scaleFactor, 1 * _scaleFactor, tempHeight);
                    }
                    ofPopStyle();
                }
            }
            
        }
        
        if (_isBeingPrinted) {
            gmSetTitleInfo = TRUE; //create new title size und umbruch
        }
        
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
            
            ofRectangle rect = gmFontStash.getBoundingBoxSize(dummyString, tempFontSize, 0, 0);
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
            gmFontStash.drawMultiLine(dummyString, tempFontSize, x + rect.width*0.015, y+rect.height + rect.height*0.15);
            
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
    
    vector<fakGrabbedStill> grabbedStill;
    
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
    int gmGridWidth;
    int gmGridHeight;
    bool gmShowFramesUI;
    
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
    
    ofxFontStash gmFontStash;
    ofxFontStash gmFontStashFranchise;
    int tempFontSize[24] = {6, 10, 14, 18, 22, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 80, 92, 108, 128, 256, 300};

    int franchiseFontRightSize;
    int stringMargin;
    ofFilePath gmLoadedFilePath;
    bool gmSetTitleInfo;
    string gmInfCol1, gmInfCol2, gmInfCol3;
    string gmMIFileName, gmMIFileExtension, gmMIFormat, gmMIFormatString, gmMIFileSizeString, gmMIDurationString1, gmMIFrameCount, gmMIWidth, gmMIHeight, gmMIDisplayAspectRatioString, gmMIFrameRateString;
    string gmMIVFormat, gmMIFormatInfo, gmMIBitRate, gmMIPixelAspectRatio, gmMIDisplayAspectRatio, gmMIFrameRate_ModeString, gmMIColorSpace, gmMIChromaSubsampling, gmMIBitDepthString, gmMIInterlacementString;
    string gmMIAFormat, gmMIAChannelsString, gmMIASamplingRate;

    // used for rounded corner mask
    ofShader shader;
    ofFbo maskFbo;
    
};

#endif /* defined(__fakBreakupVideo__fakGrabbedMovie__) */
