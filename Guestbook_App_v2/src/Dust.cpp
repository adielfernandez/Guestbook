//
//  Dust.cpp
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/28/14.
//
//

#include "Dust.h"

Dust::Dust(ofImage *_sprite){
    

    sprite = _sprite;

    acc.set(0);
    damping = 0.97;

    
    currentRot = 0;
    rotSpeed = ofRandom(0.1, 2);
    
    noiseRand = ofRandom(1000);
    turnAmt = ofRandom(1);
    scale = 0.5;


    col1 = ofColor(255, 246, 215);
    col2 = ofColor(153, 147, 128);
    
    color = col1.getLerped(col2, ofRandom(1));

    

    
    //give initial position
    pos.set(0);
    
    //give an initial velocity, to the right with variation
    vel.set(0, -1);
    vel.rotate(ofRandom(-5, 5), ofVec3f(0, 0, 1));
    vel.scale(ofRandom(0.5, 3));

    
    lifeSpan = 200;
    dead = false;
    
    age = (int)ofRandom(lifeSpan);

    
}



void Dust::update(ofVec3f bikePos, float _bikeScale){
    

        
    //if we're less than a certain age we're still alive
    if(age < lifeSpan){
        
//        if(age < lifeSpan * 0.3){
//            
//            trans = ofLerp(trans, 255, 0.1);
//            
//        } else {
//            //fade the color out
            trans = ofLerp(trans, 0, 0.05);
        
//        }
        
        turnAmt *= 1.005;
        
        float noise = ofNoise(ofGetElapsedTimef() * 0.1, noiseRand);
        float noiseTurn = ofMap(noise, 0, 1, -turnAmt, turnAmt);
        
        vel.rotate(noiseTurn, ofVec3f(0, 0, 1));
        
        if(currentRot > 360){
//            currentRot = 0;
        }
        
        scale += 0.05;
        
        currentRot -= rotSpeed;
        
        age++;
        
        
        
    } else {
        
        //if too old, we're dead
        dead = true;
        
    }
    
    
    //reset and bring back to life
    if(dead){
        age = 0;
        trans = 255;
        scale = 0.5;
        pos.set(bikePos);
        
        vel.set(0, -1);
        vel.rotate(ofRandom(-10, 10), ofVec3f(0, 0, 1));
        vel.scale(ofRandom(0.5, 4));
        
        dead = false;
    }

    
    bikeScale = _bikeScale;
    
    
    
    pos += vel;
    vel += acc;
    vel *= damping;
    
    acc.set(0);
    
}





void Dust::draw(){

    float spriteWidth = sprite -> width * scale;
    float spriteHeight = sprite -> height * scale;
    
    //draw smoke puff
    ofPushMatrix();{
        
        ofTranslate(pos.x, pos.y);
        ofRotate(currentRot);
        ofScale(bikeScale, bikeScale);
        ofSetColor(color, trans);
        sprite -> draw(-spriteWidth/2, -spriteHeight/2, spriteWidth, spriteHeight);
        
    }ofPopMatrix();
    
    
    
}