//
//  fakScrollBar.h
//  fakMoviePrint
//
//  Created by fakob on 13/05/15.
//
//

#include "ofxMultiTouchPad.h"


#ifndef fakMoviePrint_fakScrollBar_h
#define fakMoviePrint_fakScrollBar_h

class fakScrollBar {
    
public:
    
    fakScrollBar(){}
    
    
    // Functions
    
    void setup(int _sbID, int _sbScrollAreaX, int _sbScrollAreaY, int _sbScrollAreaWidth, int _sbScrollAreaHeight, float _sbScrollBarX, float _sbScrollBarY, int _sbScrollBarWidth, float _sbLoose){
        sbFirstClick = false;
        sbFirstClickMouseY = 0;
        sbScrollBarWidth = _sbScrollBarWidth;
        sbScrollBarHeight = _sbScrollAreaHeight/2;
        sbScrollAreaWidth = _sbScrollAreaWidth;
        sbScrollAreaHeight = _sbScrollAreaHeight;
        sbScrollBarX = _sbScrollBarX;
        sbScrollBarY = _sbScrollBarY;
        sbScrollAreaX = _sbScrollAreaX;
        sbScrollAreaY = _sbScrollAreaY;
        sbScrollBarY = sbScrollAreaY + sbScrollAreaHeight/2 - sbScrollBarHeight/2;
        sbScrollBarYNew = sbScrollBarY;
        sbScrollBarYMin = _sbScrollAreaY;
        sbScrollBarYMax = _sbScrollAreaY + sbScrollAreaHeight - sbScrollBarHeight;
        sbLoose = _sbLoose;
        setToTop();
        sbScrollBarDrag = false;
        sbOvershootHeight = 20;
        sbMouseIsScrolling = false;
        sbCalculateScrollInertia = false;
        sbMaxBottomOverShootArea = false;
        sbMaxTopOverShootArea = false;
        scrollMultiplier = 10.0;
        
        // should not be lower then 1000ms as the magic mouse sends still after having stopped scrolling
        // as the magic mouse sends shaky values it is difficult to control it
        sbBounceBackTimeMs = 1000;
        
        sbNumberOfTouches = 0;
        sbMaxNumberOfTouches = 0;
    }
    
    void setScrollHeight(float _sbScrollHeight){
        float _sbRatio = (float)sbScrollAreaHeight/_sbScrollHeight;
        if (_sbRatio > 1.0) {
            _sbRatio = 1.0;
            sbActive = FALSE;
        } else {
            sbActive = TRUE;
        }
        sbScrollBarHeight = sbScrollAreaHeight*_sbRatio;
        sbScrollBarYMax = sbScrollAreaY + sbScrollAreaHeight - sbScrollBarHeight;
    }
    
    void updateScrollBar(int _newWindowWidth, int _newWindowHeight, int _newScrollHeight, int _headerHeight, int _footerHeight){
        unregisterMouseEvents();
        setup(0, _newWindowWidth-sbScrollBarWidth, _headerHeight, sbScrollBarWidth, _newWindowHeight-_headerHeight-_footerHeight, _newWindowWidth-sbScrollBarWidth, _newWindowHeight/2, sbScrollBarWidth, 16);
        setScrollHeight((float)_newScrollHeight);
        if (sbActive) {
            registerMouseEvents();
        }
        ofLog(OF_LOG_VERBOSE, "sbActive:" + ofToString(sbActive) );
    }
    
    void setScrollBarWidth(int _scrollBarWidth){
        sbScrollBarWidth = _scrollBarWidth;
    }
    
    float valMinMax(float val, float minv, float maxv) {
        return min(max(val, minv), maxv);
    }
    
    float getRelativePos(){
        if (sbActive) {
            float relativeValue = (sbScrollBarY-sbScrollBarYMin)/(sbScrollBarYMax-sbScrollBarYMin);
            return (relativeValue);
        } else {
            return 0.0;
        }
    }
    
    void setToTop(){
        sbScrollBarY = sbScrollBarYMin;
    }
    
    void registerMouseEvents(){
        ofRegisterMouseEvents(this);
    }
    
    void unregisterMouseEvents(){
        ofUnregisterMouseEvents(this);
    }
    
    void registerTouchEvents(){
        ofAddListener(pad.update, this, &fakScrollBar::padUpdates);
        ofAddListener(pad.touchRemoved, this, &fakScrollBar::removedTouch);
    }
    
    void unregisterTouchEvents(){
        ofRemoveListener(pad.update, this, &fakScrollBar::padUpdates);
    }
    
    bool insideScrollArea(float _x, float _y ){
        return _x >= sbScrollAreaX && _x < sbScrollAreaX + sbScrollAreaWidth && _y >= sbScrollAreaY && _y < sbScrollAreaY + sbScrollAreaHeight;
    }
    
    bool insideScrollBar(float _x, float _y ){
        return _x >= sbScrollBarX && _x < sbScrollBarX + sbScrollBarWidth && _y >= sbScrollBarY && _y < sbScrollBarY + sbScrollBarHeight;
    }
    
    void mouseMoved(ofMouseEventArgs & args){
        sbRollOverScrollBar = FALSE;
        sbRollOverScrollArea = FALSE;
        if (insideScrollArea(args.x, args.y)) {
            sbRollOverScrollArea = TRUE;
            if (insideScrollBar(args.x, args.y)) {
                sbRollOverScrollBar = TRUE;
            }
        }
    }
    
    void mouseDragged(ofMouseEventArgs & args){
        if (sbRollOverScrollArea) {
            sbScrollBarDrag = TRUE;
        }
        sbMousePos = ofVec2f(args.x, args.y);
    }
    
    void mouseReleased(ofMouseEventArgs & args){
        sbMousePos = ofVec2f(args.x, args.y);
        sbFirstClick = false;
        sbScrollBarDrag = false;
    }
    
    void mousePressed(ofMouseEventArgs & args){
        if (sbRollOverScrollArea) {
            sbScrollBarDrag = TRUE;
        }
        sbMousePos = ofVec2f(args.x, args.y);
    }
    
    void mouseScrolled(ofMouseEventArgs & args){
        //        ofLog(OF_LOG_VERBOSE, "scrollAmount x:y " + ofToString(args.x) + ":" + ofToString(args.y) );
        sbScrollBarDrag = false;
        
        if (sbMaxNumberOfTouches > 0){ // check if touchmouse
            if ((sbNumberOfTouches != 0)) {
                sbMouseScrollVelocity = ofVec2f(args.x * scrollMultiplier, args.y * scrollMultiplier);
                sbMouseIsScrolling = true;
                sbCalculateScrollInertia = true;
                ofNotifyEvent(sbScrollingGoingOn, args, this);
                ofResetElapsedTimeCounter();
            }
        } else { // no touchmouse used
            if ((ofGetElapsedTimeMillis() > 50) || (ofGetElapsedTimeMillis() < 10)) {
                sbMouseScrollVelocity = ofVec2f(args.x * scrollMultiplier, args.y * scrollMultiplier);
                sbMouseIsScrolling = true;
                sbCalculateScrollInertia = true;
                ofNotifyEvent(sbScrollingGoingOn, args, this);
                ofResetElapsedTimeCounter();
            }
        }
    }
    
    void padUpdates(int & t) {
        sbNumberOfTouches = t;
        sbMaxNumberOfTouches = fmax(t, sbNumberOfTouches);
        //        ofLog(OF_LOG_VERBOSE, "padUpdates " + ofToString(t));
    }
    
    void removedTouch(int & r) {
        sbNumberOfTouches = r;
        //        ofLog(OF_LOG_VERBOSE, "removedTouch " + ofToString(r));
    }
    
    void update(){
        if (sbActive) {
            
            
            if (sbScrollBarDrag) {
                if (sbRollOverScrollBar) {
                    if (!sbFirstClick) {
                        sbFirstClickMouseY = sbMousePos.y - sbScrollBarY;
                        sbFirstClick = true;
                    }
                    //                    ofLog(OF_LOG_VERBOSE, "_mouseY:sbY " + ofToString(sbMousePos.y) + " :sbY " + ofToString(sbScrollBarY));
                    sbScrollBarYNew = valMinMax(sbMousePos.y - sbFirstClickMouseY, sbScrollBarYMin, sbScrollBarYMax);
                    if ((abs(sbScrollBarYNew - sbScrollBarY)) > 1) {
                        sbScrollBarY = sbScrollBarY + (sbScrollBarYNew-sbScrollBarY)/sbLoose;
                    }
                } else { // click insidescrollarea but not inside insidescrollbar
                    sbScrollBarYNew = valMinMax(sbMousePos.y - sbScrollBarHeight/2, sbScrollBarYMin, sbScrollBarYMax);
                    if ((abs(sbScrollBarYNew - sbScrollBarY)) > 1) {
                        sbScrollBarY = sbScrollBarY + (sbScrollBarYNew-sbScrollBarY)/sbLoose;
                    }
                }
                //                ofLog(OF_LOG_VERBOSE, "_____________dragUpdate " + ofToString(sbMousePos.y) );
                
                
            } else if (sbCalculateScrollInertia){
                //                ofLog(OF_LOG_VERBOSE, "_____________scrollUpdate " + ofToString(sbMouseScrollVelocity.y) );
                if ((sbScrollBarY >= (sbScrollBarYMax + sbOvershootHeight - 1.0))) {
                    sbMaxBottomOverShootArea = true;
                }
                if ((sbScrollBarY <= (sbScrollBarYMin - sbOvershootHeight + 1.0))){
                    sbMaxTopOverShootArea = true;
                }
                if ((sbScrollBarY > sbScrollBarYMax) && !sbMouseIsScrolling) {
                    if (sbMaxBottomOverShootArea) {
                        float tempDelta = (sbOvershootHeight - 1.0) * pow(getTimeRampDown(),4);
                        sbScrollBarY = sbScrollBarYMax + tempDelta;
                        //                        ofLog(OF_LOG_VERBOSE, "sbMaxBottomOverShootArea " + ofToString(sbMaxBottomOverShootArea) + " sbScrollBarY " + ofToString(sbScrollBarY) + " tempDelta " + ofToString(tempDelta));
                    } else {
                        int howFarInOverShootArea = sbScrollBarY - sbScrollBarYMax;
                        sbScrollBarY = sbScrollBarYMax + howFarInOverShootArea * getTimeRampDown();
                    }
                } else if ((sbScrollBarY < sbScrollBarYMin) && !sbMouseIsScrolling) {
                    if (sbMaxTopOverShootArea){
                        float tempDelta = (sbOvershootHeight - 1.0) * pow(getTimeRampDown(),4);
                        sbScrollBarY = sbScrollBarYMin - tempDelta;
                        //                        ofLog(OF_LOG_VERBOSE, "sbMaxTopOverShootArea " + ofToString(sbMaxTopOverShootArea) + " sbScrollBarY " + ofToString(sbScrollBarY) + " tempDelta " + ofToString(tempDelta));
                    } else {
                        int howFarInOverShootArea = sbScrollBarY - sbScrollBarYMin;
                        sbScrollBarY = sbScrollBarYMin + howFarInOverShootArea * getTimeRampDown();
                    }
                } else {
                    sbMouseScrollVelocity.y = sbMouseScrollVelocity.y / sbLoose;
                    sbScrollBarY = sbScrollBarY + sbMouseScrollVelocity.y;
                    sbScrollBarY = valMinMax(sbScrollBarY, sbScrollBarYMin-sbOvershootHeight, sbScrollBarYMax+sbOvershootHeight);
                    //                    ofLog(OF_LOG_VERBOSE, "sbNormalScrollArea " + ofToString(sbMaxTopOverShootArea) + " sbScrollBarY " + ofToString(sbScrollBarY));
                }
                if (ofGetElapsedTimeMillis() > 50) {
                    sbMouseIsScrolling = false;
                    if (ofGetElapsedTimeMillis() > sbBounceBackTimeMs) {
                        sbCalculateScrollInertia = false;
                    }
                }
            }
        }
        //        ofLog(OF_LOG_VERBOSE, ofToString(ofGetElapsedTimeMillis()));
        //        ofLog(OF_LOG_VERBOSE, "sbScrollBarY " + ofToString(sbScrollBarY));
    }
    
    float getTimeRampDown(){
        return (1.0 - valMinMax(ofGetElapsedTimeMillis()/(float)sbBounceBackTimeMs, 0.0, 1.0));
    }
    
    void draw(){
        if (sbActive) {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(238, 71, 0, 127);
            ofRectRounded(sbScrollBarX, sbScrollBarY, sbScrollBarWidth, sbScrollBarHeight, sbScrollBarWidth);
            ofPopStyle();
        }
    }
    
    // Properties
    ofEvent<ofVec2f> sbClickedInside;
    ofEvent<ofVec2f> sbScrollingGoingOn;
    
    int sbID; // ID in Array
    float sbScrollBarX; //x position of Slider upper left corner
    float sbScrollBarY; //y position of Slider upper left corner
    float sbScrollBarYNew;
    int sbScrollBarWidth; //drawn width of ScrollBar
    int sbScrollBarHeight; //drawn height of ScrollBar
    float sbScrollBarYMin;
    float sbScrollBarYMax;
    float sbLoose; //inertia factor
    bool sbActive;
    
    int sbScrollAreaWidth;
    int sbScrollAreaHeight;
    int sbScrollAreaX;//x position of ScrollBar upper left corner
    int sbScrollAreaY;//y position of ScrollBar upper left corner
    
    bool sbFirstClick;
    int sbFirstClickMouseY;
    bool sbRollOverScrollBar;
    bool sbRollOverScrollArea;
    
    bool sbScrollBarDrag;
    bool sbMouseIsScrolling;
    bool sbCalculateScrollInertia;
    
    ofVec2f sbMousePos;
    ofVec2f sbMouseScrollVelocity;
    
    int sbOvershootHeight;
    bool sbMaxBottomOverShootArea;
    bool sbMaxTopOverShootArea;
    float scrollMultiplier;
    
    int sbBounceBackTimeMs;
    
    ofxMultiTouchPad pad;
    int sbNumberOfTouches;
    int sbMaxNumberOfTouches; //check if touchmouse is used
};


#endif
