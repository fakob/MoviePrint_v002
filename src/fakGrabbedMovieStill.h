//
//  fakGrabbedMovieStill.h
//  fakBreakupVideo
//
//  Created by Jakob on 3/2/13.
//
//

#ifndef fakBreakupVideo_fakGrabbedMovieStill_h
#define fakBreakupVideo_fakGrabbedMovieStill_h

class fakGrabbedMovieStill {
    
public:
    
    fakGrabbedMovieStill(){}
    
    
    // Functions
    void registerMouseEvents(){
        ofRegisterMouseEvents(this);
    }
    void unregisterMouseEvents(){
        ofUnregisterMouseEvents(this);
    }
    
    void mouseMoved(ofMouseEventArgs & args){ // check if mouse is inside buttons
        gsRollOver = FALSE;
        gsRollOver0 = FALSE;
        gsRollOver1 = FALSE;
        gsRollOver2 = FALSE;
        gsRollOver3 = FALSE;
        gsRollOver4 = FALSE;
        if (inside(args.x, args.y)) {
            gsRollOver = TRUE;
            if (insideButton(1, args.x, args.y)) {
                gsRollOver1 = TRUE;
                ofVec2f transferID = ofVec2f(gsID, 1);
                ofNotifyEvent(gsMovedInside, transferID, this);
            } else if (insideButton(2, args.x, args.y)) {
                gsRollOver2 = TRUE;
                ofVec2f transferID = ofVec2f(gsID, 2);
                ofNotifyEvent(gsMovedInside, transferID, this);
            } else if (insideButton(3, args.x, args.y)) {
                gsRollOver3 = TRUE;
                ofVec2f transferID = ofVec2f(gsID, 3);
                ofNotifyEvent(gsMovedInside, transferID, this);
            } else if (insideButton(4, args.x, args.y)) {
                gsRollOver4 = TRUE;
                ofVec2f transferID = ofVec2f(gsID, 4);
                ofNotifyEvent(gsMovedInside, transferID, this);
            } else {
                gsRollOver0 = FALSE;
                ofVec2f transferID = ofVec2f(gsID, 0);
                ofNotifyEvent(gsMovedInside, transferID, this);
            }
            
        }
    }
    void mouseDragged(ofMouseEventArgs & args){}
    void mouseReleased(ofMouseEventArgs & args){}
    void mousePressed(ofMouseEventArgs & args){ //check if mouse is pressed inside still
        if (inside(args.x, args.y)) {
            // if the mouse is pressed over the circle an event will be notified (broadcasted)
            // the circleEvent object will contain the mouse position, so this values are accesible to any class that is listening.
            ofVec2f mousePos = ofVec2f(args.x, args.y);
            ofNotifyEvent(gsClickedInside, gsID, this);
            ofLog(OF_LOG_VERBOSE, "inside ID:" + ofToString(gsID) + " : " + ofToString(mousePos) );
        }
    }
    void mouseScrolled(ofMouseEventArgs & args){
//        ofLog(OF_LOG_VERBOSE, "scrollAmount x:y " + ofToString(args.x) + ":" + ofToString(args.y) );
    }
    
    bool inside(float _x, float _y ){ //function to check if mouse is inside grabbedStill
        return _x >= gsX && _x < gsX + gsDrawWidth && _y >= gsY && _y < gsY + gsDrawHeight && _y > gsUpperLimitY && _y < gsLowerLimitY && _x > gsLeftLimitX && _x < gsRightLimitX;
    }
    bool insideButton(int buttonID, float _x, float _y ){ //function to check if mouse is over one ov the Buttons inside grabbedStill
        
        float buttonWidth = gsDrawHeight/2;
        float offsetX, offsetY;
        
        switch (buttonID)
        {
            case 1:
                offsetX = gsX;
                offsetY = gsY;
                break;
            case 2:
                offsetX = gsX + gsDrawWidth - buttonWidth;
                offsetY = gsY;
                break;
            case 3:
                offsetX = gsX;
                offsetY = gsY + gsDrawHeight - buttonWidth;
                break;
            case 4:
                offsetX = gsX + gsDrawWidth - buttonWidth;
                offsetY = gsY + gsDrawHeight - buttonWidth;
                break;
            default:
                break;
        }
        
        return _x >= offsetX && _x < offsetX + buttonWidth && _y >= offsetY && _y < offsetY + buttonWidth && _y > gsUpperLimitY && _y < gsLowerLimitY && _x > gsLeftLimitX && _x < gsRightLimitX;
    }
    
    // Properties
    ofEvent<int> gsClickedInside;
    ofEvent<ofVec2f> gsMovedInside;
    
    ofImage gsImage; //image into which the videoframe is grabbed
    ofTexture gsTexture; //texture into which the grabbedStill is copied - this is drawn to screen - this is also resized if necessary
    int gsID; // ID in Array
    int gsFrameNumber; // current Framenumber
    bool gsManipulated = FALSE; // indicating if the framenumber is automatically assigned or if it was manually manipulated
    bool gsToBeUpdated = TRUE; // This frame has changed somehow, it needs to be redrawn
    bool gsToBeGrabbed = TRUE; // The framenumber has changed so the frame has to be grabbed again (threadable)
    //    bool gsFrameCorrupt = FALSE; //set if frame is corrupt
    int gsX; //x position of grabbedStill
    int gsY; //y position of grabbedStill
    int gsWidth; //original width of grabbedStill
    int gsHeight; //original height of grabbedStill
    int gsDrawWidth; //drawn width of grabbedStill
    int gsDrawHeight; //drawn height of grabbedStill
    int gsUpperLimitY; // Lower Limit for turning off mouseevent testing if over the header
    int gsLowerLimitY; // Higher Limit for turning off mouseevent testing if over the footer
    int gsLeftLimitX; // Left Limit for turning off mouseevent testing if over the left menu
    int gsRightLimitX; // Right Limit for turning off mouseevent testing if over the right scrollbar
    float gsResizeFactor; //factor of which the original and the drawn differ
    bool gsRollOver = FALSE;
    bool gsRollOver0 = FALSE;
    bool gsRollOver1 = FALSE;
    bool gsRollOver2 = FALSE;
    bool gsRollOver3 = FALSE;
    bool gsRollOver4 = FALSE;
    
};


#endif
