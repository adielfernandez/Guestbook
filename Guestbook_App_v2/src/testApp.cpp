#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    //Spacebrew Setup
    string host = "54.68.243.245";
    string name = "Guestbook App";
    string description = "Receives and visualizes guest messages";
    
    spacebrew.addSubscribe("incoming", "string");
    spacebrew.addPublish("connection", "boolean");
    spacebrew.connect(host, name, description);
    
    Spacebrew::addListener(this, spacebrew);
    
    
    //test string to log
    fileName = ofGetTimestampString();
    fileName.append(".txt");

    //prepare file header info
    string fileHeader = "Messages stored for ";
    fileHeader.append(ofGetTimestampString());
    fileHeader.append("\n\n");
    
    //add the header to the buffer
    messageLog.append(fileHeader.c_str(), fileHeader.length());
    
    //load images
    use16x9 = false;
    
    if(use16x9){
        backgroundSummer.loadImage("backgrounds/16x9summer.png");
        backgroundFall.loadImage("backgrounds/16x9fall.png");
        leafMask.loadImage("backgrounds/16x9mask.png");
        sky.loadImage("backgrounds/16x9sky.png");
    } else {
        backgroundSummer.loadImage("backgrounds/4x3summerShadow.png");
        backgroundFall.loadImage("backgrounds/4x3fallShadow.png");
        leafMask.loadImage("backgrounds/4x3mask.png");
        sky.loadImage("backgrounds/4x3sky.png");
    }
    
    logo.loadImage("backgrounds/logoSmall.png");
    

    
    //load cloud sprites
    int numCloudSprites = 9;
    for(int i = 0; i < numCloudSprites; i++){
        
        string cloudFile = "clouds/0" + ofToString(i) + ".png";
        ofImage c;
        
        cloudSprites.push_back(c);
        cloudSprites[i].loadImage(cloudFile);
        
    }
    
    //setup clouds
    int numClouds = 15;
    int numImages = 8;
    
    for(int i = 0; i < numClouds; i++){
        Cloud c(&cloudSprites[i % numImages]);
        cloudList.push_back(c);
    }
    
    
    
    //load font
    font.loadFont("font/pironv2.ttf", 35);
    font.setLetterSpacing(1.0);
    UIfont.loadFont("font/champagne.ttf", 40);
    UIfont.setLetterSpacing(0.98);
    
    //Environment
//    ofBackground(239, 236, 225);

    seasonChangeSpeed = 0.03;
    foregroundTrans = 255;
    foregroundGoalTrans = foregroundTrans;
    

    summerSky = ofColor(48, 140, 204);
    fallSky = ofColor(250, 160, 170);
    currentSky = summerSky;
    goalSky = summerSky;
    
    isItSummer = true;

    //set up the wind field
    wind.setup( ofGetWindowWidth(), ofGetWindowHeight(), 40 );

    wind.setDonut();
//    wind.setPerlinDonut();
    
    
    //load up leaf sprites
    int numTreeSprites = 3;
    for(int i = 0; i < numTreeSprites; i++){
        
        string leafFile = "treeLeavesShadow/" + ofToString(i) + ".png";
        ofImage l;
        
        treeLeafSprites.push_back(l);
        treeLeafSprites[i].loadImage(leafFile);
        
    }
    
    //load small leaf sprites
    int numLetterSprites = 3;
    for(int i = 0; i < numLetterSprites; i++){
        
        string leafFile = "letterLeavesShadow/" + ofToString(i) + ".png";
        ofImage l;
        
        letterLeafSprites.push_back(l);
        letterLeafSprites[i].loadImage(leafFile);
        
    }
    
    //go through leaf mask image and make leaves where
    //ever there is red in the image
    int res = 19;
    float randomScatter = 15;
    int yHeightToScan = 670;
    
    for(int y = 0; y < yHeightToScan; y += res){
        for(int x = 0; x < leafMask.width/2; x += res){
        
            ofColor thisPixel = leafMask.getColor(x, y);
            
            if((int)thisPixel.a > 0){
                int spriteSelect = round(ofRandom(2));
                
                TreeLeaf l(&treeLeafSprites[spriteSelect]);
                l.pos.set(x + ofRandom(-randomScatter, randomScatter), y + ofRandom(-randomScatter, randomScatter));
//                l.pos.set(x, y);
                l.baseX = l.pos.x;
                leafList.push_back(l);
                
            }
        }
    }
    
    
    //load bike graphics
    bike.loadImage("bike/bikeShadow.png");
    wheel.loadImage("bike/wheelShadow.png");
    wheel.setAnchorPercent(0.5, 0.5);
    dustSprite.loadImage("bike/sunSmoke.png");

    
    debugVisuals = false;
    drawWind = false;
    animating = false;
    animationType = 0;
    letterFall = false;
    
    
//    vector<Dust> temp;
//    messengerBike.loadDustList(&temp);
    
    scaleToSmall = true;
    
}

//--------------------------------------------------------------
void testApp::update(){

    
    //update tree leaves
    for(int i = 0; i < leafList.size(); i++){
        leafList[i].update();
    }
    
    //update clouds
    for(int i = 0; i < cloudList.size(); i++){
        cloudList[i].update();
    }
    
    //lerp Sky color
    currentSky = currentSky.getLerped(goalSky, seasonChangeSpeed);
    
    
    //fade/switch backgrounds (SUMMER: foregroundGoalTrans = 255, FALL: 0)
    foregroundTrans = ofLerp(foregroundTrans, foregroundGoalTrans, seasonChangeSpeed);
    
    
    
    //If we're not currently animating something and its been long enough
    //since we ended the last one start again
    if(animating == false){

        //wait a bit if we're doing the leaves animation or go straight in we're doing the bike
        if(ofGetElapsedTimeMillis() - waitTimer > 4000 || animationType == 1){
            
            //lets go through the checked list and animate the first unchecked
            for(int i = 0; i < checked.size(); i++){
                
                //if we get one that is unchecked:
                //flip boolean, log which one we need to animate and
                //stop going through the checked list
                if(checked[i] == false){
                    
                    //reset stuff
                    animating = true;
                    checked[i] = true;
                    
                    animationTime = ofGetElapsedTimeMillis();
                    

                    
                    //create the Letter objects based on the message/sender data
                    //split the message up into words and store them in a vector
                    vector<string> wordList;
                    wordList = ofSplitString(messageList[i], " ");
                    
                    //clear the previous letterList
                    letterList.clear();
                    
                    //then go through the vector and create leaves for each element
                    int currentLine = 0;
                    int placeOnThisLine = 0;
                    int lineWidth = 450;
                    int spaceWidth = 25;
                    startPos.set(530, 200);
                    
                    
                    //-------------------- Animation select --------------------
                    
                    //---------- LEAVES ----------
                    if(animationType == 0){
                        
                        //reset stuff
                        leavesToRelease = 0;
                        releaseTimer = 0;
                        letterFall = false;
                        
                        //trigger the fall enviroment transition
                        setFall();

                        //first do the message, then do the sender afterwards
                        //go through each word
                        for(int i = 0; i < wordList.size(); i++){
                            
                            //then each letter
                            for(int j = 0; j < wordList[i].length(); j++){
                                
                                //choose a leaf sprite image
                                int spriteSelect = round(ofRandom(2));
                                
                                string thisLetter = ofToString(wordList[i][j]);
                                
                                //make a letterleaf
                                LetterLeaf l(&letterLeafSprites[spriteSelect], &font, thisLetter);
                                
                                //do some math to figure out what its final position should be
                                l.finalLocation.x = startPos.x + placeOnThisLine;
                                l.finalLocation.y = startPos.y + font.getLineHeight() * currentLine;
                                
                                letterList.push_back(l);
                                
                                //then store the line position data for the next string to be made
                                placeOnThisLine += font.stringWidth(thisLetter);
                                
                            }
                            
                            //Then add a space for between words
                            placeOnThisLine += spaceWidth;
                            
                            //if we're at the end of our text "bounding box"
                            //drop to the next line
                            if(placeOnThisLine > lineWidth){
                                placeOnThisLine = 0;
                                currentLine++;
                            }
                            
                        }
                        
                        
                        //split up the sender into words
                        vector<string> senderName;
                        senderName = ofSplitString(senderList[i], " ");
                        
                        placeOnThisLine = 0;
                        
                        //go through each word
                        for(int i = 0; i < senderName.size(); i++){
                            
                            //then each letter
                            for(int j = 0; j < senderName[i].length(); j++){
                                
                                //choose a leaf sprite image
                                int spriteSelect = round(ofRandom(2));
                                
                                string thisLetter = ofToString(senderName[i][j]);
                                
                                //make a letterleaf
                                LetterLeaf l(&letterLeafSprites[spriteSelect], &font, thisLetter);
                                
                                //do some math to figure out what its final position should be
                                l.finalLocation.x = startPos.x + placeOnThisLine;
                                l.finalLocation.y = startPos.y + font.getLineHeight() * (currentLine + 2);
                                
                                letterList.push_back(l);
                                
                                //move over to next letter position
                                placeOnThisLine += font.stringWidth(thisLetter);
                                
                            }

                            //move words over
                            placeOnThisLine += spaceWidth;
                            
                        }
                        
                        //then randomize the vector so the letters all fly
                        //in randomly instead of in order
                        ofRandomize(letterList);
                        
                        
                    }
                    
                    //---------- BIKES ----------
                    else if(animationType == 1){
                        
                        _letterDust.clear();
                        
                        //first do the message, then do the sender afterwards
                        //go through each word
                        for(int i = 0; i < wordList.size(); i++){
                            

                            //then each letter
                            for(int j = 0; j < wordList[i].length(); j++){
                                
                                
                                string thisLetter = ofToString(wordList[i][j]);
                                
                                LetterDust d(&dustSprite, &font, thisLetter);
                                
                                
                                //do some math to figure out what its final position should be
                                d.finalLocation.x = startPos.x + placeOnThisLine;
                                d.finalLocation.y = startPos.y + font.getLineHeight() * currentLine;
                                
                                _letterDust.push_back(d);
                                
                                //then store the line position data for the next string to be made
                                placeOnThisLine += font.stringWidth(thisLetter);
                                
                            }
                            
                            //Then add a space for between words
                            placeOnThisLine += spaceWidth;
                            
                            //if we're at the end of our text "bounding box"
                            //drop to the next line
                            if(placeOnThisLine > lineWidth){
                                placeOnThisLine = 0;
                                currentLine++;
                            }
                            
                            
                        }
                        
                        //split up the sender into words
                        vector<string> senderName;
                        senderName = ofSplitString(senderList[i], " ");
                        
                        placeOnThisLine = 0;
                        
                        //go through each word
                        for(int i = 0; i < senderName.size(); i++){
                            
                            //then each letter
                            for(int j = 0; j < senderName[i].length(); j++){

                                string thisLetter = ofToString(senderName[i][j]);
                                
                                LetterDust d(&dustSprite, &font, thisLetter);
                                
                                
                                //do some math to figure out what its final position should be
                                d.finalLocation.x = startPos.x + placeOnThisLine;
                                d.finalLocation.y = startPos.y + font.getLineHeight() * (currentLine + 2);
                                
                                _letterDust.push_back(d);
                                
                                //move over to next letter position
                                placeOnThisLine += font.stringWidth(thisLetter) + 2;
                                
                            }
                            
                            //move words over
                            placeOnThisLine += spaceWidth;
                            
                        }
                        
                        //then randomize the vector so the letters all fly
                        //in randomly instead of in order
                        ofRandomize(_letterDust);

                        //then pass the dust list to the bike after setting it up
                        messengerBike.setup(&bike, &wheel, &dustSprite);
                        messengerBike.loadLetterDust(&_letterDust);


                        
                    }
                    
                    
                    
                    
                    
                    
                    //then lets store which animation we're on and stop checking
                    //for other messages so we can do the animation stuff
                    currentMessageShowing = i;
                    
                    break;
                }
                
            }
            
        }
        
   
    } else {

        //-------------------- if animating == TRUE  --------------------
        
        //---------- LEAVES ----------
        if(animationType == 0){
            
            //wait a second or two to let the season change sink in
            if(ofGetElapsedTimeMillis() - animationTime > 3000){
                
                //update the LetterLeaves but only so many at a time
                if(ofGetElapsedTimeMillis() - releaseTimer > 100 && leavesToRelease < letterList.size()){
                    leavesToRelease += 3;
                    releaseTimer = ofGetElapsedTimeMillis();
                }
                
                //for loop that checks only the leaves that have been released
                for(int i = 0; i < ofClamp(leavesToRelease, 0, letterList.size()); i++){
                    letterList[i].update();
                }
                
                
                
                if(letterFall == false){
                    
                    //go through and see if all the leaves have "hit" their spots
                    //then make them all fall at once at different rates
                    for(int i = 0; i < letterList.size(); i++){
                        if(letterList[i].hit == false){
                            break;
                        }
                        
                        //if we're at the end of the list and we're still
                        //checking then make them fall
                        if(i == letterList.size() - 1){
                            letterFall = true;
                            endTime = ofGetElapsedTimeMillis();
                            
                        }
                    }
                    
                } else {
                
                
                    //check to see if enough time has passed before setting
                    //"animating" to false again
                    if(ofGetElapsedTimeMillis() - endTime > 8000){
                        
                        for(int i = 0; i < letterList.size(); i++){
                            letterList[i].letterFall = true;
                        }
                        
                    }
                    
                    //wait for leaves to finish falling before wrapping up animation
                    if(ofGetElapsedTimeMillis() - endTime > 10000){
                        animating = false;
                        setSummer();
                        waitTimer = ofGetElapsedTimeMillis();
                        letterList.clear();
                        animationType = 1;
                    }

                    
                } 

            }
            
            
        }
        
        //---------- BIKES ----------
        else if(animationType == 1){
            
            
            messengerBike.update();

            if(messengerBike.done){
                animating = false;
                waitTimer = ofGetElapsedTimeMillis();
                animationType = 0;
                messengerBike.done = false;
            }


        }
    
        

    }
    
    

    
    //-------------------- Pick random message if waiting --------------------
    if(animating == false && checked.size() > 0){
        
        //if its been 20 seconds since the last message ended and
        //there are no new messages to display, pick a random message and
        //uncheck it so it will display.
        if(ofGetElapsedTimeMillis() - waitTimer > 20000){
            
            //cast as int and clamp to make sure no BAD ACCESS errors
            int randomIndex = (int)ofClamp(ofRandom(checked.size() - 1), 0, checked.size() - 1);
            
            checked[randomIndex] = false;
            
            
            //Then set the waitTimer to current time plus a few seconds so it
            //shows right away.
            waitTimer = ofGetElapsedTimeMillis() + 3000;
            
        }
        
        
        
    }
    
    
    
    
    
    
    
    
    
    
    
    //---------- SPACEBREW CONNECTION ----------
    //Send a boolean true repeatedly so people know if they are connected or not
    
    if(ofGetElapsedTimeMillis() - connectionTimer > 333){
        spacebrew.sendBoolean("connection", true);
        connectionTimer = ofGetElapsedTimeMillis();
    }
    
    

    
    
    
    
    
}


//--------------------------------------------------------------
void testApp::draw(){
    
    ofPushMatrix();{
        
        if(scaleToSmall){
            ofScale(0.85333, 0.85333);
        }
        
        
        //draw environment stuff
        ofBackground(255);
        
        //Background Sumer
        ofSetColor(currentSky);
        sky.draw(0, 0);
        
        //---------- BIKE BEHIND ----------
        //draw the bike behind the tree depending on the bike's state
        if(animating && animationType == 1 && messengerBike.drawBehind){
            messengerBike.draw();
        }
        
        ofSetColor(255, foregroundTrans);
        backgroundSummer.draw(0, 0);
        
        ofSetColor(255, 255 - foregroundTrans);
        backgroundFall.draw(0, 0);
        
        
        //draw clouds
        for(int i = 0; i < cloudList.size(); i++){
            cloudList[i].draw();
        }
        
        //---------- tree leaves ----------
        //draw leaves
        for(int i = leafList.size() - 1; i >= 0; i--){
            leafList[i].draw();
        }

        
        //---------- LETTER LEAVES ----------
        if(animationType == 0){
            
            
            //draw the LetterLeaves but only so many at a time
            for(int i = 0; i < ofClamp(leavesToRelease, 0, letterList.size()); i++){
                letterList[i].draw();
            }
            
            
        }
        
        
        //---------- BIKE FRONT ----------
        //draw the bike behind the tree depending on the bike's state
        if(animating && animationType == 1 && messengerBike.drawBehind == false){
            messengerBike.draw();
        }
        
        
        
                
        
        //draw debugging visuals
        if(debugVisuals){
            drawDebugVis();
        }

        
    }ofPopMatrix();
    
    
    
    //LOGO
    float logoScale = 1;
    ofSetColor(255);
    logo.draw(40, 0, logo.width * logoScale, logo.height * logoScale);
    
    
    
    //Draw Message status
//    if(messageList.size() > 0) {
//        
//        ofSetColor(53, 81, 111);
//        string messageStat = "Message " + ofToString(currentMessageShowing + 1) + " of " + ofToString(messageList.size());
//        int padding = 10;
//        
//        UIfont.drawString(messageStat, padding, logo.height * logoScale + 10);
//    }
    
    //Draw URL
    
    string mess = "To send a message, visit:";
    float messLength = UIfont.stringWidth(mess);
    
    string URL = "www.bit.ly/iana-and-oliver";
    float URLLength = UIfont.stringWidth(URL);
    
    int rightPadding = 15;
    int topPadding = 10;
    
    
    ofSetColor(255, 255 * 0.4);
    ofRect(ofGetWindowWidth() - URLLength - rightPadding - 10, 15, 500, 65);
    ofSetColor(36, 56, 77);
    UIfont.drawString(URL, ofGetWindowWidth() - URLLength - rightPadding, topPadding + UIfont.getLineHeight() - 5);
    
    
//    UIfont.drawString(mess, ofGetWindowWidth() - messLength - rightPadding, topPadding + UIfont.getLineHeight() - 5);
//    ofSetColor(180, 0, 0);
//    UIfont.drawString(URL, ofGetWindowWidth() - URLLength - rightPadding, topPadding + UIfont.getLineHeight() * 2 - 10);
    
}





//--------------------------------------------------------------
void testApp::setSummer(){
    
    //set sky goal
    goalSky = summerSky;
    
    //set foreground transparency to switch between images
    foregroundGoalTrans = 255;
    
    //set the goal of all the leaves to the new colors
    for(int i = 0; i < leafList.size(); i++){
        leafList[i].setSummer();
    }
    
    //change cloud speed
    for(int i = 0; i < cloudList.size(); i++){
        cloudList[i].summer = true;
    }

    
}

//--------------------------------------------------------------
void testApp::setFall(){
    
    //set sky goal
    goalSky = fallSky;
    
    //set foreground transparency to switch between images
    foregroundGoalTrans = 0;
    
    //set the goal of all the leaves to the new colors
    for(int i = 0; i < leafList.size(); i++){
        leafList[i].setFall();
    }
    
    //change cloud speed
    for(int i = 0; i < cloudList.size(); i++){
        cloudList[i].summer = false;
    }
    
}

//--------------------------------------------------------------
void testApp::onMessage(Spacebrew::Message &msg){
    
    if(msg.name == "incoming"){
        
        //parse string into pieces
        vector<string> pieces;
        pieces = ofSplitString(msg.value, "***");
        
        //Log data to file with formatting
        messageLog.append("Message:\n");
        messageLog.append(pieces[0]);
        messageLog.append("\n\nFrom:\n");
        messageLog.append(pieces[1]);
        messageLog.append("\n\n----------\n\n");
        
        //send buffer to file
        ofBufferToFile(fileName, messageLog);
        
        //store them locally in vectors so we can animate and stuff
        messageList.push_back(pieces[0]);
        senderList.push_back("-" + pieces[1]);
        checked.push_back(false);
        
    }
    
}




//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key == 'd' || key == 'D'){
        debugVisuals = !debugVisuals;
    }
    
    if(key == 's' || key == 'S'){
        setSummer();
    }
    
    if(key == 'f' || key == 'F'){
        setFall();
    }
    
    if(key == 'w' || key == 'W'){
        drawWind = !drawWind;
    }
    
    if(key == 'm' || key == 'M'){
        
        string m = "This is a test of the emergency broadcasting system. If this had been a real emergency, your face would have melted off or you'd be on fire right now. Count your blessings it isn't an emergency.";
    
        string s = "-Your mom";
        
        messageList.push_back(m);
        senderList.push_back(s);
        checked.push_back(false);
        
    }
    
    if(key == ' '){
        scaleToSmall = !scaleToSmall;
    }

    if(key == '1'){
        animationType = 0;
    }
    
    if(key == '2'){
        animationType = 1;
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::drawDebugVis(){
    
    //basic debug stuff
    ofSetColor(0);
    ofDrawBitmapString("Time:              " + ofToString(ofGetElapsedTimeMillis()), 10, 20);
    ofDrawBitmapString("Framerate:         " + ofToString(ofGetFrameRate()), 10, 35);
    ofDrawBitmapString("Num static leaves: " + ofToString(leafList.size()), 10, 50);
    ofDrawBitmapString("Animation Type:    " + ofToString(animationType), 10, 65);
    if(letterList.size() > 0){
    ofDrawBitmapString("Leaves Released:   " + ofToString(leavesToRelease/letterList.size()), 10, 80);
    }
    
    //message list and queue list
    ofDrawBitmapString("MessageList Entries:  " + ofToString(messageList.size()), 300, 20);
    ofDrawBitmapString("SenderList Entries:   " + ofToString(senderList.size()), 300, 35);

    
    //calculate how many entries have been checked
    int numChecked = 0;
    for(int i = 0; i < checked.size(); i++){
        if(checked[i]){
            numChecked++;
        }
    }
    ofDrawBitmapString("Entries Animated:     " + ofToString(numChecked), 300, 50);
    ofDrawBitmapString("Are we animating?:    " + ofToString(animating), 300, 65);
    
    //draw background colors where mouse is
    if(mouseX > 0 && mouseX < ofGetWindowWidth() && mouseY > 0 && mouseY < ofGetWindowHeight()){
        ofColor thisCol = backgroundSummer.getColor(mouseX, mouseY);
        ofDrawBitmapString("r: " + ofToString((int)thisCol.r) + " g: " + ofToString((int)thisCol.g) + " b: " + ofToString((int)thisCol.b) + " a: " + ofToString((int)thisCol.a), mouseX, mouseY);
        ofDrawBitmapString("X: " + ofToString(mouseX) + " Y: " + ofToString(mouseY), mouseX, mouseY + 20);
    }
    
    if(drawWind){
        ofSetColor(0);
        wind.draw();
    }
    
}









