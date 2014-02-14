#include "testApp.h"
#include "ofxXmlSettings.h"

#define FAK_ORANGECOLOR ofColor(238, 71, 0, 255)
#define FAK_DARKORANGECOLOR ofColor(99, 30, 0, 255)
#define FAK_DARKDARKORANGECOLOR ofColor(75, 21, 0, 255)
#define FAK_MIDDLEDARKORANGECOLOR ofColor(170, 50, 0, 255)
#define FAK_LIGHTERMIDDLEDARKORANGECOLOR ofColor(185, 55, 0, 255)

#define FAK_GREENCOLOR ofColor(117, 130, 16, 255)


//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    drawNotify = FALSE;
    
    setResourcePath();
    
    setupFinished = FALSE;
    updateWindowResized = FALSE;
    devTurnOffMovieSwitch = TRUE;
    showDroppedList = FALSE;
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
    scrubFade = 255.0;
    scrubWindowW = 640;
    scrubWindowGridNumber = 0;
    scrubDelta = 0;
    totalFrames = 0;
    itemToPrint = 0;
    loadNewMovieToBeScrubbedBool = FALSE;
    
    // UI Values
    leftMargin = 15;
    rightMargin = 15;
    topMargin = 5;
    bottomMargin = 5;
    headerHeight = 5;
    footerHeight = 50;
    gridMargin = 2;
    gridMarginRatio = 10.0/320.0;
    manualGridMargin = FALSE;
    loaderBarHeight = 20;
    timeSliderHeight = 24;
    gridColumns = 4;
    gridNumber = 4;
    menuWidth = 255;
    
    threadIsRunning = FALSE;
    
    printScale = 2.0;
    printSizeWidth = 1024;
    showPrintScreen = FALSE;
    finishedPrinting = TRUE;
    printFormat = OF_IMAGE_FORMAT_PNG;
    
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
    gridSetManually = TRUE;

    gridWidth = 80;
    gridHeight = 45;
    scrollBarWidth = 15;
    scrollAmountRel = 0;
    scrollListAmountRel = 0;
        
    counterToUpdate = 0;
    counterToLoad = 0;
    counterToPrint = 0;
    
    Tweenzor::init();
    showMenu = FALSE;
    Tweenzor::add(&tweenzorX1, 0.f, 1.f, 0.f, 0.5f, EASE_IN_OUT_EXPO);
    Tweenzor::add(&tweenzorY1, 0.f, 1.f, 0.f, 0.5f, EASE_IN_OUT_EXPO);

    startImage.loadImage("MoviePrint_StartBildschirm_v001_00000.png");
    backgroundImage.loadImage("MoviePrint_Background_v001_00000.jpg");
    printImage.loadImage("MoviePrint_Print_v001_00000.png");
    loadMovieImage.loadImage("MoviePrint_Loading_v001_00000.png");
    updatingImage.loadImage("MoviePrint_Updating_v001_00000.png");
    inPointImage.loadImage("MoviePrint_InPoint_v001_00000.png");
    outPointImage.loadImage("MoviePrint_OutPoint_v001_00000.png");
    printListImage.loadImage("MoviePrint_PrintList_v001_00000.png");
        
    // load standard movie
    loadedFile = "Nothing";

    loadedMovie.gmUpperLimitY = headerHeight;
    loadedMovie.gmLowerLimitY = ofGetHeight() - footerHeight;
    loadedMovie.gmLeftLimitX = menuWidth;
    loadedMovie.gmRightLimitX = ofGetWidth() - rightMargin;
    calculateNewGrid(ofGetWidth(), ofGetHeight());
    loadedMovie.setup(loadedFile, numberOfStills, gridWidth, gridHeight);
    totalFrames = 100;
    loadedMovie.createGrid(numberOfStills, gridWidth, gridHeight);

    updateInOut = FALSE;
    manipulateSlider = FALSE;
    updateDropped = FALSE;
    
    
    scrollBar.setup(0, ofGetWidth()-scrollBarWidth, 0, scrollBarWidth, ofGetHeight(), ofGetWidth()-scrollBarWidth, ofGetHeight()/2, scrollBarWidth, 4);
    scrollBar.setScrollHeight((float)gridAreaHeight);
    scrollBar.registerMouseEvents();
    ofAddListener(scrollBar.sbScrollingGoingOn, this, &testApp::scrollEvent);

    scrollBarList.setup(0, ofGetWidth()-scrollBarWidth, 0, scrollBarWidth, ofGetHeight(), ofGetWidth()-scrollBarWidth, ofGetHeight()/2, scrollBarWidth, 4);
    scrollBarList.setScrollHeight(0.5);
    scrollBarList.unregisterMouseEvents();
    ofAddListener(scrollBarList.sbScrollingGoingOn, this, &testApp::scrollEvent);
    
    setGUI1();  
    setGUI2();
    gui2->loadSettings("guiSettings2.xml");
    
    moveInOutTimeline();
    
    setupFinished = TRUE;
}

//--------------------------------------------------------------
void testApp::setGUI1(){
    
    // setup gui
    gui = new ofxUICanvas(ofGetWidth()/2-gridAreaWidth/2-OFX_UI_GLOBAL_WIDGET_SPACING, ofGetHeight()-(footerHeight/2) - timeSliderHeight/2, ofGetWidth(),footerHeight); //ofxUICanvas(float x, float y, float width, float height)
    gui->setFont("Ubuntu-Light.ttf");
    drawPadding = FALSE;
    gui->setDrawBack(FALSE);
    ofColor paddingColor = ofColor(255,0,0,200);
    ofColor backgroundColor = ofColor(100,100,100,200);
    gui->setColorPadded(paddingColor);
    gui->setColorBack(backgroundColor);
    
	gui->addWidgetDown(new ofxUIRangeSliderJak("RSLIDER", 0.0, (float)totalFrames, 0.0, 100.0, gridAreaWidth, timeSliderHeight));
    timeSlider = (ofxUIRangeSliderJak *) gui->getWidget("RSLIDER");
    timeSlider->setLabelPrecision(0);
    timeSlider->setLabelVisible(FALSE);
//    timeSlider->setDrawPadding(FALSE);
//    timeSlider->setDrawPaddingOutline(FALSE);
//    timeSlider->setDrawOutlineHighLight(FALSE);
//    timeSlider->setDrawOutline(FALSE);
//    gui->setDrawPadding(FALSE);
//    gui->setDrawPaddingOutline(FALSE);
//    gui->setDrawOutlineHighLight(FALSE);
//    gui->setDrawOutline(FALSE);
    
//    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(0,25));
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_OUTLINE, ofColor(0,0,0,0));
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_OUTLINE_HIGHLIGHT, ofColor(0,0,0,0));
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL, FAK_DARKORANGECOLOR);
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL_HIGHLIGHT, FAK_LIGHTERMIDDLEDARKORANGECOLOR);
    


//    ofColor tempColor (30, 30, 30, 255);
//    timeSlider->setColorBack(tempColor);

    timeSlider->setColorPadded(paddingColor);

    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    //    gui->loadSettings("GUI/guiSettings.xml");
    //    gui->disable();

}

//--------------------------------------------------------------
void testApp::setGUI2(){
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = menuWidth-xInit;
	hideGUI2 = false;
    
    
	
	gui2 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    gui2->setFont("Ubuntu-Light.ttf");

    gui2->addWidgetDown(new ofxUILabel("SETTINGS", OFX_UI_FONT_LARGE));
    gui2->addSpacer(length-xInit, 1);
	gui2->addWidgetDown(new ofxUILabel("SET RASTER", OFX_UI_FONT_MEDIUM));
    vector<string> names2;
	names2.push_back("Set Columns and Rows");
	names2.push_back("Set Number of Frames");
    //	names2.push_back("SceneDetection");
    gui2->addWidgetDown(new ofxUIRadio( dim, dim, "Grid-Number", names2, OFX_UI_ORIENTATION_VERTICAL));
    setFitManually = (ofxUIRadio *) gui2->getWidget("Grid-Number");
    setFitManually->activateToggle("Set Columns and Rows");
    
	gui2->addSlider_jak("Columns", 4, 10, gridColumns, length-xInit,dim);
	gui2->addSlider_jak("Rows", 1, 20, gridRows, length-xInit,dim);
   	gui2->addSlider_jak("Number", 4, 200, gridNumber, length-xInit,dim);
    columnSlider = (ofxUISlider_jak *) gui2->getWidget("Columns");
    rowSlider = (ofxUISlider_jak *) gui2->getWidget("Rows");
    numberSlider = (ofxUISlider_jak *) gui2->getWidget("Number");
    
    vector<string> names;
	names.push_back("Frames");
	names.push_back("TimeCode");
	names.push_back("off");
    gui2->addSpacer(length-xInit, 1);
    gui2->addWidgetDown(new ofxUILabel("SHOW INFO", OFX_UI_FONT_MEDIUM));
	gui2->addWidgetDown(new ofxUIRadio( dim, dim, "RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_VERTICAL));
    setFrameDisplay = (ofxUIRadio *) gui2->getWidget("RADIO HORIZONTAL");
    //    ((ofxUIRadio*)gui2->getWidget("RADIO HORIZONTAL"))->activateToggle("TimeCode");
    setFrameDisplay->activateToggle("TimeCode");
    
    
    vector<string> names3;
    names3.push_back("png");
    names3.push_back("jpg");
    ddl = new ofxUIDropDownList(length-xInit, "Choose Output Format", names3, OFX_UI_FONT_MEDIUM);
    ddl->setAllowMultiple(FALSE);
    ddl->setAutoClose(true);
    //    gui2->addSpacer(length-xInit, 2);
    //    gui2->addWidgetDown(ddl);
    
    vector<string> names4;
    names4.push_back("1024px wide");
    names4.push_back("2048px wide");
    names4.push_back("3072px wide");
    names4.push_back("4096px wide");
    ddl2 = new ofxUIDropDownList(length-xInit, "MoviePrint Width", names4, OFX_UI_FONT_MEDIUM);
    ddl2->setAllowMultiple(FALSE);
    ddl2->setAutoClose(true);
    gui2->addSpacer(length-xInit, 1);
    gui2->addWidgetDown(ddl2);

    
    
    ofLog(OF_LOG_VERBOSE, "getColorBack" + ofToString(gui2->getColorBack()));
    ofLog(OF_LOG_VERBOSE, "getDrawBack" + ofToString(gui2->getDrawBack()));
    ofLog(OF_LOG_VERBOSE, "getColorFill" + ofToString(gui2->getColorFill()));
    ofLog(OF_LOG_VERBOSE, "getDrawFill" + ofToString(gui2->getDrawFill()));
    ofLog(OF_LOG_VERBOSE, "getColorFillHighlight" + ofToString(gui2->getColorFillHighlight()));
    ofLog(OF_LOG_VERBOSE, "getDrawFillHighLight" + ofToString(gui2->getDrawFillHighLight()));
    ofLog(OF_LOG_VERBOSE, "getColorOutline" + ofToString(gui2->getColorOutline()));
    ofLog(OF_LOG_VERBOSE, "getDrawOutline" + ofToString(gui2->getDrawOutline()));
    ofLog(OF_LOG_VERBOSE, "getColorOutlineHighlight" + ofToString(gui2->getColorOutlineHighlight()));
    ofLog(OF_LOG_VERBOSE, "getDrawOutlineHighLight" + ofToString(gui2->getDrawOutlineHighLight()));
    ofLog(OF_LOG_VERBOSE, "getColorPadded" + ofToString(gui2->getColorPadded()));
    ofLog(OF_LOG_VERBOSE, "getDrawPadding" + ofToString(gui2->getDrawPadding()));
    ofLog(OF_LOG_VERBOSE, "getColorPaddedOutline" + ofToString(gui2->getColorPaddedOutline()));
    ofLog(OF_LOG_VERBOSE, "getDrawPaddingOutline" + ofToString(gui2->getDrawPaddingOutline()));
    
    ofxUIColor cb = ofxUIColor(FAK_LIGHTERMIDDLEDARKORANGECOLOR); // OFX_UI_COLOR_BACK
    ofxUIColor co = ofxUIColor( 255, 255, 255, 200); // OFX_UI_COLOR_OUTLINE
    ofxUIColor coh = ofxUIColor( 255, 255, 255, 255 ); // OFX_UI_COLOR_OUTLINE_HIGHLIGHT
    ofxUIColor cf = ofxUIColor( 255, 255, 255, 200 ); // OFX_UI_COLOR_FILL
    ofxUIColor cfh = ofxUIColor( 255, 255, 255, 200 ); // OFX_UI_COLOR_FILL_HIGHLIGHT
    ofxUIColor cp = ofxUIColor( 255, 255, 255, 100 ); // OFX_UI_COLOR_PADDED
    ofxUIColor cpo = ofxUIColor( 255, 255, 255, 200 ); // OFX_UI_COLOR_PADDED_OUTLINE
    gui2->setUIWidgetColorsBack(cb); //
//    gui2->setUIWidgetColorsFill(cf,cfh); //

//    gui2->setTheme(OFX_UI_THEME_TEALLIME);
    
//    ofColor tempColor = ofColor(0,0,0,255);
    gui2->setColorBack(FAK_ORANGECOLOR);

	ofAddListener(gui2->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::setGUI3(){

}



//--------------------------------------------------------------
void testApp::calculateNewGrid(int _windowWidth, int _windowHeight){

    
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
    
    int gridFactor = floor((float)(_windowWidth - ((gridColumns * gridMargin) + leftMargin + rightMargin))/(float)gridColumns);
    ofLog(OF_LOG_VERBOSE, "gridFactor: " + ofToString(gridFactor));
   
//    // limit window width and height so that its value doesnt get to small
    if (gridFactor < possStillResWidth169[0]) {
        gridFactor = possStillResWidth169[0];
    }
    
    if (fequal(gridRatio, 0.75, 0.02)) {
        int j = 0;
        for (j; j < 637; j++) {
            if (gridFactor < possStillResWidth43[j]){
                break;
            }
        }
        gridWidth = possStillResWidth43[j-1];
        gridHeight = gridWidth*gridRatio;
    } else {
        int j = 0;
        for (j; j < 637; j++) {
            if (gridFactor < possStillResWidth169[j]){
                break;
            }
        }
        gridWidth = possStillResWidth169[j-1];
        gridHeight = gridWidth*gridRatio;
    }
    if (!manualGridMargin) {
        gridMargin = gridWidth * gridMarginRatio;
    }
    
    if (gridSetManually) {
        numberOfStills = gridColumns*gridRows;
    } else {
        numberOfStills = gridNumber;
//        gridColumns = 4;
        gridRows = gridNumber/gridColumns;
        
    }
    
    int tempGridAreaHeader = 1;

    gridAreaWidth = (gridColumns * (gridWidth + gridMargin) - gridMargin);
    gridAreaHeight = ((numberOfStills / gridColumns) * (gridHeight + gridMargin)) + (tempGridAreaHeader * (gridHeight + gridMargin)) - gridMargin;
    ofLog(OF_LOG_VERBOSE, "gridMargin: " + ofToString(gridMargin));
    ofLog(OF_LOG_VERBOSE, "gridHeight: " + ofToString(gridHeight));
    ofLog(OF_LOG_VERBOSE, "gridAreaHeight: " + ofToString(gridAreaHeight));

    if (!(gridTimeArray == 0)){
        delete[] gridTimeArray;
    }
    gridTimeArray = new (nothrow) int[numberOfStills];
    if (gridTimeArray == 0){
        ofLog(OF_LOG_VERBOSE, "Error: memory could not be allocated" );
    } else {
        for (int i=0; i<numberOfStills; i++) {
            gridTimeArray[i] = i;
        }
    }
    
    loadedMovie.gmGridWidth = gridWidth;
    loadedMovie.gmGridHeight = gridHeight;
    
    scrollBar.updateScrollBar(_windowWidth, _windowHeight, gridAreaHeight, 0, 0);
    scrollBar.setToTop();
    scrollAmountRel = scrollBar.getRelativePos();
    
    ofxNotify() << "New Grid is Calculated - Total Number of Stills: " + ofToString(numberOfStills);

    
    ofLog(OF_LOG_VERBOSE, "gridWidth: " + ofToString(gridWidth));
    ofLog(OF_LOG_VERBOSE, "gridHeight: " + ofToString(gridHeight));

}

//--------------------------------------------------------------
void testApp::loadNewMovie(string _newMoviePath, bool _wholeRange, bool _loadInBackground, bool _loadScrubMovie){
    
    movieLoading = TRUE;
    if (!_loadInBackground) {
        moveToMovie();
    }
    
    loadedMovie.stop(TRUE);

    ofxNotify() << "Movie has started to load";
    loadedMovie.loadNewMovieToBeGrabbed(_newMoviePath, numberOfStills);
    windowResized(ofGetWidth(), ofGetHeight());
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
    updateAllStills();

    if (_loadScrubMovie) {
        loadNewMovieToBeScrubbedBool = TRUE;
    }
    
    if (!_loadInBackground) {
        showDroppedList = FALSE;
        droppedList.glUpdateMovieFromList = FALSE;
    }
    devTurnOffMovieSwitch = FALSE;
    movieLoading = FALSE;
    if (movieProperlyLoaded){
        ofxNotify() << "Movie has finished to load";
    } else{
        ofxNotify() << "Movie could not be loaded";
        ofxNotify() << "Movie could not be loaded";
        ofxNotify() << "Movie could not be loaded";
        ofxNotify() << "Movie could not be loaded";
    }
    scrollBar.updateScrollBar(ofGetWidth(), ofGetHeight(), gridAreaHeight, 0, 0);
    scrollBar.setToTop();
    scrollAmountRel = scrollBar.getRelativePos();
    
    ofLog(OF_LOG_VERBOSE, "scrollAmountRel: " + ofToString(scrollAmountRel));
    
    timer.setStartTime();
    finishedLoadingMovie = TRUE;
    
    ofLog(OF_LOG_VERBOSE, "Finished Loading Movie--------------------------------------------");
    
    if (!loadedMovie.isMovieLoaded) {
        gui->setVisible(FALSE);
        scrollBar.unregisterMouseEvents();
        loadedMovie.disableMouseEvents();
    }
    
}



//--------------------------------------------------------------
void testApp::update(){
    
    threadIsRunning = loadedMovie.isThreadRunning();

    Tweenzor::update( ofGetElapsedTimeMillis() );

//    loadedMovie.update();
    
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
            if (timer.getElapsedSeconds() > 0.5) {
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
    
    // update Tweener
    if (!updateInOut || !updateScrub || !updateWindowResized) {
        Tweener.update();
    }
    
    // update Movie for Scrubbing or InOut manipulations
    if (loadedMovie.gmScrubMovie || updateInOut) {
        loadedMovie.gmMovieScrub.update();
    }
    
    gui2->setPosition(-menuWidth * tweenzorX1, 0);
    gui->setPosition((ofGetWidth()/2-gridAreaWidth/2-OFX_UI_GLOBAL_WIDGET_SPACING) + menuWidth - menuWidth * tweenzorX1, ofGetHeight()-(footerHeight/2 + timeSliderHeight/2) * tweenzorY1);
    
    if (loadedMovie.isMovieLoaded) { // if no movie is loaded or we are in dev mode then only draw rects
        
        // update while scrubbing
        if (loadedMovie.gmScrubMovie) {
            updateInOut = FALSE;
            updateScrub = TRUE;
            int i = loadedMovie.gmScrubID;
            
            scrubDelta = (ofGetMouseX() - loadedMovie.grabbedStill[i].gsX - gridWidth/2); // used this before i let the mouse jump in the middle
//            scrubDelta = (ofGetMouseX() - ofGetWidth()/2);  // used this with mouse jump in the middle
            if (abs(scrubDelta) >= 0 && abs(scrubDelta) < gridWidth/4) {
                scrubDelta = 0;
            }
            if (scrubDelta >= gridWidth/4) {
                scrubDelta = scrubDelta - gridWidth/4;
            } else if (scrubDelta <= -gridWidth/4) {
                scrubDelta = scrubDelta + gridWidth/4;
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

            ofLog(OF_LOG_VERBOSE, "scrubDelta:" + ofToString(scrubDelta));
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
            if (timeSlider->hitLow) {
                scrubWindowGridNumber = 0;
                if (loadedMovie.gmHasNoFrames) {
                    loadedMovie.gmMovieScrub.setPosition((float)(uiSliderValueLow - 1)/(float)(loadedMovie.gmTotalFrames-1));
                    loadedMovie.gmMovieScrub.nextFrame();
                } else {
                    loadedMovie.gmMovieScrub.setFrame(uiSliderValueLow);
                }
                loadedMovie.grabbedStill[scrubWindowGridNumber].gsFrameNumber = uiSliderValueLow;
            } else if (timeSlider->hitHigh)
            {
                scrubWindowGridNumber = numberOfStills-1;
                if (loadedMovie.gmHasNoFrames) {
                    loadedMovie.gmMovieScrub.setPosition((float)(uiSliderValueHigh - 1)/(float)(loadedMovie.gmTotalFrames-1));
                    loadedMovie.gmMovieScrub.nextFrame();
                } else {
                    loadedMovie.gmMovieScrub.setFrame(uiSliderValueHigh);
                }
                loadedMovie.grabbedStill[scrubWindowGridNumber].gsFrameNumber = uiSliderValueHigh;
            } else if (timeSlider->hitCenter)
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
    
    if (updateWindowResized == TRUE && !currPrintingList) {
        updateInOut = FALSE;
        updateScrub = FALSE;
        Tweener.addTween(scrubFade, 0, 0.5);
        if(scrubFade < 5){
            updateWindowResized = FALSE;
            loadedMovie.createGrid(numberOfStills, gridWidth, gridHeight);
            updateAllStills();
            devTurnOffMovieSwitch = FALSE;
        }
    }
    
    if (showDroppedList) {
        gui->setVisible(FALSE);
        loadedMovie.disableMouseEvents();
    }
    
    if (droppedList.glUpdateMovieFromList) {
        printListNotImage = FALSE;
        showDroppedList = FALSE;
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
        showDroppedList = FALSE;
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
        if (scrollGrid) {
            if (!scrollBar.sbMouseScrollCalculateInertia && !scrollBar.sbScrollBarDrag) {
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
        if (scrollList) {
            if (!scrollBarList.sbMouseScrollCalculateInertia && !scrollBar.sbScrollBarDrag) {
                scrollList = false;
            } else {
//            scrollBarList.dragUpdate(mouseX, mouseY);
//            scrollListAmountRel = scrollBarList.getRelativePos();
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
        printListToPNG();
    }

    if (showPrintScreen && !finishedPrinting) {
        // to ensure that print screen is showing before printing starts
        counterToPrint++;
        if (counterToPrint > 1) {
            if (printListNotImage) {
                ofLog(OF_LOG_VERBOSE, "Start Printing List-------------------------------------------");
                itemToPrint = 0;
                resetItemsToPrint();
                printListToPNG();
            } else {
                ofLog(OF_LOG_VERBOSE, "Start Printing------------------------------------------------");
                printImageToPNG(printSizeWidth);
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
    
    if(showLoadMovieScreen){
        
        drawLoadMovieScreen();
        
    } else if(showFBO){ // test
        
        gmFboToSave.draw(100, 100);
        
    } else {
        
        if (showDroppedList) {
            
            drawList(scrollListAmountRel);
            scrollBarList.draw();
            
        } else {
            
            if (!loadedMovie.isMovieLoaded) { // if no movie is loaded
                if (!showDroppedList) {
                    gui->setVisible(FALSE);
                    drawStartScreen();
                }
            } else if (devTurnOffMovieSwitch){// if we are in dev mode then only draw rects
                // draw only rect
                
                drawResizeScreen();
                
            } else {
                
                ofPushStyle();
                ofSetColor(255, 255, 255, 255);
                //            backgroundImage.draw(ofGetWidth()/2-backgroundImage.getWidth()/2, ofGetHeight()/2-backgroundImage.getHeight()/2, ofGetWidth(), ofGetHeight());
                backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
                ofPopStyle();
                
                // draw all frames
                drawMoviePrint(1, FALSE, FALSE, scrollAmountRel);
                
                // draw the In and Out Point Manipulations
                if (updateInOut) {

                    ofPushStyle();
                    
                    ofEnableAlphaBlending();
                    ofSetColor(0,(int)(scrubFade/255)*155);
                    ofRect(0, 0, ofGetWidth(), ofGetHeight());
                    ofSetColor(255,255,255,(int)scrubFade);

                    loadedMovie.gmMovieScrub.draw(ofGetWidth()/2-scrubWindowW/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH);
                    loadedMovie.drawStillUI(scrubWindowGridNumber, ofGetWidth()/2-scrubWindowW/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH, (scrubFade/255));
                    
                    ofSetColor(255, 255, 255, (int)(scrubFade/255)*255);
                    
                    if (timeSlider->hitLow) {
                        inPointImage.draw(ofGetWidth()/2-inPointImage.getWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-inPointImage.getHeight()/2);
                    }
                    if (timeSlider->hitHigh) {
                        outPointImage.draw(ofGetWidth()/2-outPointImage.getWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-outPointImage.getHeight()/2);
                    }
                    if (timeSlider->hitCenter) {
                        inPointImage.draw(ofGetWidth()/2-inPointImage.getWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-inPointImage.getHeight()/2);
                        outPointImage.draw(ofGetWidth()/2-outPointImage.getWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-outPointImage.getHeight()/2);
                    }
                    
                    ofPopStyle();
                    
                    if(scrubFade < 5){
                        updateInOut = FALSE;
                        manipulateSlider = FALSE;
                    }

                    ofSetColor(255);

                }
                
                // draw the scrubbed video
                if (updateScrub) {
                    
                    ofEnableAlphaBlending();
                    ofSetColor(0,(scrubFade/255)*200);
                    if (showTimeline){
                        ofRect(0, 0, ofGetWidth(), ofGetHeight()-footerHeight);
                    } else {
                        ofRect(0, 0, ofGetWidth(), ofGetHeight());
                    }
                    // draw the scrubSpeed
                    ofSetColor(FAK_ORANGECOLOR,(int)scrubFade);
                    ofRect(ofGetWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2+scrubWindowH/2, scrubDelta*30.0, loaderBarHeight);
                    ofSetColor(255,255,255,(int)scrubFade);
                    int j = loadedMovie.gmScrubID;
                    loadedMovie.gmMovieScrub.draw(ofGetWidth()/2-scrubWindowW/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH);
                    loadedMovie.drawStillUI(j, ofGetWidth()/2-scrubWindowW/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH, (float)(scrubFade/255));
                    
                    ofDisableAlphaBlending();
                    ofSetColor(255);
                    if(scrubFade < 5){
                        updateScrub = FALSE;
                        loadedMovie.gmScrubMovie = FALSE;
                    }
                }
                
                scrollBar.draw();
                drawUI(1, FALSE);
                
            }
            
        }
        if (showPrintScreen) {
            drawPrintScreen();
        } else {
            if(showUpdateScreen){
                drawUpdateScreen();
            }
        }
    }
    
    ofxNotify::draw(drawNotify);
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
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
    
    if(gui2->hasKeyboardFocus())
    {
        return;
    }
    
    switch (key)
    {
//        case 'f':
//        {
//            if (loadedMovie.vfFramesToTimeSwitch) {
//                loadedMovie.vfFramesToTimeSwitch = FALSE;
//            } else {
//                loadedMovie.vfFramesToTimeSwitch = TRUE;
//            }
//        }
//            break;
//            
        case 'l':
        {
            if (droppedFiles.size() > 1) {
                if (loadedMovie.isMovieLoaded) {
                    showDroppedList = !showDroppedList;
                    if (showDroppedList == FALSE) {
                        moveToMovie();
                    }
                    if (showDroppedList == TRUE) {
                        moveToList();
                    }
                }
            }
            
        }
            break;
            
        case 'p':
        {
            if (loadedMovie.isMovieLoaded || showDroppedList) {
                finishedPrinting = FALSE;
                showPrintScreen = TRUE;
            }
        }
            break;
            
        case 'h':
        case 'H':
        {
            Tweenzor::resetAllTweens();
            
            float _end = 0.f;
            float _begin = 0.f;
            if (!showMenu) {
                _end = 0.f;
                _begin = 1.f;
            } else {
                _end = 1.f;
                _begin = 0.f;
            }
            Tweenzor::add( &tweenzorX1, _begin, _end, 0.f, 0.5f , EASE_IN_OUT_EXPO);
//            Tweenzor::addCompleteListener( Tweenzor::getTween(&tweenzorX1), this, &testApp::onCompleteTweenzor);
            
            showMenu = !showMenu;

            if (showMenu){
                loadedMovie.setAllLimitsLeft(menuWidth);
                droppedList.setAllLimitsLeft(menuWidth);
            } else {
                loadedMovie.setAllLimitsLeft(leftMargin);
                droppedList.setAllLimitsLeft(leftMargin);
            }

        
        }
			break;
            
//        case 'x':
//        {
//            showFBO = !showFBO;
//            
//        }
			break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    currentKey = -1;
    
    if (timeSlider->getState()) {
        
        if (key == OF_KEY_RIGHT || key == OF_KEY_LEFT || key == OF_KEY_UP || key == OF_KEY_DOWN) {
            uiSliderValueLow = timeSlider->getScaledValueLow();
            uiSliderValueHigh = timeSlider->getScaledValueHigh();
            updateAllStills();
    ofLog(OF_LOG_VERBOSE, "asdfadfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfaf" );

        }
    }
    
    manipulateSlider = FALSE;
    loadedMovie.gmScrubMovie = FALSE;
    
    superKeyPressed = FALSE;
    shiftKeyPressed = FALSE;

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    if (y > ofGetHeight() - footerHeight/1.2) {
        if (!showTimeline) {
            ofLog(OF_LOG_VERBOSE, "Show Timeline------------------------------------------------");
            finishedTimeline = FALSE;
            showTimeline = TRUE;
            moveInOutTimeline();
        }

//        keyPressed(116);
//        timer.setStartTime();
    } else {
        if (!ofGetMousePressed()) {
            timer.setStartTime();
            finishedTimeline = TRUE;
//            ofLog(OF_LOG_VERBOSE, "Finished Showing Timeline--------------------------------------------");
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    ofLog(OF_LOG_VERBOSE, "which button is clicked:" + ofToString(button));

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

            
            if (!showDroppedList) {
                
                if (loadedMovie.grabbedStill[loadedMovie.gmRollOverMovieID].gsRollOver){
                    
                    rollOverMovieID = loadedMovie.gmRollOverMovieID;
                    rollOverMovieButtonID = loadedMovie.gmRollOverMovieButtonID;
                    rollOverClicked = TRUE;
                    ofLog(OF_LOG_VERBOSE, "clicked in rolloverstate ID =" + ofToString(loadedMovie.gmRollOverMovieID));
                    
//                    // set Mouse to middle of screen so one can scrub
//                    if (loadedMovie.gmRollOverMovieButtonID == 0) {
//                        CGPoint point;
//                        point.x = ofGetWidth() / 2;
//                        point.y = ofGetHeight() / 2;
////                        CGSetLocalEventsSuppressionInterval(0);
//                        CGWarpMouseCursorPosition(point);
//                    }
                    
                }
                updateInOut = FALSE;
                ofLog(OF_LOG_VERBOSE, "the mouse was clicked" );
                Tweener.removeTween(scrubFade);
                scrubFade = 255;
                
            }
        }
    }
    
    ofLog(OF_LOG_VERBOSE, "mousePressed - Button:" + ofToString(button) );

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if (loadedMovie.isMovieLoaded) {
        ofLog(OF_LOG_VERBOSE, "mouseReleased" );
        
        if (!showDroppedList) {
            if (loadedMovie.isThreadRunning()) {
                loadedMovie.stop(false);
            }
            if (updateInOut) {
                ofLog(OF_LOG_VERBOSE, "mouseReleased - updateInOut True" );
                Tweener.addTween(scrubFade, 0, 1);
                updateAllStills();
            }
            if (updateScrub) {
                Tweener.addTween(scrubFade, 0, 1);
                
                int i = loadedMovie.gmScrubID;
                loadedMovie.grabbedStill[i].gsManipulated = TRUE;
                loadedMovie.grabbedStill[i].gsToBeGrabbed = TRUE;
                loadedMovie.grabbedStill[i].gsToBeUpdated = TRUE;
                if (!loadedMovie.isThreadRunning()) {
                    loadedMovie.start();
                }
            }
            if (rollOverClicked) {
                rollOverButtonsClicked(rollOverMovieID, rollOverMovieButtonID);
            }
        }
    }
    
    manipulateSlider = FALSE;
    loadedMovie.gmScrubMovie = FALSE;
    scrollGrid = FALSE;
    scrollList = FALSE;
    loadedMovie.gmRollOver = FALSE;
    
}

//--------------------------------------------------------------
void testApp::mouseScrolled(double x, double y){
//    scrollValueY = fmin(ofGetWindowHeight()-30,fmax(0.0,scrollValueY + y));
//    ofLog(OF_LOG_VERBOSE, "scrollAmount x:y " + ofToString(x) + ":" + ofToString(y) );
//    scrollBar.scrollUpdate(x, y, gridHeight*2);
//    scrollAmountRel = scrollBar.getRelativePos();
//    ofLog(OF_LOG_VERBOSE, "scrollBarAmount:" + ofToString(scrollAmountRel) );
//    if (!updateScrub) {
//        if (showDroppedList) {
//            scrollList = TRUE;
//        } else {
//            scrollGrid = TRUE;
//        }
//    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    if (!currPrintingList) {
        if (windowResizedOnce != 0) {

            Tweener.removeTween(scrubFade);
            scrubFade = 255;
            devTurnOffMovieSwitch = TRUE;
            updateWindowResized = TRUE;
            calculateNewGrid(w, h);
            loadedMovie.setAllLimitsUpper(headerHeight);
            droppedList.setAllLimitsUpper(headerHeight);
            if (showMenu){
                loadedMovie.setAllLimitsLeft(menuWidth);
                droppedList.setAllLimitsLeft(menuWidth);
            } else {
                loadedMovie.setAllLimitsLeft(leftMargin);
                droppedList.setAllLimitsLeft(leftMargin);
            }
            loadedMovie.setAllLimitsLower(ofGetHeight());
            droppedList.setAllLimitsLower(ofGetHeight());
            loadedMovie.setAllLimitsRight(ofGetWidth() - rightMargin);
            droppedList.setAllLimitsRight(ofGetWidth() - rightMargin);
        }
//    if (showDroppedList) {
        scrollBarList.updateScrollBar(w, h, droppedList.getListHeight(), 0, 0);
        scrollBarList.setToTop();
        scrollListAmountRel = scrollBarList.getRelativePos();
//    }
    
    gui->setPosition(ofGetWidth()/2-gridAreaWidth/2-OFX_UI_GLOBAL_WIDGET_SPACING, h -(footerHeight/2 + timeSliderHeight/2) * tweenzorY1);
    gui->setWidth(w);
    gui2->setHeight(h);
    timeSlider->setWidth(gridAreaWidth);
    ofLog(OF_LOG_VERBOSE, "Timeslider Width:" + ofToString(timeSlider->getWidth()) );;

    windowResizedOnce++;
    }
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
    if (setupFinished) {
        
        if( dragInfo.files.size() > 0 ){
            
            loadNewMovie("", FALSE, TRUE, FALSE);

            droppedList.unregisterEvents(droppedFiles.size());
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
            scrollBarList.updateScrollBar(ofGetWidth(), ofGetHeight(), droppedList.getListHeight(), 0, 0);
            scrollBarList.setToTop();
            scrollListAmountRel = scrollBarList.getRelativePos();

            if(droppedFiles.size() > 1){
                moveToList();
            } else if(droppedFiles.size() > 0){
                ofLog(OF_LOG_VERBOSE, "Start LoadingMovie------------------------------------------------");
                updateMovieFromDrop = TRUE;
                
            }

        }
    }
}

//--------------------------------------------------------------
void testApp::exit(){
    
    loadedMovie.stop(false);
    
//    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
    
    gui2->saveSettings("guiSettings2.xml");
	delete gui2;

}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
	ofLog(OF_LOG_VERBOSE, "got event from: " + name );

	if(name == "LOADER")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        slider->update();
    }
    else if(name == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());
    }
    else if(name == "RSLIDER")
    {
        if (timeSlider->hitLow) {
            if ((timeSlider->getScaledValueHigh()-timeSlider->getScaledValueLow() < numberOfStills)) {
                timeSlider->setValueLow(timeSlider->getScaledValueHigh()-(numberOfStills-1));
            }
        }
        if (timeSlider->hitHigh) {
            if ((timeSlider->getScaledValueHigh()-timeSlider->getScaledValueLow() < numberOfStills)) {
                timeSlider->setValueHigh(timeSlider->getScaledValueLow()+(numberOfStills-1));
            }
        }
        if (timeSlider->hitCenter) {
            if (timeSlider->getScaledValueHigh() >= totalFrames-1) {
                if ((timeSlider->getScaledValueHigh()-timeSlider->getScaledValueLow() < numberOfStills)) {
                    timeSlider->setValueLow(timeSlider->getScaledValueHigh()-(numberOfStills-1));
                }
            }
            if (timeSlider->getScaledValueLow() == 0) {
                if ((timeSlider->getScaledValueHigh()-timeSlider->getScaledValueLow() < numberOfStills)) {
                    timeSlider->setValueHigh(timeSlider->getScaledValueLow()+(numberOfStills-1));
                }
            }
        }
        uiSliderValueLow = timeSlider->getScaledValueLow();
        uiSliderValueHigh = timeSlider->getScaledValueHigh();
        manipulateSlider = TRUE;
    }
	else if(name == "Columns")
	{
		ofxUISlider_jak *slider = (ofxUISlider_jak *) e.widget;
		ofLog(OF_LOG_VERBOSE, "Columns " + ofToString(slider->getScaledValue()));
		gridColumns = (int)slider->getScaledValue();
        windowResized(ofGetWidth(), ofGetHeight());
	}
	else if(name == "Rows")
	{
		ofxUISlider_jak *slider = (ofxUISlider_jak *) e.widget;
		ofLog(OF_LOG_VERBOSE, "Rows " + ofToString(slider->getScaledValue()));
		gridRows = (int)slider->getScaledValue();
        windowResized(ofGetWidth(), ofGetHeight());
	}
	else if(name == "Number")
	{
		ofxUISlider_jak *slider = (ofxUISlider_jak *) e.widget;
		ofLog(OF_LOG_VERBOSE, "Number " + ofToString(slider->getScaledValue()));
        gridColumns = 4;
		gridNumber = (int)slider->getScaledValue();
        windowResized(ofGetWidth(), ofGetHeight());
	}
	else if(name == "PrintScale")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		printScale = slider->getScaledValue();
        ofLog(OF_LOG_VERBOSE, "PrintScale: " + ofToString(printScale));
        if (isnan(printScale) || printScale > 20.0 || printScale < 1.0) {
            printScale = 1.0;
        }
        int tempOutputSizeWidth = (gridAreaWidth + gridMargin * 2) * printScale;
        int tempOutputSizeHeight = (gridAreaHeight + gridMargin * 2) * printScale;
        
        ofLog(OF_LOG_VERBOSE, "tempOutputSize: " + ofToString(tempOutputSizeWidth) + "x" + ofToString(tempOutputSizeHeight));


	}
    else if(name == "TEXT INPUT")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
        {
            ofLog(OF_LOG_VERBOSE, "ON ENTER: ");
            //            ofUnregisterKeyEvents((testApp*)this);
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
        {
            ofLog(OF_LOG_VERBOSE, "ON FOCUS: ");
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            ofLog(OF_LOG_VERBOSE, "ON BLUR: ");
            //            ofRegisterKeyEvents(this);
        }
        string output = textinput->getTextString();
        ofLog(OF_LOG_VERBOSE, output );
    }
    else if(name == "Set Columns and Rows")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            gridSetManually = TRUE;
            columnSlider->setVisible(TRUE);
            rowSlider->setVisible(TRUE);
            numberSlider->setVisible(FALSE);
            windowResized(ofGetWidth(), ofGetHeight());
        }
    }
    else if(name == "Set Number of Frames")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            gridSetManually = FALSE;
            rowSlider->setVisible(FALSE);
            columnSlider->setVisible(true);
            numberSlider->setVisible(true);
            windowResized(ofGetWidth(), ofGetHeight());
        }
	}
    else if(name == "TimeCode")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            loadedMovie.gmShowFramesUI = TRUE;
            loadedMovie.vfFramesToTimeSwitch = TRUE;
        }

	}
    else if(name == "Frames")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            loadedMovie.gmShowFramesUI = TRUE;
            loadedMovie.vfFramesToTimeSwitch = FALSE;
        }
	}
    else if(name == "off")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            loadedMovie.gmShowFramesUI = FALSE;
        }
	}
    else if(name == "jpg")
	{
        printFormat = OF_IMAGE_FORMAT_JPEG;
	}
    else if(name == "png")
	{
        printFormat = OF_IMAGE_FORMAT_PNG;
	}
    else if(name == "1024px wide")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            printSizeWidth = 1024;
        }
	}
    else if(name == "2048px wide")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            printSizeWidth = 2048;
        }
	}
    else if(name == "3072px wide")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            printSizeWidth = 3072;
        }
	}
    else if(name == "4096px wide")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            printSizeWidth = 4096;
        }
	}

}

//--------------------------------------------------------------
void testApp::scrollEvent(ofVec2f & e){
    if (!updateScrub) {
        if (showDroppedList) {
            scrollList = TRUE;
        } else {
            scrollGrid = TRUE;
        }
    }
}



//--------------------------------------------------------------
void testApp::drawUI(int _scaleFactor, bool _hideInPNG){
    
    float tempX = ofGetHeight() - footerHeight * tweenzorY1;
    ofSetColor(30, 30, 30);
    ofRect(0, tempX * _scaleFactor, ofGetWidth() * _scaleFactor, footerHeight * _scaleFactor);
    ofSetColor(255);
    
//    if (!_hideInPNG) {
//        // draw the percentage loaded bar
//        // the background
//        ofSetColor(100);
//        ofRect(ofGetWidth()/2-gridAreaWidth/2, tempX + loaderBarHeight, gridAreaWidth, loaderBarHeight);
//        // draw the percentage
//        ofSetColor(238, 71, 0);
//        ofRect(ofGetWidth()/2-gridAreaWidth/2, tempX + loaderBarHeight, gridAreaWidth*loadValue, loaderBarHeight);
//    }
}

//--------------------------------------------------------------
void testApp::drawMoviePrint(int _scaleFactor, bool _hideInPNG, bool _isBeingPrinted, float _scrollAmountRel){
    
    int _tempGridAreaHeight = gridAreaHeight;
    if (!_isBeingPrinted) {
        _tempGridAreaHeight = _tempGridAreaHeight + 40;
    }
    
    float _scrollAmount = 0;
    if (scrollBar.sbActive) {
        _scrollAmount = ((_tempGridAreaHeight-scrollBar.sbScrollAreaHeight)+(bottomMargin+topMargin)*2)*(1-_scrollAmountRel)+(ofGetHeight()/2 - _tempGridAreaHeight/2)-(bottomMargin+topMargin);
    }
//    ofLog(OF_LOG_VERBOSE, "scrollAmountRel: " + ofToString(scrollAmountRel));
    if (isnan(_scrollAmount)) {
        _scrollAmount = 0;
    }
    float tempX = (((gridWidth+gridMargin)*(0%gridColumns))+(ofGetWidth()/2 - gridAreaWidth/2) + menuWidth - menuWidth * tweenzorX1) * _scaleFactor;
    float tempY = ((ofGetHeight()/2 - _tempGridAreaHeight/2)+_scrollAmount) * _scaleFactor;
    loadedMovie.drawGmMoviePrint(tempX, tempY, gridColumns, gridMargin, _scrollAmount, _scaleFactor, 1, _isBeingPrinted, TRUE, superKeyPressed, shiftKeyPressed);
}

//--------------------------------------------------------------
void testApp::drawList(float _scrollAmountRel){
    
    float _scrollAmount = 0;
    if (scrollBarList.sbActive) {
//        _scrollAmount = ((droppedList.getListHeight()-scrollBarList.sbAreaHeight)+(bottomMargin+topMargin)*2)*(1-_scrollAmountRel)+(ofGetHeight()/2 - droppedList.getListHeight()/2)-(bottomMargin+topMargin);
        _scrollAmount = ((droppedList.getListHeight()-scrollBarList.sbScrollAreaHeight)+(bottomMargin+topMargin)*2)*-1*(_scrollAmountRel)+topMargin;
    }
    if (isnan(_scrollAmount)) {
        _scrollAmount = 0;
    }
//    ofLog(OF_LOG_VERBOSE, "droppedList.getListHeight(): " + ofToString(droppedList.getListHeight()));
//    ofLog(OF_LOG_VERBOSE, "scrollBarList.sbAreaHeight: " + ofToString(scrollBarList.sbAreaHeight));
//    ofLog(OF_LOG_VERBOSE, "_scrollAmountRel: " + ofToString(_scrollAmountRel));
//    ofLog(OF_LOG_VERBOSE, "_scrollAmount: " + ofToString(_scrollAmount));

    ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
    
    droppedList.draw(leftMargin + menuWidth - menuWidth * tweenzorX1, topMargin, ofGetWidth() - scrollBarWidth - rightMargin - leftMargin, _scrollAmount);

    
}

//--------------------------------------------------------------
void testApp::drawPrintScreen(){
    
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 127);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255, 255);
    if (currPrintingList) {
        printListImage.draw(ofGetWidth()/2-printImage.getWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-printImage.getHeight()/2);
    } else {
        printImage.draw(ofGetWidth()/2-printImage.getWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-printImage.getHeight()/2);

    }
    ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::drawStartScreen(){

    ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    startImage.draw(ofGetWidth()/2-startImage.getWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-startImage.getHeight()/2);
    ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::drawResizeScreen(){
    
    
    ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    for(int i=0; i<(numberOfStills+gridColumns); i++)
    {
        ofSetColor(30,(int)scrubFade);
        ofRectRounded(((gridWidth+gridMargin)*(i%gridColumns))+(ofGetWidth()/2 - gridAreaWidth/2), ((gridHeight+gridMargin)*(i/gridColumns))+(ofGetHeight()/2 - gridAreaHeight/2), gridWidth, gridHeight, gridWidth/64);
    }

    ofPopStyle();

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
    float tempX = ofGetWidth()/2 - tempLoaderBarWidth/2 + menuWidth - menuWidth * tweenzorX1;
    float tempY = ofGetHeight()/2 - tempLoaderBarHeight/2;
        // draw the percentage loaded bar
        // the background
        ofSetColor(100);
        ofRect(tempX, tempY + tempOffsetY, tempLoaderBarWidth, tempLoaderBarHeight);
        // draw the percentage
        ofSetColor(238, 71, 0);
        ofRect(tempX, tempY + tempOffsetY, tempLoaderBarWidth*loadValue, tempLoaderBarHeight);

    ofSetColor(255, 255, 255, 255);
    updatingImage.draw(ofGetWidth()/2-updatingImage.getWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-updatingImage.getHeight()/2);
    ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::drawLoadMovieScreen(){

    ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    loadMovieImage.draw(ofGetWidth()/2-loadMovieImage.getWidth()/2 + menuWidth - menuWidth * tweenzorX1, ofGetHeight()/2-loadMovieImage.getHeight()/2);
    ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::moveInOutTimeline(){
    
    Tweenzor::resetAllTweens();
    
    float _end = 0.f;
    float _begin = 0.f;
    if (showTimeline) {
        _end = 1.f;
        _begin = 0.f;
    } else {
        _end = 0.f;
        _begin = 1.f;
    }
    Tweenzor::add( &tweenzorY1, _begin, _end, 0.f, 0.5f , EASE_IN_OUT_EXPO);
    //            Tweenzor::addCompleteListener( Tweenzor::getTween(&tweenzorX1), this, &testApp::onCompleteTweenzor);
    
    if (showTimeline){
        loadedMovie.setAllLimitsLower(ofGetHeight() - footerHeight);
        droppedList.setAllLimitsLower(ofGetHeight() - footerHeight);
    } else {
        loadedMovie.setAllLimitsLower(ofGetHeight());
        droppedList.setAllLimitsLower(ofGetHeight());
    }
    
}



//--------------------------------------------------------------
void testApp::printImageToPNG(int _printSizeWidth){
    
    string newPrintPath = appPathUpStr + "/MoviePrints";
    ofDirectory dir(newPrintPath);
    if(!dir.exists()){
        dir.create(true);
    }
    //now you can be sure that path exists
    ofLog(OF_LOG_VERBOSE, dir.getOriginalDirectory() );
    
    if (loadedMovie.isMovieLoaded) {
        float _newScaleFactor = (float)_printSizeWidth / (float)(gridAreaWidth + gridMargin * 2);
        int outputWidth = (gridAreaWidth + gridMargin * 2) * _newScaleFactor;
        int outputHeight = (gridAreaHeight + gridMargin * 2) * _newScaleFactor;
//        if (outputWidth > 9999.0) {
//            _scaleFactor = 9950.0 / (gridAreaWidth + gridMargin * 2);
//            outputWidth = (gridAreaWidth + gridMargin * 2) * _scaleFactor;
//            outputHeight = (gridAreaHeight + gridMargin * 2) * _scaleFactor;
//        }

        printFormat = OF_IMAGE_FORMAT_PNG; // jpeg not yet working
        
        if (printFormat == OF_IMAGE_FORMAT_JPEG) {
            gmFboToSave.allocate(outputWidth, outputHeight, GL_RGB);
            gmPixToSave.allocate(outputWidth, outputHeight, OF_IMAGE_FORMAT_JPEG);
            //            gmPixToSave.allocate(outputWidth, outputHeight, OF_PIXELS_RGB);
        }
        else {
            gmFboToSave.allocate(outputWidth, outputHeight, GL_RGBA);
            //        gmPixToSave.allocate(outputWidth,outputHeight,OF_IMAGE_COLOR);
            //            gmPixToSave.allocate(outputWidth, outputHeight, OF_IMAGE_FORMAT_PNG);
            gmPixToSave.allocate(outputWidth, outputHeight, OF_PIXELS_RGB);
        }
        //        gmPixToSave.allocate(outputWidth,outputHeight,OF_IMAGE_COLOR);
        //        ofPixelFormat test;
        //        ofImageFormat tests;
        ofPushMatrix();
        ofPushStyle();
        gmFboToSave.begin();
        //        gmPixToSave.clear();
        //        ofDisableAlphaBlending();
        //        ofClear(0,0,0,0); //add this
        ofBackground(0, 0, 0, 0);
        ofSetColor(255, 255, 255, 255);
        //        drawUI(scaleFactor, TRUE);
        //        drawMoviePrint(scaleFactor, TRUE, TRUE, 0);
        //        ofLog(OF_LOG_VERBOSE, "gmFboToSave:getDepthBuffer" + ofToString(gmFboToSave.getDepthBuffer()));
        //        ofLog(OF_LOG_VERBOSE, "gmFboToSave:getDepthBuffer" + ofToString(gmFboToSave.);
        //        ofLog(OF_LOG_VERBOSE, "gmPixToSave:getNumChannels" + ofToString(gmPixToSave.getNumChannels()));
        loadedMovie.drawGmMoviePrint(gridMargin, gridMargin, gridColumns, gridMargin, 0, _newScaleFactor, 1, TRUE, TRUE, superKeyPressed, shiftKeyPressed);
        //        ofRect(100, 100, 300, 100);
        gmFboToSave.end();
        //        ofLog(OF_LOG_VERBOSE, "gmPixToSave:getImageType" + ofToString(gmPixToSave.getImageType()));
        gmFboToSave.readToPixels(gmPixToSave);
        
        // put some stuff in the pixels
        //        int i = 0;
        //        while( i < 300) {
        ////            while( i < gmPixToSave.size()) {
        //            char c = gmPixToSave[i];
        ////            ofLog(OF_LOG_VERBOSE, "gmPixToSave out:" + c );
        //            ofLog(OF_LOG_VERBOSE, "gmPixToSave out:" + ofToString(c));
        //            i++;
        //        }
        
        ofLog(OF_LOG_VERBOSE, "gmPixToSave:getImageType" + ofToString(gmPixToSave.getImageType()));
        ofPopStyle();
        ofPopMatrix();
        //        ofImage testImage;
        //        testImage.grabScreen(0, 0, 200, 200);
        //        ofLoadImage(gmPixToSave, testImage);
        string pathName = loadedMovie.gmMovie.getMoviePath();
        pathName = loadedFilePath.getFileName(pathName, TRUE);
        string formatExtension;
        if (printFormat == OF_IMAGE_FORMAT_JPEG) {
            formatExtension = "jpg";
        } else {
            formatExtension = "png";
        }
        string imageName = pathName + "_MoviePrint." + formatExtension;
        imageName = newPrintPath + "/" + imageName;
        //        gmPixToSave.setChannel(4, gmPixToSave);
        ofSaveImage(gmPixToSave, imageName, OF_IMAGE_QUALITY_MEDIUM);
        ofLog(OF_LOG_VERBOSE, "Finished saving" + ofToString(imageName) );
    }
    
    if (!currPrintingList) {
        timer.setStartTime();
        finishedPrinting = TRUE;
    }
    
    ofLog(OF_LOG_VERBOSE, "Finished Printing-------------------------------------------- currPrintingList" + ofToString(currPrintingList));
    
}

//--------------------------------------------------------------
void testApp::printListToPNG(){
    
    if (!droppedList.glDroppedItem[itemToPrint].itemProperties.ipTriedToPrint && itemToPrint == 0 && !movieIsBeingGrabbed && !currPrintingList) {
        currPrintingList = TRUE;
        droppedList.unregisterEvents(droppedFiles.size());
        droppedList.setActiveItem(itemToPrint);
        loadNewMovie(droppedList.glDroppedItem[itemToPrint].gliFile.path(), TRUE, TRUE, FALSE);
    }
    if (!droppedList.glDroppedItem[itemToPrint].itemProperties.ipTriedToPrint && !movieIsBeingGrabbed && currPrintingList) {
        ofLog(OF_LOG_VERBOSE, "printImageToPNG: " + ofToString(itemToPrint) );
        if (loadedMovie.isMovieLoaded){
            calculateNewGrid(ofGetWidth(), ofGetHeight());
            printImageToPNG(printSizeWidth);
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
        currPrintingList = FALSE;
        moveToList();
        ofLog(OF_LOG_VERBOSE, "Finished Printing List-------------------------------------------");
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
        if (_rollOverMovieID == 0) {
            setInPoint(j);
        } else if (_rollOverMovieID == (numberOfStills-1)) {
            setOutPoint(j);
        } else {
            loadedMovie.grabbedStill[_rollOverMovieID].gsFrameNumber = j;
            loadedMovie.grabbedStill[_rollOverMovieID].gsManipulated = TRUE;
            loadedMovie.grabbedStill[_rollOverMovieID].gsToBeGrabbed = TRUE;
            loadedMovie.grabbedStill[_rollOverMovieID].gsToBeUpdated = TRUE;
            if (!loadedMovie.isThreadRunning()) {
                loadedMovie.start();
            }
        }
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
        if (_rollOverMovieID == 0) {
            setInPoint(j);
        } else if (_rollOverMovieID == (numberOfStills-1)) {
            setOutPoint(j);
        } else {
            loadedMovie.grabbedStill[_rollOverMovieID].gsFrameNumber = j;
            loadedMovie.grabbedStill[_rollOverMovieID].gsManipulated = TRUE;
            loadedMovie.grabbedStill[_rollOverMovieID].gsToBeGrabbed = TRUE;
            loadedMovie.grabbedStill[_rollOverMovieID].gsToBeUpdated = TRUE;
            if (!loadedMovie.isThreadRunning()) {
                loadedMovie.start();
            }
        }
    }
    rollOverClicked = FALSE;
}

//--------------------------------------------------------------
void testApp::setInPoint(int _inPoint){
    int i = _inPoint;
    int j = timeSlider->getScaledValueHigh();
    if ((timeSlider->getScaledValueHigh()-i < numberOfStills)) {
        j = i + (numberOfStills - 1);
        if (j > (totalFrames-1)) {
            j = (totalFrames-1);
            i = j - (numberOfStills - 1);
        }
    }
    timeSlider->setValueLow(i);
    timeSlider->setValueHigh(j);
    uiSliderValueLow = i;
    uiSliderValueHigh = j;
    updateAllStills();
    ofLog(OF_LOG_VERBOSE, "manipulated InPoint" );
}

//--------------------------------------------------------------
void testApp::setOutPoint(int _outPoint){
    int i = timeSlider->getScaledValueLow();
    int j = _outPoint;
    if ((j - timeSlider->getScaledValueLow() < numberOfStills)) {
        i = j - (numberOfStills - 1);
        if (i < 0) {
            i = 0;
            j = (numberOfStills - 1);
            
        }
    }
    timeSlider->setValueLow(i);
    timeSlider->setValueHigh(j);
    uiSliderValueLow = i;
    uiSliderValueHigh = j;
    updateAllStills();
    ofLog(OF_LOG_VERBOSE, "manipulated OutPoint" );
}

//--------------------------------------------------------------
void testApp::updateTimeSlider(bool _wholeRange) {
    
    timeSlider->setMax(totalFrames-1);
    timeSlider->setValueLow(0);
    
    if (_wholeRange) {
        if (totalFrames > 250) {
            timeSlider->setValueLow(25);
            timeSlider->setValueHigh(totalFrames-26);
        } else {
            timeSlider->setValueHigh(totalFrames-1);
        }
    } else {
        timeSlider->setValueHigh(numberOfStills);
    }
    uiSliderValueLow = timeSlider->getScaledValueLow();
    uiSliderValueHigh = timeSlider->getScaledValueHigh();
}

//--------------------------------------------------------------
void testApp::updateAllStills(){
    
    ofLog(OF_LOG_VERBOSE, "Start Updating------------------------------------------------");
    finishedUpdating = FALSE;
    showUpdateScreen = TRUE;
    
    if (uiSliderValueLow < 0) {
        uiSliderValueLow = 0;
    }
    if (uiSliderValueHigh > (totalFrames-1)) {
        uiSliderValueHigh = (totalFrames-1);
    }

    
    for (int i=0; i<numberOfStills; i++) {
        gridTimeArray[i] = ofMap(float(i)/(numberOfStills - 1), 0.0, 1.0, uiSliderValueLow, uiSliderValueHigh, TRUE);
    }
    loadedMovie.updateAllFrameNumbers(gridTimeArray);
    movieIsBeingGrabbed = TRUE;
    loadedMovie.start();
    
    ofxNotify() << "Thread is started - " + ofToString(numberOfStills) + " Stills are being updated";

}



//--------------------------------------------------------------
void testApp::moveToMovie(){
    
    showDroppedList = FALSE;
    
    droppedList.unregisterEvents(droppedFiles.size());
    loadedMovie.enableMouseEvents();
    scrollBarList.unregisterMouseEvents();
    scrollBar.registerMouseEvents();
    gui->setVisible(TRUE);
    printListNotImage = FALSE;
    
    updateInOut = FALSE;
    manipulateSlider = FALSE;
}

//--------------------------------------------------------------
void testApp::moveToList(){
    
    showDroppedList = TRUE;
    scrollBar.unregisterMouseEvents();
    scrollBarList.registerMouseEvents();
    loadedMovie.stop(TRUE);
    loadedMovie.disableMouseEvents();
    gui->setVisible(FALSE);
    droppedList.registerEvents();
    printListNotImage = TRUE;
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
void testApp::onCompleteTweenzor(float* arg) {
	ofLog(OF_LOG_VERBOSE, "testApp :: onComplete : arg = " + ofToString(*arg));
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
