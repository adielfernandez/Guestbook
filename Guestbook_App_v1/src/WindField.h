//
//  WindField.h
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/28/14.
//
//

#ifndef __Guestbook_App_v1__WindField__
#define __Guestbook_App_v1__WindField__

#include <iostream>

#endif /* defined(__Guestbook_App_v1__WindField__) */

#include "ofMain.h"
#pragma once


typedef vector<ofVec2f> VectorList;

class WindField{
    
public:
    WindField();
    
    void setup( int width, int height, int res );

    void draw();
    
    void setDonut();
    void setPerlin();
    void setPerlinDonut();
    void setWhirlpool();
    
    ofVec2f getForceAtPosition( ofVec2f pos );
    
    vector<VectorList> forceField;
    int fieldWidth, fieldHeight, resolution;
    int rows, cols;
    
    
    //visualize field to debug
    //Code courtesy of Charley Whitney, Parsons MFADT
    //Algorithmic Animation (fall 2013)
    void drawVector( const ofVec3f &start, const ofVec3f &end, float headLength = 0.2f, float headRadius = 0.05f );
    void drawVectorAt( const ofVec2f &vec, const ofVec2f &pos, const float &strength = 10.0 );
    
};