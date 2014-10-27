#pragma once

#include "ofMain.h"
#include "ofxSpacebrew.h"
#include "TreeLeaf.h"
#include "Cloud.h"
#include "LetterLeaf.h"
#include "WindField.h"
#include "Bike.h"
#include "Dust.h"
#include "LetterDust.h"

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
    

    
    //debug visuals
    bool debugVisuals;
    void drawDebugVis();
    bool drawWind;
    bool use16x9;
	bool scaleToSmall;
    
    
    //Spacebrew Stuff
    Spacebrew::Connection spacebrew;
    void onMessage( Spacebrew::Message & msg );
    int connectionTimer;
    
    //Writing to file
    string fileName;
    ofBuffer messageLog;

    //Font stuff
    ofTrueTypeFont font;
    ofTrueTypeFont UIfont;
    
    
    //Where to form messages on screen
    ofPoint startPos;
    
    vector<string> messageList;
    vector<string> senderList;
    vector<bool> checked;
    
    //Environment Stuff
    ofImage logo;
    ofImage backgroundSummer;
    ofImage backgroundFall;
    ofImage sky;

    //which animation to use
    //0 = Tree leaves
    //1 = Bike dust
    //2 = Birds         (NOT YET IMPLEMENTED)
    int animationType;
    
    bool animating;
    int currentMessageShowing;
    int animationTime;
    int waitTimer;
    
    
    //---------- LEAVES ----------
    void setSummer();
    void setFall();
    bool isItSummer;
    
    //sprites
    vector<ofImage> treeLeafSprites;
    vector<ofImage> letterLeafSprites;
    
    vector<TreeLeaf> leafList;
    vector<LetterLeaf> letterList;

    ofImage leafMask;
    
    float foregroundTrans;
    float foregroundGoalTrans;
    float seasonChangeSpeed;
    
    ofColor summerCol1, summerCol2;
    ofColor fallCol1, fallCol2;
    ofColor summerSky, fallSky;
    ofColor currentSky, goalSky;
    
    int releaseTimer;
    int endTime;
    int leavesToRelease;
    bool letterFall;
    
    vector<ofImage> cloudSprites;
    vector<Cloud> cloudList;

    //---------- BIKES ----------
    ofImage bike;
    ofImage wheel;
    ofImage dustSprite;
    
    float groundLevel;
    

    Bike messengerBike;
    vector<LetterDust> _letterDust;
    
    
    
    
    //---------- Not Used ----------

    //Wind field
    WindField wind;
    
    

    

    
    


    


    
    
    
};
