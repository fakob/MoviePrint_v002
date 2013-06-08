//
//  fakGrabbedList.h
//  fakBreakupVideo
//
//  Created by Jakob on 3/14/13.
//
//

#include "ofMain.h"

#ifndef fakBreakupVideo_fakGrabbedListItem_h
#define fakBreakupVideo_fakGrabbedListItem_h

#define FAK_ORANGECOLOR ofColor(238, 71, 0, 255)
#define FAK_DARKORANGECOLOR ofColor(99, 30, 0, 255)
#define FAK_DARKDARKORANGECOLOR ofColor(75, 21, 0, 255)
#define FAK_MIDDLEDARKORANGECOLOR ofColor(170, 50, 0, 255)
#define FAK_LIGHTERMIDDLEDARKORANGECOLOR ofColor(185, 55, 0, 255)


struct fakItemProperties{
    int ipID;
    string ipName;
    bool ipPrinted;
    bool ipTriedToPrint;
};

class fakGrabbedListItem {
    
public:
    
    fakGrabbedListItem(){}
    
    // Functions
    void registerMouseEvents(){
        ofRegisterMouseEvents(this);
    }
    
    void unregisterMouseEvents(){
        ofUnregisterMouseEvents(this);
    }
    
    void registerKeyEvents(){
        ofRegisterKeyEvents(this);
    }
    
    void unregisterKeyEvents(){
        ofUnregisterKeyEvents(this);
    }
    
    void keyPressed(ofKeyEventArgs & args){
        switch (args.key)
        {
            case OF_KEY_DOWN:
            {
                if (gliActiveID < gliSize-1) {
                    gliActiveID++;
                    
                } else {
                    gliActiveID = 0;
                }
            }
                break;
                
            case OF_KEY_UP:
            {
                if (gliActiveID > 0) {
                    gliActiveID--;
                    
                } else {
                    gliActiveID = gliSize-1;
                }
                
            }
                break;
                
            case OF_KEY_RETURN:
            {
                if (gliActiveID == gliID) {
                    ofNotifyEvent(glClickedInside, gliID, this);
                    ofLog(OF_LOG_VERBOSE, "Pressed Enter glID" + ofToString(gliID));
                }
            }
                break;
            default:
                break;
        }
        
    }
    
    void keyReleased(ofKeyEventArgs & args){}
    
    void mouseMoved(ofMouseEventArgs & args){
        
        if (inside(args.x, args.y)) {
            ofNotifyEvent(glRolledOver, gliID, this);
        }
    }
    
    void mouseDragged(ofMouseEventArgs & args){}
    
    void mouseReleased(ofMouseEventArgs & args){}
    
    void mousePressed(ofMouseEventArgs & args){
        if (inside(args.x, args.y)) {
            ofNotifyEvent(glClickedInside, gliID, this);
        }
    }
    
    void setup(int _glID, int _glSize){
        gliFontStash.setup("Ubuntu-Light.ttf", 1.03);
        itemProperties.ipName = gliFile.getFileName();
        ofLog(OF_LOG_VERBOSE, itemProperties.ipName);
        itemProperties.ipID = _glID;
        itemProperties.ipPrinted = FALSE;
        itemProperties.ipTriedToPrint = FALSE;
        gliID = _glID;
        gliX = 100;
        gliY = 100 + gliID * (gliHeight + gliPadding);
        gliActiveID = 0;
        gliSize = _glSize;
    }
    
    bool inside(float _x, float _y ){ //function to check if mouse is inside grabbedStill
        return _x >= gliX && _x < gliX + gliWidth && _y >= gliY && _y < gliY + gliHeight && _y > gliUpperLimitY && _y < gliLowerLimitY && _x > gliLeftLimitX && _x < gliRightLimitX;
    }
    
    void draw(float _x, float _y, float _w){

        float tempSize = 14;
        
        ofPushStyle();
        
        gliX = _x;
        gliY = _y;
        gliWidth = _w;
        gliNameWidth = _w - gliIDWidth - gliPrintedWidth;
        
        if (gliActiveID == gliID) {
            gliActive = TRUE;
        } else {
            gliActive = FALSE;
        }
        if (gliActive) {
            ofSetColor(FAK_ORANGECOLOR);
        } else if(itemProperties.ipPrinted){
            ofSetColor(FAK_MIDDLEDARKORANGECOLOR);
        } else if(itemProperties.ipTriedToPrint && !itemProperties.ipPrinted){
            ofSetColor(0,0,255,255);
        } else {
            if(itemProperties.ipID%2==0){
                ofSetColor(30, 30, 30, 255);
            }else{
                ofSetColor(35, 35, 35, 255);
            }
        }
        ofRectRounded(gliX, gliY, gliIDWidth-1, gliHeight, gliHeight/8);
        ofRectRounded(gliX + gliIDWidth, gliY, gliNameWidth-1, gliHeight, gliHeight/8);
//        ofRectRounded(gliX + gliIDWidth + gliNameWidth, gliY, gliPrintedWidth-1, gliHeight, gliHeight/8);
        ofRectRounded(gliX + gliIDWidth + gliNameWidth, gliY, gliPrintedWidth-1, gliHeight, gliHeight/8);
        ofSetColor(255,255,255,255);
        gliFontStash.drawMultiLine(ofToString(itemProperties.ipID), tempSize, gliX + 2, gliY + gliHeight -5);
        gliFontStash.drawMultiLine(itemProperties.ipName, tempSize, gliX + gliIDWidth + 2, gliY+gliHeight-5);
        gliFontStash.drawMultiLine(ofToString(itemProperties.ipTriedToPrint), tempSize, gliX + gliIDWidth + gliNameWidth+2, gliY+gliHeight-5);
        gliFontStash.drawMultiLine(ofToString(itemProperties.ipPrinted), tempSize, gliX + gliIDWidth + gliNameWidth +2, gliY+gliHeight-5);
        
        ofPopStyle();
    }
    
    void setActive(){
        gliActive = TRUE;
    }
    
    void setInactive(){
        gliActive = FALSE;
    }
    
    // Properties
    ofEvent<int> glRolledOver;
    ofEvent<int> glClickedInside;
    
    ofFile gliFile;
    
    fakItemProperties itemProperties;
    
    ofxFontStash gliFontStash;
    
    int gliUpperLimitY; // Lower Limit for turning off mouseevent testing if over the header
    int gliLowerLimitY; // Higher Limit for turning off mouseevent testing if over the footer
    int gliLeftLimitX; // Left Limit for turning off mouseevent testing if over the left menu
    int gliRightLimitX; // Right Limit for turning off mouseevent testing if over the right scrollbar
  
    int gliHeight = 20;
    int gliWidth = 700;
    int gliIDWidth = 25;
    int gliNameWidth = 625;
    int gliPrintedWidth = 25;
    int gliTriedToPrintWidth = 25;
    int gliPadding = 2;
    int gliX;
    int gliY;
    int gliID;
    bool gliActive;
    int gliActiveID;
    int gliSize;
    
};



#endif
