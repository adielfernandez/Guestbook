//
//  Bike.h
//  Guestbook_App_v2
//
//  Created by Adiel Fernandez on 9/29/14.
//
//

#ifndef __Guestbook_App_v2__Bike__
#define __Guestbook_App_v2__Bike__

#include <iostream>

#endif /* defined(__Guestbook_App_v2__Bike__) */
#include "ofMain.h"
#include "Dust.h"
#include "LetterDust.h"
#pragma once


class Bike{
    
public:
    
    Bike();
    void setup(ofImage *_bike, ofImage *_wheel, ofImage *_dustSprite);
    
    void loadLetterDust(vector<LetterDust> *_letterDust);

    void update();
    void draw();

    ofImage *bike;
    ofImage *wheel;
    ofImage *dustSprite;
    ofColor col;
    
    
    ofVec3f pos, vel, acc;
    float groundLevel;

    float wheelAngle, wheelSpeed;
    float bikePitch;

    
    string word;
    ofVec3f wordPos;

    float scale;
    bool flip;
    
    vector<LetterDust> *letterDust;
    vector<Dust> dustList;
    
    
    bool drawBehind;
    int dustReleaseTimer;
    
    //segment controls which leg
    //of the trip:
    //0 = right to left
    //1 = left to right
    //2 = righ to left and letter release
    int segment;
    
    int dustToRelease;
    bool letterFall;
    int endTime;
    bool done;
    
    bool animateBike;
    
    
};