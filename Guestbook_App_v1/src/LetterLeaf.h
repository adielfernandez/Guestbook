//
//  LetterLeaf.h
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/28/14.
//
//

#ifndef __Guestbook_App_v1__LetterLeaf__
#define __Guestbook_App_v1__LetterLeaf__

#include <iostream>

#endif /* defined(__Guestbook_App_v1__LetterLeaf__) */
#include "ofMain.h"

#pragma once

class LetterLeaf{
    
public:
    LetterLeaf(ofImage *sprite, ofTrueTypeFont *font, string _word);
    
    void update();
    void applyForce(ofVec3f force);
    void attractToLetterPos();
    void draw();
    
    ofTrueTypeFont *font;
    
    ofImage *sprite;
    
    ofVec3f pos, vel, acc;
    ofVec3f finalLocation;
    ofVec3f letterPos;
    ofVec3f windAccel;
    
    float mass, minMass, maxMass;
    
    float attractionStrength;
    
    string thisWord;
    
    float damping;
    float currentRot, goalRot;
    float rotSpeed;
    
    float leafScale;
    float noiseRand;
    float turnAmt;
    
    ofColor color;
    ofColor textCol;
    ofColor fallCol1, fallCol2;
    
    bool hit;
    bool attract;
    bool fall;
    
    ofVec3f fallAccel;
    bool letterFall;
    ofVec3f letterVel;
    
    
};