//
//  LetterDust.h
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/28/14.
//
//

#ifndef __Guestbook_App_v1__Dust__
#define __Guestbook_App_v1__Dust__

#include <iostream>

#endif /* defined(__Guestbook_App_v1__Dust__) */
#include "ofMain.h"

#pragma once

class LetterDust{
    
public:
    LetterDust(ofImage *sprite, ofTrueTypeFont *font, string _word);
    
    void update();
    void attractToLetterPos();
    void draw();
    
    ofTrueTypeFont *font;
    ofImage *sprite;
    
    ofVec3f pos, vel, acc;
    ofVec3f finalLocation;
    ofVec3f letterPos;

    int age;
    int lifeSpan;
    
    float attractionStrength;
    
    string thisLetter;
    
    float damping;
    float currentRot, goalRot;
    float rotSpeed;
    
    float scale;
    float noiseRand;
    float turnAmt;
    float amplitude;
    
    ofColor color;
    ofColor textCol;
    ofColor col1, col2;
    float trans;
    float textTrans;

    
    bool hit;
    bool attract;
    bool fall;
    bool persist;
    bool released;
    
    ofVec3f textAccel;
    bool letterFall;
    ofVec3f letterVel;
    
    
};