#pragma once

//Import Libraries
#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxSVG.h"

//Setup Constants
#define OSCHOST "localhost" //change if separate computer
#define OSCPORT 12345
#define MAX_PTS 2000
#define LENGTH 255
#define HEIGHT 16*3/2
#define POSY 40
#define ZERO 0

class ofApp : public ofBaseApp{
    
public:
    
    //---------------OF Functions---------------
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    //---------------Custom Functions---------------
    void saveFiles();
    void setupValues();
    void recordMode();
    void drawRecording();
    void playbackMode();
    void drawPlayback();
    void setupOsc();
    void importMode(string file);
    void guiEvent(ofxUIEventArgs &e);
    
    void setupGuis();
    void setupTopGui();
    void setupRecordGui();
    void setupPlaybackGui();
    void setupDebugGui();
    void setupImportGui();
    void setupSaveGui();
    void setupCameraGui();
    void guiUpdates();
    void drawCenterCog();
    void drawCreatorArm();
    void updateGUIelements();
    
    //---------------Object Declaration---------------
    ofEasyCam camera;
    ofxXmlSettings outputXML;
    ofxXmlSettings importedXMLData;
    ofxOscReceiver oscReceive;
    ofImage *colorSampler;
    
    //---------------GUI Declaration---------------
    ofxUITabBar *guiTab;
    ofxUICanvas *guiTopBar;
    ofxUICanvas *guiPlayback;
    ofxUICanvas *guiRecord;
    ofxUICanvas *guiImport;
    ofxUICanvas *guiDebug;
    ofxUICanvas *guiCamera;
    ofxUICanvas *guiSave;
    
    //---------------Variable Declarations---------------
    ofVec3f	rotateAmount;
    ofVec3f  playbackRotateAmount;// amount to rotate in x,y
    ofVec3f  pts[MAX_PTS];
    ofVec3f	xAxisMin;
    ofVec3f	xAxisMax;
    ofVec3f	yAxisMin;
    ofVec3f	yAxisMax;
    ofVec3f	zAxisMin;
    ofVec3f	zAxisMax;
    
    ofColor shapeColor;
    ofColor lineColor;
    ofColor bgColor1;
    ofColor bgColor2;
    
    float speedOfRotation;
    float speedOfPlaybackRotation;// speed;
    float timerStart;
    float endTime;
    float rotatedAmount;
    float beat;
    float playbackRotationSpeed;
    float playbackRotationTime;
    float playbackCounter;
    float resetValue;
    float recordStartTime;
    float recordEndTime;
    float playbackStartTime;
    float playbackEndTime;
    
    int nPts;
    int captureRate;
    int debugArmPos;
    int pointCount;
    int lineCount;
    int lastTagNumber;
    int importLineCount;
    int importlastTagNumber;
    int importPointCount;
    int shapeR, shapeG, shapeB, shapeA;
    int lineR, lineG, lineB;
    int pointTester;
    int OSC_PORT;
    
    bool recordedPoint;
    bool isRecording;
    bool mode;
    bool showShape;
    bool showLines;
    bool showData;
    bool showCircles;
    bool save;
    bool showOutline;
    bool showCamera;
    bool bTimerFinished;
    bool bDrawnAnything;
    bool drawShader;
    bool rPmode;
    bool showPlaybackGui;
    bool showRecordGui;
    bool showImportGui;
    bool showDebugGui;
    bool bNextPoint;
    bool bProgressNextRotation;
    bool bImportMode;
    bool debugMode;
    
    string message;
    string saveFileName;
    string OSC_HOST;
    
};
