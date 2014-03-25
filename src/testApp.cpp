#include "testApp.h"
#include "ofxXmlSettings.h"

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

//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    drawNotify = false; // ofxNotify
    showPlaceHolder = false; // added for developing
    
    setResourcePath();
    
    setupFinished = FALSE;
    updateNewPrintGrid = FALSE;
    showListView = FALSE;
    lockedDueToInteraction = false;
    lockedDueToPrinting = false;
    updateMovieFromList = FALSE;
    printListNotImage = FALSE;
    currPrintingList = FALSE;
    movieLoading = FALSE;
    movieIsBeingGrabbed = FALSE;
    movieProperlyLoaded = TRUE;
    scrollGrid = FALSE;
    scrollList = FALSE;
    windowResizedOnce = 0;
    updateOnce = 0;
    ofBackground(30, 30, 30);
    ofSetFrameRate(60);
    loadValue = 0;
    scrubWindowW = 640;
    scrubWindowGridNumber = 0;
    scrubDelta = 0;
    totalFrames = 0;
    itemToPrint = 0;
    loadNewMovieToBeScrubbedBool = FALSE;
    windowWasResized = false;
    allMenusAreClosed = true;
    allMenusAreClosedOnce = 0;
    switchFromLogoToDropZone = false;
    
    updateGridTimeArrayToMoviePrintDataSet = false;
    
    // UI Values
    leftMargin = 5;
    rightMargin = 5;
    topMargin = 5;
    bottomMargin = 5;
    headerHeight = 40;
    footerHeight = 40;
    displayGridMargin = 5;
    loaderBarHeight = 20;
    timeSliderHeight = 10;
    gridColumns = 5;
    gridRows = 4;
    printNumberOfThumbs = 20;
    menuWidth = 255;
    listWidth = 1320;

    moviePrintDataSet.printGridColumns = 4;
    moviePrintDataSet.printGridRows = 5;
    moviePrintDataSet.printGridMargin = 5;
    moviePrintDataSet.printDisplayVideoAudioInfo = true;
    moviePrintDataSet.printDisplayTimecodeFramesOff = 0;
    moviePrintDataSet.printSingleFrames = false;
    moviePrintDataSet.printFormat = OF_IMAGE_FORMAT_PNG;
    moviePrintDataSet.printSizeWidth = 1024;
    
    
    threadIsRunning = FALSE;
    
    addToUndo = false;
    undoPosition = 0;
    maxUndoSteps = 10 + 1;
    
    showPrintScreen = FALSE;
    finishedPrinting = TRUE;
    writeMoviePrint = false;
    
    showLoadMovieScreen = FALSE;
    finishedLoadingMovie = TRUE;
    showUpdateScreen = FALSE;
    finishedUpdating = TRUE;
    showTimeline = FALSE;
    finishedTimeline = TRUE;
    
    updateMovieFromDrop = FALSE;
    
    rollOverClicked = FALSE;
    
    showFBO = FALSE;
    
    gridRows = 6;
    displayGridSetWithColumnsAndRows = false;
    printGridSetWithColumnsAndRows = true;
    showMoviePrintPreview = false;
    
    thumbWidth = 256;
    thumbHeight = 144;
    printHeaderHeight = thumbHeight/4;
    scrollBarWidth = 12;
    scrollBarMargin = 2;
    scrollAmountRel = 0;
    scrollListAmountRel = 0;

    
    fboToPreviewWidth = 1320 - leftMargin - rightMargin - thumbWidth*2 - displayGridMargin*2;
    fboToPreviewHeight = 660 - headerHeight - footerHeight;
    
    fboToPreview.allocate(fboToPreviewWidth, fboToPreviewHeight, GL_RGBA);
    
    counterToUpdate = 0;
    counterToLoad = 0;
    counterToPrint = 0;
    
    showMenu = FALSE;
    showTopMenu = false;
    tweenTimelineInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));
    tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));
    tweenBlendStartDropImageCounter.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,100,0);


    startImage.loadImage("MoviePrint_StartBildschirm_v002_00000.png");
    dropZoneImage.loadImage("MoviePrint_DropZone_v002_00000.png");
    backgroundImage.loadImage("MoviePrint_Background_v002_00000.jpg");
    printImage.loadImage("MoviePrint_Print_v002_00000.png");
    printListImage.loadImage("MoviePrint_PrintList_v002_00000.png");
    loadMovieImage.loadImage("MoviePrint_Loading_v002_00000.png");
    updatingImage.loadImage("MoviePrint_Updating_v002_00000.png");
    inPointImage.loadImage("MoviePrint_InPoint_v001_00000.png");
    outPointImage.loadImage("MoviePrint_OutPoint_v001_00000.png");
    layoutHeaderImage.loadImage("MoviePrint_Layout_Header_v001_00000.png");
    helpMenuImage.loadImage("HelpMenu_v001.png");
    backgroundImagePreview.loadImage("MoviePrint_PreviewBackground_v001_00000.png");
    
    fontStashHelveticaLight.setup("HelveticaNeueLTCom-Lt.ttf");
    fontStashHelveticaMedium.setup("HelveticaNeueLTCom-Md.ttf");

    
    // load standard movie
    loadedFile = "Nothing";
    saveMoviePrintPath = appPathUpStr + "/MoviePrints/";

    loadedMovie.gmUpperLimitY = headerHeight;
    loadedMovie.gmLowerLimitY = ofGetHeight() - footerHeight;
    loadedMovie.gmLeftLimitX = leftMargin;
    loadedMovie.gmRightLimitX = ofGetWidth() - rightMargin;
    calculateNewPrintGrid();
    loadedMovie.setup(loadedFile, numberOfStills, thumbWidth, thumbHeight, showPlaceHolder);
    totalFrames = 100;
    loadedMovie.allocateNewNumberOfStills(numberOfStills, thumbWidth, thumbHeight, showPlaceHolder, false);

    updateInOut = FALSE;
    manipulateSlider = FALSE;
    updateDropped = FALSE;
    
    scrollMultiplier = 50.0;
    scrollBar.setup(0, ofGetWindowWidth(), ofGetWindowHeight(), headerHeight + topMargin, footerHeight/2 + bottomMargin, scrollBarWidth, 16, scrollMultiplier, scrollBarMargin);

    scrollBar.setScrollHeight((float)displayGridHeight);
    scrollBar.registerMouseEvents();
    scrollBar.registerTouchEvents();
    ofAddListener(scrollBar.sbScrollingGoingOn, this, &testApp::scrollEvent);

    scrollBarList.setup(0, ofGetWindowWidth(), ofGetWindowHeight(), headerHeight + topMargin, footerHeight/2 + bottomMargin, scrollBarWidth, 16, scrollMultiplier, scrollBarMargin);
    scrollBarList.registerMouseEvents();
    scrollBarList.registerTouchEvents();
    ofAddListener(scrollBarList.sbScrollingGoingOn, this, &testApp::scrollEvent);
    
    setGUITimeline();
    setGUISettings();
    setGUISettingsMoviePrint();
    guiSettingsMoviePrint->loadSettings("guiMoviePrintSettings.xml");


    menuMovieInfo.setupMenu(1,0,0,0,0,headerHeight, true, 'T', false);
    menuMovieInfo.registerMouseEvents();
    ofAddListener(menuMovieInfo.mMenuIsBeingOpened, this, &testApp::menuIsOpened);
    ofAddListener(menuMovieInfo.mMenuIsBeingClosed, this, &testApp::menuIsClosed);

    menuMoviePrintSettings.setupMenu(5,0,0,0,0,headerHeight, true, 'T', false);
    menuMoviePrintSettings.registerMouseEvents();
    ofAddListener(menuMoviePrintSettings.mMenuIsBeingOpened, this, &testApp::menuIsOpened);
    ofAddListener(menuMoviePrintSettings.mMenuIsBeingClosed, this, &testApp::menuIsClosed);
    
    menuSettings.setupMenu(3,0,0,0,0,headerHeight, true, 'T', false);
    menuSettings.registerMouseEvents();
    ofAddListener(menuSettings.mMenuIsBeingOpened, this, &testApp::menuIsOpened);
    ofAddListener(menuSettings.mMenuIsBeingClosed, this, &testApp::menuIsClosed);
    
    menuHelp.setupMenu(2,0,0,0,0,headerHeight, true, 'T', false);
    menuHelp.registerMouseEvents();
    ofAddListener(menuHelp.mMenuIsBeingOpened, this, &testApp::menuIsOpened);
    ofAddListener(menuHelp.mMenuIsBeingClosed, this, &testApp::menuIsClosed);
    
    menuTimeline.setupMenu(0,0,0,0,0,footerHeight/2, true, 'B', false);
    menuTimeline.registerMouseEvents();
    
    menuMoveToList.setupMenu(6,0,0,0,0,leftMargin*2, true, 'L', false);
    menuMoveToList.registerMouseEvents();
    ofAddListener(menuMoveToList.mMenuIsBeingClicked, this, &testApp::menuIsClicked);
    
    moveInOutTimeline();

    
    previousMoviePrintDataSet.clear();
    addMoviePrintDataSet(undoPosition); // add loaded settings as first undo step

    setupFinished = TRUE;

}

//--------------------------------------------------------------
void testApp::setGUITimeline(){
    
    // setup gui
    guiTimeline = new ofxUICanvas(ofGetWidth()/2-displayGridWidth/2-OFX_UI_GLOBAL_WIDGET_SPACING, ofGetHeight()-(footerHeight/2) - timeSliderHeight/2, ofGetWidth(),footerHeight); //ofxUICanvas(float x, float y, float width, float height)
    drawPadding = FALSE;
    guiTimeline->setDrawBack(FALSE);

	guiTimeline->addWidgetDown(new ofxUIRangeSlider("RSLIDER", 0.0, (float)totalFrames, 0.0, 100.0, displayGridWidth, timeSliderHeight));
    uiRangeSliderTimeline = (ofxUIRangeSlider *) guiTimeline->getWidget("RSLIDER");
    uiRangeSliderTimeline->setLabelPrecision(0);
    uiRangeSliderTimeline->setLabelVisible(FALSE);

    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_OUTLINE, ofColor(0,0,0,0));
    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_OUTLINE_HIGHLIGHT, ofColor(0,0,0,0));
    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL, FAK_ORANGE1);
    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL_HIGHLIGHT, FAK_ORANGE1);
    
    uiRangeSliderTimeline->setColorBack(FAK_ORANGE5);

    uiRangeSliderTimeline->setColorPadded(FAK_ORANGE5);

    ofAddListener(guiTimeline->newGUIEvent, this, &testApp::guiEvent);

}

//--------------------------------------------------------------
void testApp::setGUISettingsMoviePrint(){
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING*2;
    float length = menuWidth-xInit;
    
    guiSettingsMoviePrint = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    guiSettingsMoviePrint->setFont("HelveticaNeueLTCom-LtCn.ttf");
    guiSettingsMoviePrint->setWidgetSpacing(10);
    
    guiSettingsMoviePrint->addLabelButton("Select Output Folder", false,length-xInit,dim);
    guiSettingsMoviePrint->addLabel("SelectedOutputFolder", saveMoviePrintPath, OFX_UI_FONT_SMALL);
    uiLabelOutputFolder = (ofxUILabel *) guiSettingsMoviePrint->getWidget("SelectedOutputFolder");
    uiLabelOutputFolder->setLabel(cropFrontOfString(saveMoviePrintPath, 40, "..."));

    guiSettingsMoviePrint->addSpacer(length-xInit, 1);
//    guiSettingsMoviePrint->addLabel("SET RASTER", OFX_UI_FONT_MEDIUM);
	guiSettingsMoviePrint->addIntSlider("PrintColumns", 1, 10, &moviePrintDataSet.printGridColumns, length-xInit,dim);
	guiSettingsMoviePrint->addIntSlider("PrintRows", 1, 20, &moviePrintDataSet.printGridRows, length-xInit,dim);
    uiSliderPrintColumns = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintColumns");
    uiSliderPrintRows = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintRows");
    guiSettingsMoviePrint->addSpacer(length-xInit, 0);
   	guiSettingsMoviePrint->addIntSlider("PrintMargin", 0, 30, &moviePrintDataSet.printGridMargin, length-xInit,dim);
    uiSliderPrintMargin = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintMargin");
    
    guiSettingsMoviePrint->addSpacer(length-xInit, 1);
//    guiSettingsMoviePrint->addLabel("SHOW INFO", OFX_UI_FONT_MEDIUM);

    guiSettingsMoviePrint->addToggle("Display Header", &moviePrintDataSet.printDisplayVideoAudioInfo, dim*1.5, dim);
    uiToggleHeaderDisplay = (ofxUIToggle *) guiSettingsMoviePrint->getWidget("Display Header");
    
    vector<string> names;
	names.push_back("Display Frames");
	names.push_back("Display TimeCode");
	names.push_back("off");
    uiRadioSetFrameDisplay = guiSettingsMoviePrint->addRadio("RADIO_HORIZONTAL", names, OFX_UI_ORIENTATION_VERTICAL, dim*1.5, dim);

    guiSettingsMoviePrint->addSpacer(length-xInit, 1);

    guiSettingsMoviePrint->addToggle("Save also individual frames", &moviePrintDataSet.printSingleFrames, dim*1.5, dim);
    uiToggleSingleFrames = (ofxUIToggle *) guiSettingsMoviePrint->getWidget("Save also individual frames");

    guiSettingsMoviePrint->addSpacer(length-xInit, 1);

//    guiSettingsMoviePrint->addLabel("Choose Output Format", OFX_UI_FONT_MEDIUM);
    vector<string> names3;
    names3.push_back("png with alpha");
    names3.push_back("jpg");
//    names3.push_back("gif");
    guiSettingsMoviePrint->addRadio("Choose Output Format", names3, OFX_UI_ORIENTATION_VERTICAL, dim*1.5, dim);
    uiRadioPrintOutputFormat =(ofxUIRadio *) guiSettingsMoviePrint->getWidget("Choose Output Format");
    
    vector<string> names4;
    names4.push_back("1024px width");
    names4.push_back("2048px width");
    names4.push_back("3072px width");
    names4.push_back("4096px width");
    guiSettingsMoviePrint->addRadio("MoviePrint Width", names4, OFX_UI_ORIENTATION_VERTICAL, dim*1.5, dim);
    uiRadioPrintOutputWidth =(ofxUIRadio *) guiSettingsMoviePrint->getWidget("MoviePrint Width");
    
    guiSettingsMoviePrint->addSpacer(length-xInit, 1);
    guiSettingsMoviePrint->addLabelButton("Save MoviePrint", false,length-xInit,dim);

    
    guiSettingsMoviePrint->setColorBack(FAK_TRANSPARENT);
	ofAddListener(guiSettingsMoviePrint->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::setGUISettings(){
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING*2;
    float length = menuWidth-xInit;
    
    guiSettings = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    guiSettings->setFont("HelveticaNeueLTCom-LtCn.ttf");
    guiSettings->setWidgetSpacing(10);

    guiSettings->addLabelButton("Refresh", false,length-xInit,dim);

    guiSettings->addLabelButton("Undo", false,length/2-xInit*1,dim);
    guiSettings->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiSettings->addLabelButton("Redo", false,length/2-OFX_UI_GLOBAL_WIDGET_SPACING*3,dim);
    guiSettings->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    uiButtonUndo =(ofxUIButton *) guiSettings->getWidget("Undo");
    uiButtonRedo =(ofxUIButton *) guiSettings->getWidget("Redo");
    
    guiSettings->addLabelButton("Show MoviePrint Preview", false,length-xInit,dim);
    
    guiSettings->setColorBack(FAK_TRANSPARENT);
	ofAddListener(guiSettings->newGUIEvent,this,&testApp::guiEvent);


}

//--------------------------------------------------------------
void testApp::calculateNewPrintSize(){
    printGridWidth = (thumbWidth + moviePrintDataSet.printGridMargin) * moviePrintDataSet.printGridColumns + moviePrintDataSet.printGridMargin;
    if (moviePrintDataSet.printDisplayVideoAudioInfo) {
        printGridHeight = (thumbHeight + moviePrintDataSet.printGridMargin) * moviePrintDataSet.printGridRows + moviePrintDataSet.printGridMargin + printHeaderHeight;
    } else {
        printGridHeight = (thumbHeight + moviePrintDataSet.printGridMargin) * moviePrintDataSet.printGridRows + moviePrintDataSet.printGridMargin;
    }
//    ofLog(OF_LOG_VERBOSE, "printGridSize" + ofToString(printGridWidth) + "x" + ofToString(printGridHeight));
}

//--------------------------------------------------------------
void testApp::calculateNewPrintGrid(){

    
    float gridRatio;
    if (loadedMovie.gmMovie.isLoaded()) { // if no movie is loaded yet, then take a 16 by 9 and square pixel ratio
        scrubWindowH = scrubWindowW * (1/loadedMovie.gmImageRatio) * loadedMovie.gmPixelRatio;
    } else {
        scrubWindowH = scrubWindowW * 0.5625 * 1;
    }
    loadedMovie.stop(TRUE);
    if (isnan(loadedMovie.gmImageRatio) || !loadedMovie.gmMovie.isLoaded()) {
        gridRatio = 0.5625;
    } else {
        gridRatio = 1.0/loadedMovie.gmImageRatio;
    }
    while (loadedMovie.isThreadRunning()) {
    }
    
    thumbHeight = thumbWidth*gridRatio;
    
    
    if (displayGridSetWithColumnsAndRows) {
        numberOfStills = moviePrintDataSet.printGridColumns*moviePrintDataSet.printGridRows;
    } else {
        numberOfStills = printNumberOfThumbs;
        moviePrintDataSet.printGridRows = ceil(numberOfStills/(float)moviePrintDataSet.printGridColumns);
        
    }

    if (!(moviePrintDataSet.gridTimeArray.empty())){
        moviePrintDataSet.gridTimeArray.clear();
    }
    moviePrintDataSet.gridTimeArray.resize(numberOfStills,0);
    if (moviePrintDataSet.gridTimeArray.empty()){
        ofLog(OF_LOG_VERBOSE, "Error: memory could not be allocated" );
    } else {
        for (int i=0; i<numberOfStills; i++) {
            moviePrintDataSet.gridTimeArray[i] = i;
        }
    }

    updateGridTimeArrayWithAutomaticInterval();

    loadedMovie.gmThumbWidth = thumbWidth;
    loadedMovie.gmThumbHeight = thumbHeight;
    
    calculateNewPrintSize();
    
    updateNewPrintGrid = true;
    tweenTimeDelay.setParameters(1,easinglinear,ofxTween::easeInOut,255.0,0.0,500,0);
    
    updateDisplayGrid();
    
    ofxNotify() << "New Grid is Calculated - Total Number of Stills: " + ofToString(numberOfStills);

}

//--------------------------------------------------------------
void testApp::updateDisplayGrid(){
    
    gridColumns = fmax(1, (int)(ofGetWindowWidth() - leftMargin - scrollBarWidth + displayGridMargin) / (thumbWidth + displayGridMargin));
    
    displayGridWidth = (gridColumns * (thumbWidth + displayGridMargin) - displayGridMargin);
    gridRows = ceil(numberOfStills/(float)gridColumns);
    displayGridHeight = (gridRows * (thumbHeight + displayGridMargin)) - displayGridMargin;
//    ofLog(OF_LOG_VERBOSE, "displayGridMargin: " + ofToString(displayGridMargin));
//    ofLog(OF_LOG_VERBOSE, "displayGridHeight: " + ofToString(thumbHeight));
//    ofLog(OF_LOG_VERBOSE, "gridAreaHeight: " + ofToString(displayGridHeight));
    
    updateAllLimits();
    updateTheScrollBar();
    updateTheListScrollBar();
    updateTimeline();
    
    ofxNotify() << "updateDisplayGrid - Total Number of Stills: " + ofToString(numberOfStills);
    
//    ofLog(OF_LOG_VERBOSE, "displayGridWidth: " + ofToString(displayGridWidth));
//    ofLog(OF_LOG_VERBOSE, "displayGridHeight: " + ofToString(displayGridHeight));
    
}

//--------------------------------------------------------------
void testApp::loadNewMovie(string _newMoviePath, bool _wholeRange, bool _loadInBackground, bool _loadScrubMovie){
    
    movieLoading = TRUE;
    if (!_loadInBackground) {
        moveToMovie();
    }
    
    loadedMovie.stop(TRUE);

    ofxNotify() << "Movie has started to load";
    loadedMovie.loadNewMovieToBeGrabbed(_newMoviePath, numberOfStills, showPlaceHolder, !_loadInBackground);
    calculateNewPrintGrid();
    if (loadedMovie.gmTotalFrames <=1) {
        movieProperlyLoaded = FALSE;
        ofxNotify() << "Movie could not be properly loaded";
        totalFrames = numberOfStills;
    } else {
        movieProperlyLoaded = TRUE;
        totalFrames = loadedMovie.gmTotalFrames;
    }
    ofLog(OF_LOG_VERBOSE, "_newMoviePath" + ofToString(_newMoviePath) );
    ofLog(OF_LOG_VERBOSE, "totalFrames" + ofToString(totalFrames) );
    
    updateTimeSlider(_wholeRange);
    updateGridTimeArrayWithAutomaticInterval();
    updateAllStills();

    if (_loadScrubMovie) {
        loadNewMovieToBeScrubbedBool = TRUE;
    }
    
    if (!_loadInBackground) {
        showListView = FALSE;
        droppedList.glUpdateMovieFromList = FALSE;
    }
    movieLoading = FALSE;
    if (movieProperlyLoaded){
        ofxNotify() << "Movie has finished to load";
    } else{
        ofxNotify() << "Movie could not be loaded";
        ofxNotify() << "Movie could not be loaded";
        ofxNotify() << "Movie could not be loaded";
        ofxNotify() << "Movie could not be loaded";
    }
    updateTheScrollBar();
    
    ofLog(OF_LOG_VERBOSE, "scrollAmountRel: " + ofToString(scrollAmountRel));
    
    timer.setStartTime();
    finishedLoadingMovie = TRUE;

    ofLog(OF_LOG_VERBOSE, "_loadInBackground: " + ofToString(_loadInBackground));
    ofLog(OF_LOG_VERBOSE, "Finished Loading Movie--------------------------------------------");
    
    // reset all undo steps and save initial DataSet
    previousMoviePrintDataSet.clear();
    addMoviePrintDataSet(0);
    
    if (!loadedMovie.isMovieLoaded) {
        guiTimeline->setVisible(FALSE);
        scrollBar.unregisterMouseEvents();
        loadedMovie.disableMouseEvents();
    }

    
    stringMovieInfo.clear();
    stringMovieInfo.push_back("Format: ");
    stringMovieInfo.push_back("Format/String: ");
    stringMovieInfo.push_back("FileSize: ");
    stringMovieInfo.push_back("Duration: ");
    stringMovieInfo.push_back("FrameCount: ");
    stringMovieInfo.push_back("Size: ");
    stringMovieInfo.push_back("DisplayAspectRatio: ");
    stringMovieInfo.push_back("FrameRate: ");
    stringMovieInfo.push_back("Codec: ");
    stringMovieInfo.push_back("Codec/Info: ");
    stringMovieInfo.push_back("BitRate: ");
    stringMovieInfo.push_back("PixelAspectRatio: ");
    stringMovieInfo.push_back("DisplayAspectRatio: ");
    stringMovieInfo.push_back("FrameRate_Mode: ");
    stringMovieInfo.push_back("ColorSpace: ");
    stringMovieInfo.push_back("ChromaSubsampling: ");
    stringMovieInfo.push_back("BitDepth: ");
    stringMovieInfo.push_back("Interlacement: ");
    stringMovieInfo.push_back("AudioCodec: ");
    stringMovieInfo.push_back("Channels: ");
    stringMovieInfo.push_back("SamplingRate: ");
    
    stringMovieData.clear();
    stringMovieData.push_back(loadedMovie.gmMIFormat);
    stringMovieData.push_back(loadedMovie.gmMIFormatString);
    stringMovieData.push_back(loadedMovie.gmMIFileSizeString);
    stringMovieData.push_back(loadedMovie.gmMIDurationString1);
    stringMovieData.push_back(loadedMovie.gmMIFrameCount);
    stringMovieData.push_back(loadedMovie.gmMIWidth);
    stringMovieData.push_back(loadedMovie.gmMIDisplayAspectRatioString);
    stringMovieData.push_back(loadedMovie.gmMIFrameRateString);
    stringMovieData.push_back(loadedMovie.gmMIVFormat);
    stringMovieData.push_back(loadedMovie.gmMIFormatInfo);
    stringMovieData.push_back(loadedMovie.gmMIBitRate);
    stringMovieData.push_back(loadedMovie.gmMIPixelAspectRatio);
    stringMovieData.push_back(loadedMovie.gmMIDisplayAspectRatio);
    stringMovieData.push_back(loadedMovie.gmMIFrameRate_ModeString);
    stringMovieData.push_back(loadedMovie.gmMIColorSpace);
    stringMovieData.push_back(loadedMovie.gmMIChromaSubsampling);
    stringMovieData.push_back(loadedMovie.gmMIBitDepthString);
    stringMovieData.push_back(loadedMovie.gmMIInterlacementString);
    stringMovieData.push_back(loadedMovie.gmMIAFormat);
    stringMovieData.push_back(loadedMovie.gmMIAChannelsString);
    stringMovieData.push_back(loadedMovie.gmMIASamplingRate);
    
}

//--------------------------------------------------------------
string testApp::cropFrontOfString(string _inputString, int _length, string _substitute){
    if (((_length + _substitute.length()) >= _inputString.length()) || (_length <= _substitute.length())) {
        return _inputString;
    } else {
        ofLog(OF_LOG_VERBOSE, "crop String to length:" + ofToString(_inputString.length()));
        return _substitute + _inputString.substr(_inputString.length()-_length - _substitute.length());
    }
}

//--------------------------------------------------------------
void testApp::update(){
    
    threadIsRunning = loadedMovie.isThreadRunning();
    
    // set window to minium width !!NOT WORKING
//    if (windowWasResized && !ofGetMousePressed()) {
//        ofSetWindowShape(fmin(ofGetWindowWidth(),1320),ofGetWindowHeight());
//        windowWasResized = false;
//    }

//    loadedMovie.update();
    
    if (tweenListInOut.update() == 0.0 || tweenListInOut.update() == 1.0) {
        lockedDueToInteraction = false;
    }
    
    if (!finishedUpdating) {
        if (!threadIsRunning && !ofGetMousePressed()) {
            timer.setStartTime();
            finishedUpdating = TRUE;
            ofLog(OF_LOG_VERBOSE, "Finished Updating Movie--------------------------------------------");
        }
    }
    if (showPrintScreen) {
        if (finishedPrinting) {
            if (timer.getElapsedSeconds() > 0.5) {
                showPrintScreen = FALSE;
            }
        }
    }
    if (showLoadMovieScreen) {
        if (finishedLoadingMovie) {
            if (timer.getElapsedSeconds() > 0.5) {
                showLoadMovieScreen = FALSE;
            }
        }

    }
    if (showUpdateScreen) {
        if (finishedUpdating) {
            if (timer.getElapsedSeconds() > 0.5) {
                showUpdateScreen = FALSE;
            }
        }
    }
    if (showTimeline) {
        if (finishedTimeline) {
            if (tweenTimelineInOut.update() > 0.5) {
                showTimeline = FALSE;
                moveInOutTimeline();
            }
        }
    }
    
    if (movieIsBeingGrabbed) {
        if (loadedMovie.allGrabbed() || !loadedMovie.isMovieLoaded) {
            movieIsBeingGrabbed = FALSE;
        }
        
    }
    
    // update Loader
    loadValue = ofMap(loadedMovie.percLoaded(), 0, 1.0, 0.0, 1.0, true);
    
    // update Movie for Scrubbing or InOut manipulations
    if (loadedMovie.gmScrubMovie || updateInOut) {
        loadedMovie.gmMovieScrub.update();
    }
    
    handlingEventOverlays();
    
    if (showMoviePrintPreview){
        if (showListView) {
            writeFboToPreview(fmin(fboToPreview.getWidth()/(float)printGridWidth, fboToPreview.getHeight()/(float)printGridHeight), true);
        } else {
            writeFboToPreview(fmin(fboToPreview.getWidth()/(float)printGridWidth, fboToPreview.getHeight()/(float)printGridHeight), false);
        }
    }
    
    // calculate rollout of ofxUI pos, scal
    guiSettingsMoviePrint->setPosition(menuMoviePrintSettings.getPositionX(), menuMoviePrintSettings.getPositionY()+headerHeight);
    guiSettingsMoviePrint->setHeight(menuMoviePrintSettings.getSizeH()-headerHeight);

    guiSettings->setPosition(menuSettings.getPositionX(), menuSettings.getPositionY()+headerHeight);
    guiSettings->setHeight(menuSettings.getSizeH()-headerHeight);
//    guiSettings->setHeight(menuSettings.getSizeH()-headerHeight);

    guiTimeline->setPosition(leftMargin - OFX_UI_GLOBAL_WIDGET_SPACING, ofGetWindowHeight() - footerHeight/2 +1 - (footerHeight/4) * menuTimeline.getRelSizeH());
    
    
    if (loadedMovie.isMovieLoaded) { // if no movie is loaded or we are in dev mode then only draw rects
        
        // update while scrubbing
        if (loadedMovie.gmScrubMovie) {
            updateInOut = FALSE;
            updateScrub = TRUE;
            int i = loadedMovie.gmScrubID;
            
            scrubDelta = (ofGetMouseX() - loadedMovie.grabbedStill[i].gsX - thumbWidth/2); // used this before i let the mouse jump in the middle
//            scrubDelta = (ofGetMouseX() - ofGetWidth()/2);  // used this with mouse jump in the middle
            if (abs(scrubDelta) >= 0 && abs(scrubDelta) < thumbWidth/4) {
                scrubDelta = 0;
            }
            if (scrubDelta >= thumbWidth/4) {
                scrubDelta = scrubDelta - thumbWidth/4;
            } else if (scrubDelta <= -thumbWidth/4) {
                scrubDelta = scrubDelta + thumbWidth/4;
            }

//            if (abs(scrubDelta) > gridWidth/2) {
//                scrubDelta = (float)scrubDelta/30.0;
//            } else if ((abs(scrubDelta) > gridWidth/4) && (abs(scrubDelta) <= gridWidth/2)) {
//                scrubDelta = (float)scrubDelta/60.0;
//            } else {
//                scrubDelta = (float)scrubDelta/200.0;
//            }
            
            
//            if (scrubDelta > 0) {
//                scrubDelta = log ((float)scrubDelta/30.0);
//            } else if (scrubDelta < 0){
//                scrubDelta = log(abs((float)scrubDelta/30.0))*-1;
//            }
            
                scrubDelta = (float)scrubDelta/60.0;

//            ofLog(OF_LOG_VERBOSE, "scrubDelta:" + ofToString(scrubDelta));
            // new Frame Number is being cropped by the movies first and last frame
            int newFrameNumber = scrubDelta + 0.5 + loadedMovie.grabbedStill[i].gsFrameNumber;
            if (newFrameNumber > loadedMovie.gmTotalFrames-1) {
                newFrameNumber = loadedMovie.gmTotalFrames-1;
            }
            if (newFrameNumber < 0) {
                newFrameNumber = 0;
            }
            
            if (loadedMovie.gmHasNoFrames) {
                int tempNewFrameNumber = newFrameNumber;
                if (tempNewFrameNumber < 5) {
                    tempNewFrameNumber = 5;
                }
                loadedMovie.gmMovieScrub.setPosition((float)(tempNewFrameNumber - 2)/(float)(loadedMovie.gmTotalFrames-1));
                loadedMovie.gmMovieScrub.nextFrame();
                loadedMovie.gmMovieScrub.nextFrame();
            } else {
                loadedMovie.gmMovieScrub.setFrame(newFrameNumber);
            }
            loadedMovie.grabbedStill[i].gsFrameNumber = newFrameNumber;
        }
        
        // update while Slider sends event
        if (manipulateSlider) {
            updateScrub = FALSE;
            updateInOut = TRUE;
            if (uiRangeSliderTimeline->hitLow) {
                scrubWindowGridNumber = 0;
                if (loadedMovie.gmHasNoFrames) {
                    loadedMovie.gmMovieScrub.setPosition((float)(uiSliderValueLow - 1)/(float)(loadedMovie.gmTotalFrames-1));
                    loadedMovie.gmMovieScrub.nextFrame();
                } else {
                    loadedMovie.gmMovieScrub.setFrame(uiSliderValueLow);
                }
                loadedMovie.grabbedStill[scrubWindowGridNumber].gsFrameNumber = uiSliderValueLow;
            } else if (uiRangeSliderTimeline->hitHigh)
            {
                scrubWindowGridNumber = numberOfStills-1;
                if (loadedMovie.gmHasNoFrames) {
                    loadedMovie.gmMovieScrub.setPosition((float)(uiSliderValueHigh - 1)/(float)(loadedMovie.gmTotalFrames-1));
                    loadedMovie.gmMovieScrub.nextFrame();
                } else {
                    loadedMovie.gmMovieScrub.setFrame(uiSliderValueHigh);
                }
                loadedMovie.grabbedStill[scrubWindowGridNumber].gsFrameNumber = uiSliderValueHigh;
            } else if (uiRangeSliderTimeline->hitCenter)
            {
                scrubWindowGridNumber = numberOfStills/2;
                if (loadedMovie.gmHasNoFrames) {
                    loadedMovie.gmMovieScrub.setPosition((float)(((uiSliderValueHigh - uiSliderValueLow)/2 + uiSliderValueLow) - 1)/(float)(loadedMovie.gmTotalFrames-1));
                    loadedMovie.gmMovieScrub.nextFrame();
                } else {
                    loadedMovie.gmMovieScrub.setFrame((uiSliderValueHigh - uiSliderValueLow)/2 + uiSliderValueLow);
                }
                loadedMovie.grabbedStill[scrubWindowGridNumber].gsFrameNumber = (uiSliderValueHigh - uiSliderValueLow)/2 + uiSliderValueLow;
            }
        }
    }
    
    if (updateNewPrintGrid == TRUE && !currPrintingList && !ofGetMousePressed()) {
        updateInOut = FALSE;
        updateScrub = FALSE;

        if(tweenTimeDelay.update() < 5){
            updateNewPrintGrid = FALSE;
            if (!showListView) {
                loadedMovie.allocateNewNumberOfStills(numberOfStills, thumbWidth, thumbHeight, showPlaceHolder, true);
                updateAllStills();
            }
        }
    }
    
    if (droppedList.glUpdateMovieFromList) {
        printListNotImage = FALSE;
        showListView = FALSE;
        finishedLoadingMovie = FALSE;
        showLoadMovieScreen = TRUE;
        moveToMovie();
        counterToUpdate++;
        if (counterToUpdate > 1) {
            loadNewMovie(droppedList.glDroppedItem[droppedList.glActiveID].gliFile.path(), TRUE, FALSE, TRUE);
            droppedList.glUpdateMovieFromList = FALSE;
            counterToUpdate = 0;
        }
    }
    
    if (updateMovieFromDrop) {
        printListNotImage = FALSE;
        showListView = FALSE;
        finishedLoadingMovie = FALSE;
        showLoadMovieScreen = TRUE;
        counterToUpdate++;
        if (counterToUpdate > 1) {
            loadNewMovie(droppedFiles[0].path(), TRUE, FALSE, TRUE);
            if (loadedMovie.isMovieLoaded) {
                moveToMovie();
            }
            updateMovieFromDrop = FALSE;
            counterToUpdate = 0;
        }
    }

    if (scrollBar.sbActive) {
//        ofLog(OF_LOG_VERBOSE, "scrollBar Active:" + ofToString(scrollAmountRel) );
        if (scrollGrid) {
//            ofLog(OF_LOG_VERBOSE, "scrollGrid True:" + ofToString(scrollAmountRel) );
            if (!scrollBar.sbCalculateScrollInertia && !scrollBar.sbScrollBarDrag) {
                scrollGrid = false;
            } else {
            scrollBar.update();
            scrollAmountRel = scrollBar.getRelativePos();
//            ofLog(OF_LOG_VERBOSE, "scrollBarAmount:" + ofToString(scrollAmountRel) );
            }
        }
    } else {
        scrollAmountRel = 0;
        
    }
    
    if (scrollBarList.sbActive) {
//        ofLog(OF_LOG_VERBOSE, "scrollBarList Active:" + ofToString(scrollListAmountRel) );
        if (scrollList) {
//            ofLog(OF_LOG_VERBOSE, "scrollList True:" + ofToString(scrollListAmountRel) );
            if (!scrollBarList.sbCalculateScrollInertia && !scrollBarList.sbScrollBarDrag) {
                scrollList = false;
            } else {
            scrollBarList.update();
            scrollListAmountRel = scrollBarList.getRelativePos();
//            ofLog(OF_LOG_VERBOSE, "scrollBarAmount:" + ofToString(scrollListAmountRel) );
            }
        }
    } else {
        scrollListAmountRel = 0;
    }
    
    if (currPrintingList) {
        updateScrub = FALSE;
        updateInOut = FALSE;
        printListToFile();
    }

    if (showPrintScreen && !finishedPrinting) {
        // to ensure that print screen is showing before printing starts
        counterToPrint++;
        if (counterToPrint > 1) {
            if (printListNotImage) {
                startListPrinting();
            } else {
                startPrinting();
            }
            counterToPrint = 0;
        }
    }
    
    
    // sollte kurze wait schleife fuer das scrubvideoloading sein - leider funktioniert das nicht so ganz - man kann zwar das scrubvideo etwas spaeter loaden, waerenddessen haelt aber trotzdem alles an
    if (loadNewMovieToBeScrubbedBool) {
        if (timer.getElapsedSeconds() > 0.1) {
            loadedMovie.loadNewMovieToBeScrubbed(loadedMovie.gmMovie.getMoviePath());
            loadNewMovieToBeScrubbedBool = FALSE;
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    
    if (!(tweenListInOut.update() == 0.0)) { // stop drawing when position is at showMovieView
        
        drawList(scrollListAmountRel);
        scrollBarList.draw();
        
    }
    
    if (!(tweenListInOut.update() == 1.0)) { // stop drawing when position is at showListView
        
        if (!loadedMovie.isMovieLoaded) { // if no movie is loaded
            if (!showListView) { // if no List View
                guiTimeline->setVisible(FALSE);
                drawStartScreen();
            }
            
        } else {
            
            // draw all frames
            drawDisplayGrid(1, FALSE, FALSE, scrollAmountRel, showPlaceHolder);
            
            // draw the In and Out Point Manipulations
            if (updateInOut) {
                
                ofPushStyle();
                
                ofEnableAlphaBlending();
                ofSetColor(0,(int)(tweenFading.update()/255)*155);
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                ofSetColor(255,255,255,(int)tweenFading.update());
                
                loadedMovie.gmMovieScrub.draw(ofGetWidth()/2-scrubWindowW/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH);
                loadedMovie.drawStillUI(scrubWindowGridNumber, ofGetWidth()/2-scrubWindowW/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH, (tweenFading.update()/255));
                
                ofSetColor(255, 255, 255, (int)(tweenFading.update()/255)*255);
                
                if (uiRangeSliderTimeline->hitLow) {
                    inPointImage.draw(ofGetWidth()/2-inPointImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-inPointImage.getHeight()/2);
                }
                if (uiRangeSliderTimeline->hitHigh) {
                    outPointImage.draw(ofGetWidth()/2-outPointImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-outPointImage.getHeight()/2);
                }
                if (uiRangeSliderTimeline->hitCenter) {
                    inPointImage.draw(ofGetWidth()/2-inPointImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-inPointImage.getHeight()/2);
                    outPointImage.draw(ofGetWidth()/2-outPointImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-outPointImage.getHeight()/2);
                }
                
                if(tweenFading.update() < 5){
                    updateInOut = FALSE;
                    manipulateSlider = FALSE;
                }
                
                ofSetColor(255);
                
                ofPopStyle();
                
            }
            
            // draw the scrubbed video
            if (updateScrub) {
                drawScrubScreen(1.0);
            }
            
            scrollBar.draw();
            
        }
        
    }
    
    drawUI(1, FALSE);

    if(showLoadMovieScreen){
        drawLoadMovieScreen();
    } else if (showPrintScreen) {
        drawPrintScreen();
    } else {
        if(showUpdateScreen && loadedMovie.isMovieLoaded){
            drawUpdateScreen();
        }
    }
    
    ofxNotify::draw(drawNotify);
}

//--------------------------------------------------------------
void testApp::writeFboToPreview(float _scaleFactor, bool _showPlaceHolder){
    fboToPreview.begin();
    ofClear(255,255,255, 0);
    drawMoviePrintPreview(_scaleFactor, _showPlaceHolder);
    fboToPreview.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (!lockedDueToInteraction && !lockedDueToPrinting) {
        
        currentKey = key;
        ofLog(OF_LOG_VERBOSE, "currentKey:" + ofToString(currentKey));
        
        switch (key) {
            case OF_KEY_LEFT_SUPER:
            case OF_KEY_RIGHT_SUPER:
                superKeyPressed = TRUE;
                break;
            case OF_KEY_LEFT_SHIFT:
            case OF_KEY_RIGHT_SHIFT:
                shiftKeyPressed = TRUE;
                break;
            default:
                break;
        }
        
        switch (key)
        {
                
            case 'l':
            {
                if (droppedFiles.size() > 1) {
                    if (loadedMovie.isMovieLoaded) {
                        showListView = !showListView;
                        if (showListView == FALSE) {
                            moveToMovie();
                        }
                        if (showListView == TRUE) {
                            moveToList();
                        }
                    }
                }
                
            }
                break;
                
            case 'p':
            {
                if (loadedMovie.isMovieLoaded || showListView) {
                    finishedPrinting = FALSE;
                    showPrintScreen = TRUE;
                }
            }
                break;
                
            case 'x':
            {
                redoStep();
            }
                break;
                
            case 'z':
            {
                undoStep();
            }
                break;
                
            case 'v':
            {
                logPreviousMoviePrintDataSet();
            }
                break;
                
            case 'c':
            {
                    string tempString = "";
                    for (int j=0; j < numberOfStills; j++) {
                        tempString = tempString + ", " + ofToString(moviePrintDataSet.gridTimeArray[j]);
                    }
                    ofLog(OF_LOG_VERBOSE, "MoviePrintDataSet" +  tempString);
            }
                break;
                
            case 'w':
            {
                if (setupFinished) {
                    
                    printListNotImage = FALSE;
                    showListView = FALSE;
                    finishedLoadingMovie = FALSE;
                    showLoadMovieScreen = TRUE;
//                    loadNewMovie("/Users/fakob/Movies/Daft Punk - Get Lucky by Shortology.mp4", TRUE, FALSE, TRUE);
                    loadNewMovie("/Users/fakob/Movies/FrameTestMovie_v001.mov", TRUE, FALSE, TRUE);
                    if (loadedMovie.isMovieLoaded) {
                        moveToMovie();
                    }
                    updateMovieFromDrop = FALSE;
                    
                }
                
            }
                break;
                
            case 'r':
            {
                updateAllStills();
            }
                break;
                
            case ' ':
            {
                toggleMoviePrintPreview();
            }
                break;
                
            default:
                break;
        }
    } else {
        ofLog(OF_LOG_VERBOSE, "lockedDueToInteraction------------------------------------------------");
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if (!lockedDueToInteraction && !lockedDueToPrinting) {
        
        currentKey = -1;
        
        if (uiRangeSliderTimeline->getState()) {
            
            if (key == OF_KEY_RIGHT || key == OF_KEY_LEFT || key == OF_KEY_UP || key == OF_KEY_DOWN) {
                uiSliderValueLow = uiRangeSliderTimeline->getScaledValueLow();
                uiSliderValueHigh = uiRangeSliderTimeline->getScaledValueHigh();
                updateGridTimeArrayWithAutomaticInterval();
                updateAllStills();
                ofLog(OF_LOG_VERBOSE, "ArrowKey Manipulation of Timeline Slider" );
                
            }
        }
        
        manipulateSlider = FALSE;
        loadedMovie.gmScrubMovie = FALSE;
        
        superKeyPressed = FALSE;
        shiftKeyPressed = FALSE;
    } else {
        ofLog(OF_LOG_VERBOSE, "lockedDueToInteraction------------------------------------------------");
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    if (!lockedDueToInteraction && !lockedDueToPrinting) {
        
        if (y > ofGetHeight() - footerHeight/1.2) {
            if (!showTimeline) {
                ofLog(OF_LOG_VERBOSE, "Show Timeline------------------------------------------------");
                finishedTimeline = FALSE;
                showTimeline = TRUE;
                moveInOutTimeline();
            }
        } else {
            if (!ofGetMousePressed()) {
                timer.setStartTime();
                finishedTimeline = TRUE;
            }
        }
    } else {
        ofLog(OF_LOG_VERBOSE, "lockedDueToInteraction------------------------------------------------");
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (!lockedDueToInteraction && !lockedDueToPrinting) {
//        ofLog(OF_LOG_VERBOSE, "which button is clicked:" + ofToString(button));
        
        if (loadedMovie.isMovieLoaded && loadedMovie.gmMovieScrub.isLoaded()) {
            if(button == 0){
                if (y > ofGetHeight() - footerHeight/1.2) {
                    if (showTimeline) {
                        ofLog(OF_LOG_VERBOSE, "Show Timeline------------------------------------------------");
                        finishedTimeline = FALSE;
                        showTimeline = TRUE;
                        //                    moveInOutTimeline();
                    }
                }
                
                if (showTimeline) {
                    if (finishedTimeline) {
                        if (timer.getElapsedSeconds() > 0.5) {
                            showTimeline = FALSE;
                            moveInOutTimeline();
                        }
                    }
                }
                
                
                if (!showListView) {
                    if (!(menuMovieInfo.getMenuActivated() || menuMoviePrintSettings.getMenuActivated() || menuHelp.getMenuActivated())) {
                        if (loadedMovie.grabbedStill[loadedMovie.gmRollOverMovieID].gsRollOver){
                            
                            rollOverMovieID = loadedMovie.gmRollOverMovieID;
                            rollOverMovieButtonID = loadedMovie.gmRollOverMovieButtonID;
                            rollOverClicked = TRUE;
                            ofLog(OF_LOG_VERBOSE, "clicked in rolloverstate ID =" + ofToString(loadedMovie.gmRollOverMovieID));
                            
                        }
                        updateInOut = FALSE;
                        ofLog(OF_LOG_VERBOSE, "the mouse was clicked" );
                        tweenFading.setParameters(1,easinglinear,ofxTween::easeInOut,0.0,255.0,200,0);
                        
                    }
                }
            }
        }
        
//        ofLog(OF_LOG_VERBOSE, "mousePressed - Button:" + ofToString(button) );
    } else {
        ofLog(OF_LOG_VERBOSE, "lockedDueToInteraction------------------------------------------------");
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if (!lockedDueToInteraction && !lockedDueToPrinting) {
        if (loadedMovie.isMovieLoaded) {
            ofLog(OF_LOG_VERBOSE, "mouseReleased" );
            
            if (!showListView) {
                if (loadedMovie.isThreadRunning()) {
                    loadedMovie.stop(false);
                }
                if (updateInOut) {
                    ofLog(OF_LOG_VERBOSE, "mouseReleased - updateInOut True" );
                    tweenFading.setParameters(1,easinglinear,ofxTween::easeInOut,255.0,0.0,500,0);
                    updateGridTimeArrayWithAutomaticInterval();
                    updateAllStills();
                    addToUndo = true;
                }
                if (updateScrub) {
                    tweenFading.setParameters(1,easinglinear,ofxTween::easeInOut,255.0,0.0,500,0);
                    
                    int i = loadedMovie.gmScrubID;
                    updateOneThumb(i, loadedMovie.grabbedStill[i].gsFrameNumber);
                    addToUndo = true;

                }
                if (rollOverClicked) {
                    rollOverButtonsClicked(rollOverMovieID, rollOverMovieButtonID);
                    addToUndo = true;
                }
            }
        }
        if (addToUndo) {
            addMoviePrintDataSet(undoPosition);
            addToUndo = false;
        }
        manipulateSlider = FALSE;
        loadedMovie.gmScrubMovie = FALSE;
        loadedMovie.gmRollOver = FALSE;
        scrollGrid = FALSE;
        scrollList = FALSE;
    } else {
        ofLog(OF_LOG_VERBOSE, "lockedDueToInteraction------------------------------------------------");
    }
}

//--------------------------------------------------------------
void testApp::mouseScrolled(double x, double y){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    listWidth = w;
    if (!currPrintingList) {
        updateDisplayGrid();
//        windowWasResized = true;
        windowResizedOnce++;
    }
}

//--------------------------------------------------------------
void testApp::updateAllLimits(){
    loadedMovie.setAllLimitsUpper(headerHeight);
    droppedList.setAllLimitsUpper(headerHeight);
    if (droppedFiles.size() > 1) { // Limit for menuMoveToList
        ofLog(OF_LOG_VERBOSE, "setLeftLimits for menuMoveToList" + ofToString(leftMargin*4));
        loadedMovie.setAllLimitsLeft(leftMargin*4);
    } else {
        loadedMovie.setAllLimitsLeft(leftMargin);
    }
    droppedList.setAllLimitsLeft(leftMargin);
    loadedMovie.setAllLimitsLower(ofGetHeight());
    droppedList.setAllLimitsLower(ofGetHeight());
    loadedMovie.setAllLimitsRight(ofGetWidth() - scrollBarWidth);
    droppedList.setAllLimitsRight(ofGetWidth() - scrollBarWidth);
}

//--------------------------------------------------------------
void testApp::updateTheListScrollBar(){
    scrollBarList.updateScrollBar(ofGetWindowWidth(), ofGetWindowHeight(), headerHeight + topMargin, 0, droppedList.getListHeight());
    scrollBarList.setToTop();
    scrollListAmountRel = scrollBarList.getRelativePos();
}

//--------------------------------------------------------------
void testApp::updateTheScrollBar(){
    scrollBar.updateScrollBar(ofGetWindowWidth(), ofGetWindowHeight(), headerHeight + topMargin, footerHeight/2 + bottomMargin, displayGridHeight);
    scrollBar.setToTop();
    scrollAmountRel = scrollBar.getRelativePos();
}

//--------------------------------------------------------------
void testApp::updateTimeline(){
    if (loadedMovie.isMovieLoaded) {
        guiTimeline->setPosition(leftMargin - OFX_UI_GLOBAL_WIDGET_SPACING, ofGetWindowHeight()  - footerHeight/2 +1 - (footerHeight/4) * menuTimeline.getRelSizeH());
        guiTimeline->setWidth(ofGetWindowWidth());
        uiRangeSliderTimeline->setWidth(displayGridWidth);
//        ofLog(OF_LOG_VERBOSE, "Timeslider Width:" + ofToString(uiRangeSliderTimeline->getWidth()));
    }
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    if (!lockedDueToInteraction && !lockedDueToPrinting) {
        
        if (setupFinished) {
            
            if( dragInfo.files.size() > 0 ){
                
                loadNewMovie("", FALSE, TRUE, FALSE);
                
                droppedList.disableMouseEvents(droppedFiles.size());
                droppedFiles.clear();
                for (int i=0; i<dragInfo.files.size(); i++) {
                    ofFile testFile(dragInfo.files[i]);
                    if (!testFile.isDirectory()) {
                        if (checkExtension(ofToString(testFile.getExtension()))){
                            droppedFiles.push_back(dragInfo.files[i]);
                        }
                    }
                }
                
                droppedList.setup(droppedFiles);
                updateTheListScrollBar();
                
                if(droppedFiles.size() > 1){
                    moveToList();
                } else if(droppedFiles.size() > 0){
                    ofLog(OF_LOG_VERBOSE, "Start LoadingMovie------------------------------------------------");
                    updateMovieFromDrop = TRUE;
                    
                }
                
            }
        }
    } else {
        ofLog(OF_LOG_VERBOSE, "lockedDueToInteraction------------------------------------------------");
    }
}

//--------------------------------------------------------------
void testApp::exit(){
    
    loadedMovie.stop(false);
    
    delete guiTimeline;
    
    delete guiSettings;

    
    guiSettingsMoviePrint->saveSettings("guiMoviePrintSettings.xml");
	delete guiSettingsMoviePrint;


}
//--------------------------------------------------------------
void testApp::logPreviousMoviePrintDataSet(){
    
    for (int i=0; i < previousMoviePrintDataSet.size(); i++) {
        if (undoPosition == i) {
            ofLog(OF_LOG_VERBOSE, "previousMoviePrintDataSet RECENT  " +  ofToString(previousMoviePrintDataSet[i].gridTimeArray));
        } else {
            ofLog(OF_LOG_VERBOSE, "previousMoviePrintDataSet Address " +  ofToString(previousMoviePrintDataSet[i].gridTimeArray));
        }
    }
    ofLog(OF_LOG_VERBOSE, "moviePrintDataSet Address " +  ofToString(moviePrintDataSet.gridTimeArray));
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
    if (!lockedDueToInteraction && !lockedDueToPrinting) {
        
        string name = e.widget->getName();
        ofLog(OF_LOG_VERBOSE, "got event from: " + name );
        
        if(name == "RSLIDER")
        {
            if (uiRangeSliderTimeline->hitLow) {
                if ((uiRangeSliderTimeline->getScaledValueHigh()-uiRangeSliderTimeline->getScaledValueLow() < numberOfStills)) {
                    uiRangeSliderTimeline->setValueLow(uiRangeSliderTimeline->getScaledValueHigh()-(numberOfStills-1));
                }
            }
            if (uiRangeSliderTimeline->hitHigh) {
                if ((uiRangeSliderTimeline->getScaledValueHigh()-uiRangeSliderTimeline->getScaledValueLow() < numberOfStills)) {
                    uiRangeSliderTimeline->setValueHigh(uiRangeSliderTimeline->getScaledValueLow()+(numberOfStills-1));
                }
            }
            if (uiRangeSliderTimeline->hitCenter) {
                if (uiRangeSliderTimeline->getScaledValueHigh() >= totalFrames-1) {
                    if ((uiRangeSliderTimeline->getScaledValueHigh()-uiRangeSliderTimeline->getScaledValueLow() < numberOfStills)) {
                        uiRangeSliderTimeline->setValueLow(uiRangeSliderTimeline->getScaledValueHigh()-(numberOfStills-1));
                    }
                }
                if (uiRangeSliderTimeline->getScaledValueLow() == 0) {
                    if ((uiRangeSliderTimeline->getScaledValueHigh()-uiRangeSliderTimeline->getScaledValueLow() < numberOfStills)) {
                        uiRangeSliderTimeline->setValueHigh(uiRangeSliderTimeline->getScaledValueLow()+(numberOfStills-1));
                    }
                }
            }
            uiSliderValueLow = uiRangeSliderTimeline->getScaledValueLow();
            uiSliderValueHigh = uiRangeSliderTimeline->getScaledValueHigh();
            manipulateSlider = TRUE;
            addToUndo = true;
        }
        else if(name == "Columns")
        {
            ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
            ofLog(OF_LOG_VERBOSE, "Columns " + ofToString(slider->getScaledValue()));
            gridColumns = (int)slider->getScaledValue();
            updateDisplayGrid();
        }
        
        else if(name == "ThumbWidth")
        {
            ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
            ofLog(OF_LOG_VERBOSE, "ThumbWidth " + ofToString(slider->getScaledValue()));
            thumbWidth = possStillResWidth169[(int)slider->getScaledValue()];
            calculateNewPrintGrid();
        }
        else if(name == "Select Output Folder")
        {
            ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
            if (button->getValue()) {
                
                string movieFileName = loadedMovie.gmMovie.getMoviePath();
                movieFileName = loadedFilePath.getFileName(movieFileName, TRUE) + "_MoviePrint";
                
                string formatExtension;
                if (moviePrintDataSet.printFormat == OF_IMAGE_FORMAT_JPEG) {
                    formatExtension = "jpg";
                } else {
                    formatExtension = "png";
                }
                
                ofFileDialogResult saveFileResult = ofSystemSaveDialog(movieFileName + "." + formatExtension, "Select a Folder");
                if (saveFileResult.bSuccess){
                    vector<string> tempVectorString = ofSplitString(saveFileResult.getPath(), "/");
                    tempVectorString.pop_back();
                    saveMoviePrintPath = ofJoinString(tempVectorString, "/") + "/";
                    ofLogVerbose("User selected saveMoviePrintPath: "  + ofToString(saveMoviePrintPath));
                } else {
                    ofLogVerbose("User hit cancel");
                }
            }
            uiLabelOutputFolder->setLabel(cropFrontOfString(saveMoviePrintPath, 40, "..."));
        }
        else if(name == "PrintColumns")
        {
            ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
//            ofLog(OF_LOG_VERBOSE, "PrintColumns " + ofToString(slider->getScaledValue()));
            moviePrintDataSet.printGridColumns = (int)slider->getScaledValue();
            if (printGridSetWithColumnsAndRows) {
                printNumberOfThumbs = moviePrintDataSet.printGridColumns * moviePrintDataSet.printGridRows;
                calculateNewPrintGrid();
            } else {
                moviePrintDataSet.printGridRows = ceil(numberOfStills/(float)moviePrintDataSet.printGridColumns);
                calculateNewPrintSize();
            }
            addToUndo = true;
        }
        else if(name == "PrintRows")
        {
            ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
//            ofLog(OF_LOG_VERBOSE, "PrintRows " + ofToString(slider->getScaledValue()));
            moviePrintDataSet.printGridRows = (int)slider->getScaledValue();
            printNumberOfThumbs = moviePrintDataSet.printGridColumns * moviePrintDataSet.printGridRows;
            calculateNewPrintGrid();
            addToUndo = true;
        }
        else if(name == "PrintNumber")
        {
            ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
//            ofLog(OF_LOG_VERBOSE, "PrintNumber " + ofToString(slider->getScaledValue()));
            printNumberOfThumbs = (int)slider->getScaledValue();
            calculateNewPrintGrid();
        }
        else if(name == "Display Header")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printDisplayVideoAudioInfo = true;
            } else {
                moviePrintDataSet.printDisplayVideoAudioInfo = false;
            }
            calculateNewPrintSize();
            addToUndo = true;
        }
        else if(name == "Save also individual frames")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printSingleFrames = true;
            } else {
                moviePrintDataSet.printSingleFrames = false;
            }
            addToUndo = true;
        }
        else if(name == "Set Columns and Rows")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                if (!(printNumberOfThumbs == (moviePrintDataSet.printGridColumns * moviePrintDataSet.printGridRows))){
                    printNumberOfThumbs = moviePrintDataSet.printGridColumns * moviePrintDataSet.printGridRows;
                    calculateNewPrintGrid();
                }
                printGridSetWithColumnsAndRows = TRUE;
                uiSliderPrintColumns->setVisible(TRUE);
                uiSliderPrintRows->setVisible(TRUE);
                uiSliderNumberOfThumbs->setVisible(FALSE);
            }
        }
        else if(name == "Set Number of Frames")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                //            calculateNewPrintGrid(); // not necessary as the number is the same
                printGridSetWithColumnsAndRows = FALSE;
                uiSliderPrintRows->setVisible(FALSE);
                uiSliderPrintColumns->setVisible(true);
                uiSliderNumberOfThumbs->setVisible(true);
            }
        }
        else if(name == "PrintMargin")
        {
            ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
//            ofLog(OF_LOG_VERBOSE, "PrintMargin " + ofToString(slider->getScaledValue()));
            moviePrintDataSet.printGridMargin = (int)slider->getScaledValue();
            calculateNewPrintSize();
            addToUndo = true;
        }
        else if(name == "Display TimeCode")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printDisplayTimecodeFramesOff = 1;
                loadedMovie.gmShowFramesUI = TRUE;
                loadedMovie.vfFramesToTimeSwitch = TRUE;
                addToUndo = true;
            }
        }
        else if(name == "Display Frames")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printDisplayTimecodeFramesOff = 2;
                loadedMovie.gmShowFramesUI = TRUE;
                loadedMovie.vfFramesToTimeSwitch = FALSE;
                addToUndo = true;
            }
        }
        else if(name == "off")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printDisplayTimecodeFramesOff = 0;
                loadedMovie.gmShowFramesUI = FALSE;
                addToUndo = true;
            }
        }
        else if(name == "jpg")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printFormat = OF_IMAGE_FORMAT_JPEG;
                addToUndo = true;
            }
        }
        else if(name == "png with alpha")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printFormat = OF_IMAGE_FORMAT_PNG;
                addToUndo = true;
            }
        }
        else if(name == "1024px width")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printSizeWidth = 1024;
                addToUndo = true;
            }
        }
        else if(name == "2048px width")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printSizeWidth = 2048;
                addToUndo = true;
            }
        }
        else if(name == "3072px width")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printSizeWidth = 3072;
                addToUndo = true;
            }
        }
        else if(name == "4096px width")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            bool val = toggle->getValue();
            if (val) {
                moviePrintDataSet.printSizeWidth = 4096;
                addToUndo = true;
            }
        }
        else if(name == "Save MoviePrint")
        {
            ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
            if (button->getValue()) {
                if (loadedMovie.isMovieLoaded || showListView) {
                    closeAllMenus();
                    finishedPrinting = FALSE;
                    showPrintScreen = TRUE;
                }
            }
        }
        else if(name == "Undo")
        {
            ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
            if (button->getValue()) {
                if (loadedMovie.isMovieLoaded || showListView) {
                    undoStep();
                }
            }
        }
        else if(name == "Redo")
        {
            ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
            if (button->getValue()) {
                if (loadedMovie.isMovieLoaded || showListView) {
                    redoStep();
                }
            }
        }
        else if(name == "Refresh")
        {
            ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
            if (button->getValue()) {
                if (loadedMovie.isMovieLoaded || showListView) {
                    updateAllStills();
                }
            }
        }
        else if(name == "Show MoviePrint Preview")
        {
            ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
            if (button->getValue()) {
                toggleMoviePrintPreview();
            }
        }

    } else {
        ofLog(OF_LOG_VERBOSE, "lockedDueToInteraction------------------------------------------------");
    }

}

//--------------------------------------------------------------
void testApp::undoStep(){
    if (undoPosition > 0) {
        undoPosition = fmin((undoPosition-1), previousMoviePrintDataSet.size() - 1);
        applyMoviePrintDataSet(previousMoviePrintDataSet[undoPosition]);
    }
    ofLog(OF_LOG_VERBOSE, "UNDO undoPosition:" + ofToString(undoPosition));
    logPreviousMoviePrintDataSet();
}

//--------------------------------------------------------------
void testApp::redoStep(){
    if (undoPosition < (previousMoviePrintDataSet.size() - 1)) {
        undoPosition = fmax(0, (undoPosition+1));
        applyMoviePrintDataSet(previousMoviePrintDataSet[undoPosition]);
    }
    ofLog(OF_LOG_VERBOSE, "REDO undoPosition:" + ofToString(undoPosition));
    logPreviousMoviePrintDataSet();
}

//--------------------------------------------------------------
void testApp::addMoviePrintDataSet(int _addToPosition){
    ofLog(OF_LOG_VERBOSE, "addMoviePrintDataSet:" + ofToString(undoPosition));
    ofLog(OF_LOG_VERBOSE, "moviePrintDataSet Adress" +  ofToString(moviePrintDataSet.gridTimeArray));
    if (previousMoviePrintDataSet.size() == 0) { // save initial settings without increasing undoPosition
        previousMoviePrintDataSet.push_back(moviePrintDataSet);
//        ofLog(OF_LOG_VERBOSE, "________________updateGridTimeArrayToMoviePrintDataSet " +  ofToString(updateGridTimeArrayToMoviePrintDataSet));
//        updateGridTimeArrayToMoviePrintDataSet = true;
        addGridTimeArrayToMoviePrintDataSet();
        ofLog(OF_LOG_VERBOSE, "ADD INIT undoPosition:" + ofToString(undoPosition));
    } else {
        if (hasChangedMoviePrintDataSet()) {
            if (_addToPosition >= (previousMoviePrintDataSet.size()-1)) { // add undo step when undoPosition at end
                previousMoviePrintDataSet.push_back(moviePrintDataSet);
                undoPosition++;
                ofLog(OF_LOG_VERBOSE, "ADD AT END undoPosition:" + ofToString(undoPosition));
                if (previousMoviePrintDataSet.size() > maxUndoSteps) {
                    previousMoviePrintDataSet.pop_front();
                    undoPosition--;
                    ofLog(OF_LOG_VERBOSE, "REMOVE FIRST undoPosition:" + ofToString(undoPosition));
                }
            } else { // if undoPosition lower than first delete all later undo Steps and then add new undo step
                int tempStepsToDelete = (previousMoviePrintDataSet.size() - 1 - _addToPosition);
                tempStepsToDelete = fmin(tempStepsToDelete, previousMoviePrintDataSet.size() - 1); // make sure not to delete more than whats there
                previousMoviePrintDataSet.erase(previousMoviePrintDataSet.end() - tempStepsToDelete,previousMoviePrintDataSet.end());
                previousMoviePrintDataSet.push_back(moviePrintDataSet);
                undoPosition = previousMoviePrintDataSet.size()-1;
                ofLog(OF_LOG_VERBOSE, "ADD INBETWEEN undoPosition:" + ofToString(undoPosition));
            }
//            ofLog(OF_LOG_VERBOSE, "________________updateGridTimeArrayToMoviePrintDataSet " +  ofToString(updateGridTimeArrayToMoviePrintDataSet));
//            updateGridTimeArrayToMoviePrintDataSet = true;
            addGridTimeArrayToMoviePrintDataSet();
        }
    }
}

//--------------------------------------------------------------
bool testApp::getAllFrameNumbers(){
//        ofLog(OF_LOG_VERBOSE, "_gridTimeArraySize:" + ofToString(_gridTimeArraySize));
//        ofLog(OF_LOG_VERBOSE, "gmNumberOfStills:" + ofToString(gmNumberOfStills));
//        ofLog(OF_LOG_VERBOSE, "grabbedStill.size():" + ofToString(grabbedStill.size()));
            while (loadedMovie.gmCurrAllocating) {
                ofLog(OF_LOG_VERBOSE, "waiting for allocating to end:" + ofToString(loadedMovie.gmCurrAllocating));
            }
        if(loadedMovie.gmNumberOfStills == loadedMovie.grabbedStill.size()){
            for (int i = 0; i<loadedMovie.gmNumberOfStills; i++) {
                moviePrintDataSet.gridTimeArray[i] = loadedMovie.grabbedStill[i].gsFrameNumber;
            }
            return true;
        }
        else {
            return false;
        }
    }

//--------------------------------------------------------------
void testApp::addGridTimeArrayToMoviePrintDataSet(){ // adds the GridTimeArray to the last previousMoviePrintDataSet
    ofLog(OF_LOG_VERBOSE, "addGridTimeArrayToMoviePrintDataSet:" + ofToString(undoPosition));
//    if (previousMoviePrintDataSet.size() != 0) { // first update the manipulated frames and get the frameNumbers into the gridTimeArray
//        getAllFrameNumbers();
//    }
    previousMoviePrintDataSet.back().gridTimeArray.clear();
    previousMoviePrintDataSet.back().gridTimeArray.resize(numberOfStills,0);
    if (previousMoviePrintDataSet.back().gridTimeArray.empty()){
        ofLog(OF_LOG_VERBOSE, "Error: memory could not be allocated" );
    } else {
        if ((!moviePrintDataSet.gridTimeArray.empty()) && loadedMovie.isMovieLoaded){
            for (int i=0; i<numberOfStills; i++) {
//                ofLog(OF_LOG_VERBOSE, "moviePrintDataSet.gridTimeArray[i]:" + ofToString(moviePrintDataSet.gridTimeArray[i]));
//                moviePrintDataSet.gridTimeArray[i] = loadedMovie.grabbedStill[i].gsFrameNumber;
                previousMoviePrintDataSet.back().gridTimeArray[i] = moviePrintDataSet.gridTimeArray[i];
//                previousMoviePrintDataSet.back().gridTimeArray[i] = loadedMovie.grabbedStill[i].gsFrameNumber;
            }
        } else {
            for (int i=0; i<numberOfStills; i++) {
                previousMoviePrintDataSet.back().gridTimeArray[i] = i;
            }
        }
    }
    updateGridTimeArrayToMoviePrintDataSet = false;
    ofLog(OF_LOG_VERBOSE, "________________updateGridTimeArrayToMoviePrintDataSet " +  ofToString(updateGridTimeArrayToMoviePrintDataSet));
}

//--------------------------------------------------------------
bool testApp::hasChangedMoviePrintDataSet(){
    ofLog(OF_LOG_VERBOSE, "hasChangedMoviePrintDataSet:" + ofToString(undoPosition));
    if (previousMoviePrintDataSet.size() > 0) {
        if (
            previousMoviePrintDataSet[undoPosition].printGridColumns == moviePrintDataSet.printGridColumns &&
            previousMoviePrintDataSet[undoPosition].printGridRows == moviePrintDataSet.printGridRows &&
            previousMoviePrintDataSet[undoPosition].printGridMargin == moviePrintDataSet.printGridMargin &&
            previousMoviePrintDataSet[undoPosition].printDisplayVideoAudioInfo == moviePrintDataSet.printDisplayVideoAudioInfo &&
            previousMoviePrintDataSet[undoPosition].printDisplayTimecodeFramesOff == moviePrintDataSet.printDisplayTimecodeFramesOff &&
            previousMoviePrintDataSet[undoPosition].printSingleFrames == moviePrintDataSet.printSingleFrames &&
            previousMoviePrintDataSet[undoPosition].printFormat == moviePrintDataSet.printFormat &&
            previousMoviePrintDataSet[undoPosition].printSizeWidth == moviePrintDataSet.printSizeWidth
            ) {
            return true;
//            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }

}

//--------------------------------------------------------------
void testApp::applyMoviePrintDataSet(moviePrintDataStruct _newMoviePrintDataSet){
    string tempName;
    ofxUIWidget *tempWidget;
    bool tempHasTheNumberOfThumbsChanged = false;

    // printGridColumns
    if (moviePrintDataSet.printGridColumns != _newMoviePrintDataSet.printGridColumns) {
        moviePrintDataSet.printGridColumns = _newMoviePrintDataSet.printGridColumns;
        uiSliderPrintColumns->setValue(moviePrintDataSet.printGridColumns);
        tempWidget = guiSettingsMoviePrint->getWidget("PrintColumns");
        guiSettingsMoviePrint->triggerEvent(tempWidget);
        tempHasTheNumberOfThumbsChanged = true;
    }
    
    // printGridRows
    if (moviePrintDataSet.printGridRows != _newMoviePrintDataSet.printGridRows) {
        moviePrintDataSet.printGridRows = _newMoviePrintDataSet.printGridRows;
        uiSliderPrintRows->setValue(moviePrintDataSet.printGridRows);
        tempWidget = guiSettingsMoviePrint->getWidget("PrintRows");
        guiSettingsMoviePrint->triggerEvent(tempWidget);
        tempHasTheNumberOfThumbsChanged = true;
    }
    
    // printGridMargin
    if(moviePrintDataSet.printGridMargin != _newMoviePrintDataSet.printGridMargin){
        moviePrintDataSet.printGridMargin = _newMoviePrintDataSet.printGridMargin;
        uiSliderPrintMargin->setValue(moviePrintDataSet.printGridMargin);
        tempWidget = guiSettingsMoviePrint->getWidget("PrintMargin");
        guiSettingsMoviePrint->triggerEvent(tempWidget);
    }
    
    // printDisplayVideoAudioInfo
    if(moviePrintDataSet.printDisplayVideoAudioInfo != _newMoviePrintDataSet.printDisplayVideoAudioInfo){
        moviePrintDataSet.printDisplayVideoAudioInfo = _newMoviePrintDataSet.printDisplayVideoAudioInfo;
        uiToggleHeaderDisplay->setValue(moviePrintDataSet.printDisplayVideoAudioInfo);
        tempWidget = guiSettingsMoviePrint->getWidget("Display Header");
        guiSettingsMoviePrint->triggerEvent(tempWidget);
    }
    
    // printDisplayTimecodeFramesOff
    if(moviePrintDataSet.printDisplayTimecodeFramesOff != _newMoviePrintDataSet.printDisplayTimecodeFramesOff){
        moviePrintDataSet.printDisplayTimecodeFramesOff = _newMoviePrintDataSet.printDisplayTimecodeFramesOff;
        switch (moviePrintDataSet.printDisplayTimecodeFramesOff) {
            case 0:
                tempName = "off";
                break;
            case 1:
                tempName = "Display TimeCode";
                break;
            case 2:
                tempName = "Display Frames";
                break;
            default:
                tempName = "off";
                break;
        }
        uiRadioSetFrameDisplay->activateToggle(tempName);
        tempWidget = guiSettingsMoviePrint->getWidget(tempName);
        guiSettingsMoviePrint->triggerEvent(tempWidget);
    }
    
    // printSingleFrames
    if(moviePrintDataSet.printSingleFrames != _newMoviePrintDataSet.printSingleFrames){
        moviePrintDataSet.printSingleFrames = _newMoviePrintDataSet.printSingleFrames;
        uiToggleSingleFrames->setValue(moviePrintDataSet.printSingleFrames);
        tempWidget = guiSettingsMoviePrint->getWidget("Save also individual frames");
        guiSettingsMoviePrint->triggerEvent(tempWidget);
    }
    
    // printFormat
    if(moviePrintDataSet.printFormat != _newMoviePrintDataSet.printFormat){
        moviePrintDataSet.printFormat = _newMoviePrintDataSet.printFormat;
        switch (moviePrintDataSet.printFormat) {
            case OF_IMAGE_FORMAT_PNG:
                tempName = "png with alpha";
                break;
            case OF_IMAGE_FORMAT_JPEG:
                tempName = "jpg";
                break;
            default:
                tempName = "png with alpha";
                break;
        }
        uiRadioPrintOutputFormat->activateToggle(tempName);
        tempWidget = guiSettingsMoviePrint->getWidget(tempName);
        guiSettingsMoviePrint->triggerEvent(tempWidget);
    }
    
    // printSizeWidth
    if(moviePrintDataSet.printSizeWidth != _newMoviePrintDataSet.printSizeWidth){
        moviePrintDataSet.printSizeWidth = _newMoviePrintDataSet.printSizeWidth;
        switch (moviePrintDataSet.printSizeWidth) {
            case 1024:
                tempName = "1024px width";
                break;
            case 2048:
                tempName = "2048px width";
                break;
            case 3072:
                tempName = "3072px width";
                break;
            case 4096:
                tempName = "4096px width";
                break;
            default:
                tempName = "1024px width";
                break;
        }
        uiRadioPrintOutputWidth->activateToggle(tempName);
        tempWidget = guiSettingsMoviePrint->getWidget(tempName);
        guiSettingsMoviePrint->triggerEvent(tempWidget);
    }
    if (loadedMovie.isMovieLoaded) {
        for (int i = 0; i<numberOfStills; i++) {
            if ((tempHasTheNumberOfThumbsChanged == true) || (moviePrintDataSet.gridTimeArray[i] != _newMoviePrintDataSet.gridTimeArray[i])) { // when the number of Thumbs has changed, all get updated - otherwise only the changed ones get updated
                moviePrintDataSet.gridTimeArray[i] = _newMoviePrintDataSet.gridTimeArray[i];
                loadedMovie.grabbedStill[i].gsFrameNumber = _newMoviePrintDataSet.gridTimeArray[i];
                loadedMovie.grabbedStill[i].gsToBeGrabbed = TRUE;
                loadedMovie.grabbedStill[i].gsToBeUpdated = TRUE;
                ofLog(OF_LOG_VERBOSE, "Still:" + ofToString(i) + " will be updated:" +  ofToString(_newMoviePrintDataSet.gridTimeArray[i]));
            }
        }
        loadedMovie.updateOrderNumber();
        if (!loadedMovie.isThreadRunning()) {
            loadedMovie.start();
        }
        
        uiRangeSliderTimeline->setValueLow(getLowestFrameNumber());
        uiRangeSliderTimeline->setValueHigh(getHighestFrameNumber());

    }
}

//--------------------------------------------------------------
void testApp::scrollEvent(ofVec2f &e){
    if (!updateScrub) {
        if (showListView) {
            scrollList = TRUE;
        } else {
            scrollGrid = TRUE;
        }
    }
}

//--------------------------------------------------------------
void testApp::drawUI(int _scaleFactor, bool _hideInPrint){
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255);

    // overlay von MoviePrintPreview
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
    ofSetColor(0, 0, 0, tweenMoviePrintPreview.update() * 100);
    ofRect(ofGetWindowWidth()/2, (ofGetWindowHeight()-headerHeight-footerHeight/2)/2 + headerHeight, ofGetWindowWidth(), (ofGetWindowHeight()-headerHeight-footerHeight/2));
    ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
    ofPopStyle();
    
    if (droppedFiles.size() > 1) { // draw
        if (loadedMovie.isMovieLoaded) {
            if (showListView == FALSE) {
                menuMoveToList.setPosition(0, 0);
                menuMoveToList.setSize(leftMargin*2, ofGetWindowHeight());
                menuMoveToList.drawMenu();
            }
        }
    }
    
    layoutHeaderImage.draw(0, 0, ofGetWindowWidth() * _scaleFactor, layoutHeaderImage.getHeight() * _scaleFactor);
    
    ofPushStyle();
    for(int i=0; i<gridColumns; i++)
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
        ofRect((0 + displayGridMargin + (thumbWidth+displayGridMargin) * i) * _scaleFactor, (0 + headerHeight*0.7) * _scaleFactor, thumbWidth * _scaleFactor, headerHeight* 0.2 * _scaleFactor);
    }
    ofPopStyle();
    
    float tempY = 0;
    int tempXPos = 0;
    int menuHeightInRows = 4;
    int originalThumbHeight = 144;
    
    tempXPos = gridColumns-3;
    menuSettings.setPosition((leftMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuSettings.setSize(thumbWidth, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*1 - displayGridMargin);
    menuSettings.drawMenu();
    
    if (loadedMovie.isMovieLoaded) {
        tempXPos = 0;
        menuMovieInfo.setPosition((leftMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
        menuMovieInfo.setSize(thumbWidth, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
        menuMovieInfo.drawMenu();
        drawMovieInfo((leftMargin + displayGridMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, headerHeight + displayGridMargin*3, menuMovieInfo.getRelSizeH());
        fontStashHelveticaMedium.draw(loadedMovie.gmMIFileName, 10, (int)(leftMargin + 33 * _scaleFactor), (int)((0 + headerHeight*0.6) * _scaleFactor));
    }

    tempXPos = gridColumns-2;
    menuHelp.setPosition((leftMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuHelp.setSize(thumbWidth, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
    menuHelp.drawMenu();
    ofSetColor(255, 255, 255, menuHelp.getRelSizeH() * 255);
    helpMenuImage.draw((leftMargin + displayGridMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, headerHeight + displayGridMargin*1, helpMenuImage.getWidth(), helpMenuImage.getHeight() * menuHelp.getRelSizeH());
    
    
    tempXPos = gridColumns-1;

    ofSetColor(255, 255, 255, 255);
    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
    float tempXPosLerp = ofLerp((ofGetWindowWidth()-scrollBarWidth)/2.0, ((leftMargin + (thumbWidth + displayGridMargin)*tempXPos)/2.0) * _scaleFactor, menuMoviePrintSettings.getRelSizeH());
        fboToPreview.draw(tempXPosLerp, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*menuHeightInRows/2.0 - displayGridMargin, tweenMoviePrintPreview.update() * fboToPreviewWidth, tweenMoviePrintPreview.update() * fboToPreviewHeight);
    ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
    
    menuMoviePrintSettings.setPosition((leftMargin + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuMoviePrintSettings.setSize(thumbWidth, headerHeight + topMargin + (originalThumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
    menuMoviePrintSettings.drawMenu();


    ofSetColor(255, 255, 255, 255);
    
    if (!showListView) {
        if (loadedMovie.isMovieLoaded) {
            menuTimeline.setPosition(0, ofGetWindowHeight());
            menuTimeline.setSize(ofGetWindowWidth(), footerHeight/2);
            menuTimeline.drawMenu();
        }
    }
    
    ofPopStyle;
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::toggleMoviePrintPreview(){
    showMoviePrintPreview = !showMoviePrintPreview;
    if (showMoviePrintPreview) {
        tweenMoviePrintPreview.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,300,0);
    } else {
        tweenMoviePrintPreview.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,300,0);
    }
}

//--------------------------------------------------------------
void testApp::setVisibilityMoviePrintPreview(bool _visibility){
    if (showMoviePrintPreview != _visibility) {
        toggleMoviePrintPreview();
    }
}

//--------------------------------------------------------------
void testApp::drawDisplayGrid(float _scaleFactor, bool _hideInPNG, bool _isBeingPrinted, float _scrollAmountRel, bool _showPlaceHolder){
    ofPushMatrix();
    ofPushStyle();
    float _scrollAmount = 0;
    if (scrollBar.sbActive) {
        _scrollAmount = ((displayGridHeight - (ofGetWindowHeight() - headerHeight - topMargin - bottomMargin)) * -1) * _scrollAmountRel;
    }
    if (isnan(_scrollAmount)) {
        _scrollAmount = 0;
    }
    float tempX = (leftMargin + listWidth * tweenListInOut.update()) * _scaleFactor;
    float tempY = (_scrollAmount + headerHeight + topMargin)  * _scaleFactor;
//    ofLog(OF_LOG_VERBOSE, "_scrollAmount:"+ ofToString(_scrollAmount) +  " tempY:"+ ofToString(tempY) +  "_scrollAmount:"+ ofToString(_scrollAmount));
    loadedMovie.drawGridOfStills(tempX, tempY, gridColumns, displayGridMargin, _scrollAmount, _scaleFactor, 1, _isBeingPrinted, TRUE, superKeyPressed, shiftKeyPressed, _showPlaceHolder);
    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::drawList(float _scrollAmountRel){
    if (droppedFiles.size() > 1){
    float _scrollAmount = 0;
    if (scrollBarList.sbActive) {
//        _scrollAmount = ((droppedList.getListHeight()-scrollBarList.sbAreaHeight)+(bottomMargin+topMargin)*2)*(1-_scrollAmountRel)+(ofGetHeight()/2 - droppedList.getListHeight()/2)-(bottomMargin+topMargin);
        _scrollAmount = ((droppedList.getListHeight()-scrollBarList.sbScrollAreaHeight)+(bottomMargin+topMargin+headerHeight)*2)*-1*(_scrollAmountRel)+topMargin;
    }
    if (isnan(_scrollAmount)) {
        _scrollAmount = 0;
    }

        ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
    
    droppedList.draw(leftMargin - listWidth + listWidth * tweenListInOut.update(), topMargin + headerHeight, ofGetWidth() - scrollBarWidth - rightMargin - leftMargin, _scrollAmount);

    }
}

//--------------------------------------------------------------
void testApp::drawMovieInfo(float _x, float _y, float _fade){
    float tempFontHeightSmall = 14;
    
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, _fade * 255);
    
    for (int i=0; i<stringMovieInfo.size(); i++) {
        float tempWidthOfPath = fontStashHelveticaLight.getBBox(stringMovieInfo[i], tempFontHeightSmall, 0, 0).getWidth();
        fontStashHelveticaLight.draw(stringMovieInfo[i],tempFontHeightSmall, _x, (int)((i * tempFontHeightSmall*1.2)*_fade + _y));
        fontStashHelveticaMedium.draw(stringMovieData[i], tempFontHeightSmall, (int)(_x + tempWidthOfPath), (int)((i * tempFontHeightSmall*1.2)*_fade + _y));
    }
    
    ofPopStyle();
    

    
}


//--------------------------------------------------------------
void testApp::drawPrintScreen(){
    ofPushStyle();
    ofPushMatrix();
    if (showListView) {
        ofTranslate(-ofGetWindowWidth(), 0);
    }
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 127);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255, 255);
    if (currPrintingList) {
        printListImage.draw(ofGetWidth()/2-printImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-printImage.getHeight()/2);
    } else {
        printImage.draw(ofGetWidth()/2-printImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-printImage.getHeight()/2);

    }
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::drawStartScreen(){
    ofPushMatrix();
    ofPushStyle();
    if (tweenBlendStartDropImageCounter.update() > 0.99) {
        if (switchFromLogoToDropZone) {
            tweenBlendStartDropImage.setParameters(1,easingsine,ofxTween::easeInOut,1.0,0.0,1000,0);
            tweenBlendStartDropImageCounter.setParameters(1,easinglinear,ofxTween::easeInOut,0.0,1.0,3000,0);
        } else {
            tweenBlendStartDropImage.setParameters(1,easingsine,ofxTween::easeInOut,0.0,1.0,1000,0);
            tweenBlendStartDropImageCounter.setParameters(1,easinglinear,ofxTween::easeInOut,0.0,1.0,6000,0);
        }
        switchFromLogoToDropZone = !switchFromLogoToDropZone;
    }

    ofSetColor(255, 255, 255, 255);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255, 255 * tweenBlendStartDropImage.update());
    startImage.draw(ofGetWidth()/2-startImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-startImage.getHeight()/2);
    ofSetColor(255, 255, 255, 255 * (1-tweenBlendStartDropImage.update()));
    dropZoneImage.draw(ofGetWidth()/2-startImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-startImage.getHeight()/2);

    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::drawUpdateScreen(){

    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 100);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    int tempLoaderBarWidth = 262;
    int tempLoaderBarHeight = 20;
    int tempOffsetY = 155;
    float tempX = ofGetWidth()/2 - tempLoaderBarWidth/2 + listWidth * tweenListInOut.update();
    float tempY = ofGetHeight()/2 - tempLoaderBarHeight/2;
        // draw the percentage loaded bar
        // the background
        ofSetColor(100);
        ofRect(tempX, tempY + tempOffsetY, tempLoaderBarWidth, tempLoaderBarHeight);
        // draw the percentage
        ofSetColor(238, 71, 0);
        ofRect(tempX, tempY + tempOffsetY, tempLoaderBarWidth*loadValue, tempLoaderBarHeight);

    ofSetColor(255, 255, 255, 255);
    updatingImage.draw(ofGetWidth()/2-updatingImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-updatingImage.getHeight()/2);
    ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::drawLoadMovieScreen(){

    ofPushStyle();
    ofSetColor(255, 255, 255, 200);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255, 255);
    loadMovieImage.draw(ofGetWidth()/2-loadMovieImage.getWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-loadMovieImage.getHeight()/2);
    ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::drawMoviePrintPreview(float _scaleFactor, bool _showPlaceHolder){
    ofPushStyle();
    ofPushMatrix();
    _scaleFactor = _scaleFactor * 0.95;
    float tempX = (fboToPreviewWidth - _scaleFactor * printGridWidth) / 2;
    float tempY = (fboToPreviewHeight - _scaleFactor * printGridHeight) / 2;
    ofTranslate(tempX, tempY);
    ofSetColor(255);
    backgroundImagePreview.draw(0, 0, _scaleFactor * printGridWidth, _scaleFactor * printGridHeight);
    loadedMovie.drawMoviePrint(0, 0, moviePrintDataSet.printGridColumns, moviePrintDataSet.printGridRows, moviePrintDataSet.printGridMargin, _scaleFactor, 1, _showPlaceHolder, printHeaderHeight, moviePrintDataSet.printDisplayVideoAudioInfo, true);
    
    // drawing frame
    float tempFrameWidth = 3;
    ofSetColor(220);
    ofRect(0, 0 - tempFrameWidth, _scaleFactor * printGridWidth + tempFrameWidth, tempFrameWidth);
    ofRect(0 - tempFrameWidth, 0 - tempFrameWidth, tempFrameWidth, _scaleFactor * printGridHeight + tempFrameWidth);
    ofRect(0 + _scaleFactor * printGridWidth, 0, tempFrameWidth, _scaleFactor * printGridHeight + tempFrameWidth);
    ofRect(0 - tempFrameWidth, 0 + _scaleFactor * printGridHeight, _scaleFactor * printGridWidth + tempFrameWidth, tempFrameWidth);
    // drawing shadow
    ofSetColor(0,200);
    ofRect(0 + _scaleFactor * printGridWidth + tempFrameWidth, 0, tempFrameWidth, _scaleFactor * printGridHeight + tempFrameWidth*2);
    ofRect(0, 0 + _scaleFactor * printGridHeight + tempFrameWidth, _scaleFactor * printGridWidth + tempFrameWidth, tempFrameWidth);
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::drawScrubScreen(float _scaleFactor){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(0,(tweenFading.update()/255)*100);
    ofRect(0, 0, ofGetWidth(), ofGetHeight()-footerHeight/2);

    // draw the scrubMovie
    ofSetColor(255,(int)tweenFading.update());
    int j = loadedMovie.gmScrubID;
    loadedMovie.gmMovieScrub.draw(ofGetWidth()/2-scrubWindowW/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH);
    loadedMovie.drawStillUI(j, ofGetWidth()/2-scrubWindowW/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH, (float)(tweenFading.update()/255));
    
    // drawing frame
    float tempX = ofGetWidth()/2-scrubWindowW/2 + listWidth * tweenListInOut.update();
    float tempY = ofGetHeight()/2-scrubWindowH/2;
    float tempFrameWidth = 3;
    ofSetColor(220,(int)tweenFading.update());
    ofRect(tempX, tempY - tempFrameWidth, _scaleFactor * scrubWindowW + tempFrameWidth, tempFrameWidth);
    ofRect(tempX - tempFrameWidth, tempY - tempFrameWidth, tempFrameWidth, _scaleFactor * scrubWindowH + tempFrameWidth);
    ofRect(tempX + _scaleFactor * scrubWindowW, tempY, tempFrameWidth, _scaleFactor * scrubWindowH + tempFrameWidth);
    ofRect(tempX - tempFrameWidth, tempY + _scaleFactor * scrubWindowH, _scaleFactor * scrubWindowW + tempFrameWidth, tempFrameWidth);
    // drawing shadow
    ofSetColor(0,200*(tweenFading.update()/255.0));
    ofRect(tempX + _scaleFactor * scrubWindowW + tempFrameWidth, tempY, tempFrameWidth, _scaleFactor * scrubWindowH + tempFrameWidth*2);
    ofRect(tempX, tempY + _scaleFactor * scrubWindowH + tempFrameWidth, _scaleFactor * scrubWindowW + tempFrameWidth, tempFrameWidth);

    // draw the scrubSpeed
    ofSetColor(FAK_ORANGECOLOR,(int)tweenFading.update());
    ofRect(ofGetWidth()/2 + listWidth * tweenListInOut.update(), ofGetHeight()/2+scrubWindowH/2+tempFrameWidth*3, scrubDelta*30.0, loaderBarHeight/2);
    ofSetColor(255,255,255,(int)tweenFading.update());
    
    ofDisableAlphaBlending();
    ofSetColor(255);
    if(tweenFading.update() < 5){
        updateScrub = FALSE;
        loadedMovie.gmScrubMovie = FALSE;
    }
    ofPopStyle();

}

//--------------------------------------------------------------
void testApp::moveInOutTimeline(){
    
    if (showTimeline) {
        tweenTimelineInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),0);
    } else {
        tweenTimelineInOut.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,ofRandom(600, 1000),0);
    }
   
    if (showTimeline){
        loadedMovie.setAllLimitsLower(ofGetHeight() - footerHeight);
        droppedList.setAllLimitsLower(ofGetHeight() - footerHeight);
    } else {
        loadedMovie.setAllLimitsLower(ofGetHeight());
        droppedList.setAllLimitsLower(ofGetHeight());
    }
    
}

//--------------------------------------------------------------
void testApp::printImageToFile(int _printSizeWidth){
    
    // if folder doesnt exist, create standard
    ofDirectory dir(saveMoviePrintPath);
    if((saveMoviePrintPath == "") || !dir.exists()){
        saveMoviePrintPath = appPathUpStr + "/MoviePrints/";
        uiLabelOutputFolder->setLabel(cropFrontOfString(saveMoviePrintPath, 40, "..."));
        dir.open(saveMoviePrintPath);
        if(!dir.exists()){
            dir.create(true);
        }
    }

    //now you can be sure that path exists
    ofLog(OF_LOG_VERBOSE, dir.getOriginalDirectory() );
    
    ofPixels gmPixToSave;
    ofFbo fboToSave;
    
    if (loadedMovie.isMovieLoaded) {
        float _newScaleFactor = (float)_printSizeWidth / (float)(printGridWidth);
        int outputWidth = printGridWidth * _newScaleFactor;
        int outputHeight = printGridHeight * _newScaleFactor;
        ofLog(OF_LOG_VERBOSE, "outputSize before: " + ofToString(outputWidth) + "x" + ofToString(outputHeight));
        int tempMaxDimension = 16000;
        if (outputWidth > outputHeight) {
            if (outputWidth > tempMaxDimension) {
                _newScaleFactor = tempMaxDimension / printGridWidth;
                outputWidth = tempMaxDimension;
                outputHeight = printGridHeight * _newScaleFactor;
            }
        } else {
            if (outputHeight > tempMaxDimension) {
                _newScaleFactor = tempMaxDimension / printGridHeight;
                outputHeight = tempMaxDimension;
                outputWidth = printGridWidth * _newScaleFactor;
            }
        }
        
        if (moviePrintDataSet.printFormat == OF_IMAGE_FORMAT_JPEG) {
            // make sure that the jpg size can be divided by 4
            if ((outputWidth%2) == 1) {
                outputWidth = outputWidth + 1;
            }
            if ((outputWidth%4) == 2) {
                outputWidth = outputWidth + 2;
            }
            if ((outputHeight%2) == 1) {
                outputHeight = outputHeight + 1;
            }
            if ((outputHeight%4) == 2) {
                outputHeight = outputHeight + 2;
            }
            ofLog(OF_LOG_VERBOSE, "outputSize: " + ofToString(outputWidth) + "x" + ofToString(outputHeight));
            fboToSave.allocate(outputWidth, outputHeight, GL_RGB);
            gmPixToSave.allocate(outputWidth, outputHeight, OF_PIXELS_RGB);
        }
        else {
            ofLog(OF_LOG_VERBOSE, "outputSize: " + ofToString(outputWidth) + "x" + ofToString(outputHeight));
            fboToSave.allocate(outputWidth, outputHeight, GL_RGBA);
            gmPixToSave.allocate(outputWidth, outputHeight, OF_PIXELS_RGB);
        }

        ofPushMatrix();
        ofPushStyle();
        fboToSave.begin();
        ofClear(0,0,0,0);
        ofBackground(0, 0, 0, 0);
        ofSetColor(255, 255, 255, 255);

        loadedMovie.drawMoviePrint(0, 0, moviePrintDataSet.printGridColumns, moviePrintDataSet.printGridRows, moviePrintDataSet.printGridMargin, _newScaleFactor, 1, showPlaceHolder, printHeaderHeight, moviePrintDataSet.printDisplayVideoAudioInfo, false);

        fboToSave.end();
        fboToSave.readToPixels(gmPixToSave);
                ofLog(OF_LOG_VERBOSE, "gmPixToSave:getImageType" + ofToString(gmPixToSave.getImageType()));
        ofPopStyle();
        ofPopMatrix();

        string pathName = loadedMovie.gmMovie.getMoviePath();
        string fileName = loadedFilePath.getFileName(pathName, TRUE);
        string formatExtension;
        if (moviePrintDataSet.printFormat == OF_IMAGE_FORMAT_JPEG) {
            formatExtension = "jpg";
        } else {
            formatExtension = "png";
        }
        string imageName = fileName + "_MoviePrint." + formatExtension;
        imageName = saveMoviePrintPath + imageName;
        
        if (moviePrintDataSet.printSingleFrames) {
            string singleImagePath = saveMoviePrintPath+fileName+"/";
            ofDirectory dir2(singleImagePath);
            if(!dir2.exists()){
                dir2.create(true);
            }
            for (int i=0; i<loadedMovie.gmNumberOfStills; i++) {
                string singleImageName = fileName + "_" + ofToString(i, 3, '0') + "." + formatExtension;
                singleImageName = singleImagePath + singleImageName;
                ofSaveImage(loadedMovie.grabbedStill[i].gsImage, singleImageName, OF_IMAGE_QUALITY_HIGH);
            }
        }
        
        
        ofSaveImage(gmPixToSave, imageName, OF_IMAGE_QUALITY_HIGH);
        ofLog(OF_LOG_VERBOSE, "Finished saving" + ofToString(imageName) );
    }
    
    if (!currPrintingList) {
        stopPrinting();
    } else {
        ofLog(OF_LOG_VERBOSE, "Finished Printing file in list------------------------------- currPrintingList" + ofToString(currPrintingList));
    }
    
}

//--------------------------------------------------------------
void testApp::printListToFile(){
    
    if (!droppedList.glDroppedItem[itemToPrint].itemProperties.ipTriedToPrint && itemToPrint == 0 && !movieIsBeingGrabbed && !currPrintingList) {
        currPrintingList = TRUE;
        droppedList.disableMouseEvents(droppedFiles.size());
        droppedList.setActiveItem(itemToPrint);
        loadNewMovie(droppedList.glDroppedItem[itemToPrint].gliFile.path(), TRUE, TRUE, FALSE);
    }
    if (!droppedList.glDroppedItem[itemToPrint].itemProperties.ipTriedToPrint && !movieIsBeingGrabbed && currPrintingList) {
        ofLog(OF_LOG_VERBOSE, "printImageToFile: " + ofToString(itemToPrint) );
        if (loadedMovie.isMovieLoaded){
            calculateNewPrintGrid();
            printImageToFile(moviePrintDataSet.printSizeWidth);
            droppedList.glDroppedItem[itemToPrint].itemProperties.ipPrinted = TRUE;
        }
        droppedList.glDroppedItem[itemToPrint].itemProperties.ipTriedToPrint = TRUE;
        ofLog(OF_LOG_VERBOSE, "ipTriedToPrint: " + ofToString(droppedList.glDroppedItem[itemToPrint].itemProperties.ipTriedToPrint) );
        ofLog(OF_LOG_VERBOSE, "ipPrinted: " + ofToString(droppedList.glDroppedItem[itemToPrint].itemProperties.ipPrinted) );
        for (int i=0; i < droppedList.glDroppedItem.size(); i++) {
            if (!droppedList.glDroppedItem[i].itemProperties.ipTriedToPrint){
                itemToPrint = i;
                i = droppedList.glDroppedItem.size();
            }
        }
        droppedList.setActiveItem(itemToPrint);
        loadNewMovie(droppedList.glDroppedItem[itemToPrint].gliFile.path(), TRUE, TRUE, FALSE);
    }
    
    int tempIterator = 0;
    for (int i=0; i < droppedList.glDroppedItem.size(); i++) {
        if (droppedList.glDroppedItem[i].itemProperties.ipTriedToPrint){
            tempIterator++;
        }
    }
    if (tempIterator >= droppedList.glDroppedItem.size()) {
        stopListPrinting();
    }

    timer.setStartTime();
    finishedPrinting = TRUE;
    
}

//--------------------------------------------------------------
void testApp::resetItemsToPrint(){
    for (int i=0; i < droppedList.glDroppedItem.size(); i++) {
        droppedList.glDroppedItem[i].itemProperties.ipTriedToPrint = FALSE;
        droppedList.glDroppedItem[i].itemProperties.ipPrinted = FALSE;
    }
}


//--------------------------------------------------------------
void testApp::rollOverButtonsClicked(int _rollOverMovieID, int _rollOverMovieButtonID){
    if (_rollOverMovieButtonID == 3) {
        setInPoint(loadedMovie.grabbedStill[_rollOverMovieID].gsFrameNumber);
        loadedMovie.gmRollOver = FALSE;
        ofLog(OF_LOG_VERBOSE, "manipulated InPoint" );

    } else if (_rollOverMovieButtonID == 4) {
        setOutPoint(loadedMovie.grabbedStill[_rollOverMovieID].gsFrameNumber);
        loadedMovie.gmRollOver = FALSE;
        ofLog(OF_LOG_VERBOSE, "manipulated OutPoint" );
        
    } else if (_rollOverMovieButtonID == 1) {
        ofLog(OF_LOG_VERBOSE, "frame backwards" );
        int j = loadedMovie.grabbedStill[_rollOverMovieID].gsFrameNumber;
        if(shiftKeyPressed) {
            j = j - 10;
        } else if(superKeyPressed){
            j = j - 100;
        } else {
            j = j - 1;
        }
        if (j < 0) {
            j = 0;
        } else if (j > (totalFrames-1)) {
            j = (totalFrames-1);
        }
//        if (_rollOverMovieID == 0) {
//            setInPoint(j);
//        } else if (_rollOverMovieID == (numberOfStills-1)) {
//            setOutPoint(j);
//        } else {
//            updateOneThumb(_rollOverMovieID, j);
//        }
        updateOneThumb(_rollOverMovieID, j);
    } else if (_rollOverMovieButtonID == 2) {
        ofLog(OF_LOG_VERBOSE, "frame forward" );
        int j = loadedMovie.grabbedStill[_rollOverMovieID].gsFrameNumber;
        if(shiftKeyPressed) {
            j = j + 10;
        } else if(superKeyPressed){
            j = j + 100;
        } else {
            j = j + 1;
        }
        if (j < 0) {
            j = 0;
        } else if (j > (totalFrames-1)) {
            j = (totalFrames-1);
        }
//        if (_rollOverMovieID == 0) {
//            setInPoint(j);
//        } else if (_rollOverMovieID == (numberOfStills-1)) {
//            setOutPoint(j);
//        } else {
//            updateOneThumb(_rollOverMovieID, j);
//        }
        updateOneThumb(_rollOverMovieID, j);
    }
    rollOverClicked = FALSE;
}

//--------------------------------------------------------------
int testApp::getLowestFrameNumber(){
    return *min_element(moviePrintDataSet.gridTimeArray.begin(), moviePrintDataSet.gridTimeArray.end());
}

//--------------------------------------------------------------
int testApp::getHighestFrameNumber(){
    return *max_element(moviePrintDataSet.gridTimeArray.begin(), moviePrintDataSet.gridTimeArray.end());
}

//--------------------------------------------------------------
void testApp::updateOneThumb(int _thumbID, int _newFrameNumber){
    moviePrintDataSet.gridTimeArray[_thumbID] = _newFrameNumber;
    loadedMovie.grabbedStill[_thumbID].gsFrameNumber = _newFrameNumber;
    loadedMovie.grabbedStill[_thumbID].gsManipulated = TRUE;
    loadedMovie.grabbedStill[_thumbID].gsToBeGrabbed = TRUE;
    loadedMovie.grabbedStill[_thumbID].gsToBeUpdated = TRUE;
    loadedMovie.updateOrderNumber();
    
    if (!loadedMovie.isThreadRunning()) {
        loadedMovie.start();
    }

    uiRangeSliderTimeline->setValueLow(getLowestFrameNumber());
    uiRangeSliderTimeline->setValueHigh(getHighestFrameNumber());

    ofLog(OF_LOG_VERBOSE, "manipulated one thumb" );
}

//--------------------------------------------------------------
void testApp::setInPoint(int _inPoint){
    int i = _inPoint;
    int j = uiRangeSliderTimeline->getScaledValueHigh();
    if ((uiRangeSliderTimeline->getScaledValueHigh()-i < numberOfStills)) {
        j = i + (numberOfStills - 1);
        if (j > (totalFrames-1)) {
            j = (totalFrames-1);
            i = j - (numberOfStills - 1);
        }
    }
    uiRangeSliderTimeline->setValueLow(i);
    uiRangeSliderTimeline->setValueHigh(j);
    uiSliderValueLow = i;
    uiSliderValueHigh = j;
    updateGridTimeArrayWithAutomaticInterval();
    updateAllStills();
    ofLog(OF_LOG_VERBOSE, "manipulated InPoint" );
}

//--------------------------------------------------------------
void testApp::setOutPoint(int _outPoint){
    int i = uiRangeSliderTimeline->getScaledValueLow();
    int j = _outPoint;
    if ((j - uiRangeSliderTimeline->getScaledValueLow() < numberOfStills)) {
        i = j - (numberOfStills - 1);
        if (i < 0) {
            i = 0;
            j = (numberOfStills - 1);
            
        }
    }
    uiRangeSliderTimeline->setValueLow(i);
    uiRangeSliderTimeline->setValueHigh(j);
    uiSliderValueLow = i;
    uiSliderValueHigh = j;
    updateGridTimeArrayWithAutomaticInterval();
    updateAllStills();
    ofLog(OF_LOG_VERBOSE, "manipulated OutPoint" );
}

//--------------------------------------------------------------
void testApp::updateTimeSlider(bool _wholeRange) {
    
    uiRangeSliderTimeline->setMax(totalFrames-1);
    uiRangeSliderTimeline->setValueLow(0);
    
    if (_wholeRange) {
        if (totalFrames > 250) {
            uiRangeSliderTimeline->setValueLow(25);
            uiRangeSliderTimeline->setValueHigh(totalFrames-26);
        } else {
            uiRangeSliderTimeline->setValueHigh(totalFrames-1);
        }
    } else {
        uiRangeSliderTimeline->setValueHigh(numberOfStills);
    }
    uiSliderValueLow = uiRangeSliderTimeline->getScaledValueLow();
    uiSliderValueHigh = uiRangeSliderTimeline->getScaledValueHigh();
}

//--------------------------------------------------------------
void testApp::updateGridTimeArrayWithAutomaticInterval(){

    if (uiSliderValueLow < 0) {
        uiSliderValueLow = 0;
    }
    if (uiSliderValueHigh > (totalFrames-1)) {
        uiSliderValueHigh = (totalFrames-1);
    }
    
    for (int i=0; i<numberOfStills; i++) {
        if (numberOfStills == 1) {
            moviePrintDataSet.gridTimeArray[i] = ofMap(0.5, 0.0, 1.0, uiSliderValueLow, uiSliderValueHigh, TRUE);
            
        } else {
            moviePrintDataSet.gridTimeArray[i] = ofMap(float(i)/(numberOfStills - 1), 0.0, 1.0, uiSliderValueLow, uiSliderValueHigh, TRUE);
        }
    }
}

//--------------------------------------------------------------
void testApp::updateAllStills(){
    ofLog(OF_LOG_VERBOSE, "Start Updating------------------------------------------------");
    finishedUpdating = FALSE;
    showUpdateScreen = TRUE;
   
    loadedMovie.updateAllFrameNumbers(&moviePrintDataSet.gridTimeArray);

//    if (updateGridTimeArrayToMoviePrintDataSet) {
//        addGridTimeArrayToMoviePrintDataSet();
//    }

    movieIsBeingGrabbed = TRUE;
    loadedMovie.start();
    
    ofxNotify() << "Thread is started - " + ofToString(numberOfStills) + " Stills are being updated";
}

//--------------------------------------------------------------
void testApp::menuIsOpened(int &e){
//    ofLog(OF_LOG_VERBOSE, "menuIsOpened:" + ofToString(e));
    if (e == 5) {
        setVisibilityMoviePrintPreview(true);
    }
    allMenusAreClosed = false;
    allMenusAreClosedOnce = 1;
}

//--------------------------------------------------------------
void testApp::menuIsClosed(int &e){
//    ofLog(OF_LOG_VERBOSE, "menuIsClosed:" + ofToString(e));
    if (e == 5) {
        setVisibilityMoviePrintPreview(false);
    }
    if (!menuMovieInfo.getMenuActivated() && !menuMoviePrintSettings.getMenuActivated() && !menuHelp.getMenuActivated()) {
        allMenusAreClosed = true;
        allMenusAreClosedOnce = 0;
//        ofLog(OF_LOG_VERBOSE, "allMenusAreClosed:" + ofToString(allMenusAreClosedOnce));
    }
}

//--------------------------------------------------------------
void testApp::menuIsClicked(int &e){
//    ofLog(OF_LOG_VERBOSE, "menuIsClicked:" + ofToString(e));
    if (e == 6) {
        if (droppedFiles.size() > 1) {
            if (loadedMovie.isMovieLoaded) {
                showListView = !showListView;
                if (showListView == TRUE) {
                    moveToList();
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::startPrinting(){
    lockedDueToPrinting = true;
    inactivateAllMenus();
    ofLog(OF_LOG_VERBOSE, "Start Printing------------------------------------------------");
    printImageToFile(moviePrintDataSet.printSizeWidth);
}

//--------------------------------------------------------------
void testApp::stopPrinting(){
    timer.setStartTime();
    finishedPrinting = TRUE;
    activateAllMenus();
    droppedList.enableMouseEvents();
    lockedDueToPrinting = false;
    ofLog(OF_LOG_VERBOSE, "Finished Printing-------------------------------------------- ");
}

//--------------------------------------------------------------
void testApp::startListPrinting(){
    lockedDueToPrinting = true;
    inactivateAllMenus();
    ofLog(OF_LOG_VERBOSE, "Start Printing List-------------------------------------------");
    itemToPrint = 0;
    droppedList.disableMouseEvents(droppedFiles.size());
    resetItemsToPrint();
    printListToFile();
}

//--------------------------------------------------------------
void testApp::stopListPrinting(){
    currPrintingList = FALSE;
    moveToList();
    activateAllMenus();
    droppedList.enableMouseEvents();
    lockedDueToPrinting = false;
    ofLog(OF_LOG_VERBOSE, "Finished Printing List-------------------------------------------");
}

//--------------------------------------------------------------
void testApp::moveToMovie(){
    lockedDueToInteraction = true;
    closeAllMenus();
    
    showListView = FALSE;
    
    droppedList.disableMouseEvents(droppedFiles.size());
    loadedMovie.enableMouseEvents();
    
    scrollBarList.unregisterMouseEvents();
    scrollBarList.unregisterTouchEvents();
    scrollBar.registerMouseEvents();
    scrollBar.registerTouchEvents();
    
    guiTimeline->setVisible(TRUE);
    menuTimeline.registerMouseEvents();
    menuTimeline.setMenuActive();
    
    printListNotImage = FALSE;
    updateInOut = FALSE;
    manipulateSlider = FALSE;
    
    if (tweenListInOut.update() != 0.0) {
        tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),0);
    }

}

//--------------------------------------------------------------
void testApp::moveToList(){
    lockedDueToInteraction = true;
    closeAllMenus();
    
    showListView = TRUE;
    
    scrollBar.unregisterMouseEvents();
    scrollBar.unregisterTouchEvents();
    
    scrollBarList.registerMouseEvents();
    scrollBarList.registerTouchEvents();
    
    loadedMovie.stop(TRUE);
    loadedMovie.disableMouseEvents();
    
    guiTimeline->setVisible(FALSE);
    menuTimeline.unRegisterMouseEvents();
    menuTimeline.setMenuInactive();
    
    droppedList.enableMouseEvents();
    printListNotImage = TRUE;
    
    if (tweenListInOut.update() != 1.0) {
        tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,ofRandom(600, 1000),0);
    }
}

//--------------------------------------------------------------
void testApp::handlingEventOverlays(){
    // check if one of the topMenus is active and in this case turn of the mouseEvents for the thumbs
    if (menuMovieInfo.getMenuActivated() || menuMoviePrintSettings.getMenuActivated() || menuHelp.getMenuActivated() || menuSettings.getMenuActivated()) {
        if (loadedMovie.getMouseEventsEnabled()) {
            loadedMovie.disableMouseEvents();
        }
        if (showListView) {
            droppedList.disableMouseEvents(droppedFiles.size());
        }
    } else {
        if (allMenusAreClosedOnce == 0) {
            if (!lockedDueToInteraction && !lockedDueToPrinting) {
                allMenusAreClosedOnce++;
                if (!loadedMovie.getMouseEventsEnabled() && !showListView) {
                    loadedMovie.enableMouseEvents();
                }
                if (showListView) {
                    droppedList.enableMouseEvents();
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::closeAllMenus(){
    menuMoviePrintSettings.closeMenuManually();
    menuMovieInfo.closeMenuManually();
    menuHelp.closeMenuManually();
    ofLog(OF_LOG_VERBOSE, "closeMenuManually-------------------------------------------");
}

//--------------------------------------------------------------
void testApp::inactivateAllMenus(){
    menuMoviePrintSettings.setMenuInactive();
    menuMovieInfo.setMenuInactive();
    menuHelp.setMenuInactive();
    ofLog(OF_LOG_VERBOSE, "inactivateAllMenus-------------------------------------------");
}

//--------------------------------------------------------------
void testApp::activateAllMenus(){
    menuMoviePrintSettings.setMenuActive();
    menuMovieInfo.setMenuActive();
    menuHelp.setMenuActive();
    ofLog(OF_LOG_VERBOSE, "activateAllMenus-------------------------------------------");
}

//--------------------------------------------------------------
bool testApp::checkExtension(string _tempExtension){
    bool tempReturn = FALSE;
    int length = _tempExtension.length();
    for(int i=0; i < length; ++i)
    {
        _tempExtension[i] = tolower(_tempExtension[i]);
    }
    
    if (_tempExtension == "mp4" ||
        _tempExtension == "mov" ||
        _tempExtension == "m4v" ||
        _tempExtension == "qt" ||
        _tempExtension == "wmv" ||
        _tempExtension == "avi" ||
        _tempExtension == "pdf" ||
//        _tempExtension == "jpg" ||
//        _tempExtension == "jpe" ||
//        _tempExtension == "jpeg" ||
//        _tempExtension == "gif" ||
//        _tempExtension == "png" ||
//        _tempExtension == "tif" ||
//        _tempExtension == "tiff" ||
//        _tempExtension == "bmp" ||
        _tempExtension == "3gp" ||
        _tempExtension == "3gp2" ||
        _tempExtension == "3g2" ||
        _tempExtension == "3gpp" ||
        _tempExtension == "mkv" ||
        _tempExtension == "dv" ||
//        _tempExtension == "flv" ||
//        _tempExtension == "swf" ||
//        _tempExtension == "vob" ||
        _tempExtension == "divx" ||
//        _tempExtension == "mxf" ||
        _tempExtension == "m1v" ||
        _tempExtension == "mpeg" ||
        _tempExtension == "mpeg1" ||
        _tempExtension == "mpeg2" ||
        _tempExtension == "mpeg4" ||
        _tempExtension == "mpg" ||
        _tempExtension == "mpv" ||
        _tempExtension == "mpv2" ||
        _tempExtension == "yuv" ||
        _tempExtension == "m2v")
    {
        tempReturn = TRUE;
    }
    
    return tempReturn;
}

//--------------------------------------------------------------
void testApp::setResourcePath(){
    CFURLRef appUrl = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef appPath = CFURLCopyFileSystemPath(appUrl, kCFURLPOSIXPathStyle);
    char appPathChars[PATH_MAX];
    CFStringGetCString(appPath, appPathChars, FILENAME_MAX, kCFStringEncodingASCII );
    appPathStr = string(appPathChars);
    appPathUpStr = appPathStr.substr(0,appPathStr.find_last_of("\\/"));
    CFRelease(appUrl);
    CFRelease(appPath);
    
    CFURLRef resourceUrl = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
    CFStringRef resourcePath = CFURLCopyFileSystemPath(resourceUrl, kCFURLPOSIXPathStyle);
    char resourcePathChars[PATH_MAX];
    CFStringGetCString(resourcePath, resourcePathChars, FILENAME_MAX, kCFStringEncodingASCII );
    string resourcePathStr = string(resourcePathChars);
    CFRelease(resourcePath);
    CFRelease(resourceUrl);
    
    string newPath = appPathStr + "/" + resourcePathStr + "/"; // the absolute path to the resources folder
    ofLog(OF_LOG_VERBOSE, "appPathStr: " + ofToString(appPathStr) );
    ofLog(OF_LOG_VERBOSE, "appPathUpStr: " + ofToString(appPathUpStr) );
    ofLog(OF_LOG_VERBOSE, "newDataPath: " + ofToString(newPath) );
    ofSetDataPathRoot(newPath); // tell OF to look for resources here
}

//--------------------------------------------------------------
bool testApp::fequal(float _x, float _y, float _t){
    float diff = abs(_x-_y);
    if (diff < _t) {
        return TRUE;
    } else  {
        return FALSE;
    }
}
