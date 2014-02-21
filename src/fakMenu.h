//
//  fakMenu.h
//  MoviePrint
//
//  Created by fakob on 14/02/21.
//
//

#ifndef MoviePrint_fakMenu_h
#define MoviePrint_fakMenu_h

#include "ofMain.h"
#include "ofxTween.h"
#include "ofxUI.h"

#define FAK_ORANGECOLOR ofColor(238, 71, 0, 255)
#define FAK_DARKORANGECOLOR ofColor(99, 30, 0, 255)
#define FAK_DARKDARKORANGECOLOR ofColor(75, 21, 0, 255)
#define FAK_MIDDLEDARKORANGECOLOR ofColor(170, 50, 0, 255)
#define FAK_LIGHTERMIDDLEDARKORANGECOLOR ofColor(185, 55, 0, 255)

#define FAK_GREENCOLOR ofColor(117, 130, 16, 255)
#define FAK_LIGHTGRAY ofColor(205, 205, 205, 255)
#define FAK_MIDDLEGRAY ofColor(195, 195, 195, 255)

class fakMenu{
    
public:
    
    fakMenu(){

    }
    
    // functions
    
    void setupMenu(float _mMenuX, float _mMenuY, float _mMenuWidth, float _mMenuHeight, float _mMenuRollOverHeight){
        mMenuX = _mMenuX;
        mMenuY = _mMenuY;
        mMenuWidth = _mMenuWidth;
        mMenuHeight = _mMenuHeight;
        mMenuRollOverHeight = _mMenuRollOverHeight;
        tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,1000,0);
        
        mInsideMenuHead = FALSE;
    }
    
    bool insideMenuHead(float _x, float _y ){
        float minX = mMenuX;
        float maxX = mMenuX + mMenuWidth;
        float minY = mMenuY;
        float maxY = mMenuY + mMenuRollOverHeight;
        return _x >= minX && _x < maxX && _y >= minY && _y < maxY;
    }

    bool insideMenu(float _x, float _y ){
        float minX = mMenuX;
        float maxX = mMenuX + mMenuWidth;
        float minY = mMenuY;
        float maxY = mMenuY + mMenuHeight;
        return _x >= minX && _x < maxX && _y >= minY && _y < maxY;
    }
    
    void mouseMoved(ofMouseEventArgs & args){
        if (mInsideMenuHead) {
            if (!insideMenu(args.x, args.y)) {
                mInsideMenuHead = false;
                tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,500,0);
            }
        } else {
            if (insideMenuHead(args.x, args.y)) {
                mInsideMenuHead = TRUE;
                tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,500,0);
            }
        }
    }
    
    void mouseDragged(ofMouseEventArgs & args){
    }
    
    void mouseReleased(ofMouseEventArgs & args){
    }
    
    void mousePressed(ofMouseEventArgs & args){
    }
    
    void mouseScrolled(ofMouseEventArgs & args){
    }

    void registerMouseEvents(){
        ofRegisterMouseEvents(this);
    }
    
    void unRegisterMouseEvents(){
        ofUnregisterMouseEvents(this);
    }
    
    void setPosition(float _mMenuX, float _mMenuY){
        mMenuX = _mMenuX;
        mMenuY = _mMenuY;
    }
    
	void setSize(float _mMenuWidth, float _mMenuHeight){
        mMenuWidth = _mMenuWidth;
        mMenuHeight = _mMenuHeight;
    }
    
    float getPositionX(){
        return mMenuX;
    }
    
    float getPositionY(){
        return mMenuY;
    }
    
	float getSizeW(){
        return mMenuWidth;
    }
    
	float getSizeH(){
        return mMenuRollOverHeight + (mMenuHeight - mMenuRollOverHeight) * tweenMenuInOut.update();
    }
    
    void updateMenu(){

    }
    
    void drawMenu(){
        
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(FAK_GREENCOLOR);
        if (mInsideMenuHead) {
            ofRect(mMenuX, mMenuY, mMenuWidth, mMenuRollOverHeight + (mMenuHeight - mMenuRollOverHeight) * tweenMenuInOut.update());
        } else {
            ofRect(mMenuX, mMenuY, mMenuWidth, mMenuRollOverHeight + (mMenuHeight - mMenuRollOverHeight) * tweenMenuInOut.update());
        }
        ofPopStyle();
    }
    

    
    // Properties
    
    bool mInsideMenuHead;
    
    float mMenuX;
    float mMenuY;
    float mMenuWidth;
    float mMenuHeight;
    float mMenuRollOverHeight;
    
    
    ofxTween tweenMenuInOut;
    
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

};

#endif /* defined(__fakBreakupVideo__fakGrabbedMovie__) */
