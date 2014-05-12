#pragma once

#include "ofMain.h"

#include "ofxUI.h"
#include "ofxMidi.h"

//#include "AVpolySynth.h"
#include "AVglobals.h"
#include "RECtrack.h"

class testApp : public ofBaseApp, public ofxMidiListener {
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    /*===== M.I.D.I. ======*/
    
    // this is your listener function
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    // this is your ofxMidiIn object
    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
    
    stringstream text;
    
    /*==== AUDIO =====*/
    
    void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
    void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
	
    /* external i/o signal patches */
    
    vector <double> audioOutL;
    vector <double> audioOutR;
    
    vector <double> audioInL;
    vector <double> audioInR;
    
    vector <double> Aux1L;
    vector <double> Aux1R;
    
    
    /* generators */
    
    //avVOICE mysynth;
    // avPOLYSYNTH mysynth;
    // float* synthOut;
    
    recTRACK mytrack;
    float* trackOut;
    float* trackIn;
    
    /* audio ready and ...go */
    
    ofSoundStream soundStream;
    
    
    
};
