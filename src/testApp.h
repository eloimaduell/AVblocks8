#pragma once

#include "ofMain.h"
#include "ofxUI.h"

#include "ofxMidi.h"

#include "AVpolySynth.h"
#include "AVglobals.h"
#include "AVSoundStream.h"

class testApp : public ofBaseApp, public ofxMidiListener{
    
public:
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void setupParameterListeners();
    void setupGUIs();
    void setupMIDI();
    void setupAudio();
    void drawWaveform();

    void randomize();
    expFunc eFn; // used in randomize
    
    /*===== Parameters and listening functions =======*/
    
    ofParameter<bool> masterPower; void synthmasterPower(bool & _value);
    ofParameter<bool> isPlayingSAW; void synthisPlayingSAW(bool & _value);
    ofParameter<bool> isPlayingTRI; void synthisPlayingTRI(bool & _value);
    ofParameter<bool> isPlayingSQU; void synthisPlayingSQU(bool & _value);
    ofParameter<bool> isPlayingPULSE; void synthisPlayingPULSE(bool & _value);
    ofParameter<bool> isPlayingNOISE; void synthisPlayingNOISE(bool & _value);
    ofParameter<bool> isPlayingSINBUF; void synthisPlayingSINBUF(bool & _value);
    ofParameter<float> kPWM; void synthkPWM(float & _value);
    ofParameter<float> kPanSAW; void synthkPanSAW(float & _value);
    ofParameter<float> kPanTRI; void synthkPanTRI(float & _value);
    ofParameter<float> kPanSQU; void synthkPanSQU(float & _value);
    ofParameter<float> kPanPULSE; void synthkPanPULSE(float & _value);
    ofParameter<float> kPanNOISE; void synthkPanNOISE(float & _value);
    ofParameter<float> kPanSINBUF; void synthkPanSINBUF(float & _value);
    ofParameter<float> kVolSIN; void synthkVolSIN(float & _value);
    ofParameter<float> kVolSAW; void synthkVolSAW(float & _value);
    ofParameter<float> kVolTRI; void synthkVolTRI(float & _value);
    ofParameter<float> kVolSQU; void synthkVolSQU(float & _value);
    ofParameter<float> kVolPULSE; void synthkVolPULSE(float & _value);
    ofParameter<float> kVolNOISE; void synthkVolNOISE(float & _value);
    ofParameter<int> kHarmSIN; void synthkHarmSIN(int & _value);
    ofParameter<int> kHarmSAW; void synthkHarmSAW(int & _value);
    ofParameter<int> kHarmTRI; void synthkHarmTRI(int & _value);
    ofParameter<int> kHarmSQU; void synthkHarmSQU(int & _value);
    ofParameter<int> kHarmPULSE; void synthkHarmPULSE(int & _value);
    ofParameter<float> A1; void synthA1(float & _value);
    ofParameter<float> D1; void synthD1(float & _value);
    ofParameter<float> S1; void synthS1(float & _value);
    ofParameter<float> R1; void synthR1(float & _value);
    ofParameter<float> A2; void synthA2(float & _value);
    ofParameter<float> D2; void synthD2(float & _value);
    ofParameter<float> S2; void synthS2(float & _value);
    ofParameter<float> R2; void synthR2(float & _value);
    ofParameter<float> kEnv2ToFilter; void synthkEnv2ToFilter(float & _value);
    ofParameter<bool> isFilter1Playing; void synthisFilter1Playing(bool & _value);
    ofParameter<float> kFilter1Frequency; void synthkFilter1Frequency(float & _value);
    ofParameter<float> kFilter1Resonance; void synthkFilter1Resonance(float & _value);
    
    ofParameter<bool> isLfoPlaying; void synthisLfoPlaying(bool & _value);
    ofParameter<int> kWaveformLfo; void synthkWaveformLfo(int & _value);
    ofParameter<int> kLFOmode; void synthkLFOmode(int & _value);
    ofParameter<float> kLFOfreq; void synthkLFOfreq(float & _value);
    
    ofParameter<float> klfoModPitchSINBUFamount;void synthklfoModPitchSINBUFamount(float & _value);//54
    ofParameter<float> klfoModPitchTRIamount;void synthklfoModPitchTRIamount(float & _value);//55
    ofParameter<float> klfoModPitchSAWamount;void synthklfoModPitchSAWamount(float & _value);//56
    ofParameter<float> klfoModPitchSQUamount;void synthklfoModPitchSQUamount(float & _value);//57
    ofParameter<float> klfoModPitchPWMamount;void synthklfoModPitchPWMamount(float & _value);//58
    ofParameter<float> klfoModPulsePWMamount;void synthklfoModPulsePWMamount(float & _value);//59
    ofParameter<float> klfoOffset; void synthklfoOffset(float & _value);//60
    
    ofParameter<float> kReverbRoomSize; void reverbkReverbRoomSize(float & _value);
    ofParameter<float> kReverbDamp; void reverbkReverbDamp(float & _value);
    ofParameter<float> kReverbWet; void reverbkReverbWet(float & _value);
    ofParameter<bool> isPlayingREVERB; void reverbisPlayingREVERB(bool & _value);
    ofParameter<float> kReverbwidth; void reverbkReverbwidth(float & _value);
    
    ofParameter<bool> setPlayDistor; void fxsetPlayDistor( bool & _value);
    ofParameter<int> setLevelDistor; void fxsetLevelDistor ( int &_value);
    ofParameter<float> setGainDistor; void fxsetGainDistor (float &_value);
    ofParameter<int> setDistorMode; void fxsetDistorMode ( int &_value);
    
    
    
    // ofParameter<bool> setPlaySAMPLER; void synthsetPlaySAMPLER(bool & _value);
    // ofParameter<int> LoopMode; void synthLoopMode(int & _value);
    
    /*===== END Parameters, listening functions =======*/
    
    
    /* ==== GUI == */
    
    ofxUICanvas *guiSettings;
    ofxUICanvas *guiGlobal;
    
    ofxUICanvas *guiOSCSIN;
    ofxUICanvas *guiOSCTRI;
    ofxUICanvas *guiOSCSAW;
    ofxUICanvas *guiOSCSQU;
    ofxUICanvas *guiOSCPWM;
    ofxUICanvas *guiOSCNOS;
    
    ofxUICanvas *guiENV1;
    ofxUICanvas *guiENV2;
    ofxUICanvas *guiFILTER1;
    ofxUICanvas *guiLFO1;
    ofxUICanvas *guiREVERB;
    ofxUICanvas *guiDISTOR;
    
    ofxUILabel *labelFPS;

    void guiEvent(ofxUIEventArgs &e);
    
    
    /* ==== END GUI == */
    
    
    /*===== MIDI ======*/
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    stringstream text;
    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
    ofxMidiOut midiOut;
    
    /*==== END MIDI ====*/
    
    /*===== AUDIO ====*/
    
    void audioOut(float * input, int bufferSize, int nChannels);
    
    /* external i/o signal patches */
    
    vector <float> audioOutL;
    vector <float> audioOutR;
    
    vector <float> audioInL;
    vector <float> audioInR;
    
    vector <float> Aux1L;
    vector <float> Aux1R;
    
    //------------------- AV synth
    
    float* synthOut;
     
    avPOLYSYNTH mysynth;
    
    
    // -------------------- instantiate soundstream
    
    ofSoundStream soundStream;
    
    AVSoundStream avSoundStream;
    
    /*===== END AUDIO ====*/
    
    // ELOI
    bool    keyIsPressed;
    void setupAudioDevice(int i);
    vector<string> audioDeviceList;

    
    vector <float> audioDrawL;
    vector <float> audioDrawR;
    int audioDrawLength;
    int audioDrawPos;


    
    
};
