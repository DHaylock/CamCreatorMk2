#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowTitle("Cam Creator");
    setupGuis();
    setupValues();
    //setupOsc();
}
//--------------------------------------------------------------
void ofApp::update(){
    
    //---------------Select Which Mode---------------
    if (rPmode == true)
    {
        recordMode();
        setupOsc();
    }
    else if(rPmode == false)
    {
        playbackMode();
    }
    else
    {
        ofLog(OF_LOG_ERROR, "ERROR: No Mode Selected");
    }
    
    updateGUIelements();
    
}
//--------------------------------------------------------------
void ofApp::draw()
{
    //---------------Fancy Background ( Will add a none fancy background soon) ---------------
    ofBackgroundGradient(bgColor1,bgColor2, OF_GRADIENT_CIRCULAR);
    
    //---------------Select Which Drawing Mode---------------
    if (rPmode == true)
    {
        drawRecording();
    }
    else if(rPmode == false)
    {
        drawPlayback();
    }
    else
    {
        ofLog(OF_LOG_NOTICE,"ERROR: No Valid Mode Selected");
    }
    
    drawCreatorArm();
    
}
//--------------------------------------------------------------
void ofApp::exit()
{
    //---------------Save GUI Values---------------
    guiPlayback->saveSettings("GUI/playbackSettings.xml");
    guiRecord->saveSettings("GUI/recordSettings.xml");
    guiImport->saveSettings("GUI/importSettings.xml");
    guiCamera->saveSettings("GUI/cameraSettings.xml");
    guiDebug->saveSettings("GUI/debugSettings.xml");
    
    
    //---------------Delete Pointers---------------
    delete guiTab;
    delete guiTopBar;
    delete guiPlayback;
    delete guiImport;
    delete guiDebug;
    delete guiRecord;
    delete guiSave;
    delete colorSampler;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (key)
	{
        case 'h':
            guiTab->toggleVisible();
            break;
        case 'S':
            saveFiles();
            break;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}
//-------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    
    if (bImportMode == true)
    {
        
        if (dragInfo.files.size() > 0) {
            //ofLog(OF_LOG_ERROR, "FILE Dragged on "+ ofToString(dragInfo.files));
            importMode(dragInfo.files[0]);
        }
        
    }
    else
    {
        //Nothing
    }
    
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::setupValues()
{
    //---------------Screen Stuff---------------
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    
    //---------------Variable Setup---------------
    recordedPoint = false;
    /*showShape = false;
     showLines = false;
     showCircles = false;
     showData = false;
     showOutline = false;
     showCamera = false;
     drawShader = false;*/
    save = false;
    mode = true;
    bProgressNextRotation = false;
    
    //lineColor = ofColor::white;
    //shapeColor = ofColor::white;
    //bgColor1 = ofColor::gray;
    //bgColor2 = (1, 17, 30, 255);
    
    rotatedAmount = ZERO;
    nPts = ZERO;
    //captureRate = 15;
    //rotateAmount.set(ZERO,ZERO,1);
    //speedOfRotation = 1.0f;
    //speedOfPlaybackRotation = 1.0f;
    //playbackRotateAmount.set(ZERO,ZERO,1.0f);
    //debugArmPos = 300.0f;
    playbackCounter = ZERO;
    //playbackRotationSpeed = 1;
    //playbackRotationTime = 20000;
    resetValue = 360;
    pointTester = ZERO;
    
    xAxisMin.set(-100,ZERO,ZERO);
    xAxisMax.set(100,ZERO,ZERO);
    yAxisMin.set(ZERO,-100,ZERO);
    yAxisMax.set(ZERO,100,ZERO);
    zAxisMin.set(ZERO,ZERO,-100);
    zAxisMax.set(ZERO,ZERO,100);
    
    lastTagNumber	= ZERO;
    pointCount		= ZERO;
    lineCount		= ZERO;
    
    ofVec3f center(ofGetWidth()/2, ofGetHeight()/2,ZERO);
    
    xAxisMin += center;
    xAxisMax += center;
    yAxisMin += center;
    yAxisMax += center;
    zAxisMin += center;
    zAxisMax += center;
    
    OSC_PORT = OSCPORT;
    OSC_HOST = OSCHOST;
    
    saveFileName = "Test";
    
    bDrawnAnything = false;
    //camera.setGlobalPosition(-600,527,1001);
    
}
//--------------------------------------------------------------
void ofApp::setupGuis()
{
    setupPlaybackGui();
    setupRecordGui();
    setupDebugGui();
    setupImportGui();
    setupCameraGui();
    setupSaveGui();
    
    guiPlayback->loadSettings("GUI/playbackSettings.xml");
    guiRecord->loadSettings("GUI/recordSettings.xml");
    guiImport->loadSettings("GUI/importSettings.xml");
    guiCamera->loadSettings("GUI/cameraSettings.xml");
    guiDebug->loadSettings("GUI/debugSettings.xml");
    
    guiTab = new ofxUITabBar();
    guiTab->setWidth(ofGetWidth());
    guiTab->setHeight(30);
    guiTab->addWidgetRight(new ofxUILabel("Cam Creator",OFX_UI_FONT_LARGE));
    guiTab->addFPS(OFX_UI_FONT_LARGE);
    guiTab->addLabelToggle("Fullscreen", false,100,30);
    guiTab->addCanvas(guiRecord);
    guiTab->addCanvas(guiPlayback);
    guiTab->addCanvas(guiImport);
    guiTab->addCanvas(guiDebug);
    guiTab->addCanvas(guiSave);
    ofAddListener(guiTab->newGUIEvent, this, &ofApp::guiEvent);
}
//--------------------------------------------------------------
void ofApp::setupSaveGui()
{
    
    guiSave = new ofxUICanvas(870,0,400,33);
    guiSave->setName("Save");
    guiSave->setTheme(OFX_UI_THEME_HAYLOCK);
    guiSave->addWidgetRight(new ofxUITextInput(LENGTH, "Save Name", "FileName", OFX_UI_FONT_MEDIUM));
    guiSave->addWidgetRight(new ofxUILabelButton("Save Files", false,LENGTH/2,HEIGHT,OFX_UI_FONT_MEDIUM));
    ofAddListener(guiSave->newGUIEvent, this, &ofApp::guiEvent);
    
}
//--------------------------------------------------------------
void ofApp::setupPlaybackGui()
{
    colorSampler = new ofImage();
    colorSampler->loadImage("GUI/colorSamplerImage.png");
    
    guiPlayback = new ofxUICanvas(ZERO,POSY,LENGTH*2,800);
    guiPlayback->setTheme(OFX_UI_THEME_HAYLOCK);
    guiPlayback->setName("Playback");
    guiPlayback->addWidgetDown(new ofxUILabel("Playback", OFX_UI_FONT_LARGE));
    guiPlayback->addSpacer();
    guiPlayback->addWidgetDown(new ofxUILabel("Play Speed", OFX_UI_FONT_MEDIUM));
    guiPlayback->addWidgetRight(new ofxUINumberDialer(ZERO, 1000, 500, 2, "Play_Speed", OFX_UI_FONT_MEDIUM));
    guiPlayback->addWidgetRight(new ofxUILabel("Play Time", OFX_UI_FONT_MEDIUM));
    guiPlayback->addWidgetRight(new ofxUINumberDialer(ZERO, 30000, 1, 2, "Play_Rate", OFX_UI_FONT_MEDIUM));
    guiPlayback->addWidgetDown(new ofxUILabel("Degrees Rotated", OFX_UI_FONT_MEDIUM));
    guiPlayback->addWidgetRight(new ofxUINumberDialer(ZERO, 360, 1, 2, "Degrees_RotatedPB", OFX_UI_FONT_MEDIUM));
    guiPlayback->addSpacer();
    guiPlayback->addWidgetDown(new ofxUILabelToggle("Show Camera",false,LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false));
    guiPlayback->addWidgetRight(new ofxUILabelToggle("Show Lines",false,LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false));
    guiPlayback->addWidgetDown(new ofxUILabelToggle("Show Shape",false,LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false));
    guiPlayback->addWidgetRight(new ofxUILabelToggle("Show Outline",false,LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false));
    guiPlayback->addWidgetDown(new ofxUILabelToggle("Show Circles",false,LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false));
    guiPlayback->addWidgetRight(new ofxUILabelToggle("Show Data",false,LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false));
    guiPlayback->addSpacer();
    guiPlayback->addWidgetDown(new ofxUILabel("Shape Color", OFX_UI_FONT_MEDIUM));
    guiPlayback->addWidgetDown(new ofxUIImageSampler(LENGTH, LENGTH, colorSampler, "Shape_Color"));
    guiPlayback->addWidgetRight(new ofxUIImageSampler(LENGTH, LENGTH, colorSampler, "Line_Color"));
    guiPlayback->addWidgetNorthOf(new ofxUILabel("Line Color", OFX_UI_FONT_MEDIUM),"Line_Color");
    guiPlayback->addWidgetSouthOf(new ofxUIIntSlider("SAlpha", 0.0, 255.0, 255, LENGTH, HEIGHT),"Shape_Color");
    guiPlayback->addWidgetSouthOf(new ofxUIIntSlider("LAlpha", 0.0, 255.0, 255, LENGTH, HEIGHT),"Line_Color");
    guiPlayback->autoSizeToFitWidgets();
    ofAddListener(guiPlayback->newGUIEvent, this, &ofApp::guiEvent);
    guiPlayback->setVisible(false);
}
//--------------------------------------------------------------
void ofApp::setupRecordGui()
{
    
    guiRecord = new ofxUICanvas(ZERO,POSY,LENGTH,300);
    guiRecord->setTheme(OFX_UI_THEME_HAYLOCK);
    guiRecord->setName("Record");
    guiRecord->addWidgetDown(new ofxUILabel("Record", OFX_UI_FONT_LARGE));
    guiRecord->addSpacer();
    guiRecord->addWidgetDown(new ofxUILabelButton("Record Data", false,LENGTH,HEIGHT,OFX_UI_FONT_LARGE));
    guiRecord->addWidgetDown(new ofxUILabelButton("Clear", false,LENGTH,HEIGHT,OFX_UI_FONT_LARGE));
    //guiRecord->addWidgetDown(new ofxUILabelButton("Save", false,LENGTH,HEIGHT,OFX_UI_FONT_MEDIUM));
    guiRecord->addSpacer();
    guiRecord->addWidgetDown(new ofxUILabel("Record Settings", OFX_UI_FONT_MEDIUM));
    guiRecord->addSpacer();
    guiRecord->addWidgetDown(new ofxUILabel("Record Rate", OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetRight(new ofxUINumberDialer(ZERO, 1000, 100, 1, "Record_Rate", OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetDown(new ofxUILabel("Rotation Rate", OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetRight(new ofxUINumberDialer(ZERO, 2, 1, 2, "Rotation_Rate", OFX_UI_FONT_MEDIUM));
    guiRecord->addSpacer();
    guiRecord->addWidgetDown(new ofxUILabelToggle("OSC or Debug",false, LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false));
    guiRecord->addSpacer();
    guiRecord->addWidgetDown(new ofxUILabel("OSC",OFX_UI_FONT_MEDIUM));
    guiRecord->addSpacer();
    guiRecord->addWidgetDown(new ofxUILabel("OSC Host",OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetRight(new ofxUITextInput(LENGTH, "HOST", "localhost", OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetDown(new ofxUILabel("OSC Port", OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetRight(new ofxUINumberDialer(ZERO, 100000, 100, 0, "OSC_PORT", OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetDown(new ofxUIBiLabelSlider(LENGTH,350, 800, 350.0f,"Arm Pos", "Outer","Inner", OFX_UI_FONT_MEDIUM));
    guiRecord->addSpacer();
    guiRecord->addWidgetDown(new ofxUILabel("Points Added", OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetRight(new ofxUINumberDialer(ZERO, 1500, 1, 2, "Points_Added", OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetDown(new ofxUILabel("Degrees Rotated", OFX_UI_FONT_MEDIUM));
    guiRecord->addWidgetRight(new ofxUINumberDialer(ZERO, 360, 1, 2, "Degrees_Rotated", OFX_UI_FONT_MEDIUM));
    guiRecord->autoSizeToFitWidgets();
    guiRecord->setVisible(false);
    ofAddListener(guiRecord->newGUIEvent, this, &ofApp::guiEvent);
    
}
//--------------------------------------------------------------
void ofApp::setupDebugGui()
{
    colorSampler = new ofImage();
    colorSampler->loadImage("GUI/colorSamplerImage.png");
    string textString = "Hello, this application allows you to create CAM cogs from realtime and prerecorded data. To change the background color simply select a color from the pallete below.";
    
    guiDebug = new ofxUICanvas(ZERO,POSY,LENGTH*2,800);
    guiDebug->setTheme(OFX_UI_THEME_HAYLOCK);
    guiDebug->setName("Misc");
    guiDebug->addWidgetDown(new ofxUITextArea("Info", textString, LENGTH*2, 50));
    guiDebug->addSpacer();
    guiDebug->addWidgetDown(new ofxUILabel("Background Color", OFX_UI_FONT_LARGE));
    guiDebug->addSpacer();
    guiDebug->addWidgetDown(new ofxUIImageSampler(LENGTH, LENGTH, colorSampler, "Gradient_Color_1"));
    guiDebug->addWidgetRight(new ofxUIImageSampler(LENGTH, LENGTH, colorSampler, "Gradient_Color_2"));
    guiDebug->addWidgetSouthOf(new ofxUIIntSlider("BG1Alpha", 0.0, 255.0, 255, LENGTH, HEIGHT),"Gradient_Color_1");
    guiDebug->addWidgetSouthOf(new ofxUIIntSlider("BG2Alpha", 0.0, 255.0, 255, LENGTH, HEIGHT),"Gradient_Color_2");
    
    guiDebug->autoSizeToFitWidgets();
    ofAddListener(guiDebug->newGUIEvent, this, &ofApp::guiEvent);
    guiDebug->setVisible(true);
}
//--------------------------------------------------------------
void ofApp::setupCameraGui()
{
    guiCamera = new ofxUICanvas(264,170,LENGTH,800);
    guiCamera->setTheme(OFX_UI_THEME_HAYLOCK);
    guiCamera->addWidgetDown(new ofxUILabel("Camera Controls", OFX_UI_FONT_LARGE));
    guiCamera->addWidgetDown(new ofxUILabelToggle("Use Mouse",false, LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false));
    guiCamera->addWidget(new ofxUI2DPad("Camera Position",ofPoint(0,ofGetWidth()), ofPoint(0,ofGetHeight()), ofPoint(0,320), LENGTH-2,LENGTH-2));
    guiCamera->autoSizeToFitWidgets();
    guiCamera->setVisible(false);
}
//--------------------------------------------------------------
void ofApp::setupImportGui()
{
    string textString = "Ensure that your data is formatted like the below example \n<pts> \n  <xpt>value</xpt>\n  <ypt>value</ypt>\n<pts>";
    
    
    guiImport = new ofxUICanvas(ZERO,POSY,ofGetWidth()/2,300);
    guiImport->setTheme(OFX_UI_THEME_HAYLOCK);
    guiImport->setName("Import");
    guiImport->addWidgetDown(new ofxUILabel("Import", OFX_UI_FONT_LARGE));
    //guiImport->addWidgetDown(new ofxUITextArea("Info",textString,LENGTH,50));
    guiImport->addWidgetRight(new ofxUILabelToggle("Active",false, LENGTH-50, HEIGHT,OFX_UI_FONT_MEDIUM,false));
    guiImport->addWidgetDown(new ofxUILabel("Imported File: ", OFX_UI_FONT_MEDIUM));
    guiImport->addWidgetRight(new ofxUITextArea("File Imported","Nil",ofGetWidth(),20,OFX_UI_FONT_SMALL));
    guiImport->addWidgetDown(new ofxUILabel("No File Loaded", OFX_UI_FONT_MEDIUM));
    guiImport->addWidgetDown(new ofxUILabel("Test", OFX_UI_FONT_MEDIUM));
    guiImport->autoSizeToFitWidgets();
    ofAddListener(guiImport->newGUIEvent, this, &ofApp::guiEvent);
    guiImport->setVisible(false);
}
//--------------------------------------------------------------
void ofApp::updateGUIelements()
{
    ((ofxUINumberDialer *) guiRecord->getWidget("Points_Added"))->setValue(nPts);
    ((ofxUINumberDialer *) guiRecord->getWidget("Degrees_Rotated"))->setValue(rotatedAmount);
    ((ofxUINumberDialer *) guiPlayback->getWidget("Degrees_RotatedPB"))->setValue(playbackCounter);
}
//--------------------------------------------------------------
void ofApp::drawRecording()
{
    ofSetLineWidth(2);
    ofSetColor(lineColor);
    ofNoFill();
    ofBeginShape();
    for (int i = ZERO; i < nPts; i++){
        ofVertex(pts[i].x, pts[i].y);
        ofCircle(pts[i].x,pts[i].y,3);
    }
    ofEndShape(true);
    
    drawCenterCog();
}
//--------------------------------------------------------------
void ofApp::playbackMode()
{
    if (playbackCounter >= 359.5) {
        playbackCounter = 0;
    }
    
    float timer = ofGetElapsedTimeMillis() - playbackStartTime;
    if(timer >= playbackEndTime && !bProgressNextRotation)
    {
        bProgressNextRotation = true;
        playbackCounter += playbackRotationSpeed;
        // rotate the points
        /*if (showCircles == true) {
         ofVec3f center(ofGetWidth()/2, ofGetHeight()/2,0);
         // move the points so that their center (ofGetW/2, ofGetH/2) is at 0,0,0
         for (int i = ZERO; i < nPts; i++){
         pts[i]-= center;
         }
         
         for (int i = ZERO; i < nPts; i++){
         pts[i] = pts[i].rotated(speedOfPlaybackRotation, playbackRotateAmount);
         }
         // move them back
         for (int i = 0; i < nPts; i++){
         pts[i]+= center;
         }
         }*/
        bProgressNextRotation = false;
        playbackStartTime = ofGetElapsedTimeMillis();
    }
}
//--------------------------------------------------------------
void ofApp::importMode(string file)
{
    ((ofxUITextArea *) guiImport->getWidget("File Imported"))->setTextString(file);
    
    //Reset the Value
    
    
	if (importedXMLData.loadFile(file))
    {
        ((ofxUILabel *) guiImport->getWidget("No File Loaded"))->setLabel("File Loaded");
        
        importlastTagNumber = 0;
        importPointCount = 0;
        importLineCount = 0;
        
        int numOfPtsTags = importedXMLData.getNumTags("CAMCOORDS:PT");
        ofLog(OF_LOG_ERROR,ofToString(numOfPtsTags));
        
        if (numOfPtsTags > 0) {
            importedXMLData.pushTag("CAMCOORDS",numOfPtsTags-1);
            
            int numOfPTs = importedXMLData.getNumTags("PT");
            
            if (numOfPTs > 0) {
                int totalToRead = MIN(numOfPTs, MAX_PTS);
                nPts = totalToRead;
                for(int i = 0; i < totalToRead; i++){
                    float x = importedXMLData.getValue("PT:X", 0, i);
                    float y = importedXMLData.getValue("PT:Y", 0, i);
                    pts[i].x = x;
                    pts[i].y = y;
                    ofLog(OF_LOG_ERROR,ofToString(pts[i]));
                    //nPts = i;
                }
                
            }
            
        }
        
    }
    else
    {
        ((ofxUILabel *) guiImport->getWidget("No File Loaded"))->setLabel("Unable to Load File");
    }
}
//--------------------------------------------------------------
void ofApp::recordMode()
{
    ofVec3f center(ofGetWidth()/2, ofGetHeight()/2,0);
    
    bDrawnAnything = true;
    
    if (isRecording == true) {
        // move the points so that their center (ofGetW/2, ofGetH/2) is at 0,0,0
        for (int i = ZERO; i < nPts; i++){
            pts[i]-= center;
        }
        
        // rotate the points
        for (int i = ZERO; i < nPts; i++){
            pts[i] = pts[i].rotated(speedOfRotation, rotateAmount);
        }
        
        // move them back
        for (int i = 0; i < nPts; i++){
            pts[i]+= center;
        }
        if (mode == true)
        {
            if (rotatedAmount <= 359.5)
            {
                // check for waiting messages
                while(oscReceive.hasWaitingMessages())
                {
                    // get the next message
                    message = "Incoming Data";
                    ofxOscMessage m;
                    oscReceive.getNextMessage(&m);
                    
                    // check messages
                    if(m.getAddress() == "/CamMakerData"){
                        
                        isRecording = true;
                        
                        // update the timer this frame
                        float timer = ofGetElapsedTimeMillis() - recordStartTime;
                        if(timer >= recordEndTime && !bNextPoint) {
                            bNextPoint = true;
                            ofMessage msg("Captured Point");
                            ofSendMessage(msg);
                            recordedPoint = true;
                            pts[nPts].x = m.getArgAsInt32(0);
                            pts[nPts].y = ofGetHeight()/2;
                            nPts++;
                            bNextPoint = false;
                            recordStartTime = ofGetElapsedTimeMillis();
                        }
                        
                        else{
                            recordedPoint = false;
                        }
                    }
                }
            }
            else
            {
                isRecording = false;
            }
        }
        
        if (mode ==false )
        {
            
            if (rotatedAmount <= 359.5){
                isRecording = true;
                
                float timer = ofGetElapsedTimeMillis() - recordStartTime;
                if(timer >= recordEndTime && !bNextPoint) {
                    bNextPoint = true;
                    ofMessage msg("Captured Point");
                    ofSendMessage(msg);
                    recordedPoint = true;
                    pts[nPts].x = debugArmPos;
                    pts[nPts].y = ofGetHeight()/2;
                    nPts++;
                    bNextPoint = false;
                    recordStartTime = ofGetElapsedTimeMillis();
                }
                else{
                    recordedPoint = false;
                }
            }
            
            else
            {
                isRecording = false;
            }
        }
        
        zAxisMin -= center;
        zAxisMax -= center;
        
        zAxisMin.rotate(speedOfRotation, rotateAmount);
        zAxisMax.rotate(speedOfRotation, rotateAmount);
        
        zAxisMin += center;
        zAxisMax += center;
    }
    else
    {
        
    }
    
    if (rotatedAmount <= 359.5) {
        rotatedAmount += rotateAmount.z;
    }
    else{
        rotatedAmount +=0;
    }
    
}
//--------------------------------------------------------------
void ofApp::drawCreatorArm()
{
    ofPushStyle();
    if (isRecording == true)
    {
        ofSetColor(255,255,0);
        ofSetLineWidth(2);
        ofLine(debugArmPos,0,debugArmPos,ofGetHeight());
        ofLine(0,ofGetHeight()/2,debugArmPos,ofGetHeight()/2);
        
        ofNoFill();
        if (recordedPoint == true) { ofSetColor(ofColor::green); }
        else { ofSetColor(ofColor::red); }
        ofCircle(debugArmPos,ofGetHeight()/2, 6);
    }
    ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::drawPlayback()
{
    if (showCamera == true) {
        camera.begin();
        ofPushMatrix();
        ofScale(-1, 1,1);
    }
    ofPushMatrix();
    ofTranslate(0,0,-100);
    ofVec3f center(ofGetWidth()/2, ofGetHeight()/2,0);
    
    ofPushMatrix();
    ofTranslate(center);
    ofRotateZ(playbackCounter);
    //ofRotate(playbackRotateAmount.z, 0, 0, 1);
    ofPushMatrix();
    ofTranslate(-center);
    
    //--------------------------------------
    if (showShape == true) {
        ofSetColor(shapeColor);
        ofFill();
        ofBeginShape();
        for (int i = ZERO; i < nPts; i++){
            ofVertex(pts[i].x, pts[i].y);
        }
        ofEndShape(true);
    }
    else
    {
    }
    //--------------------------------------
    if (showOutline == true) {
        ofPushStyle();
        ofSetLineWidth(2);
        ofSetColor(lineColor);
        ofNoFill();
        ofBeginShape();
        for (int i = ZERO; i < nPts; i++){
            ofVertex(pts[i].x, pts[i].y);
        }
        ofEndShape(true);
        ofPopStyle();
    }
    else
    {
    }
    //--------------------------------------
    if (showLines == true) {
        if (nPts > ZERO) {
            
            for (int i = ZERO; i < nPts; i++) {
                ofSetColor(lineColor);
                ofLine(ofGetWidth()/2, ofGetHeight()/2,pts[i].x,pts[i].y);
                ofSetColor(lineColor);
                ofCircle(pts[i].x,pts[i].y,2);
                ofDrawBitmapString(ofToString(pts[0]), pts[0]);
            }
        }
    }
    else
    {
    }
    //--------------------------------------
    if (showCircles == true) {
        if (nPts > ZERO) {
            for (int i = ZERO; i < nPts; i++) {
                ofSetColor(lineColor);
                ofCircle(pts[i].x,pts[i].y,5);
            }
            ofDrawBitmapString(ofToString(pts[0]), pts[0]);
        }
    }
    else
    {
    }
    
    //--------------------------------------
    if (showData == true) {
        if (nPts > ZERO) {
            for (int i = ZERO; i < nPts; i++) {
                ofSetColor(lineColor);
                ofSetLineWidth(2);
                ofNoFill();
                ofBeginShape();
                for (int i = ZERO; i < nPts; i++){
                    ofVertex(pts[i].x, pts[i].y);
                }
                ofEndShape(true);
                ofPopStyle();
            }
            ofFill();
            ofSetColor(0, 255, 0,200);
            ofDrawBitmapString("0 Degrees",pts[0].x+5,pts[0].y);
            ofCircle(pts[0].x,pts[0].y, 10);
            ofLine(center, pts[0]);
            
            ofSetColor(0, 0, 0,200);
            ofDrawBitmapString("90 Degrees",pts[nPts/4].x+5,pts[nPts/4].y);
            ofCircle(pts[nPts/4].x,pts[nPts/4].y, 10);
            ofLine(center, pts[nPts/4]);
            
            ofSetColor(0, 0,0,200);
            ofDrawBitmapString("180 Degrees",pts[nPts/2].x+5,pts[nPts/2].y);
            ofCircle(pts[nPts/2].x,pts[nPts/2].y, 10);
            ofLine(center, pts[nPts/2]);
            
            ofSetColor(0, 0,0,200);
            ofDrawBitmapString("270 Degrees",pts[nPts/4*3].x+5,pts[nPts/4*3].y);
            ofCircle(pts[nPts/4*3].x,pts[nPts/4*3].y, 10);
            ofLine(center, pts[nPts/4*3]);
            
            ofSetColor(255, 0, 0,200);
            ofDrawBitmapString("End Point",pts[nPts-1].x+10,pts[nPts-1].y);
            ofCircle(pts[nPts-1].x,pts[nPts-1].y, 10);
            ofLine(center, pts[nPts-1]);
        }
    }
    else
    {
    }
    
    drawCenterCog();
    
    ofPopMatrix();
    ofPopMatrix();
    ofPopMatrix();
    
    if (showCamera == true) {
        ofPopMatrix();
        camera.end();
    }
}
//--------------------------------------------------------------
void ofApp::drawCenterCog()
{
    
    ofPushStyle();
    ofSetColor(0,200);
    ofNoFill();
    ofSetCircleResolution(100);
    ofCircle(ofGetWidth()/2, ofGetHeight()/2, 100);
    ofCircle(ofGetWidth()/2, ofGetHeight()/2, 20);
    ofCircle(ofGetWidth()/2-50, ofGetHeight()/2, 10);
    ofCircle(ofGetWidth()/2+50, ofGetHeight()/2, 10);
    ofCircle(ofGetWidth()/2, ofGetHeight()/2-50, 10);
    ofCircle(ofGetWidth()/2, ofGetHeight()/2+50, 10);
    ofPopStyle();
    
}
//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "Record")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        guiRecord->setVisible(toggle->getValue());
        //recordMode = toggle->getValue();
        rPmode = true;
    }
    else if(e.widget->getName() == "OSC")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        //guiDebug->setVisible(toggle->getValue());
        debugMode = toggle->getValue();
    }
    else if(e.widget->getName() == "Playback")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        guiPlayback->setVisible(toggle->getValue());
        //playbackMode =  toggle->getValue();
        rPmode = false;
    }
    else if(e.widget->getName() == "Import")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        guiImport->setVisible(toggle->getValue());
        //bImportMode = toggle->getValue();
    }
    else if(e.widget->getName() == "Active")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        //dguiImport->setVisible(toggle->getValue());
        bImportMode = toggle->getValue();
    }
    else if(e.widget->getName() == "Record_Rate")
    {
        ofxUINumberDialer *dial = (ofxUINumberDialer *) e.widget;
        recordEndTime = (float)(dial->getValue());
    }
    else if(e.widget->getName() == "Rotation_Rate")
    {
        ofxUINumberDialer *dial = (ofxUINumberDialer *) e.widget;
        rotateAmount.set(0,0,(float)(dial->getValue()));
        speedOfRotation = dial->getValue();
    }
    else if(e.widget->getName() == "Play_Speed")
    {
        ofxUINumberDialer *dial = (ofxUINumberDialer *) e.widget;
        playbackRotationSpeed = dial->getValue();
        speedOfPlaybackRotation = dial->getValue();
        playbackRotateAmount.set(ZERO,ZERO,dial->getValue());
        
    }
    else if(e.widget->getName() == "OSC_PORT")
    {
        ofxUINumberDialer *dial = (ofxUINumberDialer *) e.widget;
        OSC_PORT = dial->getValue();
    }
    else if(e.widget->getName() == "Play_Rate")
    {
        ofxUINumberDialer *dial = (ofxUINumberDialer *) e.widget;
        playbackEndTime = dial->getValue();
        cout << dial->getValue() << endl;
    }
    else if(e.widget->getName() == "Shape_Color")
    {
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        shapeColor.r = sampler->getColor().r;
        shapeColor.g = sampler->getColor().g;
        shapeColor.b = sampler->getColor().b;
    }
    else if(e.widget->getName() == "Line_Color")
    {
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        lineColor.r = sampler->getColor().r;
        lineColor.g = sampler->getColor().g;
        lineColor.b = sampler->getColor().b;
    }
    else if(e.widget->getName() == "SAlpha")
    {
        ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
        shapeColor.a = slider->getValue();
    }
    else if(e.widget->getName() == "LAlpha")
    {
        ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
        lineColor.a = slider->getValue();
    }
    else if(e.widget->getName() == "Fullscreen")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());
    }
    else if(e.widget->getName() == "Show Camera")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        showCamera = toggle->getValue();
        guiCamera->setVisible(toggle->getValue());
    }
    else if(e.widget->getName() == "Show Lines")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        showLines = toggle->getValue();
    }
    else if(e.widget->getName() == "Show Shape")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        showShape = toggle->getValue();
    }
    else if(e.widget->getName() == "Show Data")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        showData = toggle->getValue();
    }
    else if(e.widget->getName() == "Show Circles")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        showCircles = toggle->getValue();
    }
    else if(e.widget->getName() == "Show Outline")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        showOutline = toggle->getValue();
    }
    else if(e.widget->getName() == "OSC or Debug")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        mode = toggle->getValue();
    }
    else if(e.widget->getName() == "Clear")
    {
        ofxUIButton *toggle = (ofxUIButton *) e.widget;
        nPts = 0;
        rotatedAmount = 0;
    }
    else if(e.widget->getName() == "Save Name")
    {
        ofxUITextInput * textField = (ofxUITextInput *) e.widget;
        saveFileName = textField->getTextString();
    }
    else if(e.widget->getName() == "Save Files")
    {
        ofxUIButton *toggle = (ofxUIButton *) e.widget;
        save = true;
        saveFiles();
        return;
    }
    else if(e.widget->getName() == "Record Data")
    {
        ofxUIButton *toggle = (ofxUIButton *) e.widget;
        nPts = 0;
        rotatedAmount = 0;
        isRecording = true;
    }
    else if(e.widget->getName() == "Draw Shader")
    {
        ofxUIButton *toggle = (ofxUIButton *) e.widget;
        drawShader =  toggle->getValue();
    }
    else if(e.widget->getName() == "Record/Playback")
    {
        ofxUIButton *toggle = (ofxUIButton *) e.widget;
        //rPmode =  toggle->getValue();
    }
    else if(e.widget->getName() == "Rotation Speed")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        
        int var = (int)(slider->getScaledValue());
        //rotateAmount.set(0,0,var);
    }
    else if(e.widget->getName() == "Arm Pos")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        debugArmPos = (int)(slider->getScaledValue());
    }
    else if(e.widget->getName() == "Gradient_Color_1")
    {
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        bgColor1.r = sampler->getColor().r;
        bgColor1.g = sampler->getColor().g;
        bgColor1.b = sampler->getColor().b;
        
        cout << bgColor1 << endl;
        
    }
    else if(e.widget->getName() == "Gradient_Color_2")
    {
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        bgColor2.r = sampler->getColor().r;
        bgColor2.g = sampler->getColor().g;
        bgColor2.b = sampler->getColor().b;
        cout << bgColor2 << endl;
    }
    else if(e.widget->getName() == "BG1Alpha")
    {
        ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
        bgColor1.a = slider->getValue();
    }
    else if(e.widget->getName() == "BG2Alpha")
    {
        ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
        bgColor2.a = slider->getValue();
    }
}
//--------------------------------------------------------------
void ofApp::setupOsc()
{
    oscReceive.setup(OSC_PORT);
}
//--------------------------------------------------------------
void ofApp::saveFiles()
{
    ofxSVG svg;
    svg.addLayer("Points");
    ofColor s = (lineColor.r,lineColor.g,lineColor.b);
    ofColor f = (shapeColor.r,shapeColor.g,shapeColor.b);
    svg.stroke("#000", 2);
    svg.fill("#FFF");
    svg.beginPolygon();
    if (nPts > 0) {
        for(int i = ZERO; i < nPts; i++)
        {
            svg.vertex(pts[i].x, pts[i].y);
        }
    }
    else
    {
        ofLog(OF_LOG_ERROR, "Error: No Points to Save");
    }
    svg.endPolygon();
    svg.circle(ofGetWidth()/2,ofGetHeight()/2, 100);
    svg.saveToFile(saveFileName + ".svg");
    lastTagNumber	= outputXML.addTag("CAMCOORDS");
    for (int i = ZERO; i < nPts; i++) {
        
        if( outputXML.pushTag("CAMCOORDS", lastTagNumber) ){
            int tagNum = outputXML.addTag("PT");
            outputXML.setValue("PT:X", pts[i].x, tagNum);
            outputXML.setValue("PT:Y",pts[i].y, tagNum);
            outputXML.popTag();
        }
    }
    
    outputXML.saveFile(saveFileName + ".xml");
    
    string hr;
    ofImage img;
    img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
    if(ofGetHours() < 12)
    {
        hr = " AM";
    }
    else
    {
        hr = " PM";
    }
    img.saveImage(saveFileName + ".png");
}



