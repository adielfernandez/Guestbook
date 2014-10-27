//
//  LetterDust.cpp
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/28/14.
//
//

#include "LetterDust.h"

LetterDust::LetterDust(ofImage *_sprite, ofTrueTypeFont *_font, string _letter){
    
    font = _font;
    sprite = _sprite;
    thisLetter = _letter;
    
    acc.set(0);
    damping = 0.999;
    
    attractionStrength = 0.6;
    
    currentRot = 0;
    rotSpeed = ofRandom(0.1, 2);
    
    noiseRand = ofRandom(1000);
    turnAmt = ofRandom(1);
    scale = 0.5;
    
    col1 = ofColor(255, 246, 215);
    col2 = ofColor(153, 147, 128);
    
    color = col1.getLerped(col2, ofRandom(1));
    textCol = ofColor(color, 0);
    trans = 100;
    textTrans = 0;
    
    attract = false;
    hit = false;
    
    //give initial position
    pos.set(0);
    
    //give an initial velocity, to the right with variation
    vel.set(0, -1);
    vel.rotate(ofRandom(-5, 5), ofVec3f(0, 0, 1));
    vel.scale(ofRandom(2, 5));
    
    fall = false;
    textAccel.set(0, -ofRandom(0.1, 0.7));

    letterFall = false;
    letterVel.set(0);
    
    age = 0;
    released = false;
}



void LetterDust::update(){
    
    if(hit == false){
        letterPos = finalLocation;
        textCol = color;
        textTrans = 0;
        
        //if we're less than a certain age, follow the wind, if we're over that
        //age go to position
        if(age < 100){
            
            turnAmt *= 1.002;
            
            float noise = ofNoise(ofGetElapsedTimef() * 0.1, noiseRand);
            float noiseTurn = ofMap(noise, 0, 1, -turnAmt, turnAmt);
            
            vel.rotate(noiseTurn, ofVec3f(0, 0, 1));

            scale *= 1.005;
            
            
            pos += vel;
            vel += acc;
            vel *= damping;
            
            acc.set(0);
            
        } else {
            
            ofVec2f diff = pos - finalLocation;
            float distSq = diff.lengthSquared();
            
            float pct = ofMap(distSq, 0, 4000 * 4000, 1.0, 0.2);
            diff.normalize();
            
            acc.x -= diff.x * attractionStrength * pct;
            acc.y -= diff.y * attractionStrength * pct;
            
            
            damping = ofMap(distSq, 25 * 25, 0, 0.97, 0.2, true);
            
            pos += vel;
            vel += acc;
            vel *= damping;
            
            acc.set(0);
            
            //if we're close to it, just lerp directly into position and make it bigger
            if(distSq < 10 * 10){
                
                //slowly fix the positions so they line up nicer
                pos.x = ofLerp(pos.x, finalLocation.x, 0.1);
                pos.y = ofLerp(pos.y, finalLocation.y, 0.1);
                
                //and lerp the color into visibility
                textTrans = ofLerp(textTrans, 255, 0.03);
                
            }
            
            //if we're on it stop it and consider it "hit", else continue updating positions
            if(distSq < 2 * 2){
                hit = true;

                vel.set(0);
                acc.set(0);
                damping = 1.0;
            }

            //after being released, fade them in to be more opaque
            trans = ofLerp(trans, 200, 0.03);
            
        }
        

        
        
        
    } else {            //else hit == true
        

        
        
        if(letterFall == false){
            
            //if we've hit, fade away the smoke and fade the text black
//            trans = ofLerp(trans, 0, 0.05);
//            textCol.lerp(ofColor(0), 0.05);
            //and lerp the color into visibility
            textTrans = ofLerp(textTrans, 255, 0.03);
            pos = finalLocation;
            
            //fade them in and shrink them into the letter
            if(scale > 0){
                scale *= 0.98;
                trans = ofLerp(trans, 255, 0.05);
            }
            
            if(scale < 0.01){
                scale = 0;
                textCol.lerp(ofColor(0), 0.04);
            }
            
            
            
        } else {
            
            letterPos += letterVel;
            letterVel += textAccel;
            
            textTrans = ofLerp(textTrans, 0, 0.05);

            
            
            amplitude = ofLerp(amplitude, 50, 0.05);
            
            
            float noiseSpeed = 1.3;
            float noise = ofMap(ofNoise(ofGetElapsedTimef() * noiseSpeed, noiseRand), 0, 1, -amplitude, amplitude);
            
            letterPos.x = finalLocation.x + noise;
            
            
            
            pos += vel;
            vel += acc;
            vel *= damping;
            
            acc.set(0);
            
            
            
            
        }
        
        
        
        
        
    }
    
    age++;

    
    
}





void LetterDust::draw(){

    //draw letter
    ofPushMatrix();{
        
        ofTranslate(letterPos);
        ofSetColor(textCol, textTrans);

        font -> drawString(thisLetter, 0, font -> getLineHeight() * 1/3);
//        ofSetColor(255, 0, 0);
//        ofCircle(0, 0, 5);
        
        
    }ofPopMatrix();
    
    //draw smoke puff
    ofPushMatrix();{
        
        ofTranslate(pos);
        ofRotate(currentRot);
        ofScale(scale, scale);
        
        ofSetColor(color, trans);
        sprite -> draw(-(sprite -> width)/2 + font -> stringWidth(thisLetter)/2, -(sprite -> height)/2);
        
//        ofSetColor(0, 0, 255);
//        ofCircle(0, 0, 5);
        
    }ofPopMatrix();
    
    
    
}