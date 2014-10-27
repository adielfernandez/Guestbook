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
    
    age = 0;
    
    minMass = 0.3;
    maxMass = 1.5;
    
    mass = ofRandom(minMass,maxMass);

    attractionStrength = 0.35;
    
    currentRot = 0;
    rotSpeed = ofRandom(0.5, 4);
    
    noiseRand = ofRandom(1000);
    turnAmt = ofRandom(1);
    leafScale = 0.4;
    

    fallCol1 = ofColor(209, 64, 34);
    fallCol2 = ofColor(224, 197, 42);
    
    color = fallCol1.getLerped(fallCol2, ofRandom(1));
    textCol = ofColor(0);
    
    attract = true;
    hit = false;
    
    //give initial position
    pos.set(ofRandom(150, 340), ofRandom(250, 540));
    
    //give an initial velocity, to the right with variation
//    vel.set(0, 0);
    vel.set(0.8, 1);
    vel.rotate(ofRandom(-5, 5), ofVec3f(0, 0, 1));
    vel.scale(ofRandom(2, 5));
    
    fall = false;
    fallAccel.set(0, ofRandom(0.5, 2.5));
    textCol = ofColor(0, 0);
    
    windAccel.set(ofRandom(0.05, 2), ofRandom(-1, 1));

    letterFall = false;
    letterVel.set(0);
    
    groundLevel = ofRandom(760, ofGetWindowHeight());
    
}



void LetterLeaf::update(){
    

    
    if(hit == false){
        letterPos = finalLocation;
        textCol = ofColor(0, 0);
        
        
        //if we're less than a certain age, follow the wind, if we're over that
        //age go to position
        if(age < 300){
            
            turnAmt *= 1.002;
            
            float noise = ofNoise(ofGetElapsedTimef() * 0.1, noiseRand);
            float noiseTurn = ofMap(noise, 0, 1, -turnAmt, turnAmt);
            
            vel.rotate(noiseTurn, ofVec3f(0, 0, 1));
//            acc = windAccel;
            
            
            if(currentRot > 360){
                currentRot = 0;
            }
            
            if(pos.y > groundLevel){
                vel.set(0);
                rotSpeed = 0;
            }
            
            currentRot += rotSpeed;
            
            
        } else {
            
            ofVec2f diff = pos - finalLocation;
            float distSq = diff.lengthSquared();
            
            float pct = ofMap(distSq, 0, 4000 * 4000, 1.0, 0.2);
            diff.normalize();
            
            acc.x -= diff.x * attractionStrength * pct;
            acc.y -= diff.y * attractionStrength * pct;
            
            
            damping = ofMap(distSq, 25 * 25, 0, 0.97, 0.2, true);
            
            //if we're close to it, just turn it directly to position and make it bigger 
            if(distSq < 10 * 10){
                acc = -diff;
                leafScale = ofLerp(leafScale, 0.65, 0.05);
            }
            
            
            //if we're on it stop it and consider it "hit"
            if(distSq < 1.5 * 1.5){
                hit = true;
                attract = false;
                vel.set(0);
                acc.set(0);
                turnAmt = 0;
                damping = 1.0;
                
            }
            
            //if we are attracting, slow down leaf rotation
            currentRot = ofLerp(currentRot, 0, 0.04);

            
        }
        

        
    } else {
        


        
        acc = fallAccel;
        textCol.lerp(ofColor(0, 255), 0.50);
        
        
        //drop letters after time
        if(letterFall){
            letterPos += letterVel;
            letterVel += acc;
        }
        
        
    }
    

    
    pos += vel;
    vel += acc;
    vel *= damping;
    
    acc.set(0);
    

    age++;



    
    

    
    
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
//        ofDrawBitmapString(ofToString(age), 20, 0);
        
    }ofPopMatrix();
    
    
    
}