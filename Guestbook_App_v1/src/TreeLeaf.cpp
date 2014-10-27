//
//  TreeLeaves.cpp
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/27/14.
//
//

#include "TreeLeaf.h"



TreeLeaf::TreeLeaf(ofImage *_sprite){
    
    sprite = _sprite;
    
    vel.set(0);
    acc.set(0);
    currentBaseRot = 40;
    goalBaseRot = currentBaseRot;
//    leafScale = 0.45;
    
    noiseOffset = ofRandom(1000);
    noiseSpeed = ofRandom(0.1, 0.2);
    
    summerCol1 = ofColor(119, 193, 77);
    summerCol2 = ofColor(60, 97, 46);
    fallCol1 = ofColor(209, 64, 34);
    fallCol2 = ofColor(255, 197, 42);
    
    currentColor = summerCol1.getLerped(summerCol2, ofRandom(1));
    goalColor = currentColor;
    
    summer = true;
    
}



void TreeLeaf::update(){
    
    
    //give leaves a little rotation with some noise
    float amplitude = 50;
    
    
    currentColor = currentColor.getLerped(goalColor, 0.05);
    currentBaseRot = ofLerp(currentBaseRot, goalBaseRot, 0.01);
    
    float noise = (ofNoise(ofGetElapsedTimef() * noiseSpeed, noiseOffset) - 0.5);
    
    rot = currentBaseRot + noise * amplitude;

    
    float sway = 30;
    if(summer == false){
        
        pos.x = baseX - noise * sway;
        
    } else {
        pos.x = baseX;
        
        
        
        
    }

}


void TreeLeaf::setSummer(){
    ofColor newCol = summerCol1.getLerped(summerCol2, ofRandom(1));
    goalColor = newCol;
    noiseSpeed = ofRandom(0.1, 0.2);
    goalBaseRot = 40;
    summer = true;

}


void TreeLeaf::setFall(){
    
    //get XY noise val
    float noiseScale = 0.008;
    float lerpVal = ofNoise(pos.x * noiseScale, pos.y * noiseScale + 10);
    
    ofColor newCol = fallCol1.getLerped(fallCol2, ofRandom(1));
//    ofColor newCol = fallCol1.getLerped(fallCol2, lerpVal);
    goalColor = newCol;

    goalBaseRot = -15;
    noiseSpeed = ofRandom(0.4, 0.8);
    summer = false;
    
}

void TreeLeaf::draw(){
    
    
    ofPushMatrix();

    ofTranslate(pos);
    ofRotate(rot);
//    ofScale(leafScale, leafScale);
    ofSetColor(currentColor, 255 * 0.9);
    sprite -> draw(0, 0);

    ofPopMatrix();
    
    
}