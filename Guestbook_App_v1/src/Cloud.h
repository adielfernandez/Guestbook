//
//  Cloud.h
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/28/14.
//
//

#ifndef __Guestbook_App_v1__Cloud__
#define __Guestbook_App_v1__Cloud__

#include <iostream>

#endif /* defined(__Guestbook_App_v1__Cloud__) */
#include "ofMain.h"
#pragma once



class Cloud {
    
public:
    
    Cloud(ofImage *_image);
    void update();
    void draw();
    
    ofImage *image;
    
    ofVec3f pos, vel;
    float speedMult, goalMult;
    
    float leftBound, rightBound;
    float upperBound, lowerBound;
    
    float cloudScale;
    
    bool summer;
    
    
};