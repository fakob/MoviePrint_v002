//
//  fakScrollBar.h
//  fakMoviePrint
//
//  Created by fakob on 13/05/15.
//
//

#ifndef fakMoviePrint_fakScrollBar_h
#define fakMoviePrint_fakScrollBar_h

class fakScrollBar {
    
public:
    
    fakScrollBar(){}
    
    
    // Functions
    void registerMouseEvents(){
        ofRegisterMouseEvents(this);
    }
    
    void unregisterMouseEvents(){
        ofUnregisterMouseEvents(this);
    }
    
    void mouseMoved(ofMouseEventArgs & args){}
    
    void mouseDragged(ofMouseEventArgs & args){
        if (inside(args.x, args.y)) {
            ofVec2f mousePos = ofVec2f(args.x, args.y);
            ofNotifyEvent(sbClickedInside, mousePos, this);
            ofLog(OF_LOG_VERBOSE, ofToString(sbID) + " : " + ofToString(mousePos) );
        }
    }
    
    void mouseReleased(ofMouseEventArgs & args){}
    
    void mousePressed(ofMouseEventArgs & args){
        if (inside(args.x, args.y)) {
// if the mouse is pressed over the circle an event will be notified (broadcasted)
// the circleEvent object will contain the mouse position, so this values are accesible to any class that is listening.
            ofVec2f mousePos = ofVec2f(args.x, args.y);
            ofNotifyEvent(sbClickedInside, mousePos, this);
            ofLog(OF_LOG_VERBOSE, ofToString(sbID) + " : " + ofToString(mousePos) );
        }
    }
    
    void mouseScrolled(ofMouseEventArgs & args){
//        ofLog(OF_LOG_VERBOSE, "scrollAmount x:y " + ofToString(args.x) + ":" + ofToString(args.y) );
    }
    
    bool inside(float _x, float _y ){ //function to check if mouse is inside grabbedStill
        return _x >= sbAreaX && _x < sbAreaX + sbAreaWidth && _y >= sbAreaY && _y < sbAreaY + sbAreaHeight;
    }

    void setup(int _sbID, int _sbAreaX, int _sbAreaY, int _sbAreaWidth, int _sbAreaHeight, float _sbX, float _sbY, int _sbWidth, int _sbHeight, int _sbLoose){
        sbWidth = _sbWidth;
        sbHeight = _sbHeight;
        sbAreaWidth = _sbAreaWidth;
        sbAreaHeight = _sbAreaHeight;
        sbX = _sbX;
        sbY = _sbY;
        sbAreaX = _sbAreaX;
        sbAreaY = _sbAreaY;
        sbY = sbAreaY + sbAreaHeight/2 - sbHeight/2;
        sbNewY = sbY;
        sbMin = _sbAreaY;
        sbMax = _sbAreaY + sbAreaHeight - sbHeight;
        sbLoose = _sbLoose;
    }
    
    void setScrollBarHeight(float _sbRatio){
        if (_sbRatio > 1.0) {
            _sbRatio = 1.0;
            sbActive = FALSE;
        } else {
            sbActive = TRUE;
        }
        sbRatio = _sbRatio;
        sbHeight = sbAreaHeight*sbRatio;
        sbMax = sbAreaY + sbAreaHeight - sbHeight;
    }
    
    void updateScrollBar(int _newWindowWidth, int _newWindowHeight, int _gridAreaHeight, int _headerHeight, int _footerHeight){
        unregisterMouseEvents();
        setup(0, _newWindowWidth-sbWidth, _headerHeight, sbWidth, _newWindowHeight-_headerHeight-_footerHeight, _newWindowWidth-sbWidth, _newWindowHeight/2, sbWidth, _newWindowHeight/2, 16);
        setScrollBarHeight((float)sbAreaHeight/(float)_gridAreaHeight);
        if (sbActive) {
//            sbY = sbMin;
            registerMouseEvents();
        }
        ofLog(OF_LOG_VERBOSE, "sbActive:" + ofToString(sbActive) );
    }
    
    void setScrollBarWidth(int _scrollBarWidth){
        sbWidth = _scrollBarWidth;
    }
    
    float constrainBar(float val, float minv, float maxv) {
        return min(max(val, minv), maxv);
    }
    
    float getPos(){
        if (sbActive) {
            float relativeValue = (sbY-sbMin)/(sbMax-sbMin);
            return (relativeValue);
        }
    }
    
    void setToTop(){
        sbY = sbMin;
        
    }
    // update scrollbar position through dragging
    void dragUpdate(int _mouseX, int _mouseY){
        if (sbActive) {
            sbNewY = constrainBar(_mouseY-sbHeight/2, sbMin, sbMax);
            
            if ((abs(sbNewY - sbY)) > 1) {
                sbY = sbY + (sbNewY-sbY)/sbLoose;
            }
        }
    }

    // update scrollbar position through scrolling
    void scrollUpdate(double _scrollX, double _scrollY, int _scrollMultiplier){
        if (sbActive) {
            sbNewY = constrainBar(sbY - _scrollY*_scrollMultiplier, sbMin, sbMax);
            if ((abs(sbNewY - sbY)) > 1) {
                sbY = sbY + (sbNewY-sbY)/sbLoose;
            }
        }
    }
    
    void draw(){
        if (sbActive) {
            
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(238, 71, 0, 127);
            ofRect(sbX, sbY, sbWidth, sbHeight);
            ofPopStyle();
        }
    }
    
// Properties
    ofEvent<ofVec2f> sbClickedInside;
    
    int sbID; // ID in Array
    float sbX; //x position of Slider
    float sbY; //y position of Slider
    float sbNewY;
    int sbWidth; //drawn width of ScrollBar
    int sbHeight; //drawn height of ScrollBar
    float sbMin;
    float sbMax;
    int sbLoose;
    bool sbOver;
    bool sbLocked;
    float sbRatio;
    bool sbActive;
    
    int sbAreaWidth;
    int sbAreaHeight;
    int sbAreaX;//x position of ScrollBar
    int sbAreaY;//y position of ScrollBar
    float sbHitPoint;
    int currMouseY;
    
};


#endif
