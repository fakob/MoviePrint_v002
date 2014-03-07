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
#define FAK_SHADOW ofColor(0, 0, 0, 32)
#define FAK_GRAY ofColor(59, 59, 59, 255)

//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    drawNotify = false; // ofxNotify
    showPlaceHolder = false; // added for developing
    
    setResourcePath();
    
    setupFinished = FALSE;
    updateNewPrintGrid = FALSE;
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
    windowWasResized = false;

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
    printGridColumns = 4;
    printGridRows = 5;
    printNumberOfThumbs = 20;
    menuWidth = 255;
    
    threadIsRunning = FALSE;
    
    printScale = 2.0;
    printSizeWidth = 1024;
    showPrintScreen = FALSE;
    finishedPrinting = TRUE;
    printFormat = OF_IMAGE_FORMAT_PNG;
    printSingleFrames = false;
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
    showMoviePrintPreview = true;
    
    thumbWidth = 256;
    thumbHeight = 144;
    printHeaderHeight = thumbHeight/4;
    scrollBarWidth = 12;
    scrollBarMargin = 2;
    scrollAmountRel = 0;
    scrollListAmountRel = 0;

    
    fboToPreviewWidth = 1320 - leftMargin - rightMargin - thumbWidth*2 - displayGridMargin*2;
    fboToPreviewHeight = 660 - headerHeight - footerHeight;
    
    fboToPreview.allocate(fboToPreviewWidth, fboToPreviewHeight, GL_RGBA );
    fboToPreview.begin();
	ofClear(255,255,255, 0);
    fboToPreview.end();
    
    counterToUpdate = 0;
    counterToLoad = 0;
    counterToPrint = 0;
    
    showMenu = FALSE;
    showTopMenu = false;
    tweenTimelineInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));
    tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));


    startImage.loadImage("MoviePrint_StartBildschirm_v001_00000.png");
    backgroundImage.loadImage("MoviePrint_Background_v001_00000.jpg");
    printImage.loadImage("MoviePrint_Print_v001_00000.png");
    loadMovieImage.loadImage("MoviePrint_Loading_v001_00000.png");
    updatingImage.loadImage("MoviePrint_Updating_v001_00000.png");
    inPointImage.loadImage("MoviePrint_InPoint_v001_00000.png");
    outPointImage.loadImage("MoviePrint_OutPoint_v001_00000.png");
    printListImage.loadImage("MoviePrint_PrintList_v001_00000.png");
    layoutHeaderImage.loadImage("MoviePrint_Layout_Header_v001_00000.png");
    
    // load standard movie
    loadedFile = "Nothing";
    saveMoviePrintPath = appPathUpStr + "/MoviePrints/";

    loadedMovie.gmUpperLimitY = headerHeight;
    loadedMovie.gmLowerLimitY = ofGetHeight() - footerHeight;
    loadedMovie.gmLeftLimitX = menuWidth;
    loadedMovie.gmRightLimitX = ofGetWidth() - rightMargin;
    calculateNewPrintGrid();
    loadedMovie.setup(loadedFile, numberOfStills, thumbWidth, thumbHeight, showPlaceHolder);
    totalFrames = 100;
    loadedMovie.allocateNewNumberOfStills(numberOfStills, thumbWidth, thumbHeight, showPlaceHolder);

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
    scrollBarList.setScrollHeight(0.5);
    scrollBarList.registerMouseEvents();
    scrollBarList.registerTouchEvents();
    
    setGUITimeline();
    setGUIMovieInfo();
    setGUISettings();
    setGUIMoviePrintPreview();
    guiSettings1->loadSettings("guiSettings1.xml");
    setGUISettingsMoviePrint();
    guiSettingsMoviePrint->loadSettings("guiMoviePrintSettings.xml");


    menuMovieInfo.setupMenu(1,0,0,0,0,headerHeight, true, true, false);
    menuMovieInfo.registerMouseEvents();
    
    menuSettings.setupMenu(4,0,0,0,0,headerHeight, false, true, false);
    menuSettings.registerMouseEvents();
    
    menuMoviePrintPreview.setupMenu(3,0,0,0,0,headerHeight, true, true, true);
    menuMoviePrintPreview.registerMouseEvents();
    
    menuMoviePrintSettings.setupMenu(5,0,0,0,0,headerHeight, true, true, false);
    menuMoviePrintSettings.registerMouseEvents();
    
    menuHelp.setupMenu(2,0,0,0,0,headerHeight, false, true, false);
    menuHelp.registerMouseEvents();
    
    menuTimeline.setupMenu(0,0,0,0,0,footerHeight/2, true, false, false);
    menuTimeline.registerMouseEvents();
    
    moveInOutTimeline();
    
    setupFinished = TRUE;
}

//--------------------------------------------------------------
void testApp::setGUITimeline(){
    
    // setup gui
    guiTimeline = new ofxUICanvas(ofGetWidth()/2-displayGridWidth/2-OFX_UI_GLOBAL_WIDGET_SPACING, ofGetHeight()-(footerHeight/2) - timeSliderHeight/2, ofGetWidth(),footerHeight); //ofxUICanvas(float x, float y, float width, float height)
    guiTimeline->setFont("HelveticaNeueLTCom-LtCn.ttf");
    drawPadding = FALSE;
    guiTimeline->setDrawBack(FALSE);
//    ofColor paddingColor = ofColor(FAK_ORANGE3);
//    ofColor backgroundColor = ofColor(FAK_ORANGE3);
//    guiTimeline->setColorBack(FAK_GRAY);
    
	guiTimeline->addWidgetDown(new ofxUIRangeSlider("RSLIDER", 0.0, (float)totalFrames, 0.0, 100.0, displayGridWidth, timeSliderHeight));
    uiRangeSliderTimeline = (ofxUIRangeSlider *) guiTimeline->getWidget("RSLIDER");
    uiRangeSliderTimeline->setLabelPrecision(0);
    uiRangeSliderTimeline->setLabelVisible(FALSE);
//    uiRangeSliderTimeline->setDrawPadding(FALSE);
//    uiRangeSliderTimeline->setDrawPaddingOutline(FALSE);
//    uiRangeSliderTimeline->setDrawOutlineHighLight(FALSE);
//    uiRangeSliderTimeline->setDrawOutline(FALSE);
//    guiTimeline->setDrawPadding(FALSE);
//    guiTimeline->setDrawPaddingOutline(FALSE);
//    guiTimeline->setDrawOutlineHighLight(FALSE);
//    guiTimeline->setDrawOutline(FALSE);
    
//    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(0,25));
    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_OUTLINE, ofColor(0,0,0,0));
    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_OUTLINE_HIGHLIGHT, ofColor(0,0,0,0));
    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL, FAK_ORANGE1);
    guiTimeline->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL_HIGHLIGHT, FAK_ORANGE1);
    


//    ofColor tempColor (30, 30, 30, 255);
    uiRangeSliderTimeline->setColorBack(FAK_ORANGE3);

    uiRangeSliderTimeline->setColorPadded(FAK_ORANGE3);

    ofAddListener(guiTimeline->newGUIEvent, this, &testApp::guiEvent);
    //    guiTimeline->loadSettings("GUI/guiSettings.xml");
//    guiTimeline->disable();

}

//--------------------------------------------------------------
void testApp::setGUISettings(){
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = menuWidth-xInit;
    
//    vector<string> names2;
//	names2.push_back("Set Columns and Rows");
//	names2.push_back("Set Number of Frames");
    
    vector<string> names;
	names.push_back("Frames");
	names.push_back("TimeCode");
	names.push_back("off");
    
    guiSettings1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    guiSettings1->setFont("HelveticaNeueLTCom-LtCn.ttf");

    guiSettings1->addLabel("SETTINGS", OFX_UI_FONT_LARGE);
    guiSettings1->addSpacer(length-xInit, 1);
    guiSettings1->addLabel("SET RASTER", OFX_UI_FONT_MEDIUM);

//    guiSettings1->addRadio("Grid-Number", names2, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
//    uiRadioSetFitManually = (ofxUIRadio *) guiSettings1->getWidget("Grid-Number");
//    uiRadioSetFitManually->activateToggle("Set Columns and Rows");
//    
//	guiSettings1->addIntSlider("Columns", 4, 10, gridColumns, length-xInit,dim);
//	guiSettings1->addIntSlider("Rows", 1, 20, gridRows, length-xInit,dim);
//   	guiSettings1->addIntSlider("Number", 4, 200, gridNumber, length-xInit,dim);
//   	guiSettings1->addIntSlider("ThumbWidth", 0, 18, 1, length-xInit,dim);
//    uiSliderColumns = (ofxUIIntSlider *) guiSettings1->getWidget("Columns");
//    uiSliderRows = (ofxUIIntSlider *) guiSettings1->getWidget("Rows");
//    uiSliderNumberOfThumbs = (ofxUIIntSlider *) guiSettings1->getWidget("Number");
//    uiSliderThumbWidth = (ofxUIIntSlider *) guiSettings1->getWidget("ThumbWidth");
//    uiSliderThumbWidth->setVisible(FALSE);

    guiSettings1->addSpacer(length-xInit, 1);
    guiSettings1->addLabel("SHOW INFO", OFX_UI_FONT_MEDIUM);
    ofxUIRadio *uiRadioSetFrameDisplay = guiSettings1->addRadio("RADIO_HORIZONTAL", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
//    uiRadioSetFrameDisplay = (ofxUIRadio *) guiSettings1->getWidget("RADIO HORIZONTAL");
    uiRadioSetFrameDisplay->activateToggle("TimeCode");
    
    guiSettings1->setColorBack(FAK_TRANSPARENT);
    
    guiSettings1->setVisible(FALSE);
//	ofAddListener(guiSettings1->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::setGUISettingsMoviePrint(){
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING*3;
    float length = menuWidth-xInit;
    
    guiSettingsMoviePrint = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    guiSettingsMoviePrint->setFont("HelveticaNeueLTCom-LtCn.ttf");
    guiSettingsMoviePrint->setWidgetSpacing(10);
    
    guiSettingsMoviePrint->addButton("Select Output Folder", false,dim*1.5,dim);
    guiSettingsMoviePrint->addLabel("SelectedOutputFolder", saveMoviePrintPath, OFX_UI_FONT_SMALL);
    uiLabelOutputFolder = (ofxUILabel *) guiSettingsMoviePrint->getWidget("SelectedOutputFolder");
    uiLabelOutputFolder->setLabel(cropFrontOfString(saveMoviePrintPath, 40, "..."));

    guiSettingsMoviePrint->addLabel("SET RASTER", OFX_UI_FONT_MEDIUM);
//    vector<string> names2;
//	names2.push_back("Set Columns and Rows");
//	names2.push_back("Set Number of Frames");
//    guiSettingsMoviePrint->addRadio("Grid-Number", names2, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
//    uiRadioSetFitManually = (ofxUIRadio *) guiSettingsMoviePrint->getWidget("Grid-Number");
//    uiRadioSetFitManually->activateToggle("Set Columns and Rows");
	guiSettingsMoviePrint->addIntSlider("PrintColumns", 1, 10, &printGridColumns, length-xInit,dim);
	guiSettingsMoviePrint->addIntSlider("PrintRows", 1, 20, &printGridRows, length-xInit,dim);
//   	guiSettingsMoviePrint->addIntSlider("PrintNumber", 4, 200, &printNumberOfThumbs, length-xInit,dim);
   	guiSettingsMoviePrint->addIntSlider("PrintMargin", 0, 30, &printGridMargin, length-xInit,dim);
    uiSliderPrintColumns = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintColumns");
    uiSliderPrintRows = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintRows");
    uiSliderNumberOfThumbs = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintNumber");
    uiSliderPrintMargin = (ofxUIIntSlider *) guiSettingsMoviePrint->getWidget("PrintMargin");

//    guiSettingsMoviePrint->addToggle("ShowMoviePrintPreview", &showMoviePrintPreview);
    guiSettingsMoviePrint->addToggle("DisplayVideoAudioInfo", &printDisplayVideoAudioInfo, dim*1.5, dim);
    guiSettingsMoviePrint->addToggle("PrintSaveSingleFrames", printSingleFrames, dim*1.5, dim);
    
    vector<string> names;
	names.push_back("Frames");
	names.push_back("TimeCode");
	names.push_back("off");
    guiSettingsMoviePrint->addSpacer(length-xInit, 1);
    guiSettingsMoviePrint->addLabel("SHOW INFO", OFX_UI_FONT_MEDIUM);
    ofxUIRadio *uiRadioSetFrameDisplay = guiSettingsMoviePrint->addRadio("RADIO_HORIZONTAL", names, OFX_UI_ORIENTATION_VERTICAL, dim*1.5, dim);
    uiRadioSetFrameDisplay->activateToggle("TimeCode");
    
    guiSettingsMoviePrint->addSpacer(length-xInit, 1);

    guiSettingsMoviePrint->addLabel("Choose Output Format", OFX_UI_FONT_MEDIUM);
    vector<string> names3;
    names3.push_back("png with alpha");
    names3.push_back("jpg");
//    names3.push_back("gif");
    guiSettingsMoviePrint->addRadio("Choose Output Format", names3, OFX_UI_ORIENTATION_VERTICAL, dim*1.5, dim);
    uiRadioPrintOutputFormat =(ofxUIRadio *) guiSettingsMoviePrint->getWidget("Choose Output Format");
    
    guiSettingsMoviePrint->addSpacer(length-xInit, 1);

    guiSettingsMoviePrint->addLabel("MoviePrint Width", OFX_UI_FONT_MEDIUM);
    vector<string> names4;
    names4.push_back("1024px wide");
    names4.push_back("2048px wide");
    names4.push_back("3072px wide");
    names4.push_back("4096px wide");
    guiSettingsMoviePrint->addRadio("MoviePrint Width", names4, OFX_UI_ORIENTATION_VERTICAL, dim*1.5, dim);
    uiRadioPrintOutputFormat =(ofxUIRadio *) guiSettingsMoviePrint->getWidget("MoviePrint Width");
    
    guiSettingsMoviePrint->setColorBack(FAK_TRANSPARENT);
	ofAddListener(guiSettingsMoviePrint->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::setGUIMoviePrintPreview(){
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = menuWidth-xInit + (thumbWidth + displayGridMargin)*1;
    
    guiMoviePrintPreview = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    guiMoviePrintPreview->setFont("HelveticaNeueLTCom-LtCn.ttf");
    
//    guiMoviePrintPreview->addButton("WriteMoviePrint", writeMoviePrint);
//    guiMoviePrintPreview->addBaseDraws("IMAGE CAPTION", &fboToPreview, false);
    
    guiMoviePrintPreview->setColorBack(FAK_ORANGE3);
    guiMoviePrintPreview->setColorBack(FAK_TRANSPARENT);
	ofAddListener(guiMoviePrintPreview->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::setGUIMovieInfo(){
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = menuWidth-xInit;
    
    vector<string> names2;
	names2.push_back("Set Columns and Rows");
	names2.push_back("Set Number of Frames");
    
    vector<string> names;
	names.push_back("Frames");
	names.push_back("TimeCode");
	names.push_back("off");
    
    guiMovieInfo = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    guiMovieInfo->setFont("HelveticaNeueLTCom-LtCn.ttf");
    
    guiMovieInfo->addLabel("MOVIE INFO", OFX_UI_FONT_LARGE);

    guiMovieInfo->addWidgetDown(new ofxUILabel("HELP", OFX_UI_FONT_LARGE));
    guiMovieInfo->addSpacer(length-xInit, 1);
    guiMovieInfo->addWidgetDown(new ofxUILabel("Drop in one or more movies", OFX_UI_FONT_SMALL));
    //    guiMovieInfo->addSpacer(10, 1);
    string textString = "This widget is a text area widget. Use this when you need to display a paragraph of text. It takes care of formatting the text to fit the block.";
    guiMovieInfo->addSpacer();
    
    guiMovieInfo->addTextArea("gmMIFileName", "gmMIFileName", OFX_UI_FONT_SMALL);
    gmMIFileName = (ofxUITextArea *) guiMovieInfo->getWidget("gmMIFileName");

    guiMovieInfo->addTextArea("gmMIFilePath", "gmMIFilePath", OFX_UI_FONT_SMALL);
    gmMIFilePath = (ofxUITextArea *) guiMovieInfo->getWidget("gmMIFilePath");
    
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIFormat", "gmMIFormat", OFX_UI_FONT_SMALL));
    gmMIFormat = (ofxUILabel *) guiMovieInfo->getWidget("gmMIFormat");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIFormatString", "gmMIFormatString", OFX_UI_FONT_SMALL));
    gmMIFormatString = (ofxUILabel *) guiMovieInfo->getWidget("gmMIFormatString");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIFileSizeString", "gmMIFileSizeString", OFX_UI_FONT_SMALL));
    gmMIFileSizeString = (ofxUILabel *) guiMovieInfo->getWidget("gmMIFileSizeString");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIDurationString1", "gmMIDurationString1", OFX_UI_FONT_SMALL));
    gmMIDurationString1 = (ofxUILabel *) guiMovieInfo->getWidget("gmMIDurationString1");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIFrameCount", "gmMIFrameCount", OFX_UI_FONT_SMALL));
    gmMIFrameCount = (ofxUILabel *) guiMovieInfo->getWidget("gmMIFrameCount");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIWidth", "gmMIWidth", OFX_UI_FONT_SMALL));
    gmMIWidth = (ofxUILabel *) guiMovieInfo->getWidget("gmMIWidth");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIDisplayAspectRatioString", "gmMIDisplayAspectRatioString", OFX_UI_FONT_SMALL));
    gmMIDisplayAspectRatioString = (ofxUILabel *) guiMovieInfo->getWidget("gmMIDisplayAspectRatioString");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIFrameRateString", "gmMIFrameRateString", OFX_UI_FONT_SMALL));
    gmMIFrameRateString = (ofxUILabel *) guiMovieInfo->getWidget("gmMIFrameRateString");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIVFormat", "gmMIVFormat", OFX_UI_FONT_SMALL));
    gmMIVFormat = (ofxUILabel *) guiMovieInfo->getWidget("gmMIVFormat");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIFormatInfo", "gmMIFormatInfo", OFX_UI_FONT_SMALL));
    gmMIFormatInfo = (ofxUILabel *) guiMovieInfo->getWidget("gmMIFormatInfo");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIBitRate", "gmMIBitRate", OFX_UI_FONT_SMALL));
    gmMIBitRate = (ofxUILabel *) guiMovieInfo->getWidget("gmMIBitRate");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIPixelAspectRatio", "gmMIPixelAspectRatio", OFX_UI_FONT_SMALL));
    gmMIPixelAspectRatio = (ofxUILabel *) guiMovieInfo->getWidget("gmMIPixelAspectRatio");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIDisplayAspectRatio", "gmMIDisplayAspectRatio", OFX_UI_FONT_SMALL));
    gmMIDisplayAspectRatio = (ofxUILabel *) guiMovieInfo->getWidget("gmMIDisplayAspectRatio");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIFrameRate_ModeString", "gmMIFrameRate_ModeString", OFX_UI_FONT_SMALL));
    gmMIFrameRate_ModeString = (ofxUILabel *) guiMovieInfo->getWidget("gmMIFrameRate_ModeString");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIColorSpace", "gmMIColorSpace", OFX_UI_FONT_SMALL));
    gmMIColorSpace = (ofxUILabel *) guiMovieInfo->getWidget("gmMIColorSpace");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIChromaSubsampling", "gmMIChromaSubsampling", OFX_UI_FONT_SMALL));
    gmMIChromaSubsampling = (ofxUILabel *) guiMovieInfo->getWidget("gmMIChromaSubsampling");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIBitDepthString", "gmMIBitDepthString", OFX_UI_FONT_SMALL));
    gmMIBitDepthString = (ofxUILabel *) guiMovieInfo->getWidget("gmMIBitDepthString");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIInterlacementString", "gmMIInterlacementString", OFX_UI_FONT_SMALL));
    gmMIInterlacementString = (ofxUILabel *) guiMovieInfo->getWidget("gmMIInterlacementString");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIAFormat", "gmMIAFormat", OFX_UI_FONT_SMALL));
    gmMIAFormat = (ofxUILabel *) guiMovieInfo->getWidget("gmMIAFormat");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIAChannelsString", "gmMIAChannelsString", OFX_UI_FONT_SMALL));
    gmMIAChannelsString = (ofxUILabel *) guiMovieInfo->getWidget("gmMIAChannelsString");
    guiMovieInfo->addWidgetDown(new ofxUILabel("gmMIASamplingRate", "gmMIASamplingRate", OFX_UI_FONT_SMALL));
    gmMIASamplingRate = (ofxUILabel *) guiMovieInfo->getWidget("gmMIASamplingRate");
    
    
    ofLog(OF_LOG_VERBOSE, "getColorBack" + ofToString(guiMovieInfo->getColorBack()));
    ofLog(OF_LOG_VERBOSE, "getDrawBack" + ofToString(guiMovieInfo->getDrawBack()));
    ofLog(OF_LOG_VERBOSE, "getColorFill" + ofToString(guiMovieInfo->getColorFill()));
    ofLog(OF_LOG_VERBOSE, "getDrawFill" + ofToString(guiMovieInfo->getDrawFill()));
    ofLog(OF_LOG_VERBOSE, "getColorFillHighlight" + ofToString(guiMovieInfo->getColorFillHighlight()));
    ofLog(OF_LOG_VERBOSE, "getDrawFillHighLight" + ofToString(guiMovieInfo->getDrawFillHighLight()));
    ofLog(OF_LOG_VERBOSE, "getColorOutline" + ofToString(guiMovieInfo->getColorOutline()));
    ofLog(OF_LOG_VERBOSE, "getDrawOutline" + ofToString(guiMovieInfo->getDrawOutline()));
    ofLog(OF_LOG_VERBOSE, "getColorOutlineHighlight" + ofToString(guiMovieInfo->getColorOutlineHighlight()));
    ofLog(OF_LOG_VERBOSE, "getDrawOutlineHighLight" + ofToString(guiMovieInfo->getDrawOutlineHighLight()));
    ofLog(OF_LOG_VERBOSE, "getColorPadded" + ofToString(guiMovieInfo->getColorPadded()));
    ofLog(OF_LOG_VERBOSE, "getDrawPadding" + ofToString(guiMovieInfo->getDrawPadding()));
    ofLog(OF_LOG_VERBOSE, "getColorPaddedOutline" + ofToString(guiMovieInfo->getColorPaddedOutline()));
    ofLog(OF_LOG_VERBOSE, "getDrawPaddingOutline" + ofToString(guiMovieInfo->getDrawPaddingOutline()));
    
    ofxUIColor cb = ofxUIColor(FAK_LIGHTERMIDDLEDARKORANGECOLOR); // OFX_UI_COLOR_BACK
    ofxUIColor co = ofxUIColor( 255, 255, 255, 200); // OFX_UI_COLOR_OUTLINE
    ofxUIColor coh = ofxUIColor( 255, 255, 255, 255 ); // OFX_UI_COLOR_OUTLINE_HIGHLIGHT
    ofxUIColor cf = ofxUIColor( 255, 255, 255, 200 ); // OFX_UI_COLOR_FILL
    ofxUIColor cfh = ofxUIColor( 255, 255, 255, 200 ); // OFX_UI_COLOR_FILL_HIGHLIGHT
    ofxUIColor cp = ofxUIColor( 255, 255, 255, 100 ); // OFX_UI_COLOR_PADDED
    ofxUIColor cpo = ofxUIColor( 255, 255, 255, 200 ); // OFX_UI_COLOR_PADDED_OUTLINE
    
    guiMovieInfo->setColorBack(FAK_TRANSPARENT);

}

//--------------------------------------------------------------
void testApp::setGUIHelp1(){
	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = menuWidth-xInit;

    guiHelp1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    guiHelp1->setFont("HelveticaNeueLTCom-LtCn.ttf");
    guiMovieInfo->setColorBack(FAK_ORANGECOLOR);
    
    guiHelp1->addLabel("MOVIE INFO", OFX_UI_FONT_LARGE);
    
 	guiHelp1->addWidgetDown(new ofxUILabel("HELP", OFX_UI_FONT_LARGE));
    guiHelp1->addSpacer(length-xInit, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Drop in one or more movies", OFX_UI_FONT_SMALL));
    //    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Set the desired In- and Outpoints", OFX_UI_FONT_SMALL));
    //    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Press P to make a MoviePrint", OFX_UI_FONT_SMALL));
    //    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Press Esc to quit MoviePrint", OFX_UI_FONT_SMALL));
    guiHelp1->addSpacer(length-xInit, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Prints are saved as PNGs in a folder", OFX_UI_FONT_SMALL));
    guiHelp1->addWidgetDown(new ofxUILabel("named MoviePrints on the same level", OFX_UI_FONT_SMALL));
    guiHelp1->addSpacer(10, 1);
        guiHelp1->addWidgetDown(new ofxUILabel("Hovering over a Still one can", OFX_UI_FONT_SMALL));
    guiHelp1->addWidgetDown(new ofxUILabel("-jump 1/10/100 (Shift/Cmd) frames", OFX_UI_FONT_SMALL));
    guiHelp1->addWidgetDown(new ofxUILabel(" forward or backward", OFX_UI_FONT_SMALL));
    guiHelp1->addWidgetDown(new ofxUILabel("-set this Still as In- or Outpoint", OFX_UI_FONT_SMALL));
    guiHelp1->addWidgetDown(new ofxUILabel("-scrub the Still using the mouse", OFX_UI_FONT_SMALL));
    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Set In- and Outpoints using the", OFX_UI_FONT_SMALL));
    guiHelp1->addWidgetDown(new ofxUILabel("timeline on the bottom", OFX_UI_FONT_SMALL));
    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("With Multiple movies", OFX_UI_FONT_SMALL));
    //    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Press P to make MoviePrints of all", OFX_UI_FONT_SMALL));
    //    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Press L to switch to ListView", OFX_UI_FONT_SMALL));
    guiHelp1->addSpacer(length-xInit, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Press H to view Settings and Help", OFX_UI_FONT_SMALL));
    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Choose between >Set rows manually<", OFX_UI_FONT_SMALL));
    guiHelp1->addWidgetDown(new ofxUILabel("and >Fit to Screen<", OFX_UI_FONT_SMALL));
    //    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Set >Columns< and >Rows<", OFX_UI_FONT_SMALL));
    //    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Show >Frames< or >TimeCode<", OFX_UI_FONT_SMALL));
    //    guiHelp1->addSpacer(10, 1);
    guiHelp1->addWidgetDown(new ofxUILabel("Choose >MoviePrint width<", OFX_UI_FONT_SMALL));

    
    
    
    ofLog(OF_LOG_VERBOSE, "getColorBack" + ofToString(guiMovieInfo->getColorBack()));
    ofLog(OF_LOG_VERBOSE, "getDrawBack" + ofToString(guiMovieInfo->getDrawBack()));
    ofLog(OF_LOG_VERBOSE, "getColorFill" + ofToString(guiMovieInfo->getColorFill()));
    ofLog(OF_LOG_VERBOSE, "getDrawFill" + ofToString(guiMovieInfo->getDrawFill()));
    ofLog(OF_LOG_VERBOSE, "getColorFillHighlight" + ofToString(guiMovieInfo->getColorFillHighlight()));
    ofLog(OF_LOG_VERBOSE, "getDrawFillHighLight" + ofToString(guiMovieInfo->getDrawFillHighLight()));
    ofLog(OF_LOG_VERBOSE, "getColorOutline" + ofToString(guiMovieInfo->getColorOutline()));
    ofLog(OF_LOG_VERBOSE, "getDrawOutline" + ofToString(guiMovieInfo->getDrawOutline()));
    ofLog(OF_LOG_VERBOSE, "getColorOutlineHighlight" + ofToString(guiMovieInfo->getColorOutlineHighlight()));
    ofLog(OF_LOG_VERBOSE, "getDrawOutlineHighLight" + ofToString(guiMovieInfo->getDrawOutlineHighLight()));
    ofLog(OF_LOG_VERBOSE, "getColorPadded" + ofToString(guiMovieInfo->getColorPadded()));
    ofLog(OF_LOG_VERBOSE, "getDrawPadding" + ofToString(guiMovieInfo->getDrawPadding()));
    ofLog(OF_LOG_VERBOSE, "getColorPaddedOutline" + ofToString(guiMovieInfo->getColorPaddedOutline()));
    ofLog(OF_LOG_VERBOSE, "getDrawPaddingOutline" + ofToString(guiMovieInfo->getDrawPaddingOutline()));
    
    ofxUIColor cb = ofxUIColor(FAK_LIGHTERMIDDLEDARKORANGECOLOR); // OFX_UI_COLOR_BACK
    ofxUIColor co = ofxUIColor( 255, 255, 255, 200); // OFX_UI_COLOR_OUTLINE
    ofxUIColor coh = ofxUIColor( 255, 255, 255, 255 ); // OFX_UI_COLOR_OUTLINE_HIGHLIGHT
    ofxUIColor cf = ofxUIColor( 255, 255, 255, 200 ); // OFX_UI_COLOR_FILL
    ofxUIColor cfh = ofxUIColor( 255, 255, 255, 200 ); // OFX_UI_COLOR_FILL_HIGHLIGHT
    ofxUIColor cp = ofxUIColor( 255, 255, 255, 100 ); // OFX_UI_COLOR_PADDED
    ofxUIColor cpo = ofxUIColor( 255, 255, 255, 200 ); // OFX_UI_COLOR_PADDED_OUTLINE
    
    
}

//--------------------------------------------------------------
void testApp::calculateNewPrintSize(){
    printGridWidth = (thumbWidth + printGridMargin) * printGridColumns + printGridMargin;
    if (printDisplayVideoAudioInfo) {
        printGridHeight = (thumbHeight + printGridMargin) * printGridRows + printGridMargin + printHeaderHeight;
    } else {
        printGridHeight = (thumbHeight + printGridMargin) * printGridRows + printGridMargin;
    }
    ofLog(OF_LOG_VERBOSE, "printGridSize" + ofToString(printGridWidth) + "x" + ofToString(printGridHeight));
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
        numberOfStills = printGridColumns*printGridRows;
    } else {
        numberOfStills = printNumberOfThumbs;
        printGridRows = ceil(numberOfStills/(float)printGridColumns);
        
    }

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
    
    loadedMovie.gmThumbWidth = thumbWidth;
    loadedMovie.gmThumbHeight = thumbHeight;
    
    calculateNewPrintSize();
    
    updateNewPrintGrid = true;
    
    updateDisplayGrid();
    
    ofxNotify() << "New Grid is Calculated - Total Number of Stills: " + ofToString(numberOfStills);

}

//--------------------------------------------------------------
void testApp::updateDisplayGrid(){
    
    gridColumns = fmax(1, (int)(ofGetWindowWidth() - leftMargin - scrollBarWidth + displayGridMargin) / (thumbWidth + displayGridMargin));
    
    displayGridWidth = (gridColumns * (thumbWidth + displayGridMargin) - displayGridMargin);
    gridRows = ceil(numberOfStills/(float)gridColumns);
    displayGridHeight = (gridRows * (thumbHeight + displayGridMargin)) - displayGridMargin;
    ofLog(OF_LOG_VERBOSE, "displayGridMargin: " + ofToString(displayGridMargin));
    ofLog(OF_LOG_VERBOSE, "displayGridHeight: " + ofToString(thumbHeight));
    ofLog(OF_LOG_VERBOSE, "gridAreaHeight: " + ofToString(displayGridHeight));
    
    updateAllLimits();
    updateTheScrollBar();
    updateTheListScrollBar();
    updateTimeline();
    
    ofxNotify() << "updateDisplayGrid - Total Number of Stills: " + ofToString(numberOfStills);
    
    ofLog(OF_LOG_VERBOSE, "displayGridWidth: " + ofToString(displayGridWidth));
    ofLog(OF_LOG_VERBOSE, "displayGridHeight: " + ofToString(displayGridHeight));
    
}

//--------------------------------------------------------------
void testApp::loadNewMovie(string _newMoviePath, bool _wholeRange, bool _loadInBackground, bool _loadScrubMovie){
    
    movieLoading = TRUE;
    if (!_loadInBackground) {
        moveToMovie();
    }
    
    loadedMovie.stop(TRUE);

    ofxNotify() << "Movie has started to load";
    loadedMovie.loadNewMovieToBeGrabbed(_newMoviePath, numberOfStills, showPlaceHolder);
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
    updateTheScrollBar();
    
    ofLog(OF_LOG_VERBOSE, "scrollAmountRel: " + ofToString(scrollAmountRel));
    
    timer.setStartTime();
    finishedLoadingMovie = TRUE;
    
    ofLog(OF_LOG_VERBOSE, "Finished Loading Movie--------------------------------------------");
    
    if (!loadedMovie.isMovieLoaded) {
        guiTimeline->setVisible(FALSE);
        scrollBar.unregisterMouseEvents();
        loadedMovie.disableMouseEvents();
    }
    int tempMoveX = 0;
    gmMIFileName->setTextString(loadedMovie.gmMIFileName);
    tempMoveX = gmMIFileName->getTextHeightMinusOne();
    gmMIFilePath->setTextString("FilePath: " + loadedMovie.gmMIFilePathOhne);
    gmMIFilePath->setYOffset(tempMoveX);
    tempMoveX += gmMIFilePath->getTextHeightMinusOne();
    gmMIFormat->setLabel(ofToString(loadedMovie.gmMIFormat));
    gmMIFormat->setYOffset(tempMoveX);
    gmMIFormatString->setLabel(ofToString(loadedMovie.gmMIFormatString));
    gmMIFormatString->setYOffset(tempMoveX);
    gmMIFileSizeString->setLabel(ofToString(loadedMovie.gmMIFileSizeString));
    gmMIFileSizeString->setYOffset(tempMoveX);
    gmMIDurationString1->setLabel(ofToString(loadedMovie.gmMIDurationString1));
    gmMIDurationString1->setYOffset(tempMoveX);
    gmMIFrameCount->setLabel(ofToString(loadedMovie.gmMIFrameCount));
    gmMIFrameCount->setYOffset(tempMoveX);
    gmMIWidth->setLabel(ofToString(loadedMovie.gmMIWidth));
    gmMIWidth->setYOffset(tempMoveX);
    gmMIDisplayAspectRatioString->setLabel(ofToString(loadedMovie.gmMIDisplayAspectRatioString));
    gmMIDisplayAspectRatioString->setYOffset(tempMoveX);
    gmMIFrameRateString->setLabel(ofToString(loadedMovie.gmMIFrameRateString));
    gmMIFrameRateString->setYOffset(tempMoveX);
    gmMIVFormat->setLabel(ofToString(loadedMovie.gmMIVFormat));
    gmMIVFormat->setYOffset(tempMoveX);
    gmMIFormatInfo->setLabel(ofToString(loadedMovie.gmMIFormatInfo));
    gmMIFormatInfo->setYOffset(tempMoveX);
    gmMIBitRate->setLabel(ofToString(loadedMovie.gmMIBitRate));
    gmMIBitRate->setYOffset(tempMoveX);
    gmMIPixelAspectRatio->setLabel(ofToString(loadedMovie.gmMIPixelAspectRatio));
    gmMIPixelAspectRatio->setYOffset(tempMoveX);
    gmMIDisplayAspectRatio->setLabel(ofToString(loadedMovie.gmMIDisplayAspectRatio));
    gmMIDisplayAspectRatio->setYOffset(tempMoveX);
    gmMIFrameRate_ModeString->setLabel(ofToString(loadedMovie.gmMIFrameRate_ModeString));
    gmMIFrameRate_ModeString->setYOffset(tempMoveX);
    gmMIColorSpace->setLabel(ofToString(loadedMovie.gmMIColorSpace));
    gmMIColorSpace->setYOffset(tempMoveX);
    gmMIChromaSubsampling->setLabel(ofToString(loadedMovie.gmMIChromaSubsampling));
    gmMIChromaSubsampling->setYOffset(tempMoveX);
    gmMIBitDepthString->setLabel(ofToString(loadedMovie.gmMIBitDepthString));
    gmMIBitDepthString->setYOffset(tempMoveX);
    gmMIInterlacementString->setLabel(ofToString(loadedMovie.gmMIInterlacementString));
    gmMIInterlacementString->setYOffset(tempMoveX);
    gmMIAFormat->setLabel(ofToString(loadedMovie.gmMIAFormat));
    gmMIAFormat->setYOffset(tempMoveX);
    gmMIAChannelsString->setLabel(ofToString(loadedMovie.gmMIAChannelsString));
    gmMIAChannelsString->setYOffset(tempMoveX);
    gmMIASamplingRate->setLabel(ofToString(loadedMovie.gmMIASamplingRate));
    gmMIASamplingRate->setYOffset(tempMoveX);
    
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
    
    // update Tweener
    if (!updateInOut || !updateScrub || !updateNewPrintGrid) {
        Tweener.update();
    }
    
    // update Movie for Scrubbing or InOut manipulations
    if (loadedMovie.gmScrubMovie || updateInOut) {
        loadedMovie.gmMovieScrub.update();
    }
    
    // check if one of the topMenus is active and in this case turn of the mouseEvents for the thumbs
    if (menuMovieInfo.getMenuActivated() || menuSettings.getMenuActivated() || menuMoviePrintPreview.getMenuActivated() || menuMoviePrintSettings.getMenuActivated() || menuHelp.getMenuActivated()) {
        
        if (loadedMovie.getMouseEventsEnabled()) {
            loadedMovie.disableMouseEvents();
        }
        
        // Preview gets closed when menuMoviePrintSettings is not active
        if (menuMoviePrintPreview.getMenuActivated() && !menuMoviePrintSettings.getMenuActivated() && !menuMoviePrintPreview.getInsideMenuHead()) {
            menuMoviePrintPreview.closeMenuManually();
        }
        
        // Preview gets opened when menuMoviePrintSettings is active and showMoviePrintPreview is true
        if (menuMoviePrintSettings.getMenuActivated() && showMoviePrintPreview && !menuMoviePrintPreview.getMenuActivated()){
            menuMoviePrintPreview.openMenuManually();
        }
    } else {
        if (!loadedMovie.getMouseEventsEnabled()) {
            loadedMovie.enableMouseEvents();
        }
    }
    
    if (menuMoviePrintPreview.getMenuActivated()){
        writeFboToPreview(fmin(fboToPreview.getWidth()/(float)printGridWidth, fboToPreview.getHeight()/(float)printGridHeight), false);
    }
    
    // calculate rollout of ofxUI pos, scal
    guiMovieInfo->setPosition(menuMovieInfo.getPositionX(), menuMovieInfo.getPositionY()+headerHeight);
    guiMovieInfo->setHeight(menuMovieInfo.getSizeH()-headerHeight);

//    guiSettings1->setPosition(menuSettings.getPositionX(), menuSettings.getPositionY()+headerHeight);
//    guiSettings1->setWidth(menuSettings.getSizeW());
//    guiSettings1->setHeight(menuSettings.getSizeH()-headerHeight);
    
//    guiMoviePrintPreview->setPosition(menuMoviePrintPreview.getPositionX()-(thumbWidth + displayGridMargin)*1, menuMoviePrintPreview.getPositionY()+headerHeight);
//    guiMoviePrintPreview->setWidth(menuMoviePrintPreview.getSizeW() + (thumbWidth + displayGridMargin)*1);
//    guiMoviePrintPreview->setHeight(menuMoviePrintPreview.getSizeH()-headerHeight);
    
    guiSettingsMoviePrint->setPosition(menuMoviePrintSettings.getPositionX(), menuMoviePrintSettings.getPositionY()+headerHeight);
    guiSettingsMoviePrint->setHeight(menuMoviePrintSettings.getSizeH()-headerHeight);
    
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
        Tweener.addTween(scrubFade, 0, 0.5);
        if(scrubFade < 5){
            updateNewPrintGrid = FALSE;
            loadedMovie.allocateNewNumberOfStills(numberOfStills, thumbWidth, thumbHeight, showPlaceHolder);
            updateAllStills();
            devTurnOffMovieSwitch = FALSE;
        }
    }
    
    if (showDroppedList) {
        guiTimeline->setVisible(FALSE);
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
        if (scrollList) {
            if (!scrollBarList.sbCalculateScrollInertia && !scrollBar.sbScrollBarDrag) {
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
        
    } else if(showPlaceHolder){ // test
        
//        ofPushStyle();
//        ofSetColor(FAK_LIGHTGRAY);
//        ofRect(0, 0, ofGetWidth(), ofGetHeight());
//        ofPopStyle();
//        drawDisplayGrid(1, FALSE, FALSE, scrollAmountRel, showPlaceHolder);
//        scrollBar.draw();
//        drawUI(1, false);
        
//        loadedMovie.drawTitle(1.0);
//        ofPushMatrix();
//        ofTranslate(100, 100);
//        ofScale(0.2,0.2);
//        loadedMovie.fboHeaderInformation.draw(0,0);
//        ofPopMatrix();
    
    } else {
        
        if (showDroppedList) {
            
            drawList(scrollListAmountRel);
            scrollBarList.draw();
            
        } else {
            
            if (!loadedMovie.isMovieLoaded) { // if no movie is loaded
                if (!showDroppedList) {
                    guiTimeline->setVisible(FALSE);
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
                drawDisplayGrid(1, FALSE, FALSE, scrollAmountRel, showPlaceHolder);
                
                // draw the In and Out Point Manipulations
                if (updateInOut) {

                    ofPushStyle();
                    
                    ofEnableAlphaBlending();
                    ofSetColor(0,(int)(scrubFade/255)*155);
                    ofRect(0, 0, ofGetWidth(), ofGetHeight());
                    ofSetColor(255,255,255,(int)scrubFade);

                    loadedMovie.gmMovieScrub.draw(ofGetWidth()/2-scrubWindowW/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH);
                    loadedMovie.drawStillUI(scrubWindowGridNumber, ofGetWidth()/2-scrubWindowW/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH, (scrubFade/255));
                    
                    ofSetColor(255, 255, 255, (int)(scrubFade/255)*255);
                    
                    if (uiRangeSliderTimeline->hitLow) {
                        inPointImage.draw(ofGetWidth()/2-inPointImage.getWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-inPointImage.getHeight()/2);
                    }
                    if (uiRangeSliderTimeline->hitHigh) {
                        outPointImage.draw(ofGetWidth()/2-outPointImage.getWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-outPointImage.getHeight()/2);
                    }
                    if (uiRangeSliderTimeline->hitCenter) {
                        inPointImage.draw(ofGetWidth()/2-inPointImage.getWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-inPointImage.getHeight()/2);
                        outPointImage.draw(ofGetWidth()/2-outPointImage.getWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-outPointImage.getHeight()/2);
                    }
                    
                    if(scrubFade < 5){
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
void testApp::writeFboToPreview(float _scaleFactor, bool _showPlaceHolder){
    fboToPreview.begin();
    ofClear(255,255,255, 0);
    drawMoviePrintPreview(_scaleFactor, _showPlaceHolder);
    fboToPreview.end();
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
    
    if(guiSettings1->hasKeyboardFocus())
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
           
            if (showMenu) {
                tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),0);
            } else {
                tweenListInOut.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,ofRandom(600, 1000),0);
            }
            
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
            
        case 'x':
        {
            showFBO = !showFBO;
            
        }
			break;
            
        case 'z':
        {
            showPlaceHolder = !showPlaceHolder;
            
        }
			break;
            
        case 'w':
        {
            if (setupFinished) {
                
                loadNewMovie("", FALSE, TRUE, FALSE);
                
                printListNotImage = FALSE;
                showDroppedList = FALSE;
                finishedLoadingMovie = FALSE;
                showLoadMovieScreen = TRUE;
                    loadNewMovie("/Users/fakob/Movies/Daft Punk - Get Lucky by Shortology.mp4", TRUE, FALSE, TRUE);
                    if (loadedMovie.isMovieLoaded) {
                        moveToMovie();
                    }
                    updateMovieFromDrop = FALSE;
                
                
            }
            
        }
            break;

        case ' ':
        {
            ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + "." + ofToLower("EXT"), "Select a Folder");
            if (saveFileResult.bSuccess){
                ofLogVerbose("getName(): "  + saveFileResult.getName());
                ofLogVerbose("getPath(): "  + saveFileResult.getPath());
                saveMoviePrintPath = loadedMovie.getMoviePathName();
                ofLogVerbose("saveMoviePrintPath: "  + ofToString(saveMoviePrintPath));
            } else {
                ofLogVerbose("User hit cancel");
            }
        }
			break;

        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    currentKey = -1;
    
    if (uiRangeSliderTimeline->getState()) {
        
        if (key == OF_KEY_RIGHT || key == OF_KEY_LEFT || key == OF_KEY_UP || key == OF_KEY_DOWN) {
            uiSliderValueLow = uiRangeSliderTimeline->getScaledValueLow();
            uiSliderValueHigh = uiRangeSliderTimeline->getScaledValueHigh();
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

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
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
    loadedMovie.setAllLimitsLeft(leftMargin);
    droppedList.setAllLimitsLeft(leftMargin);
    loadedMovie.setAllLimitsLower(ofGetHeight());
    droppedList.setAllLimitsLower(ofGetHeight());
    loadedMovie.setAllLimitsRight(ofGetWidth() - scrollBarWidth);
    droppedList.setAllLimitsRight(ofGetWidth() - scrollBarWidth);
}

//--------------------------------------------------------------
void testApp::updateTheListScrollBar(){
    scrollBarList.updateScrollBar(ofGetWindowWidth(), ofGetWindowHeight(), headerHeight, 0, droppedList.getListHeight());
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
        guiSettings1->setHeight(ofGetWindowHeight());
        uiRangeSliderTimeline->setWidth(displayGridWidth);
        ofLog(OF_LOG_VERBOSE, "Timeslider Width:" + ofToString(uiRangeSliderTimeline->getWidth()));
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
            updateTheListScrollBar();

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
    
    delete guiTimeline;
    delete guiMovieInfo;
    delete guiHelp1;
    
    guiSettings1->saveSettings("guiSettings1.xml");
	delete guiSettings1;
    guiSettingsMoviePrint->saveSettings("guiMoviePrintSettings.xml");
	delete guiSettingsMoviePrint;


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
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {
            
            string movieFileName = loadedMovie.gmMovie.getMoviePath();
            movieFileName = loadedFilePath.getFileName(movieFileName, TRUE) + "_MoviePrint";

            string formatExtension;
            if (printFormat == OF_IMAGE_FORMAT_JPEG) {
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
	else if(name == "PrintScale")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		printScale = slider->getScaledValue();
        ofLog(OF_LOG_VERBOSE, "PrintScale: " + ofToString(printScale));
        if (isnan(printScale) || printScale > 20.0 || printScale < 1.0) {
            printScale = 1.0;
        }
	}
    else if(name == "PrintColumns")
	{
		ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
		ofLog(OF_LOG_VERBOSE, "PrintColumns " + ofToString(slider->getScaledValue()));
		printGridColumns = (int)slider->getScaledValue();
        if (printGridSetWithColumnsAndRows) {
            printNumberOfThumbs = printGridColumns * printGridRows;
            calculateNewPrintGrid();
        } else {
            printGridRows = ceil(numberOfStills/(float)printGridColumns);
            calculateNewPrintSize();
        }
	}
	else if(name == "PrintRows")
	{
		ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
		ofLog(OF_LOG_VERBOSE, "PrintRows " + ofToString(slider->getScaledValue()));
		printGridRows = (int)slider->getScaledValue();
		printNumberOfThumbs = printGridColumns * printGridRows;
        calculateNewPrintGrid();
	}
	else if(name == "PrintNumber")
	{
		ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
		ofLog(OF_LOG_VERBOSE, "PrintNumber " + ofToString(slider->getScaledValue()));
		printNumberOfThumbs = (int)slider->getScaledValue();
        calculateNewPrintGrid();
	}
    else if(name == "ShowMoviePrintPreview")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            menuMoviePrintPreview.openMenuManually();
        } else {
            menuMoviePrintPreview.closeMenuManually();
        }
        
	}
    else if(name == "DisplayVideoAudioInfo")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            printDisplayVideoAudioInfo = true;
        } else {
            printDisplayVideoAudioInfo = false;
        }
        calculateNewPrintSize();
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
            if (!(printNumberOfThumbs == (printGridColumns * printGridRows))){
                printNumberOfThumbs = printGridColumns * printGridRows;
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
		ofLog(OF_LOG_VERBOSE, "PrintMargin " + ofToString(slider->getScaledValue()));
		printGridMargin = (int)slider->getScaledValue();
        calculateNewPrintSize();
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
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            printFormat = OF_IMAGE_FORMAT_JPEG;
        }
	}
    else if(name == "png")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bool val = toggle->getValue();
        if (val) {
            printFormat = OF_IMAGE_FORMAT_PNG;
        }
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
void testApp::drawUI(int _scaleFactor, bool _hideInPrint){
    
    
    ofPushStyle();
    ofSetColor(255);

    // overlay von MoviePrintPreview
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
    ofSetColor(0, 0, 0, menuMoviePrintPreview.getRelSizeH2() * 100);
    ofRect(ofGetWindowWidth()/2, (ofGetWindowHeight()-headerHeight-footerHeight/2)/2 + headerHeight, ofGetWindowWidth(), (ofGetWindowHeight()-headerHeight-footerHeight/2));
    ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
    ofPopStyle();
    
    layoutHeaderImage.draw(0, 0, ofGetWindowWidth() * _scaleFactor, layoutHeaderImage.getHeight() * _scaleFactor);

    float tempY = 0;
    int tempXPos = 0;
    int menuHeightInRows = 4;
    
    tempXPos = gridColumns/2;
    menuMoviePrintPreview.setPosition((leftMargin + menuWidth * tweenListInOut.update() + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuMoviePrintPreview.setSize(thumbWidth, headerHeight + topMargin + (thumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
    menuMoviePrintPreview.drawMenu();

    tempXPos = 0;
    menuMovieInfo.setPosition((leftMargin + menuWidth * tweenListInOut.update() + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuMovieInfo.setSize(thumbWidth, headerHeight + topMargin + (thumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
    menuMovieInfo.drawMenu();

    tempXPos = 1;
    menuSettings.setPosition((leftMargin + menuWidth * tweenListInOut.update() + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuSettings.setSize(thumbWidth, headerHeight + topMargin + (thumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
    menuSettings.drawMenu();
    
    tempXPos = gridColumns-1;
    menuMoviePrintSettings.setPosition((leftMargin + menuWidth * tweenListInOut.update() + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuMoviePrintSettings.setSize(thumbWidth, headerHeight + topMargin + (thumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
    menuMoviePrintSettings.drawMenu();
    
    tempXPos = gridColumns-2;
    menuHelp.setPosition((leftMargin + menuWidth * tweenListInOut.update() + (thumbWidth + displayGridMargin)*tempXPos) * _scaleFactor, tempY);
    menuHelp.setSize(thumbWidth, headerHeight + topMargin + (thumbHeight + displayGridMargin)*menuHeightInRows - displayGridMargin);
    menuHelp.drawMenu();

    menuTimeline.setPosition(0, ofGetWindowHeight());
    menuTimeline.setSize(ofGetWindowWidth(), footerHeight/2);
    menuTimeline.drawMenu();
    
    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
    fboToPreview.draw((leftMargin + menuWidth * tweenListInOut.update() + (thumbWidth + displayGridMargin)*(gridColumns/2) + thumbWidth/2), headerHeight + topMargin + (thumbHeight + displayGridMargin)*menuHeightInRows/2.0 - displayGridMargin, menuMoviePrintPreview.getRelSizeH2() * fboToPreviewWidth, menuMoviePrintPreview.getRelSizeH2() * fboToPreviewHeight);
    ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
    
    ofPopStyle;
}

//--------------------------------------------------------------
void testApp::drawDisplayGrid(float _scaleFactor, bool _hideInPNG, bool _isBeingPrinted, float _scrollAmountRel, bool _showPlaceHolder){
    
    ofPushStyle();
    float _scrollAmount = 0;
    if (scrollBar.sbActive) {
        _scrollAmount = ((displayGridHeight - (ofGetWindowHeight() - headerHeight - topMargin - bottomMargin)) * -1) * _scrollAmountRel;
    }
    if (isnan(_scrollAmount)) {
        _scrollAmount = 0;
    }
    float tempX = (leftMargin + menuWidth * tweenListInOut.update()) * _scaleFactor;
    float tempY = _scrollAmount * _scaleFactor + topMargin + headerHeight;
    loadedMovie.drawGridOfStills(tempX, tempY, gridColumns, displayGridMargin, _scrollAmount, _scaleFactor, 1, _isBeingPrinted, TRUE, superKeyPressed, shiftKeyPressed, _showPlaceHolder, printHeaderHeight);
    ofPopStyle();
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
    
    droppedList.draw(leftMargin + menuWidth * tweenListInOut.update(), topMargin, ofGetWidth() - scrollBarWidth - rightMargin - leftMargin, _scrollAmount);

    
}

//--------------------------------------------------------------
void testApp::drawPrintScreen(){
    
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 127);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255, 255);
    if (currPrintingList) {
        printListImage.draw(ofGetWidth()/2-printImage.getWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-printImage.getHeight()/2);
    } else {
        printImage.draw(ofGetWidth()/2-printImage.getWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-printImage.getHeight()/2);

    }
    ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::drawStartScreen(){

    ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    startImage.draw(ofGetWidth()/2-startImage.getWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-startImage.getHeight()/2);
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
        ofRectRounded(((thumbWidth+displayGridMargin)*(i%gridColumns))+(ofGetWidth()/2 - displayGridWidth/2), ((thumbHeight+displayGridMargin)*(i/gridColumns))+(ofGetHeight()/2 - displayGridHeight/2), thumbWidth, thumbHeight, thumbWidth/64);
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
    float tempX = ofGetWidth()/2 - tempLoaderBarWidth/2 + menuWidth * tweenListInOut.update();
    float tempY = ofGetHeight()/2 - tempLoaderBarHeight/2;
        // draw the percentage loaded bar
        // the background
        ofSetColor(100);
        ofRect(tempX, tempY + tempOffsetY, tempLoaderBarWidth, tempLoaderBarHeight);
        // draw the percentage
        ofSetColor(238, 71, 0);
        ofRect(tempX, tempY + tempOffsetY, tempLoaderBarWidth*loadValue, tempLoaderBarHeight);

    ofSetColor(255, 255, 255, 255);
    updatingImage.draw(ofGetWidth()/2-updatingImage.getWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-updatingImage.getHeight()/2);
    ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::drawLoadMovieScreen(){

    ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    loadMovieImage.draw(ofGetWidth()/2-loadMovieImage.getWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-loadMovieImage.getHeight()/2);
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
    ofRect(0, 0, _scaleFactor * printGridWidth, _scaleFactor * printGridHeight);
    loadedMovie.drawMoviePrint(0, 0, printGridColumns, printGridMargin, _scaleFactor, 1, _showPlaceHolder, printHeaderHeight, printDisplayVideoAudioInfo, true);
    
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
    ofSetColor(0,(scrubFade/255)*100);
    ofRect(0, 0, ofGetWidth(), ofGetHeight()-footerHeight/2);

    // draw the scrubMovie
    ofSetColor(255,(int)scrubFade);
    int j = loadedMovie.gmScrubID;
    loadedMovie.gmMovieScrub.draw(ofGetWidth()/2-scrubWindowW/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH);
    loadedMovie.drawStillUI(j, ofGetWidth()/2-scrubWindowW/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2-scrubWindowH/2, scrubWindowW, scrubWindowH, (float)(scrubFade/255));
    
    // drawing frame
    float tempX = ofGetWidth()/2-scrubWindowW/2 + menuWidth * tweenListInOut.update();
    float tempY = ofGetHeight()/2-scrubWindowH/2;
    float tempFrameWidth = 3;
    ofSetColor(220,(int)scrubFade);
    ofRect(tempX, tempY - tempFrameWidth, _scaleFactor * scrubWindowW + tempFrameWidth, tempFrameWidth);
    ofRect(tempX - tempFrameWidth, tempY - tempFrameWidth, tempFrameWidth, _scaleFactor * scrubWindowH + tempFrameWidth);
    ofRect(tempX + _scaleFactor * scrubWindowW, tempY, tempFrameWidth, _scaleFactor * scrubWindowH + tempFrameWidth);
    ofRect(tempX - tempFrameWidth, tempY + _scaleFactor * scrubWindowH, _scaleFactor * scrubWindowW + tempFrameWidth, tempFrameWidth);
    // drawing shadow
    ofSetColor(0,200*(scrubFade/255.0));
    ofRect(tempX + _scaleFactor * scrubWindowW + tempFrameWidth, tempY, tempFrameWidth, _scaleFactor * scrubWindowH + tempFrameWidth*2);
    ofRect(tempX, tempY + _scaleFactor * scrubWindowH + tempFrameWidth, _scaleFactor * scrubWindowW + tempFrameWidth, tempFrameWidth);

    // draw the scrubSpeed
    ofSetColor(FAK_ORANGECOLOR,(int)scrubFade);
    ofRect(ofGetWidth()/2 + menuWidth * tweenListInOut.update(), ofGetHeight()/2+scrubWindowH/2+tempFrameWidth*3, scrubDelta*30.0, loaderBarHeight/2);
    ofSetColor(255,255,255,(int)scrubFade);
    
    ofDisableAlphaBlending();
    ofSetColor(255);
    if(scrubFade < 5){
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
void testApp::printImageToPNG(int _printSizeWidth){
    
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
//        if (outputWidth > 9999.0) {
//            _scaleFactor = 9950.0 / (gridAreaWidth + gridMargin * 2);
//            outputWidth = (gridAreaWidth + gridMargin * 2) * _scaleFactor;
//            outputHeight = (gridAreaHeight + gridMargin * 2) * _scaleFactor;
//        }
        
        if (printFormat == OF_IMAGE_FORMAT_JPEG) {
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
            ofLog(OF_LOG_VERBOSE, "outputSize " + ofToString(outputWidth) + "x" + ofToString(outputHeight));

            fboToSave.allocate(outputWidth, outputHeight, GL_RGB);
            gmPixToSave.allocate(outputWidth, outputHeight, OF_PIXELS_RGB);
        }
        else {
            fboToSave.allocate(outputWidth, outputHeight, GL_RGBA);
            gmPixToSave.allocate(outputWidth, outputHeight, OF_PIXELS_RGB);
        }

        ofPushMatrix();
        ofPushStyle();
        fboToSave.begin();
        ofClear(0,0,0,0);
        ofBackground(0, 0, 0, 0);
        ofSetColor(255, 255, 255, 255);

        loadedMovie.drawMoviePrint(0, 0, printGridColumns, printGridMargin, _newScaleFactor, 1, showPlaceHolder, printHeaderHeight, printDisplayVideoAudioInfo, false);

//        loadedMovie.drawGridOfStills(printGridMargin, printGridMargin, printGridColumns, printGridMargin, 0, _newScaleFactor, 1, TRUE, TRUE, superKeyPressed, shiftKeyPressed, showPlaceHolder, printHeaderHeight);
        fboToSave.end();
        fboToSave.readToPixels(gmPixToSave);
                ofLog(OF_LOG_VERBOSE, "gmPixToSave:getImageType" + ofToString(gmPixToSave.getImageType()));
        ofPopStyle();
        ofPopMatrix();

        string pathName = loadedMovie.gmMovie.getMoviePath();
        pathName = loadedFilePath.getFileName(pathName, TRUE);
        string formatExtension;
        if (printFormat == OF_IMAGE_FORMAT_JPEG) {
            formatExtension = "jpg";
        } else {
            formatExtension = "png";
        }
        string imageName = pathName + "_MoviePrint." + formatExtension;
        imageName = saveMoviePrintPath + imageName;
        ofSaveImage(gmPixToSave, imageName, OF_IMAGE_QUALITY_HIGH);
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
            calculateNewPrintGrid();
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
        if (numberOfStills == 1) {
            gridTimeArray[i] = ofMap(0.5, 0.0, 1.0, uiSliderValueLow, uiSliderValueHigh, TRUE);

        } else {
            gridTimeArray[i] = ofMap(float(i)/(numberOfStills - 1), 0.0, 1.0, uiSliderValueLow, uiSliderValueHigh, TRUE);
        }
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
    guiTimeline->setVisible(TRUE);
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
    guiTimeline->setVisible(FALSE);
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
bool testApp::fequal(float _x, float _y, float _t){
    float diff = abs(_x-_y);
    if (diff < _t) {
        return TRUE;
    } else  {
        return FALSE;
    }
}
