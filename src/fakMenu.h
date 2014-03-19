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
#define FAK_SHADOW ofColor(0, 0, 0, 130)
#define FAK_GRAY ofColor(59, 59, 59, 255)


#define FAK_GREENCOLOR ofColor(117, 130, 16, 255)
#define FAK_LIGHTGRAY ofColor(205, 205, 205, 255)
#define FAK_MIDDLEGRAY ofColor(195, 195, 195, 255)

class fakMenu{
    
public:
    
    fakMenu(){

    }
    
    // functions
    
    void setupMenu(int _ID, float _mMenuX, float _mMenuY, float _mMenuWidth, float _mMenuHeight, float _mMenuRollOverDimension, bool _mButtonActive, char _mLocationOfMenu, bool _mNoBackground){
        mMenuX = _mMenuX;
        mMenuY = _mMenuY;
        mMenuWidth = _mMenuWidth;
        mMenuHeight = _mMenuHeight;
        mMenuRollOverDimension = _mMenuRollOverDimension;
        tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));
        tweenMenuInOut2.setParameters(1,easingelastic,ofxTween::easeOut,1.0,0.0,ofRandom(600, 1000),ofRandom(0, 300));
        mMenuOffsetX = 2;
        mMenuOffsetY = 28;
        mShadowMargin = 1.5;
        mMenuStripeHeight = 8;
        mInsideMenuHead = FALSE;
        mMenuActive = _mButtonActive;
        mLocationOfMenu = _mLocationOfMenu;
        mNoBackground = _mNoBackground;
        mIsOpenManually = false;
        mMenuID = _ID;
        
        switch (mMenuID) {
            case 1:
                mMenuImage.loadImage("MoviePrint_Layout_Menu1_v001_00001.png");
                mBackgroundColor = FAK_ORANGE1;
                break;
            case 4:
                mMenuImage.loadImage("MoviePrint_Layout_Menu2_v001_00001.png");
                mBackgroundColor = FAK_ORANGE4;
                break;
            case 3:
                mMenuImage.loadImage("MoviePrint_Layout_Menu3_v001_00001.png");
                mBackgroundColor = FAK_ORANGE3;
                break;
            case 2:
                mMenuImage.loadImage("MoviePrint_Layout_Menu4_v001_00001.png");
                mBackgroundColor = FAK_ORANGE2;
                break;
            case 5:
                mMenuImage.loadImage("MoviePrint_Layout_Menu5_v001_00001.png");
                mBackgroundColor = FAK_ORANGE5;
                break;
            case 6:
                mMenuImage.loadImage("MoviePrint_Layout_Menu6_v001_00001.png");
                mBackgroundColor = FAK_ORANGE3;
                break;
            default:
                mBackgroundColor = FAK_GRAY;
                break;
        }
        
    }
    
    void setTweenIn(){
        tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,0.0,1.0,500,0);
        tweenMenuInOut2.setParameters(1,easingexpo,ofxTween::easeOut,0.0,1.0,300,0);
        ofNotifyEvent(mMenuIsBeingOpened, mMenuID, this);
    }
    
    void setTweenOut(){
        tweenMenuInOut.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,500,0);
        tweenMenuInOut2.setParameters(1,easingexpo,ofxTween::easeInOut,1.0,0.0,300,0);
        ofNotifyEvent(mMenuIsBeingClosed, mMenuID, this);
    }
    
    bool insideMenuHead(float _x, float _y ){
        float minX = mMenuX;
        float maxX = mMenuX + mMenuWidth;
        float minY, maxY;
        if (mLocationOfMenu == 'T') {
            minY = mMenuY;
            maxY = mMenuY + mMenuRollOverDimension;
        } else if (mLocationOfMenu == 'B') {
            minY = mMenuY - mMenuRollOverDimension;
            maxY = mMenuY;
        } else if (mLocationOfMenu == 'L') {
            minX = mMenuX;
            maxX = mMenuX + mMenuRollOverDimension;
            minY = mMenuY;
            maxY = mMenuY + mMenuHeight;
        }
        return _x >= minX && _x < maxX && _y >= minY && _y < maxY;
    }

    bool insideMenu(float _x, float _y ){
        float minX = mMenuX;
        float maxX = mMenuX + mMenuWidth;
        float minY, maxY;
        if (mLocationOfMenu == 'T') {
            minY = mMenuY;
            maxY = mMenuY + mMenuHeight;
        } else if (mLocationOfMenu == 'B') {
            minY = mMenuY - mMenuHeight - mMenuRollOverDimension;
            maxY = mMenuY;
        } else if (mLocationOfMenu == 'L') {
            minX = mMenuX;
            maxX = mMenuX + mMenuRollOverDimension + mMenuWidth;
            minY = mMenuY;
            maxY = mMenuY + mMenuHeight;
        }
        return _x >= minX && _x < maxX && _y >= minY && _y < maxY;
    }
    
    void openMenuManually(){
        mIsOpenManually = true;
        if (tweenMenuInOut.update() != 1.0){
            setTweenIn();
        }
    }
    
    void closeMenuManually(){
        mIsOpenManually = false;
        if (tweenMenuInOut.update() != 0.0){
            setTweenOut();
        }
    }
    
    void mouseMoved(ofMouseEventArgs & args){
        if (mMenuActive) {
            if (mInsideMenuHead) {
                if (!insideMenu(args.x, args.y)) {
                    mInsideMenuHead = false;
                    setTweenOut();
                }
            } else {
                if (insideMenuHead(args.x, args.y)) {
                    mInsideMenuHead = TRUE;
                    setTweenIn();
                }
            }
        }
    }
    
    void mouseDragged(ofMouseEventArgs & args){
    }
    
    void mouseReleased(ofMouseEventArgs & args){
    }
    
    void mousePressed(ofMouseEventArgs & args){
        if (mInsideMenuHead) {
            ofNotifyEvent(mMenuIsBeingClicked, mMenuID, this);
        }
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
        return mMenuRollOverDimension + (mMenuHeight - mMenuRollOverDimension) * tweenMenuInOut.update();
    }
    
	float getRelSizeH(){
        return tweenMenuInOut.update();
    }

    float getRelSizeH2(){
        return tweenMenuInOut2.update();
    }
    
    void updateMenu(){

    }
    
    bool getMenuActivated(){
        return mInsideMenuHead || mIsOpenManually;
    }

    
    void setMenuActive(){
        mMenuActive = true;
    }
    
    void setMenuInactive(){
        closeMenuManually();
        mMenuActive = false;
    }
    
    bool getInsideMenuHead(){
        return mInsideMenuHead;
    }
    
    void drawMenu(){
        ofPushMatrix();
        ofPushStyle();
        if (mLocationOfMenu == 'T') {
            ofEnableAlphaBlending();
            ofSetColor(FAK_WHITE);
            mMenuImage.draw(mMenuX - mMenuOffsetX, mMenuY, mMenuImage.getWidth(), mMenuImage.getHeight());
//            ofSetColor(FAK_SHADOW);
//            ofRectRounded(mMenuX - mShadowMargin*0.3 + mMenuOffsetX, mMenuY + mMenuOffsetY, mMenuWidth - mMenuOffsetX + mShadowMargin*2, mMenuStripeHeight + mMenuHeight * tweenMenuInOut.update(),4.0 * tweenMenuInOut.update());
            if (!mNoBackground) {
                ofColor tempColor = mBackgroundColor;
                ofSetColor(tempColor.lerp(FAK_ORANGE1, tweenMenuInOut.update()));
                ofRectRounded(mMenuX, mMenuY + mMenuOffsetY, mMenuWidth, mMenuStripeHeight + (mMenuHeight - mMenuOffsetY - mMenuStripeHeight) * tweenMenuInOut.update(), 4.0 * tweenMenuInOut.update());
            } else {
                ofSetColor(mBackgroundColor);
                ofRect(mMenuX, mMenuY + mMenuOffsetY, mMenuWidth, mMenuStripeHeight);
            }
        } else if (mLocationOfMenu == 'B') {
            ofEnableAlphaBlending();
            ofSetColor(FAK_SHADOW);
            ofRect(mMenuX, mMenuY - (mMenuRollOverDimension + 3.4 + mMenuHeight * tweenMenuInOut.update()), mMenuWidth, mMenuRollOverDimension + mMenuHeight * tweenMenuInOut.update());
            ofSetColor(mBackgroundColor);
            ofRect(mMenuX, mMenuY - (mMenuRollOverDimension + mMenuHeight * tweenMenuInOut.update()), mMenuWidth, mMenuRollOverDimension + mMenuHeight * tweenMenuInOut.update());
        } else if (mLocationOfMenu == 'L') {
            ofEnableAlphaBlending();
            ofColor tempColor = mBackgroundColor;
//            tempColor.set(tempColor.r, tempColor.g, tempColor.b, ((sin(ofGetElapsedTimef()*3)+1.0)/2.0)*200+30); //pulsating
            ofSetColor(tempColor.lerp(FAK_ORANGE3, tweenMenuInOut.update()));
            ofRect(mMenuX, mMenuY, mMenuRollOverDimension + mMenuWidth * tweenMenuInOut.update(), mMenuHeight);
            ofSetColor(255, 255, 255);
            mMenuImage.draw(mMenuX - mMenuRollOverDimension/2.0 + mMenuWidth * tweenMenuInOut.update()/2.0, mMenuY);
        }
        ofPopStyle();
        ofPopMatrix();
    }
    

    
    // Properties
    
    ofEvent<int> mMenuIsBeingOpened;
    ofEvent<int> mMenuIsBeingClosed;
    ofEvent<int> mMenuIsBeingClicked;

    int mMenuID;
    bool mInsideMenuHead;
    bool mMenuActive;
    char mLocationOfMenu; // 'T' for Top, 'B' for Bottom, 'L' for Left, 'R' for Right
    bool mIsOpenManually;
    bool mNoBackground;
    
    float mMenuX;
    float mMenuY;
    float mMenuWidth;
    float mMenuHeight;
    float mMenuRollOverDimension;
    int mMenuOffsetX;
    int mMenuOffsetY;
    int mMenuStripeHeight; //stripe of menu which is already visible
    float mShadowMargin;
    ofImage mMenuImage;
    ofColor mBackgroundColor;
    
    ofxTween tweenMenuInOut;
    ofxTween tweenMenuInOut2;
    
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
