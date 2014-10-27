//
//  Dust.h
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

class Dust{
    
public:
    Dust(ofImage *sprite);
    
    void update(ofVec3f bikePos, float _bikeScale);
    void draw();

    ofImage *sprite;
    
    ofVec3f pos, vel, acc;


    int age;
    int lifeSpan;
    bool dead;

    
    float damping;
    float currentRot, goalRot;
    float rotSpeed;
    
    float scale;
    float noiseRand;
    float turnAmt;
    float bikeScale;
    
    ofColor color;
    ofColor col1, col2;

    float trans;

    
};