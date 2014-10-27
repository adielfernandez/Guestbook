#pragma once

#include "ofMain.h"
#include "ofxSpacebrew.h"
#include "TreeLeaf.h"
#include "Cloud.h"
#include "LetterLeaf.h"
#include "WindField.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void setSummer();
    void setFall();
    
    //debug visuals
    bool debugVisuals;
    void drawDebugVis();
    bool drawWind;

    void stringToLeafList(string message);
		
    //Spacebrew Stuff
    Spacebrew::Connection spacebrew;
    void onMessage( Spacebrew::Message & msg );
    
    //Writing to file
    string fileName;
    ofBuffer messageLog;

    
    vector<string> messageList;
    vector<string> senderList;
    vector<bool> checked;
    
    
    bool animating;
    int currentAnimation;
    int animationTime;
    int waitTimer;
    
    //Environment Stuff
    ofImage logo;
    ofImage backgroundSummer;
    ofImage backgroundFall;
    ofImage leafMask;
    ofImage sky;
    
    float foregroundTrans;
    float foregroundGoalTrans;
    
    float seasonChangeSpeed;
    
    ofColor summerCol1, summerCol2;
    ofColor fallCol1, fallCol2;
    
    
    ofColor summerSky, fallSky;
    ofColor currentSky, goalSky;
    
    vector<ofImage> treeLeafSprites;
    vector<ofImage> letterLeafSprites;
    vector<TreeLeaf> leafList;
    
    vector<LetterLeaf> letterList;
    int releaseTimer;
    int endTime;
    int leavesToRelease;
    bool makeFall;
    
    vector<ofImage> cloudSprites;
    vector<Cloud> cloudList;
    
    //Wind field
    WindField wind;
    
    
    bool isItSummer;
    
    //Font stuff
    ofTrueTypeFont font;
    ofTrueTypeFont UIfont;

    
    //Where to form messages on screen
    ofPoint startPos;
    
    

    bool use16x9;
    
};
