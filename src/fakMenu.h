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

#define FAK_ORANGE1 ofColor(255, 80, 6, 255)
#define FAK_ORANGE2 ofColor(255, 183, 153, 255)
#define FAK_ORANGE3 ofColor(255, 147, 101, 255)
#define FAK_ORANGE4 ofColor(255, 168, 131, 255)
#define FAK_ORANGE5 ofColor(255, 211, 193, 255)

#define FAK_WHITE ofColor(255, 255, 255, 255)
#define FAK_BLACK ofColor(0, 0, 0, 255)
#define FAK_SHADOW ofColor(0, 0, 0, 32)


#define FAK_GREENCOLOR ofColor(117, 130, 16, 255)
#define FAK_LIGHTGRAY ofColor(205, 205, 205, 255)
#define FAK_MIDDLEGRAY ofColor(195, 195, 195, 255)

class fakMenu{
    
public:
    
    fakMenu(){

    }
    
    // functions
    
    void setupMenu(int _ID, float _mMenuX, float _mMenuY, float _mMenuWidth, float _mMenuHeight, float _mMenuRollOverHeight, bool _mButtonActive){
        mMenuX = _mMenuX;
        mMenuY = _mMenuY;
        mMenuWidth = _mMenuWidth;
        mMenuHeight = _mMenuHeight;
        mMenuRollOverHeight = _mMenuRollOverHeight;
        tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,1000,0);
        mMenuOffsetX = 2;
        mMenuOffsetY = 28;
        mShadowMargin = 1.5;
        mMenuStripeHeight = 8;
        mInsideMenuHead = FALSE;
        mButtonActive = _mButtonActive;
        
        
        switch (_ID) {
            case 1:
                mMenuImage.loadImage("MoviePrint_Layout_Menu1_v001_00001.png");
                mBackgroundColor = FAK_ORANGE1;
                break;
            case 2:
                mMenuImage.loadImage("MoviePrint_Layout_Menu2_v001_00001.png");
                mBackgroundColor = FAK_ORANGE2;
                break;
            case 3:
                mMenuImage.loadImage("MoviePrint_Layout_Menu3_v001_00001.png");
                mBackgroundColor = FAK_ORANGE3;
                break;
            case 4:
                mMenuImage.loadImage("MoviePrint_Layout_Menu4_v001_00001.png");
                mBackgroundColor = FAK_ORANGE4;
                break;
            case 5:
                mMenuImage.loadImage("MoviePrint_Layout_Menu5_v001_00001.png");
                mBackgroundColor = FAK_ORANGE5;
                break;
            default:
                break;
        }
        
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
        if (mButtonActive) {
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
        ofSetColor(FAK_WHITE);
        mMenuImage.draw(mMenuX - mMenuOffsetX, mMenuY, mMenuImage.getWidth(), mMenuImage.getHeight());
        
        ofPushStyle();
        ofSetColor(FAK_SHADOW);
//        ofRectRounded(mMenuX - mShadowMargin*0.3 + mMenuOffsetX, mMenuY + mMenuOffsetY, mMenuWidth - mMenuOffsetX + mShadowMargin*2, mMenuStripeHeight + mMenuHeight * tweenMenuInOut.update(),4.0 * tweenMenuInOut.update());
        ofSetColor(mBackgroundColor);
        ofRectRounded(mMenuX, mMenuY + mMenuOffsetY, mMenuWidth, mMenuStripeHeight + mMenuHeight * tweenMenuInOut.update(), 4.0 * tweenMenuInOut.update());
        ofPopStyle();

        ofPopStyle();
    }
    

    
    // Properties
    
    bool mInsideMenuHead;
    bool mButtonActive;
    
    float mMenuX;
    float mMenuY;
    float mMenuWidth;
    float mMenuHeight;
    float mMenuRollOverHeight;
    int mMenuOffsetX;
    int mMenuOffsetY;
    int mMenuStripeHeight;
    float mShadowMargin;
    ofImage mMenuImage;
    ofColor mBackgroundColor;
    
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
