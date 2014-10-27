//
//  TreeLeaves.h
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/27/14.
//
//

#ifndef __Guestbook_App_v1__TreeLeaves__
#define __Guestbook_App_v1__TreeLeaves__

#include <iostream>

#endif /* defined(__Guestbook_App_v1__TreeLeaves__) */
#include "ofMain.h"

#pragma once

class TreeLeaf{
    
public:
    TreeLeaf(ofImage *sprite);
    
    void update();
    void draw();
    void setSummer();
    void setFall();
    

    ofImage *sprite;
    
    ofVec3f pos, vel, acc;
    float baseX;

    float currentBaseRot, goalBaseRot, rot;
    float noiseOffset;
    float noiseSpeed;
    float leafScale;
    
    bool summer;
    
    ofColor currentColor, goalColor;
    
    ofColor summerCol1, summerCol2;
    ofColor fallCol1, fallCol2;
    
    
};