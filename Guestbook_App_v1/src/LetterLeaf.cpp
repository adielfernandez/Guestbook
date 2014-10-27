//
//  LetterLeaf.cpp
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/28/14.
//
//

#include "LetterLeaf.h"

LetterLeaf::LetterLeaf(ofImage *_sprite, ofTrueTypeFont *_font, string _word){
    
    font = _font;
    sprite = _sprite;
    thisWord = _word;
    
    acc.set(0);
    damping = 0.999;
    
    minMass = 0.3;
    maxMass = 1.5;
    
    mass = ofRandom(minMass,maxMass);

    attractionStrength = 0.4;
    
    currentRot = 0;
    rotSpeed = ofRandom(0.1, 2);
    
    noiseRand = ofRandom(1000);
    turnAmt = ofRandom(1);
    leafScale = 0.5;
    

    fallCol1 = ofColor(209, 64, 34);
    fallCol2 = ofColor(224, 197, 42);
    
    color = fallCol1.getLerped(fallCol2, ofRandom(1));
    textCol = ofColor(0);
    
    attract = false;
    hit = false;
    
    //give initial position
    pos.set(ofRandom(130, 360), ofRandom(290, 530));
    
    //give an initial velocity, to the right with variation
    vel.set(1, 0);
    vel.rotate(ofRandom(-10, 10), ofVec3f(0, 0, 1));
    vel.scale(ofRandom(2, 5));
    
    fall = false;
    fallAccel.set(0, ofRandom(0.5, 2.5));
    textCol = ofColor(0, 0);
    
    windAccel.set(ofRandom(1, 10), ofRandom(-1, 1));

    letterFall = false;
    letterVel.set(0);
}



void LetterLeaf::update(){
    
    pos += vel;
    vel += acc;
    vel *= damping;
    
    if(hit == false){
        letterPos = finalLocation;
        textCol = ofColor(0, 0);
        
        turnAmt *= 1.005;
        
        float noise = ofNoise(ofGetElapsedTimef() * 0.1, noiseRand);
        float noiseTurn = ofMap(noise, 0, 1, -turnAmt, turnAmt);
        
        vel.rotate(noiseTurn, ofVec3f(0, 0, 1));
        acc += windAccel;
        
    }
    

    
    if(fall == true){
        acc = fallAccel;
        textCol = ofColor(0, 255);
//        textCol = ofColor(78, 77, 78, 255);
    } else {
        acc.set(0);
    }
    
    
    
    //drop letters after time
    if(letterFall){
        letterPos += letterVel;
        letterVel += acc;
    }
    
    if(attract && leafScale < 1.0){
//        leafScale *= 1.01;
    }
    


//    currentRot += rotSpeed;
    
    

    
    
}

void LetterLeaf::applyForce(ofVec3f force){
    
    acc += (force / mass);
    
}

void LetterLeaf::attractToLetterPos(){
    
    ofVec2f diff = pos - finalLocation;
    float distSq = diff.lengthSquared();
    
    float pct = ofMap(distSq, 0, 4000 * 4000, 1.0, 0.2);
    diff.normalize();
    
    acc.x -= diff.x * attractionStrength * pct;
    acc.y -= diff.y * attractionStrength * pct;
    
    
    
    if(attract){
        
        damping = ofMap(distSq, 25 * 25, 0, 0.97, 0.2, true);

        //if we're close to it, just turn it directly to position
        if(distSq < 10 * 10){
            acc = -diff;
        }
        
        //if we're REALLY close to it, just snap into place
        if(distSq < 5 * 5){
            acc = -diff;

        }

        if(distSq < 1.5 * 1.5){
            hit = true;
            attract = false;
            vel.set(0);
            acc.set(0);
            turnAmt = 0;
            damping = 1.0;
            

        }
        
    }
    
}


void LetterLeaf::draw(){

    //draw letter
    ofPushMatrix();{
        
        ofTranslate(letterPos);
        
        ofSetColor(textCol);
        font -> drawString(thisWord, 10, font -> getLineHeight() - 5);
        
        
    }ofPopMatrix();
    
    //draw leaf
    ofPushMatrix();{
        
        ofTranslate(pos);
        ofRotate(currentRot);
        ofScale(leafScale, leafScale);
        ofSetColor(color, 255);
        sprite -> draw(0, 0);
        
    }ofPopMatrix();
    
    
    
}