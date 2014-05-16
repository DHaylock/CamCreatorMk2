#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetDataPathRoot("../Resources/data/");
    ofSetWindowTitle("Cam Creator");
    setupGuis();
    setupValues();
    setupCogCoordinates();
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
        ofLog(OF_LOG_ERROR, "ERROR: No Update Mode Selected");
        ofSystemAlertDialog("ERROR: No Update Mode Selected");
    }
    updateGUIelements();
}
//--------------------------------------------------------------
void ofApp::draw()
{
    //---------------Fancy Background ( Will add a none fancy background soon )---------------
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
        ofLog(OF_LOG_NOTICE,"ERROR: No Draw Mode Selected");
        ofSystemAlertDialog("ERROR: No Draw Mode Selected");
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
    if (key == '=')
    {
        ofVec3f centerEngraving(ofGetWidth()/2, ofGetHeight()/2,ZERO);
        engravingString = saveFileName+".cam";
        engravingChars = engravingFont.getStringAsPoints(engravingString);
            
            if (engravingChars.size() > 0)
            {
                for (int p = 0; p < engravingChars.size(); p++) {
                    EngravingText s;
                    for(int k = 0; k <(int)engravingChars[p].getOutline().size(); k++){
                        if( k!= 0)ofNextContour(true);
                        for(int i = 0; i < (int)engravingChars[p].getOutline()[k].size(); i++){
                            s.pts.push_back(ofVec3f(engravingChars[p].getOutline()[k].getVertices()[i].x+centerEngraving.x-150, engravingChars[p].getOutline()[k].getVertices()[i].y+centerEngraving.y-150,0).rotated(90,centerEngraving, ofVec3f(0,0,1)));
                        }
                        engravingStringPts.push_back(s);
                    }
                }
            }
    }
    if (key == ']')
    {
        engravingString.clear();
        engravingChars.clear();
        engravingStringPts.clear();
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
            importMode(dragInfo.files[0]);
        }
    }
    else
    {
        //Nothing
        ofSystemAlertDialog("Please Turn The Import Toggle to Active");
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
    save = false;
    //mode = true;
    bProgressNextRotation = false;
    rotatedAmount = ZERO;
    nPts = ZERO;
    playbackCounter = ZERO;
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
    engravingFont.loadFont("mono.ttf", 20,true, true,true);
    engravingString = "YourCamName.cam";
    engravingChars = engravingFont.getStringAsPoints(engravingString);

        if (engravingChars.size() > 0)
    {
        for (int p = 0; p < engravingChars.size(); p++) {
            EngravingText s;
            for(int k = 0; k <(int)engravingChars[p].getOutline().size(); k++){
                if( k!= 0)ofNextContour(true);
                for(int i = 0; i < (int)engravingChars[p].getOutline()[k].size(); i++){
                    s.pts.push_back(ofVec3f(engravingChars[p].getOutline()[k].getVertices()[i].x+center.x-150, engravingChars[p].getOutline()[k].getVertices()[i].y+center.y-150,0).rotated(90,center, ofVec3f(0,0,1)));
                    
                }
                engravingStringPts.push_back(s);
            }
        }
    }
    
    
    bDrawnAnything = false;
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
    guiSave = new ofxUICanvas(870,0,600,33);
    guiSave->setName("Save");
    guiSave->setTheme(OFX_UI_THEME_HAYLOCK);
    guiSave->addWidgetRight(new ofxUITextInput(LENGTH, "Save Name", "FileName", OFX_UI_FONT_MEDIUM));
    guiSave->addWidgetRight(new ofxUILabelButton("Save Files", false,LENGTH/2,HEIGHT,OFX_UI_FONT_MEDIUM));
    guiSave->addWidgetRight(new ofxUILabelButton("Show Save Folder",false));
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
    guiPlayback->addWidgetRight(new ofxUILabelToggle("Save Engraving",false,LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false));
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
    //guiRecord->addWidgetDown(new ofxUILabelToggle("Map Values",false, LENGTH, HEIGHT,OFX_UI_FONT_MEDIUM,false)); //Experiment
    guiRecord->addWidgetDown(new ofxUIRangeSlider("Limiter",0,592,50,500,LENGTH,HEIGHT));
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
    guiCamera = new ofxUICanvas(525,140,LENGTH,800);
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
        ofCircle(pts[i].x, pts[i].y,3);
    }
    ofEndShape(true);
    
    drawCenterCog();
    ofNoFill();
    ofSetCircleResolution(100);
    //ofSetColor(0, 0, 0,200);
    //ofCircle(ofGetWidth()/2, ofGetHeight()/2, innerCircleLimit);
    //ofCircle(ofGetWidth()/2, ofGetHeight()/2, outerCircleLimit);
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
        ofSystemAlertDialog("Error: File Loaded");
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
                            pts[nPts].x = ofClamp(m.getArgAsInt32(0),innerCircleLimit,outerCircleLimit);
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
        ofDrawBitmapString("Start Point", pts[0]);
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
        ofDrawBitmapString("Start Point", pts[0]);
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
                ofDrawBitmapString("Start Point", pts[0]);
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
            ofDrawBitmapString("Start Point", pts[0]);
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
    
    if (saveEngraving == true) {
        drawEngraving();
    }
    else
    {
        
    }
    
    ofPopMatrix();
    ofPopMatrix();
    ofPopMatrix();
    
    if (showCamera == true) {
        ofPopMatrix();
        camera.end();
    }
}
//--------------------------------------------------------------
void ofApp::setupCogCoordinates()
{
    s.setCircleResolution(50);
    s.circle(ofGetWidth()/2,ofGetHeight()/2, 100);
    s.close();// Outer Cog
    s.circle(ofGetWidth()/2, ofGetHeight()/2, 20);
    s.close();// Inner Cog
    s.circle(ofGetWidth()/2-50, ofGetHeight()/2, 10);
    s.close();// Left Cog
    s.circle(ofGetWidth()/2+50, ofGetHeight()/2, 10);
    s.close();// Right Cog
    s.circle(ofGetWidth()/2, ofGetHeight()/2-50, 10);
    s.close();// Top Cog
    s.circle(ofGetWidth()/2, ofGetHeight()/2+50, 10);
    s.close();// Bottom Cog
    
    d = s.getOutline();
    
    if (d.size() >0) {
        for (int i = 0; i < d.size(); i++) {
            for (int j = 1; j < d[i].size()-1; j++) { // Odd centre point is drawn
                if (i == 0)
                {
                    outerCog.push_back(ofVec3f(d[i][j].x,d[i][j].y,0));
                }
                else if (i == 1)
                {
                    centerCog.push_back(ofVec3f(d[i][j].x,d[i][j].y,0));
                }
                else if (i == 2)
                {
                    leftPinCog.push_back(ofVec3f(d[i][j].x,d[i][j].y,0));
                }
                else if (i == 3)
                {
                    rightPinCog.push_back(ofVec3f(d[i][j].x,d[i][j].y,0));
                }
                else if (i == 4)
                {
                    topPinCog.push_back(ofVec3f(d[i][j].x,d[i][j].y,0));
                }
                else if (i == 5)
                {
                    bottomPinCog.push_back(ofVec3f(d[i][j].x,d[i][j].y,0));
                }
            }
        }
    }
}
//--------------------------------------------------------------
void ofApp::drawEngraving()
{
    
    ofPushMatrix();
    //ofVec3f center(ofGetWidth()/2-200, ofGetHeight()/2,0);
    //ofTranslate(center);
    ofPushStyle();
    ofSetColor(0, 0, 0);
    ofNoFill();
    ofBeginShape();
    if (engravingStringPts.size() > 0)
    {
        for (int p = 0; p < engravingStringPts.size(); p++) {
            for(int k = 0; k <(int)engravingStringPts[p].pts.size(); k++){
                ofVertex(engravingStringPts[p].pts[k].x, engravingStringPts[p].pts[k].y);
            }
            ofEndShape(true);
        }
    }
    ofPopStyle();
    ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::drawCenterCog()
{
    ofPushStyle();
    ofSetColor(0,200);
    ofNoFill();
    if (outerCog.size() > 0) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        ofBeginShape();
        for (int i = 1; i < outerCog.size()-1; i++) {
            ofVertex(outerCog[i].x, outerCog[i].y);
        }
        ofEndShape(true);
    }
    if (centerCog.size() > 0) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        ofBeginShape();
        for (int i = 1; i < centerCog.size()-1; i++) {
            ofVertex(centerCog[i].x, centerCog[i].y);
        }
        ofEndShape(true);
    }
    if (leftPinCog.size() > 0) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        ofBeginShape();
        for (int i = 1; i < leftPinCog.size()-1; i++) {
            ofVertex(leftPinCog[i].x, leftPinCog[i].y);
        }
        ofEndShape(true);
    }
    if (rightPinCog.size() > 0) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        ofBeginShape();
        for (int i = 1; i < rightPinCog.size()-1; i++) {
            ofVertex(rightPinCog[i].x, rightPinCog[i].y);
        }
        ofEndShape(true);
    }
    if (topPinCog.size() > 0) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        ofBeginShape();
        for (int i = 1; i < topPinCog.size()-1; i++) {
            ofVertex(topPinCog[i].x, topPinCog[i].y);
        }
        ofEndShape(true);
    }
    if (bottomPinCog.size() > 0) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        ofBeginShape();
        for (int i = 1; i < bottomPinCog.size()-1; i++) {
            ofVertex(bottomPinCog[i].x, bottomPinCog[i].y);
        }
        ofEndShape(true);
    }
    /*ofSetCircleResolution(100);
    ofCircle(ofGetWidth()/2, ofGetHeight()/2, 100);
    ofCircle(ofGetWidth()/2, ofGetHeight()/2, 20);
    ofCircle(ofGetWidth()/2-50, ofGetHeight()/2, 10);
    ofCircle(ofGetWidth()/2+50, ofGetHeight()/2, 10);
    ofCircle(ofGetWidth()/2, ofGetHeight()/2-50, 10);
    ofCircle(ofGetWidth()/2, ofGetHeight()/2+50, 10);*/
    ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "Record")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        guiRecord->setVisible(toggle->getValue());
        guiCamera->setVisible(false);
        rPmode = true;
    }
    else if(e.widget->getName() == "OSC")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        debugMode = toggle->getValue();
        guiCamera->setVisible(false);
    }
    else if(e.widget->getName() == "Playback")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        guiPlayback->setVisible(toggle->getValue());
        rPmode = false;
    }
    else if(e.widget->getName() == "Import")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        guiImport->setVisible(toggle->getValue());
        guiCamera->setVisible(false);
    }
    else if(e.widget->getName() == "Active")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
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
    else if(e.widget->getName() == "Save Engraving")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        saveEngraving = toggle->getValue();
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
    else if(e.widget->getName() == "Limiter")
    {
        ofxUIRangeSlider *rslider = (ofxUIRangeSlider *) e.widget;
        innerCircleLimit = rslider->getScaledValueLow();
        outerCircleLimit = rslider->getScaledValueHigh();
    }
    else if(e.widget->getName() == "Clear")
    {
        ofxUIButton *toggle = (ofxUIButton *) e.widget;
        nPts = 0;
        rotatedAmount = 0;
        dxfPts.clear();
    }
    else if(e.widget->getName() == "Save Name")
    {
        ofxUITextInput * textField = (ofxUITextInput *) e.widget;
        saveFileName = textField->getTextString();
        engravingString = textField->getTextString();
    }
    else if(e.widget->getName() == "Save Files")
    {
        ofxUIButton *toggle = (ofxUIButton *) e.widget;
        save = true;
        saveFiles();
        return;
    }
    else if(e.widget->getName() == "Show Save Folder")
    {
        ofxUIButton *toggle = (ofxUIButton *) e.widget;
        ofSystemLoadDialog("Save Folder",true,"../Resources/data/");
    }
    else if(e.widget->getName() == "Record Data")
    {
        ofxUIButton *toggle = (ofxUIButton *) e.widget;
        nPts = 0;
        rotatedAmount = 0;
        isRecording = true;
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
    }
    else if(e.widget->getName() == "Gradient_Color_2")
    {
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        bgColor2.r = sampler->getColor().r;
        bgColor2.g = sampler->getColor().g;
        bgColor2.b = sampler->getColor().b;
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
    
    //ofDirectory dir(saveFileName);
    //if(!dir.exists())
    //{
    //    dir.create(true);
    //}
    //dir.setWriteable(true);
    
    //ofSystemSaveDialog(saveFileName, "Save File");
    ofxSVG svg;
    //Add the Data points
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
        ofLog(OF_LOG_ERROR, "ERROR: No Points to Save");
        ofSystemAlertDialog("ERROR: No Points to Save");
    }
    svg.endPolygon();
    
    //Add the Center Spool
    svg.addLayer("Center Cog");
    svg.circle(ofGetWidth()/2,ofGetHeight()/2, 100);
    svg.circle(ofGetWidth()/2-50, ofGetHeight()/2, 10);
    svg.circle(ofGetWidth()/2+50, ofGetHeight()/2, 10);
    svg.circle(ofGetWidth()/2, ofGetHeight()/2+50, 10);
    svg.circle(ofGetWidth()/2, ofGetHeight()/2-50, 10);
    
    //Save to Data Folder
    svg.saveToFile(saveFileName + ".svg");
    
    //Save XML Data
    lastTagNumber	= outputXML.addTag("CAMCOORDS");
    for (int i = ZERO; i < nPts; i++) {
        
        if( outputXML.pushTag("CAMCOORDS", lastTagNumber) ){
            int tagNum = outputXML.addTag("PT");
            outputXML.setValue("PT:X", pts[i].x, tagNum);
            outputXML.setValue("PT:Y",pts[i].y, tagNum);
            outputXML.setValue("PT:VIX",ofMap(pts[i].x/10,23,150,250,25), tagNum);
            outputXML.popTag();
        }
    }
    outputXML.saveFile(saveFileName + ".xml");
    
    if (nPts > 0) {
        for (int i = 0; i < nPts; i++) {
            dxfPts.push_back(ofVec3f(pts[i].x,pts[i].y,0));
        }
    }
    dxf.writeFile(saveFileName);
    dxf.addPoints(outerCog, false);
    dxf.addPoints(centerCog, false);
    dxf.addPoints(leftPinCog, false);
    dxf.addPoints(rightPinCog, false);
    dxf.addPoints(topPinCog, false);
    dxf.addPoints(bottomPinCog, false);
    
    if (saveEngraving == true) {
        
        dxf.addPoints(dxfPts, false);
        
        if (engravingStringPts.size() > 0)
        {
            for (int i = 0; i < engravingStringPts.size(); i++) {
                if (i <= engravingStringPts.size())
                {
                    dxf.addPoints(engravingStringPts[i].pts, false);
                }
                else
                {
                    dxf.addPoints(engravingStringPts.back().pts, true);
                }
            }
        }
    }
    else if(saveEngraving == false)
    {
        dxf.addPoints(dxfPts, true);
    }
    else
    {
        
    }
    
    dxf.endFile();
    
    //Save Screen Shot for Reference
    ofImage img;
    img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
    img.saveImage(saveFileName + ".png");
}