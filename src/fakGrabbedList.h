//
//  fakGrabbedList.h
//  fakBreakupVideo
//
//  Created by Jakob on 3/15/13.
//
//

#ifndef fakBreakupVideo_fakGrabbedList_h
#define fakBreakupVideo_fakGrabbedList_h


#define		IDLE_COLOR		0xFFFFFF
#define		OVER_COLOR		0x00FF00
#define		DOWN_COLOR		0xFF0000

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

class fakGrabbedList {
    
public:
    
    fakGrabbedList(){
        glDroppedItemDown.setup(1, 0);
        glDroppedItemUp.setup(2, 0);
        glDroppedItemDown.gliWidth = glListItemWidth;
        glDroppedItemDown.gliHeight = glListItemHeight;
        glDroppedItemDown.gliPadding = glListItemPadding;
        glDroppedItemDown.gliX = glX;
        glDroppedItemDown.gliY = glY + glMaxHeight * (glListItemHeight + glListItemPadding);
        glDroppedItemUp.gliWidth = glListItemWidth;
        glDroppedItemUp.gliHeight = glListItemHeight;
        glDroppedItemUp.gliPadding = glListItemPadding;
        glDroppedItemUp.gliX = glX;
        glDroppedItemUp.gliY = glY - glDroppedItemUp.gliHeight;
    }
    
    // Functions
    
    void disableMouseEvents(int _oldDroppedListSize){
        if (_oldDroppedListSize > glDroppedItem.size()) {
            _oldDroppedListSize = glDroppedItem.size();
        }
        for (int i=0; i<_oldDroppedListSize; i++) {
            glDroppedItem[i].unregisterMouseEvents();
            glDroppedItem[i].unregisterKeyEvents();
            ofRemoveListener(glDroppedItem[i].glRolledOver, this, &fakGrabbedList::rolledOver);
            ofRemoveListener(glDroppedItem[i].glClickedInside, this, &fakGrabbedList::listClick);
        }
    }
    
    void enableMouseEvents(){
        for (int i=0; i<glDroppedItem.size(); i++) {
            glDroppedItem[i].registerMouseEvents();
            glDroppedItem[i].registerKeyEvents();
            ofAddListener(glDroppedItem[i].glRolledOver, this, &fakGrabbedList::rolledOver);
            ofAddListener(glDroppedItem[i].glClickedInside, this, &fakGrabbedList::listClick);
            ofLog(OF_LOG_VERBOSE, "ofAddListener:" + ofToString(i));
        }
    }
    
    void setup(vector<ofFile> _droppedFiles){
        glFontStash.setup("HelveticaNeueLTCom-Lt.ttf", 1.03);
        scrollValue = glMaxHeight;
        glDroppedItem.clear();
        glDroppedItem.resize(_droppedFiles.size());
        for (int i=0; i<_droppedFiles.size(); i++) {
            glDroppedItem[i].gliFile = _droppedFiles[i];
            glDroppedItem[i].setup(i, _droppedFiles.size());
            glDroppedItem[i].gliWidth = glListItemWidth;
            glDroppedItem[i].gliHeight = glListItemHeight;
            glDroppedItem[i].gliPadding = glListItemPadding;
        }
        setAllLimits(glUpperLimitY, glLowerLimitY, glLeftLimitX, glRightLimitX);
        enableMouseEvents();
    }
    
    float getListHeight(){
        float tempHeight = 0;
        for (int i=0; i<glDroppedItem.size(); i++) {
            tempHeight = tempHeight + glDroppedItem[i].gliHeight + glDroppedItem[i].gliPadding;
        }
        tempHeight = tempHeight + glListItemHeight + glListItemPadding*4;
        return tempHeight;
    }
    
    void scroll(){
        for (int i=0; i<glDroppedItem.size(); i++) {
            glDroppedItem[i].gliWidth = 0;
            glDroppedItem[i].gliHeight = 0;
            glDroppedItem[i].gliX = 0;
            glDroppedItem[i].gliY = 0;
        }
        
        int tempID = 0;
        
        for (int k = (scrollValue - glMaxHeight); k < scrollValue; k++){
            glDroppedItem[k].gliWidth = glListItemWidth;
            glDroppedItem[k].gliHeight = glListItemHeight;
            glDroppedItem[k].gliX = glX;
            glDroppedItem[k].gliY = glY + tempID * (glDroppedItem[k].gliHeight + glDroppedItem[k].gliPadding);
            tempID++;
        }

    }
    
    void draw(float _x, float _y, float _width, float _scrollAmount){
        
        glX = _x;
        glY = _y;

        glListItemWidth = _width;
        
        ofPushStyle();
        
        float tempSize = 14;
        ofSetColor(FAK_GRAY);
        ofRectRounded(_x, _y + _scrollAmount, _width, glDroppedItem[0].gliHeight, glDroppedItem[0].gliHeight/8);
        ofSetColor(255);

        glFontStash.drawMultiLine("ID", tempSize, _x + 2, _y + _scrollAmount + tempSize);
        glFontStash.drawMultiLine("Name", tempSize, _x + glDroppedItem[0].gliIDWidth + 2, _y + _scrollAmount + tempSize);
//        glFontStash.drawMultiLine("Tried", tempSize, _x + glDroppedItem[0].gliIDWidth + glDroppedItem[0].gliNameWidth - 5, _y + _scrollAmount + tempSize);
        glFontStash.drawMultiLine("Printed", tempSize, _x + glDroppedItem[0].gliIDWidth + glDroppedItem[0].gliNameWidth + 2 - glFontStash.getBBox("Printed", tempSize,0,0).getMaxY(), _y + _scrollAmount + tempSize);

        for(int k = 0; k < glDroppedItem.size(); k++){
            glDroppedItem[k].gliX = _x;
            glDroppedItem[k].gliY = _y + _scrollAmount + (glDroppedItem[k].gliHeight + glDroppedItem[k].gliPadding) * (k + 1);
            glDroppedItem[k].draw(glDroppedItem[k].gliX, glDroppedItem[k].gliY, _width);
        }
        ofPopStyle();

    }
    
    void setAllLimitsUpper(int _upperLimit){
        glUpperLimitY = _upperLimit;
        for (int i=0; i<glDroppedItem.size(); i++) {
            glDroppedItem[i].gliUpperLimitY = glUpperLimitY;
        }
    }
    
    void setAllLimits(int _upperLimit, int _LowerLimit, int _leftLimit, int _rightLimit){
        glUpperLimitY = _upperLimit;
        for (int i=0; i<glDroppedItem.size(); i++) {
            glDroppedItem[i].gliUpperLimitY = glUpperLimitY;
            glDroppedItem[i].gliLowerLimitY = glLowerLimitY;
            glDroppedItem[i].gliLeftLimitX = glLeftLimitX;
            glDroppedItem[i].gliRightLimitX = glRightLimitX;
        }
    }

    void setAllLimitsLower(int _LowerLimit){
        glLowerLimitY = _LowerLimit;
        for (int i=0; i<glDroppedItem.size(); i++) {
            glDroppedItem[i].gliLowerLimitY = glLowerLimitY;
        }
    }
    
    void setAllLimitsLeft(int _leftLimit){
        glLeftLimitX = _leftLimit;
        for (int i=0; i<glDroppedItem.size(); i++) {
            glDroppedItem[i].gliLeftLimitX = glLeftLimitX;
        }
    }
    
    void setAllLimitsRight(int _rightLimit){
        glRightLimitX = _rightLimit;
        for (int i=0; i<glDroppedItem.size(); i++) {
            glDroppedItem[i].gliRightLimitX = glRightLimitX;
        }
    }
    
    void listClick(int & j){
        
        glActiveID = j;
        glUpdateMovieFromList = TRUE;
        setActiveItem(j);

    }
    
    void rolledOver(int & j){
        
        setActiveItem(j);
//        ofLog(OF_LOG_VERBOSE, "called rollover function after hit list item:" + ofToString(j));
        
    }
    
    void setActiveItem(int j){
        
        for (int i=0; i<glDroppedItem.size(); i++) {
            glDroppedItem[i].setInactive();
            glDroppedItem[i].gliActiveID = j;
        }
        glDroppedItem[j].setActive();
    }
    
    // Properties
    vector<fakGrabbedListItem> glDroppedItem;
    fakGrabbedListItem glDroppedItemUp;
    fakGrabbedListItem glDroppedItemDown;
    
    ofxFontStash glFontStash;
    
    int glUpperLimitY;
    int glLowerLimitY;
    int glLeftLimitX;
    int glRightLimitX;
    
    int glActiveID;
    bool glUpdateMovieFromList = FALSE;
    int glMaxHeight = 20;
    int scrollValue;
    int glListItemWidth = 700;
    int glListItemHeight = 20;
    int glListItemPadding = 6;
    int glX = 100;
    int glY = 100;

    
};


#endif
