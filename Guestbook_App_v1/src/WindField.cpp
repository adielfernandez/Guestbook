//
//  WindField.cpp
//  Guestbook_App_v1
//
//  Created by Adiel Fernandez on 9/28/14.
//
//

#include "WindField.h"

WindField::WindField(){
    
    
}

void WindField::setup(int width, int height, int res){
    
    fieldWidth = width;
    fieldHeight = height;
    resolution = res;
    
    cols = fieldWidth/resolution;
    rows = fieldHeight/resolution;
    
    //nested if to make a vector full of vectors
    for(int y = 0; y < rows; y++){
        
        //fill forceField with vectors
        VectorList rowVectors;
        forceField.push_back(rowVectors);
        
        //then loop through each of those vectors to add a force value
        //just a force to the right to begin with
        for(int x = 0; x < cols; x++){
            ofVec2f dir(5, 0);
            forceField[y].push_back( dir );
        }
        
    }
    
    
}



ofVec2f WindField::getForceAtPosition(ofVec2f pos){
    
    float pctX = pos.x / fieldWidth;
    float pctY = pos.y / fieldHeight;
    
    int xVal = ofClamp(pctX * cols, 0, cols - 1);
    int yVal = ofClamp(pctY * rows, 0, rows - 1);
    
    ofVec2f forceAtPos;
    forceAtPos.set(forceField[yVal][xVal]);
    
    return forceAtPos;
    
}




void WindField::draw() {
    for( int y=0; y<forceField.size(); y++){
        for( int x=0; x<forceField[y].size(); x++){
            ofVec2f np( x*resolution, y*resolution );
            drawVectorAt( forceField[y][x], np, forceField[y][x].length() );
            
            
            
            ofVec2f tmpPos(x*resolution, y*resolution);
        }
    }
}

void WindField::setPerlin() {
    for( int y=0; y<forceField.size(); y++){
        for( int x=0; x<forceField[y].size(); x++){
            float noise = ofMap( ofNoise(x*0.05, y*0.05, 9.6), 0, 0.4, 0, TWO_PI);
            forceField[y][x].set( ofVec2f( cos(noise)*30, sin(noise)*30 ) );
        }
    }
}

void WindField::setDonut() {
    for( int y=0; y<forceField.size(); y++){
        for( int x=0; x<forceField[y].size(); x++){
            
            float offsetX = 250;
            
            //find center
            ofVec2f center(fieldWidth/2 - resolution + offsetX, fieldHeight/2 - resolution);
            ofVec2f fieldPoint( x*resolution, y*resolution );
            //add perpendicular force to center
            ofVec2f thisForce(center - fieldPoint);
            
            float strength = 15;
            float dist = thisForce.lengthSquared();

            thisForce.normalize();
            thisForce.scale(strength);
            thisForce.rotate(ofMap(dist, 0, 300 * 300, 180, 0, true));
            
            forceField[y][x].set( thisForce );
        }
    }
}

void WindField::setPerlinDonut() {
    for( int y=0; y<forceField.size(); y++){
        for( int x=0; x<forceField[y].size(); x++){
            
            float offsetX = 200;
            
            //find center
            ofVec2f center(fieldWidth/2 - resolution + offsetX, fieldHeight/2 - resolution);
            ofVec2f fieldPoint( x*resolution, y*resolution );
            //add perpendicular force to center
            ofVec2f thisForce(center - fieldPoint);
            
            float strength = 60;

            float distSq = thisForce.lengthSquared();
            
            float noise = ofMap( ofNoise(x*0.09, y*0.09), 0, 0.4, 0, TWO_PI);
            ofVec2f noiseForce;
            noiseForce.set( ofVec2f( cos(noise)*20, sin(noise)*20 ) );
            
            thisForce.normalize();
            thisForce.scale(ofMap(distSq, 0, 800 * 800, strength, 0));
            thisForce.rotate(ofMap(distSq, 0, 800 * 800, 90, 0));
            
//            thisForce += noiseForce;
            
            forceField[y][x].set( thisForce );
        }
    }
}

void WindField::setWhirlpool() {
    for( int y=0; y<forceField.size(); y++){
        for( int x=0; x<forceField[y].size(); x++){
            
            float offsetX = 200;
            
            //find center
            ofVec2f center(fieldWidth/2 + offsetX, fieldHeight/2);
            ofVec2f fieldPoint( x*resolution, y*resolution );
            //add perpendicular force to center
            ofVec2f thisForce(center - fieldPoint);
            
            float strength = 30;
            float dist = thisForce.length();
            
            thisForce.normalize();
            thisForce.scale(strength);
            thisForce.rotate(ofMap(dist, 0, 800, 40, 60));
            
            forceField[y][x].set( thisForce );
        }
    }
}


void WindField::drawVectorAt( const ofVec2f &vec, const ofVec2f &pos, const float &strength ) {
    ofVec2f np = pos;
    drawVector( np, np + vec.normalized() * strength, 5, 3 );
}

void WindField::drawVector( const ofVec3f &start, const ofVec3f &end, float headLength, float headRadius ) {
	const int NUM_SEGMENTS = 32;
	float lineVerts[3*2];
	ofVec3f coneVerts[NUM_SEGMENTS+2];
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, lineVerts );
	lineVerts[0] = start.x; lineVerts[1] = start.y; lineVerts[2] = start.z;
	lineVerts[3] = end.x; lineVerts[4] = end.y; lineVerts[5] = end.z;
	glDrawArrays( GL_LINES, 0, 2 );
	
	// Draw the cone
	ofVec3f axis = ( end - start ).normalized();
    ofVec3f temp = ( axis.dot( ofVec3f(0,1,0) ) > 0.999f ) ? axis.crossed( ofVec3f(1,0,0) ) : axis.crossed( ofVec3f(0,1,0) );
	ofVec3f left = ofVec3f(axis.crossed( temp )).normalized();
	ofVec3f up = axis.crossed( left ).normalized();
    
	glVertexPointer( 3, GL_FLOAT, 0, &coneVerts[0].x );
	coneVerts[0] = ofVec3f( end + axis * headLength );
	for( int s = 0; s <= NUM_SEGMENTS; ++s ) {
		float t = (float)s / (float)NUM_SEGMENTS;
		coneVerts[s+1] = ofVec3f( end + left * headRadius * cos( t * 2.0f * 3.14159f )
                                 + up * headRadius * sin( t * 2.0f * 3.14159f ) );
	}
	glDrawArrays( GL_TRIANGLE_FAN, 0, NUM_SEGMENTS+2 );
    
	// draw the cap
	glVertexPointer( 3, GL_FLOAT, 0, &coneVerts[0].x );
	coneVerts[0] = end;
	for( int s = 0; s <= NUM_SEGMENTS; ++s ) {
		float t = s / (float)NUM_SEGMENTS;
		coneVerts[s+1] = ofVec3f( end - left * headRadius * (float)cos( t * 2 * 3.14159f )
                                 + up * headRadius * (float)sin( t * 2 * 3.14159f ) );
	}
	glDrawArrays( GL_TRIANGLE_FAN, 0, NUM_SEGMENTS+2 );
    
	glDisableClientState( GL_VERTEX_ARRAY );
}