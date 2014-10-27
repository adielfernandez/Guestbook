//
//  Cloud.cpp
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/28/14.
//
//

#include "Cloud.h"



Cloud::Cloud(ofImage *_image){
    
    image = _image;
    cloudScale = 1.2;
    
    leftBound = -400;
    rightBound = ofGetWindowWidth();
    upperBound = 20;
    lowerBound = 350;
    speedMult = 3.0f;
    goalMult = speedMult;
    
    pos.set(ofRandom(leftBound, rightBound), ofRandom(upperBound, lowerBound));
    vel.set(ofRandom(0.01, 0.1), 0);
    summer = true;
    
}

void Cloud::update(){
    
    speedMult = ofLerp(speedMult, goalMult, 0.03);
    
    if(summer){
        speedMult = 3.0;
    } else {
        speedMult = 15.0;
    }
    
    pos += vel * speedMult;
    
    if(pos.x > rightBound){
        pos.x = leftBound;
        pos.y = ofRandom(upperBound, lowerBound);
    }
    
    
}

void Cloud::draw(){
    
    ofSetColor(255, 255 * 0.7);
    image -> draw(pos, image -> width * cloudScale, image -> height * cloudScale);
    
}