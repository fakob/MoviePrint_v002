#pragma once

#include "ofMain.h"
#include "fakGrabbedMovie.h"
#include "fakGrabbedMovieStill.h"
#include "ofxUI.h"
#include "fakMemory.h"
#include "fakGrabbedListItem.h"
#include "fakGrabbedList.h"
#include "ofxNotify.h"
#include "fakScrollBar.h"
#include "ofxFontStash.h"
#include "boost/any.hpp"
#include "ofxMSATimer.h"
//#include "ofxModifierKeys.h"
#include <iostream>
#include "fakMenu.h"
#include "ofxTween.h"


class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(double x, double y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void guiEvent(ofxUIEventArgs &e);
    
    //--------------------------------------------------------------
    struct moviePrintDataStruct {
        int * gridTimeArray;
        int printGridColumns;
        int printGridRows;
        int printGridMargin;
        bool printDisplayVideoAudioInfo;
        int printDisplayTimecodeFramesOff; // 0=off, 1=timecode, 2=frames
        bool printSingleFrames;
        ofImageFormat printFormat;
        int printSizeWidth;
        
//        ~moviePrintDataStruct()
//        {
//            if (gridTimeArray) delete[] gridTimeArray;
//        }
    };
    
    moviePrintDataStruct moviePrintDataSet;
    deque<moviePrintDataStruct> previousMoviePrintDataSet;
    int undoPosition;
    int maxUndoSteps;

    
    void updateAllStills();
    void calculateNewPrintGrid();
    void calculateNewPrintSize();
    void updateDisplayGrid();
    bool fequal(float _x, float _y, float _t);
    void drawUI(int _scaleFactor, bool _hideInPNG);
    void drawDisplayGrid(float _scaleFactor, bool _hideInPNG, bool _isBeingPrinted, float _scrollAmountRel, bool _showPlaceHolder);
    void drawMoviePrintPreview(float _scaleFactor, bool _showPlaceHolder);
    void drawList(float _scrollAmountRel);
    void drawScrubScreen(float _scaleFactor);
    void printImageToFile(int _printSizeWidth);
    void printListToFile();
    void listClick(int & i);
    void listEnter(int & i);
    void loadNewMovie(string _newMoviePath, bool _wholeRange, bool _loadInBackground, bool _loadScrubMovie);
    void setResourcePath();
    void moveToMovie();
    void moveToList();
    void scrollEvent(ofVec2f & e);
    void setGUITimeline();
    void setGUIMovieInfo();
    void setGUISettings();
    void setGUIMoviePrintPreview();
    void setGUISettingsMoviePrint();
    void setGUIHelp1();
    void resetItemsToPrint();
    void updateTimeline();
    void updateTheScrollBar();
    void updateTheListScrollBar();
    void updateAllLimits();
    void writeFboToPreview(float _scaleFactor, bool _showPlaceHolder);
    void drawMovieInfo(float _x, float _y, float _fade);
    void drawHelp(float _x, float _y, float _fade);
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void toggleMoviePrintPreview();
    void setVisibilityMoviePrintPreview(bool _visibility);
    void menuIsOpened(int &e);
    void menuIsClosed(int &e);
    void menuIsClicked(int &e);
    void inactivateAllMenus();
    void activateAllMenus();
    void closeAllMenus();
    void startPrinting();
    void stopPrinting();
    void startListPrinting();
    void stopListPrinting();
    void handlingEventOverlays();
    
    void applyMoviePrintDataSet(moviePrintDataStruct _newMoviePrintDataSet);
    bool hasChangedMoviePrintDataSet();
    void addMoviePrintDataSet(int _undoPosition);
    void addGridTimeArrayToMoviePrintDataSet();
    void undoStep();
    void redoStep();

    void updateTimeSlider(bool _wholeRange);
    void drawPrintScreen();
    void drawLoadMovieScreen();
    void drawUpdateScreen();
    void drawStartScreen();
    void drawResizeScreen();
    void moveInOutTimeline();
    void setInPoint(int _inPoint);
    void setOutPoint(int _outPoint);
    void updateOneThumb(int _thumbID, int _newFrameNumber);
    bool checkExtension(string _tempExtension);
    bool writeMoviePrint;
  
    string wrapString(string _tempString, int _tempWidth, int _tempHeight);
    string StringToUpper(string strToConvert);
    string cropFrontOfString(string _inputString, int _length, string _substitute = "");

    
    // Movie
    fakGrabbedMovie loadedMovie;
    vector<string> stringMovieInfo;
    vector<string> stringMovieData;

    
    // Images
    ofImage startImage;
    ofImage dropZoneImage;
    ofImage backgroundImage;
    ofImage printImage;
    ofImage printListImage;
    ofImage loadMovieImage;
    ofImage updatingImage;
    ofImage inPointImage;
    ofImage outPointImage;
    ofImage layoutHeaderImage;
    ofImage helpMenuImage;
    ofImage backgroundImagePreview;

    bool superKeyPressed = FALSE;
    bool shiftKeyPressed = FALSE;
    
    bool threadIsRunning;
    
    bool addToUndo;
    
    bool showPrintScreen;
    bool finishedPrinting;
    bool showLoadMovieScreen;
    bool finishedLoadingMovie;
    bool showUpdateScreen;
    bool finishedUpdating;
    bool showTimeline;
    bool finishedTimeline;
    bool switchFromLogoToDropZone;
    
    bool drawNotify;
    
    bool showMenu;
    bool showTopMenu;
    
    bool setupFinished;
    bool manipulateSlider; // true when slider event
    bool updateInOut; // manipulated slider plus fading
    bool updateScrub; // scrubbing plus fading
    bool updateDropped;
    bool showListView;
    bool showMovieView;
    bool lockedDueToInteraction;
    bool lockedDueToPrinting;
    bool updateNewPrintGrid;
    bool updateMovieFromList;
    
    bool updateGridTimeArrayToMoviePrintDataSet;
    
    bool scrollGrid;
    bool scrollList;

    bool movieLoading;
    bool movieIsBeingGrabbed;
    bool movieProperlyLoaded;
    
    bool updateMovieFromDrop;
    bool currPrintingList;
    bool allMenusAreClosed;
    int allMenusAreClosedOnce;
    
    int currentKey = -1;
    
    
    int totalFrames;
    
    // ofxUI Design
    ofxUICanvas *guiTimeline;
    ofxUICanvas *guiSettingsMoviePrint;
    ofxUICanvas *guiHelp1;
    
    // ofxUICanvas *guiTimeline
    ofxUIRangeSlider *uiRangeSliderTimeline;
    
    // ofxUICanvas *guiMovieInfo
    ofxUITextArea *gmMIFileName;
    ofxUITextArea *gmMIFilePath;
    ofxUILabel *gmMIFormat;
    ofxUILabel *gmMIFormatString;
    ofxUILabel *gmMIFileSizeString;
    ofxUILabel *gmMIDurationString1;
    ofxUILabel *gmMIFrameCount;
    ofxUILabel *gmMIWidth;
    ofxUILabel *gmMIDisplayAspectRatioString;
    ofxUILabel *gmMIFrameRateString;
    ofxUILabel *gmMIVFormat;
    ofxUILabel *gmMIFormatInfo;
    ofxUILabel *gmMIBitRate;
    ofxUILabel *gmMIPixelAspectRatio;
    ofxUILabel *gmMIDisplayAspectRatio;
    ofxUILabel *gmMIFrameRate_ModeString;
    ofxUILabel *gmMIColorSpace;
    ofxUILabel *gmMIChromaSubsampling;
    ofxUILabel *gmMIBitDepthString;
    ofxUILabel *gmMIInterlacementString;
    ofxUILabel *gmMIAFormat;
    ofxUILabel *gmMIAChannelsString;
    ofxUILabel *gmMIASamplingRate;
    
    // ofxUICanvas *guiSettings1
    
    // ofxUICanvas *guiSettingsMoviePrint
    ofxUIIntSlider *uiSliderPrintRows;
    ofxUIIntSlider *uiSliderPrintColumns;
//    ofxUIIntSlider *uiSliderThumbWidth;
    ofxUIIntSlider *uiSliderPrintMargin;
    ofxUIToggle *uiToggleHeaderDisplay;
    ofxUIToggle *uiToggleSingleFrames;
    ofxUIRadio *uiRadioSetFrameDisplay;
    ofxUIRadio *uiRadioSetFitManually;
    ofxUIIntSlider *uiSliderNumberOfThumbs;
    ofxUIDropDownList *uiDropDownListPrintOutputFormat;
    ofxUIDropDownList *uiDropDownlistPrintOutputWidth;
    ofxUIRadio *uiRadioPrintOutputFormat;
    ofxUIRadio *uiRadioPrintOutputWidth;
    ofxUILabel *uiLabelOutputFolder;


    
    // ofxUICanvas *guiHelp1
    ofxUITextArea *helpText;

    
    ofxTween tweenTimelineInOut;
    ofxTween tweenListInOut;
    ofxTween tweenMoviePrintPreview;
    ofxTween tweenBlendStartDropImage;
    ofxTween tweenBlendStartDropImageCounter;
    ofxTween tweenFading;
    ofxTween tweenTimeDelay;
    
    ofxEasingBack 	easingback;
    ofxEasingBounce 	easingbounce;
    ofxEasingCirc 	easingcirc;
    ofxEasingCubic 	easingcubic;
    ofxEasingElastic easingelastic;
    ofxEasingExpo 	easingexpo;
    ofxEasingLinear 	easinglinear;
    ofxEasingQuad 	easingquad;
    ofxEasingQuart 	easingquart;
    ofxEasingQuint 	easingquint;
    ofxEasingSine	easingsine;
    
    
    bool showPlaceHolder;
    bool showFBO;
    int fboToPreviewWidth;
    int fboToPreviewHeight;
    
    // UI Design settings
    bool drawPadding;
    int leftMargin, topMargin, rightMargin, bottomMargin;
    int headerHeight;
    int footerHeight;
    int displayGridMargin;
    int scrollBarWidth;
    int scrollBarMargin;
    int loaderBarHeight;
    int timeSliderHeight;
    int gridColumns;
    int gridRows;
    int thumbWidth;
    int thumbHeight;
    int displayGridWidth;
    int displayGridHeight;
    int printGridWidth;
    int printGridHeight;
    int gridNumber;
    int numberOfStills;
    int menuWidth;
    int listWidth;
    float scrubWindowW, scrubWindowH;
    int printHeaderHeight;
    
    // Menu
    fakMenu menuMovieInfo;
    fakMenu menuMoviePrintSettings;
    fakMenu menuHelp;
    fakMenu menuTimeline;
    fakMenu menuMoveToList;
    
    ofxFontStash fontStashHelveticaLight;
    ofxFontStash fontStashHelveticaMedium;

    
    // Printing
    float printScale;
    ofFbo fboToPreview;
    bool printHideInPrint;
    int printNumberOfThumbs;
    bool showMoviePrintPreview;

    // draw List
    string loadedFile;
    ofFilePath loadedFilePath;
    vector<ofFile> droppedFiles;
    fakGrabbedList droppedList;
    bool printListNotImage;
    int itemToPrint;
    
    //Grid Setup
    bool displayGridSetWithColumnsAndRows;
    bool printGridSetWithColumnsAndRows;
    
    
    // scroll Bars
    fakScrollBar scrollBar;
    fakScrollBar scrollBarList;
    float scrollAmountRel;
    float scrollListAmountRel;
    float scrollMultiplier;
       
    // Timer
    ofxMSATimer timer;

    // Once Counter
    int windowResizedOnce;
    bool windowWasResized;
    int updateOnce;
    int counterToUpdate;
    int counterToLoad;
    int counterToPrint;

    // scrubbing
    int scrubWindowGridNumber;
    float scrubDelta;
    float   uiSliderValueLow;
    float   uiSliderValueHigh;
    int     uiID;
    bool loadNewMovieToBeScrubbedBool;
    
    // roll over
    void rollOverButtonsClicked(int _rollOverMovieID, int _rollOverMovieButtonID);
    bool rollOverClicked;
    int rollOverMovieID;
    int rollOverMovieButtonID;
    
    // loader
    float   loadValue; // loader value from 0 to 1
    
    fakMemory myMemory;
    
    string appPathStr;
    string appPathUpStr;
    string saveMoviePrintPath;
    
    bool devTurnOffMovieSwitch = FALSE;
    
    int possPrintSizes [9] = {1024, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 9216};
    
    // possible Still Resolutions Width for 16 by 9
    int possStillResWidth169 [631] = {228, 236, 256, 264, 292, 300, 320, 328, 356, 364, 384, 392, 420, 428, 448, 456, 484, 492, 512, 520, 548, 556, 576, 584, 612, 620, 640, 648, 676, 684, 704, 712, 740, 748, 768, 776, 804, 812, 832, 840, 868, 876, 896, 904, 932, 940, 960, 968, 996, 1004, 1024, 1032, 1060, 1068, 1088, 1096, 1124, 1132, 1152, 1160, 1188, 1196, 1216, 1224, 1252, 1260, 1280, 1288, 1316, 1324, 1344, 1352, 1380, 1388, 1408, 1416, 1444, 1452, 1472, 1480, 1508, 1516, 1536, 1544, 1572, 1580, 1600, 1608, 1636, 1644, 1664, 1672, 1700, 1708, 1728, 1736, 1764, 1772, 1792, 1800, 1828, 1836, 1856, 1864, 1892, 1900, 1920, 1928, 1956, 1964, 1984, 1992, 2020, 2028, 2048, 2056, 2084, 2092, 2112, 2120, 2148, 2156, 2176, 2184, 2212, 2220, 2240, 2248, 2276, 2284, 2304, 2312, 2340, 2348, 2368, 2376, 2404, 2412, 2432, 2440, 2468, 2476, 2496, 2504, 2532, 2540, 2560};
    // possible Still Resolutions Width for 4 by 3
    int possStillResWidth43 [631] = {208, 224, 240, 256, 272, 288, 304, 320, 336, 352, 368, 384, 400, 416, 432, 448, 464, 480, 496, 512, 528, 544, 560, 576, 592, 608, 624, 640, 656, 672, 688, 704, 720, 736, 752, 768, 784, 800, 816, 832, 848, 864, 880, 896, 912, 928, 944, 960, 976, 992, 1008, 1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 1184, 1200, 1216, 1232, 1248, 1264, 1280, 1296, 1312, 1328, 1344, 1360, 1376, 1392, 1408, 1424, 1440, 1456, 1472, 1488, 1504, 1520, 1536, 1552, 1568, 1584, 1600, 1616, 1632, 1648, 1664, 1680, 1696, 1712, 1728, 1744, 1760, 1776, 1792, 1808, 1824, 1840, 1856, 1872, 1888, 1904, 1920, 1936, 1952, 1968, 1984, 2000, 2016, 2032, 2048, 2064, 2080, 2096, 2112, 2128, 2144, 2160, 2176, 2192, 2208, 2224, 2240, 2256, 2272, 2288, 2304, 2320, 2336, 2352, 2368, 2384, 2400, 2416, 2432, 2448, 2464, 2480, 2496, 2512, 2528, 2544, 2560};
    
//    // possible Still Resolutions Width for 16 by 9
//    int possStillResWidth169 [637] = {108, 128, 136, 164, 172, 192, 200, 228, 236, 256, 264, 292, 300, 320, 328, 356, 364, 384, 392, 420, 428, 448, 456, 484, 492, 512, 520, 548, 556, 576, 584, 612, 620, 640, 648, 676, 684, 704, 712, 740, 748, 768, 776, 804, 812, 832, 840, 868, 876, 896, 904, 932, 940, 960, 968, 996, 1004, 1024, 1032, 1060, 1068, 1088, 1096, 1124, 1132, 1152, 1160, 1188, 1196, 1216, 1224, 1252, 1260, 1280, 1288, 1316, 1324, 1344, 1352, 1380, 1388, 1408, 1416, 1444, 1452, 1472, 1480, 1508, 1516, 1536, 1544, 1572, 1580, 1600, 1608, 1636, 1644, 1664, 1672, 1700, 1708, 1728, 1736, 1764, 1772, 1792, 1800, 1828, 1836, 1856, 1864, 1892, 1900, 1920, 1928, 1956, 1964, 1984, 1992, 2020, 2028, 2048, 2056, 2084, 2092, 2112, 2120, 2148, 2156, 2176, 2184, 2212, 2220, 2240, 2248, 2276, 2284, 2304, 2312, 2340, 2348, 2368, 2376, 2404, 2412, 2432, 2440, 2468, 2476, 2496, 2504, 2532, 2540, 2560};
//    // possible Still Resolutions Width for 4 by 3
//    int possStillResWidth43 [637] = {96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256, 272, 288, 304, 320, 336, 352, 368, 384, 400, 416, 432, 448, 464, 480, 496, 512, 528, 544, 560, 576, 592, 608, 624, 640, 656, 672, 688, 704, 720, 736, 752, 768, 784, 800, 816, 832, 848, 864, 880, 896, 912, 928, 944, 960, 976, 992, 1008, 1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 1184, 1200, 1216, 1232, 1248, 1264, 1280, 1296, 1312, 1328, 1344, 1360, 1376, 1392, 1408, 1424, 1440, 1456, 1472, 1488, 1504, 1520, 1536, 1552, 1568, 1584, 1600, 1616, 1632, 1648, 1664, 1680, 1696, 1712, 1728, 1744, 1760, 1776, 1792, 1808, 1824, 1840, 1856, 1872, 1888, 1904, 1920, 1936, 1952, 1968, 1984, 2000, 2016, 2032, 2048, 2064, 2080, 2096, 2112, 2128, 2144, 2160, 2176, 2192, 2208, 2224, 2240, 2256, 2272, 2288, 2304, 2320, 2336, 2352, 2368, 2384, 2400, 2416, 2432, 2448, 2464, 2480, 2496, 2512, 2528, 2544, 2560};
    

};