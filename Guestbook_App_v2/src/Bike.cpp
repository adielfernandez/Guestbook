//
//  Bike.cpp
//  Guestbook_App_v2
//
//  Created by Adiel Fernandez on 9/29/14.
//
//

#include "Bike.h"

Bike::Bike(){
    
}


void Bike::setup(ofImage *_bike, ofImage *_wheel, ofImage *_dustSprite){
    
    bike = _bike;
    wheel = _wheel;
    dustSprite = _dustSprite;
    
    scale = 0.4;
    
    groundLevel = 760;

    //stationary and on screen for debugging:
//    pos.set(700, 400);
//    vel.set(0, 0);
//    wheelSpeed = -2;
    
    pos.set(ofGetWindowWidth() + 200, groundLevel);
    vel.set(-10, 0);
    wheelSpeed = -vel.x * 0.75;
    
    
    bikePitch = 0;
    
    col = ofColor(255);
    
    segment = 0;
    
    dustReleaseTimer = 0;
    dustToRelease = 0;
    letterFall = false;
    endTime = 0;
    
    drawBehind = false;
    
    dustList.clear();
    
    //create non-letter forming dust particles
    for(int i = 0; i < 150; i++){
        Dust d(dustSprite);
        d.pos.set(pos);
        
        dustList.push_back(d);
    }
    
    //flip = 1 means right to left, -1 means left to right
    flip = false;
    
    animateBike = true;
}



void Bike::loadLetterDust(vector<LetterDust> *_letterDust){
    
    //load it with new stuff
    letterDust = _letterDust;
    
}


void Bike::update(){


    if(segment == 0){
        
        vel.set(-6, 0);
        groundLevel = 740;
        scale = 0.4;
        drawBehind = true;
        flip = false;
        
        if(pos.x < -500){
            segment++;
        }
        
    } else if(segment == 1){
        
        vel.set(10, 0);
        groundLevel = 770;
        scale = 0.8;
        drawBehind = false;
        flip = true;
        
        if(pos.x > ofGetWindowWidth() + 700){
            segment++;
        }
        
    } else if(segment == 2){
        
        
        vel.set(-12, 0);
        groundLevel = 850;
        scale = 1.8;
        drawBehind = false;
        flip = false;
        
        if(pos.x < -1000){
            animateBike = false;
        }
        
        
        //release a few at a time
        if(ofGetElapsedTimeMillis() - dustReleaseTimer > 50 && dustToRelease < letterDust -> size()){
            dustToRelease += 3;
            dustReleaseTimer = ofGetElapsedTimeMillis();
            
        }
        
        
        //update the letterdust if the bike is past a certain point on screen
        if(pos.x < ofGetWindowWidth() - 100){
            
            for(vector<LetterDust>::iterator it = letterDust -> begin(); it != letterDust -> begin() + ofClamp(dustToRelease, 0, letterDust -> size()); it++){
                
                if(it -> released == false){
                    it -> pos.set(pos);
                    it -> released = true;
                }
                it -> update();
            }

        
        }
        
        
        //go through and see if all the letters have "hit" their spots
        //then make them all leave at once at different rates
        if(letterFall == false){
        
            for(vector<LetterDust>::iterator it = letterDust -> begin(); it != letterDust -> end(); it++){
                if(it -> hit == false){
                    break;
                }
                
                //if we're at the end of the list and we're still
                //checking then make them fall
                if(it == letterDust -> end() - 1){
                    letterFall = true;

                    endTime = ofGetElapsedTimeMillis();
                    
                }
            }

        } else {
            
            if(ofGetElapsedTimeMillis() - endTime > 12000){
                
                //if letterFall is true, go tell each letter to fade and fall away
                for(vector<LetterDust>::iterator it = letterDust -> begin(); it != letterDust -> end(); it++){
                    
                    it -> letterFall = true;
                    
                }

                
            }
            
            if(ofGetElapsedTimeMillis() - endTime > 14000){
                done = true;

            }
            
            
        }
        
        
    }

    
    
    //only move if we're to the right of the stopping area
    if(animateBike){
        
        //add noise to y position
        float amplitudeY = 5;
        if(segment == 2){
            amplitudeY = 3;
        }
        float noiseSpeedY = abs(vel.x) * 0.8;
        float noiseY = ofMap(ofNoise(ofGetElapsedTimef() * noiseSpeedY), 0, 1, -amplitudeY, amplitudeY);
        
        pos.y = groundLevel + noiseY;
        
        float amplitudePitch = amplitudeY/2;
        float noiseSpeedPitch = abs(vel.x) * 0.8;
        bikePitch = ofMap(ofNoise(ofGetElapsedTimef() * noiseSpeedPitch), 0, 1, -amplitudePitch, amplitudePitch);
        
        
        pos += vel;
        vel += acc;
        acc.set(0);
        
        wheelAngle -= wheelSpeed;
        
        
        
        
        
        //update the regular dust
        
        bool moving;
        if(vel.x != 0){
            moving = true;
        } else {
            moving = false;
        }
        
        for(int i = 0; i < dustList.size(); i++){
            dustList[i].update(pos, scale);
        }
    }
    
    
    
    
//    change it so it only updates the regular dust when on screen


    
    
    
}

void Bike::draw(){
    
    float imageScaleDown = 0.35;
    ofPoint frontWheel(25 * imageScaleDown, 326 * imageScaleDown);
    ofPoint backWheel(768 * imageScaleDown, 326 * imageScaleDown);
    
    if(animateBike){
        
        ofPushMatrix();{
            
            
            ofTranslate(pos);
            
            if(flip){
                ofTranslate(bike -> width * 0.5 * scale, -bike -> height * 0.62 * scale);
                ofScale(-1, 1);
            } else {
                ofTranslate(-bike -> width * 0.5 * scale, -bike -> height * 0.62 * scale);
            }
            
            
            ofScale(scale, scale);
            
            //Add an extra translate so the reference position is below the rear wheel
            //i.e. where the dust will be coming from
            ofRotate(bikePitch);
            //        ofTranslate( -backWheel.x, -backWheel.y - wheel -> height * 0.42);
            
            //translate to front wheel and draw
            ofPushMatrix();{
                ofTranslate(-bike -> width * 0.48, bike -> height * 0.25);
                ofRotate(wheelAngle);
                ofSetColor(255);
                wheel -> draw(0, 0);
            }ofPopMatrix();
            
            //translate to back wheel and draw
            ofPushMatrix();{
                ofTranslate(bike -> width * 0.45, bike -> height * 0.25);
                ofRotate(wheelAngle);
                wheel -> draw(0, 0);
            }ofPopMatrix();
            
            ofSetColor(col);
            bike -> draw(-bike -> width/2, -bike -> height/2);
            
            
            
        }ofPopMatrix();
        
        
        //draw the regular dust
        for(int i = 0; i < dustList.size(); i++){
            dustList[i].draw();
        }
    }

    
    
    //draw the letter dust
    if(segment == 2 && pos.x < ofGetWindowWidth() - 100){
        for(vector<LetterDust>::iterator it = letterDust -> begin(); it != letterDust -> begin() + ofClamp(dustToRelease, 0, letterDust -> size()); it++){
            it -> draw();
        }        
    }
    
}











