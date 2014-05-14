#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    
	//ofSetVerticalSync(true);
	//ofSetFrameRate(60);
    
    setupParameterListeners();
    setupGUIs();
    setupMIDI();
    setupAudio();
    
    audioDrawPos = 0;
    audioDrawLength = 2048;
    audioDrawL.assign(audioDrawLength,0);
    audioDrawR.assign(audioDrawLength,0);
}
//--------------------------------------------------------------
void testApp::update(){
    
    labelFPS->setLabel(ofToString("this is not a synth"));
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(44,44,44);
    
    
    drawWaveform();

    //PRINT FPS ON SCREEN
    ofSetColor(128);
    ofDrawBitmapString(ofToString(int(ofGetFrameRate())),ofGetWidth()-20,ofGetHeight()-10);

    
    //PRINT MIDI ON SCREEN
    ofSetColor(100);
    int verticaloffset=12;
    int verticallapse=600;
    
    // draw the last recieved message contents to the screen
    text << "Received  " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(text.str(), 20, verticallapse);
    text.str(""); // clear
    
    text << "channel   " << midiMessage.channel;
    ofDrawBitmapString(text.str(), 20, verticallapse+(1*verticaloffset));
    text.str(""); // clear
    
    text << "pitch     " << midiMessage.pitch;
    ofDrawBitmapString(text.str(), 20, verticallapse+(2*verticaloffset));
    text.str(""); // clear
    //ofRect(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "velocity  " << midiMessage.velocity;
    ofDrawBitmapString(text.str(), 20, verticallapse+(3*verticaloffset));
    text.str(""); // clear
    //ofRect(20, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "control   " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, verticallapse+(4*verticaloffset));
    text.str(""); // clear
    //ofRect(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "value     " << midiMessage.value;
    ofDrawBitmapString(text.str(), 20, verticallapse+(5*verticaloffset));
    text.str(""); // clear
    //if(midiMessage.status == MIDI_PITCH_BEND) {
    //ofRect(20, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
    //}
    //else {
    //ofRect(20, 202, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
    //}
    
    text << "delta     " << midiMessage.deltatime;
    ofDrawBitmapString(text.str(), 20, verticallapse+(6*verticaloffset));
    text.str(""); // clear
    
    // END PRINT MIDI ON SCREEN
    
    
    
    
}


//----------------------------------------------------------------------------------------------------

void testApp::drawWaveform()
{
    //waveforms display
    
    ofNoFill();
    
    // draw the left channel:
    
    int yWaveDraw = ofGetHeight()-50;
    int yScale = 100;
    int channelSeparator=20;
    ofPushStyle();
    
    ofSetLineWidth(1);
    
    // 0 level line
    ofSetColor(128,128,128);
    ofBeginShape();
    ofVertex(0, yWaveDraw);
    ofVertex(ofGetWidth()/2 - channelSeparator, yWaveDraw);
    ofEndShape();
    ofBeginShape();
    ofVertex(ofGetWidth()/2 + channelSeparator, yWaveDraw);
    ofVertex(ofGetWidth(), yWaveDraw);
    ofEndShape();

    // LR separator
    ofSetColor(255,255,255);
    ofBeginShape();
    ofVertex(ofGetWidth()/2, ofGetHeight());
    ofVertex(ofGetWidth()/2, ofGetHeight()-yScale);
    ofEndShape();

    
    ofSetColor(100);
    ofSetLineWidth(2);
    
    ofBeginShape();
    for (unsigned int i = 0; i < (ofGetWidth()/2-10); i++){
        float x =  ofMap(i, 0, (ofGetWidth()/2-10), 0,audioDrawL.size(), true);
        ofVertex(i,yWaveDraw -audioDrawL[x]*yScale);
    }
    
    //    for (unsigned int i = 0; i < audioOutL.size(); i++){
    //        float x =  ofMap(i, 0, audioOutL.size(), 0, 200, true);
    //        ofVertex(x, 10 -audioOutL[i]*75.f);
    //    }
    ofEndShape(false);
    
    // draw the right channel:
    ofPushStyle();
    
    
    ofBeginShape();
    for (unsigned int i = 0; i < ofGetWidth()/2-10; i++){
        float x =  ofMap(i, 0, ofGetWidth()/2-10, 0,audioDrawR.size(), true);
        ofVertex(ofGetWidth()/2 + 10 + i, yWaveDraw -audioDrawR[x]*yScale);
    }
    //
    //    for (unsigned int i = 0; i < audioOutR.size(); i++){
    //        float x =  ofMap(i, 0, audioOutR.size(), 0, 200, true);
    //        ofVertex(x, 10 -audioOutR[i]*75.f);
    //    }
    
    ofEndShape(false);
    
    ofPopStyle();
}
//--------------------------------------------------------------
void testApp::exit() {
	
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
    delete guiGlobal, guiOSCSIN, guiOSCTRI, guiOSCSAW, guiOSCSQU, guiOSCNOS, guiOSCPWM, guiENV1, guiENV2, guiFILTER1;
    
}

//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {
    
	// make a copy of the latest message
	midiMessage = msg;
    
    switch (midiMessage.status)
    {
            
            // channel voice messages
        case MIDI_NOTE_OFF :
            mysynth.note_off(midiMessage.pitch);
            break;
        case MIDI_NOTE_ON :
            mysynth.note_on(midiMessage.pitch, midiMessage.velocity);
            // cout << "NOTA pitch : " << midiMessage.pitch << "\r" ;
            break;
            
        case MIDI_CONTROL_CHANGE : {/* */
            
            switch (midiMessage.control){
                case 14 : if(midiMessage.value==127) isPlayingSINBUF=!isPlayingSINBUF; break;
            }
            
        }; break;
            
        case MIDI_PROGRAM_CHANGE : {/* */}; break;
        case MIDI_PITCH_BEND : {/* */}; break;
        case MIDI_AFTERTOUCH : {/* */}; break;		// aka channel pressure
        case MIDI_POLY_AFTERTOUCH : {/* */}; break; // aka key pressure
            
            // system messages
        case MIDI_SYSEX : {/* */}; break;
        case MIDI_TIME_CODE : {/* */}; break;
        case MIDI_SONG_POS_POINTER : {/* */}; break;
        case MIDI_SONG_SELECT : {/* */}; break;
        case MIDI_TUNE_REQUEST : {/* */}; break;
        case MIDI_SYSEX_END : {/* */}; break;
        case MIDI_TIME_CLOCK : {/* */}; break;
        case MIDI_START : {/* */}; break;
        case MIDI_CONTINUE : {/* */}; break;
        case MIDI_STOP : {/* */}; break;
        case MIDI_ACTIVE_SENSING : {/* */}; break;
        case MIDI_SYSTEM_RESET : {/* */}; break;
        case MIDI_UNKNOWN : {/* */}; break;
            
    }
    
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    switch (key)
    {
        case 'a': if(!keyIsPressed)mysynth.note_on(34, 80);break;
        case 's': if(!keyIsPressed)mysynth.note_on(37, 90);break;
        case 'd': if(!keyIsPressed)mysynth.note_on(39, 100);break;
        case 'f': if(!keyIsPressed)mysynth.note_on(40, 110);break;
        case 'g': if(!keyIsPressed)mysynth.note_on(41, 110);break;
            
    }
    
    if(!keyIsPressed) keyIsPressed=true;
    
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
    
    switch (key)
    {
        case 'a': mysynth.note_off(34);break;
        case 's': mysynth.note_off(37);break;
        case 'd': mysynth.note_off(39);break;
        case 'f': mysynth.note_off(40);break;
        case 'g': mysynth.note_off(41);break;
    }
    if(keyIsPressed) keyIsPressed=false;
    
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
void testApp::audioOut(float * output, int bufferSize, int nChannels){
	
    for (int i = 0; i < bufferSize; i++){
        
        synthOut = mysynth.getAudioOut();
        
        audioOutL[i] = output[i*nChannels    ] = synthOut[0];
        audioOutR[i] = output[i*nChannels + 1] = synthOut[1];
        
        //copy data to audioDraw buffers
        audioDrawL[audioDrawPos] =audioOutL[i];
        audioDrawR[audioDrawPos] =audioOutR[i];
        audioDrawPos=(audioDrawPos+1)%audioDrawLength;
    }
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}


//--------------------------------------------------------------
void testApp::setupAudioDevice(int i)
{
    
    soundStream.stop();
    soundStream.close();
    soundStream.setDeviceID(i);

    soundStream.setup(this, 2, 0, SAMPLING_RATE, BUFFER_SIZE, 4);
    cout << "setupAudioDevice : " << i << " : " << endl;
    
}

//--------------------------------------------------------------
void testApp::setupParameterListeners()
{
    /*====== Listeners (bind ofParameters to listening functions) ======*/
    
    masterPower.addListener(this, &testApp::synthmasterPower);
    
    isPlayingSAW.addListener(this, &testApp::synthisPlayingSAW);
    isPlayingTRI.addListener(this, &testApp::synthisPlayingTRI);
    isPlayingSQU.addListener(this, &testApp::synthisPlayingSQU);
    isPlayingPULSE.addListener(this, &testApp::synthisPlayingPULSE);
    isPlayingNOISE.addListener(this, &testApp::synthisPlayingNOISE);
    isPlayingSINBUF.addListener(this, &testApp::synthisPlayingSINBUF);
    
    kPWM.addListener(this, &testApp::synthkPWM);
    kPanSAW.addListener(this, &testApp::synthkPanSAW);
    kPanTRI.addListener(this, &testApp::synthkPanTRI);
    kPanSQU.addListener(this, &testApp::synthkPanSQU);
    kPanPULSE.addListener(this, &testApp::synthkPanPULSE);
    kPanNOISE.addListener(this, &testApp::synthkPanNOISE);
    kPanSINBUF.addListener(this, &testApp::synthkPanSINBUF);
    
    kVolSIN.addListener(this, &testApp::synthkVolSIN);
    kVolSAW.addListener(this, &testApp::synthkVolSAW);
    kVolTRI.addListener(this, &testApp::synthkVolTRI);
    kVolSQU.addListener(this, &testApp::synthkVolSQU);
    kVolPULSE.addListener(this, &testApp::synthkVolPULSE);
    kVolNOISE.addListener(this, &testApp::synthkVolNOISE);
    
    kHarmSIN.addListener(this, &testApp::synthkHarmSIN);
    kHarmSAW.addListener(this, &testApp::synthkHarmSAW);
    kHarmTRI.addListener(this, &testApp::synthkHarmTRI);
    kHarmSQU.addListener(this, &testApp::synthkHarmSQU);
    kHarmPULSE.addListener(this, &testApp::synthkHarmPULSE);
    
    klfoModPitchSINBUFamount.addListener(this, &testApp::synthklfoModPitchSINBUFamount);//54
    klfoModPitchTRIamount.addListener(this, &testApp::synthklfoModPitchTRIamount);//55
    klfoModPitchSAWamount.addListener(this, &testApp::synthklfoModPitchSAWamount);//56
    klfoModPitchSQUamount.addListener(this, &testApp::synthklfoModPitchSQUamount);//57
    klfoModPitchPWMamount.addListener(this, &testApp::synthklfoModPitchPWMamount);//58
    klfoModPulsePWMamount.addListener(this, &testApp::synthklfoModPulsePWMamount);//59
    
    A1.addListener(this, &testApp::synthA1);
    D1.addListener(this, &testApp::synthD1);
    S1.addListener(this, &testApp::synthS1);
    R1.addListener(this, &testApp::synthR1);
    
    A2.addListener(this, &testApp::synthA2);
    D2.addListener(this, &testApp::synthD2);
    S2.addListener(this, &testApp::synthS2);
    R2.addListener(this, &testApp::synthR2);
    
    kEnv2ToFilter.addListener(this, &testApp::synthkEnv2ToFilter);
    isFilter1Playing.addListener(this, &testApp::synthisFilter1Playing);
    kFilter1Frequency.addListener(this, &testApp::synthkFilter1Frequency);
    kFilter1Resonance.addListener(this, &testApp::synthkFilter1Resonance);
    
    isLfoPlaying.addListener(this, &testApp::synthisLfoPlaying);
    kWaveformLfo.addListener(this, &testApp::synthkWaveformLfo);
    kLFOfreq.addListener(this, &testApp::synthkLFOfreq);
    kLFOmode.addListener(this, &testApp::synthkLFOmode);
    //klfoOffset.addListener(this, &testApp::synthklfoOffset);
    
    kReverbRoomSize.addListener(this, &testApp::reverbkReverbRoomSize);
    kReverbDamp.addListener(this, &testApp::reverbkReverbDamp);
    kReverbWet.addListener(this, &testApp::reverbkReverbWet);
    isPlayingREVERB.addListener(this, &testApp::reverbisPlayingREVERB);
    kReverbwidth.addListener(this, &testApp::reverbkReverbwidth);
    
    setPlayDistor.addListener(this,&testApp::fxsetPlayDistor);
    setLevelDistor.addListener(this,&testApp::fxsetLevelDistor);
    setGainDistor.addListener(this,&testApp::fxsetGainDistor);
    setDistorMode.addListener(this,&testApp::fxsetDistorMode);
    // setPlaySAMPLER.addListener(this, &testApp::synthsetPlaySAMPLER);
    // LoopMode.addListener(this, &testApp::synthLoopMode);
    
    
    /*===== END Listeners ===*/
    
}

//----------------------------------------------------------------------------------------------------
void testApp::setupGUIs()
{
    
    int dim = 7;
    
    // GUI   G L O B A L
    //----------------------------------------------------------------------------------------------------
    int globalGuiWidth = 130;
    guiGlobal = new ofxUICanvas(10,10,globalGuiWidth-10,200);//ofxUICanvas(float x, float y, float width, float height)
    guiGlobal->setName("GLOBALS");
    guiGlobal->setAutoDraw(false);
    int textSize=4;
    guiGlobal->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiGlobal->setFontSize(OFX_UI_FONT_MEDIUM, textSize);
    guiGlobal->setFontSize(OFX_UI_FONT_SMALL, textSize);
    labelFPS = new ofxUILabel("' this is not a synth '",OFX_UI_FONT_LARGE);
    guiGlobal->addWidgetDown(labelFPS);
    guiGlobal->addSpacer(304, 1);
    guiGlobal->addWidgetDown(new ofxUIToggle(dim*2, dim*2,true, "ON-OFF", OFX_UI_FONT_SMALL));
    guiGlobal->addWidgetDown(new ofxUIButton(dim*2, dim*2, true, "RANDOMIZE", OFX_UI_FONT_SMALL));
    guiGlobal->addWidgetDown(new ofxUIButton(dim*2, dim*2, true, "SETTINGS", OFX_UI_FONT_SMALL));
    
    ofAddListener(guiGlobal->newGUIEvent, this, &testApp::guiEvent);
    
    // GUI   S E T T I N G S
    //----------------------------------------------------------------------------------------------------
    guiSettings = new ofxUICanvas(10,10,530,400);//ofxUICanvas(float x, float y, float width, float height)
    guiSettings->setName("CONFIG SETTINGS ");
    guiSettings->setAutoDraw(false);
    guiSettings->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiSettings->setFontSize(OFX_UI_FONT_MEDIUM, textSize);
    guiSettings->setFontSize(OFX_UI_FONT_SMALL, textSize);
    
    
    // afegim un widget amb la llista dels dispositius
    int size = avSoundStream.getDeviceList();
    for(int i=0;i<size;i++)
    {
        //audioDeviceList.push_back(avSoundStream.getDeviceName(i));
        audioDeviceList.push_back(ofToString("AudioOut ") +ofToString(i) + " : "  +ofToString(avSoundStream.getDeviceName(i)));
    }
    ofxUIRadio* audioOutDevices = new ofxUIRadio("AUDIOOUTDEVICE",audioDeviceList ,OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    audioOutDevices->setWhichToTrue(3);
    guiSettings->addLabel("AUDIO OUT");
    guiSettings->addSpacer();
    guiSettings->addWidgetDown(audioOutDevices);
    
    // llista de dispositius midi
    vector<string> midiInDeviceList = midiIn.getPortList();
    ofxUIRadio* midiInDevices = new ofxUIRadio("MIDIINDEVICE",midiInDeviceList,OFX_UI_ORIENTATION_VERTICAL,dim,dim);
    guiSettings->addLabel("MIDI IN");
    guiSettings->addSpacer();
    guiSettings->addWidgetDown(midiInDevices);
    
    guiSettings->addWidgetDown(new ofxUIToggle(dim*2, dim*2, false, "FULLSCREEN", OFX_UI_FONT_SMALL));
    guiSettings->addWidgetDown(new ofxUIButton(dim*2, dim*2, true, "BACK", OFX_UI_FONT_SMALL));
    
    ofAddListener(guiSettings->newGUIEvent, this, &testApp::guiEvent);
    guiSettings->disable();
    
    
    // GUI   O S C  S I N
    //----------------------------------------------------------------------------------------------------
    int oscWidth = 120.0;
    int oscWidthForSin = 120 - 20;
    
    guiOSCSIN = new ofxUICanvas(globalGuiWidth+10,10,oscWidthForSin,200);//ofxUICanvas(float x, float y, float width, float height)
    guiOSCSIN->setName("SIN");
    guiOSCSIN->setAutoDraw(false);
    guiOSCSIN->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiOSCSIN->setFontSize(OFX_UI_FONT_MEDIUM,textSize);
    guiOSCSIN->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiOSCSIN->setWidgetSpacing(dim);
    
    guiOSCSIN->addWidgetDown(new ofxUIToggle(dim*2, dim,true, "SIN"));
    guiOSCSIN->addWidgetDown(new ofxUISlider("VOL",0.f,127.f,110.f, dim*2, 128));
    guiOSCSIN->addWidgetRight(new ofxUISlider("HRM",1,32,1, dim*2, 128));
    guiOSCSIN->addWidgetRight(new ofxUISlider("MOD",0.,1.,0.1, dim*2, 128));//ignasi
    
    guiOSCSIN->addWidgetDown(new ofxUISlider("PAN", -64.f, 64.f, 0., 50,dim*1.5));
    
    // GUI   O S C  T R I
    //----------------------------------------------------------------------------------------------------
    guiOSCTRI = new ofxUICanvas(globalGuiWidth+10+10+oscWidthForSin*1,10,oscWidthForSin,200);//ofxUICanvas(float x, float y, float width, float height)
    guiOSCTRI->setName("TRI");
    guiOSCTRI->setAutoDraw(false);
    guiOSCTRI->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiOSCTRI->setFontSize(OFX_UI_FONT_MEDIUM,textSize);
    guiOSCTRI->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiOSCTRI->setWidgetSpacing(dim);
    
    guiOSCTRI->addWidgetDown(new ofxUIToggle(dim*2, dim,true, "TRI"));
    guiOSCTRI->addWidgetDown(new ofxUISlider("VOL",0.f,127.f,110.f, dim*2, 128));
    guiOSCTRI->addWidgetRight(new ofxUISlider("HRM",1,32,1, dim*2, 128));
    guiOSCTRI->addWidgetRight(new ofxUISlider("MOD",0.,1.,0.1, dim*2, 128)); //ignasi
    guiOSCTRI->addWidgetDown(new ofxUISlider("PAN", -64.f, 64.f, 0., 50,dim*1.5));
    
    // GUI   O S C  S A W
    //----------------------------------------------------------------------------------------------------
    
    guiOSCSAW = new ofxUICanvas(globalGuiWidth+10+((oscWidthForSin+10)*2),10,oscWidthForSin,200);
    guiOSCSAW->setName("SAW");
    guiOSCSAW->setAutoDraw(false);
    guiOSCSAW->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiOSCSAW->setFontSize(OFX_UI_FONT_MEDIUM,textSize);
    guiOSCSAW->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiOSCSAW->setWidgetSpacing(dim);
    
    guiOSCSAW->addWidgetDown(new ofxUIToggle(dim*2, dim,true, "SAW"));
    guiOSCSAW->addWidgetDown(new ofxUISlider("VOL",0.f,127.f,110.f, dim*2, 128));
    guiOSCSAW->addWidgetRight(new ofxUISlider("HRM",1,32,1, dim*2, 128));
    guiOSCSAW->addWidgetRight(new ofxUISlider("MOD",0.,1.,0.1, dim*2, 128));//ignasi
    guiOSCSAW->addWidgetDown(new ofxUISlider("PAN", -64.f, 64.f, 0., 50,dim*1.5));
    
    
    // GUI   O S C  S Q U
    //----------------------------------------------------------------------------------------------------
    
    guiOSCSQU = new ofxUICanvas(globalGuiWidth+10+((oscWidthForSin+10)*3),10,oscWidthForSin,200);
    guiOSCSQU->setName("SQU");
    guiOSCSQU->setAutoDraw(false);
    guiOSCSQU->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiOSCSQU->setFontSize(OFX_UI_FONT_MEDIUM,textSize);
    guiOSCSQU->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiOSCSQU->setWidgetSpacing(dim);
    
    guiOSCSQU->addWidgetDown(new ofxUIToggle(dim*2, dim,true, "SQU"));
    guiOSCSQU->addWidgetDown(new ofxUISlider("VOL",0.f,127.f,110.f, dim*2, 128));
    guiOSCSQU->addWidgetRight(new ofxUISlider("HRM",1,32,1, dim*2, 128));
    guiOSCSQU->addWidgetRight(new ofxUISlider("MOD",0.,1.,0.1, dim*2, 128));//ignasi
    guiOSCSQU->addWidgetDown(new ofxUISlider("PAN", -64.f, 64.f, 0., 50,dim*1.5));
    
    // GUI   O S C  P W M
    //----------------------------------------------------------------------------------------------------
    int addForPWM = 25;
    guiOSCPWM = new ofxUICanvas(globalGuiWidth+10+((oscWidthForSin+10)*4),10,oscWidth+addForPWM,200);
    guiOSCPWM->setName("PWM");
    guiOSCPWM->setAutoDraw(false);
    guiOSCPWM->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiOSCPWM->setFontSize(OFX_UI_FONT_MEDIUM,textSize);
    guiOSCPWM->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiOSCPWM->setWidgetSpacing(dim);
    
    guiOSCPWM->addWidgetDown(new ofxUIToggle(dim*2, dim,true, "PWM"));
    guiOSCPWM->addWidgetDown(new ofxUISlider("VOL",0.f,127.f,110.f, dim*2, 128));
    guiOSCPWM->addWidgetRight(new ofxUISlider("HRM",1,32,1, dim*2, 128));
    guiOSCPWM->addWidgetRight(new ofxUISlider("WDT",0.f,1.f,.5f,dim*2,128));
    guiOSCPWM->addWidgetRight(new ofxUISlider("M.1",0.,1.,0.1, dim*2, 128));
    guiOSCPWM->addWidgetRight(new ofxUISlider("M.2",0.,1.,0.1, dim*2, 128));
    guiOSCPWM->addWidgetDown(new ofxUISlider("PAN", -64.f, 64.f, 0., 50,dim*1.5));
    
    
    // GUI   O S C  N O S
    //----------------------------------------------------------------------------------------------------
    int oscWidthForNos = oscWidth * 0.5;
    guiOSCNOS = new ofxUICanvas(globalGuiWidth+10+((oscWidthForSin+10)*4)+(oscWidth+addForPWM+10),10,oscWidthForNos,200);
    guiOSCNOS->setName("NOISE");
    guiOSCNOS->setAutoDraw(false);
    guiOSCNOS->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiOSCNOS->setFontSize(OFX_UI_FONT_MEDIUM,textSize);
    guiOSCNOS->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiOSCNOS->setWidgetSpacing(dim);
    
    guiOSCNOS->addWidgetDown(new ofxUIToggle(dim, dim,true, "NOS"));
    //guiOSCNOS->addWidgetSouthOf(new ofxUIRotarySlider( dim*4, -64.f, 64.f, 1.f, "PAN_NOS", OFX_UI_FONT_SMALL), "NOS");
    guiOSCNOS->addWidgetDown(new ofxUISlider("VOL",0.f,127.f,110.f, dim*2, 128));
    guiOSCNOS->addWidgetDown(new ofxUISlider("PAN", -64.f, 64.f, 0., 50,dim*1.5));
    
    
    // adding listeners ...
    
    ofAddListener(guiOSCSQU->newGUIEvent, this, &testApp::guiEvent);
    ofAddListener(guiOSCTRI->newGUIEvent, this, &testApp::guiEvent);
    ofAddListener(guiOSCSIN->newGUIEvent, this, &testApp::guiEvent);
    ofAddListener(guiOSCSAW->newGUIEvent, this, &testApp::guiEvent);
    ofAddListener(guiOSCPWM->newGUIEvent, this, &testApp::guiEvent);
    ofAddListener(guiOSCNOS->newGUIEvent, this, &testApp::guiEvent);
    
    
    // GUI   E N V 1
    //----------------------------------------------------------------------------------------------------
    int widthForENV1 = oscWidth*0.90;
    
    guiENV1 = new ofxUICanvas(globalGuiWidth+10,200 + 10*2 ,widthForENV1,200);
    guiENV1->setName("ENV VOLUMEN");
    guiENV1->setAutoDraw(false);
    guiENV1->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiENV1->setFontSize(OFX_UI_FONT_MEDIUM, textSize);
    guiENV1->setFontSize(OFX_UI_FONT_SMALL, textSize);
    
    guiENV1->addWidgetDown(new ofxUILabel("ENV VOL", OFX_UI_FONT_MEDIUM));
    guiENV1->setWidgetSpacing(dim);
    
    guiENV1->addWidgetDown(new ofxUISlider("A",0.f,1.f,0.f, dim*2, 158));
    guiENV1->addWidgetRight(new ofxUISlider("D",0.f,1.f,.1f, dim*2, 158));
    guiENV1->addWidgetRight(new ofxUISlider("S",0.f,1.f,1.f, dim*2, 158));
    guiENV1->addWidgetRight(new ofxUISlider("R",0.f,1.f,.5f, dim*2, 158));
    
    ofAddListener(guiENV1->newGUIEvent, this, &testApp::guiEvent);
    
    // GUI   E N V 2
    //----------------------------------------------------------------------------------------------------
    int widthForENV2 = oscWidth*1.10;
    
    guiENV2 = new ofxUICanvas(globalGuiWidth+10 + widthForENV1 + 10,200 + 10*2,widthForENV2,200);//ofxUICanvas(float x, float y, float width, float height)
    guiENV2->setName("ENV FILTER");
    guiENV2->setAutoDraw(false);
    guiENV2->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiENV2->setFontSize(OFX_UI_FONT_MEDIUM, textSize);
    guiENV2->setFontSize(OFX_UI_FONT_SMALL, textSize);
    
    guiENV2->addWidgetDown(new ofxUILabel("ENV FILTER", OFX_UI_FONT_MEDIUM));
    guiENV2->setWidgetSpacing(dim);
    
    guiENV2->addWidgetDown(new ofxUISlider("A",0.f,1.f,0.f, dim*2, 158));
    guiENV2->addWidgetRight(new ofxUISlider("D",0.f,1.f,.1f, dim*2, 158));
    guiENV2->addWidgetRight(new ofxUISlider("S",0.f,1.f,1.f, dim*2, 158));
    guiENV2->addWidgetRight(new ofxUISlider("R",0.f,1.f,.5f, dim*2, 158));
    guiENV2->addWidgetRight(new ofxUISlider("DEPTH",-1.f,1.f,.8f, dim*2, 158));
    
    ofAddListener(guiENV2->newGUIEvent, this, &testApp::guiEvent);
    
    // GUI   R E V E R B
    //----------------------------------------------------------------------------------------------------
    int widthForReverb = oscWidth * 1.20;
    guiREVERB = new ofxUICanvas(globalGuiWidth+10 + widthForENV1+ widthForENV2 + 10*2,200+10*2,widthForReverb,200);//ofxUICanvas(float x, float y, float width, float height)
    guiREVERB->setName("REVERB");
    guiREVERB->setAutoDraw(false);
    guiREVERB->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiREVERB->setFontSize(OFX_UI_FONT_MEDIUM, textSize);
    guiREVERB->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiREVERB->setWidgetSpacing(dim);
    guiREVERB->addWidgetDown(new ofxUIToggle(dim*2, dim,true, "REVERB"));
    guiREVERB->addWidgetDown(new ofxUISlider("SIZE",0.f,1.f,.65f, dim*2, 158));
    guiREVERB->addWidgetRight(new ofxUISlider("WIDTH",0.f,1.f,.99f, dim*2, 158));
    guiREVERB->addWidgetRight(new ofxUISlider("DAMP",0.f,1.f,0.1f, dim*2, 158));
    guiREVERB->addWidgetRight(new ofxUISlider("MIX",0.f,1.f,.3f, dim*2, 158));
    
    ofAddListener(guiREVERB->newGUIEvent, this, &testApp::guiEvent);
    
    
    // GUI   F I L T E R 1
    //----------------------------------------------------------------------------------------------------
    int widthForFILTER1 = oscWidth*1.00;
    
    guiFILTER1 = new ofxUICanvas(globalGuiWidth+10+ widthForENV1+ widthForENV2+ widthForReverb +  10*3,200+ 10*2,widthForFILTER1,85);//ofxUICanvas(float x, float y, float width, float height)
    guiFILTER1->setName("FILTER1");
    guiFILTER1->setAutoDraw(false);
    guiFILTER1->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiFILTER1->setFontSize(OFX_UI_FONT_MEDIUM, textSize);
    guiFILTER1->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiFILTER1->setWidgetSpacing(dim);
    //guiFILTER1->addWidgetDown(new ofxUILabel("LOWPASS", OFX_UI_FONT_MEDIUM));
    guiFILTER1->addWidgetDown(new ofxUIToggle(dim*2, dim,true, "LOWPASS"));
    guiFILTER1->addSpacer(0,2);
    guiFILTER1->addWidgetDown(new ofxUIRotarySlider( dim*5, 0.f, 1.f, .2f, "CUTOFF", OFX_UI_FONT_SMALL));
    guiFILTER1->addWidgetRight(new ofxUIRotarySlider( dim*5, 0.f, 1.f, .2f, "RES", OFX_UI_FONT_SMALL));
    
    ofAddListener(guiFILTER1->newGUIEvent, this, &testApp::guiEvent);
    
    // GUI   D I S T O R T
    //----------------------------------------------------------------------------------------------------
    int widthForFILTER2 = oscWidth*1.00;
    
    guiDISTOR = new ofxUICanvas(globalGuiWidth+10+ widthForENV1+ widthForENV2+ widthForReverb +  10*3,200+85 +(10*3) ,widthForFILTER2,105);//ofxUICanvas(float x, float y, float width, float height)
    guiDISTOR->setName("DISTOR");
    guiDISTOR->setAutoDraw(false);
    guiDISTOR->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiDISTOR->setFontSize(OFX_UI_FONT_MEDIUM, textSize);
    guiDISTOR->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiDISTOR->setWidgetSpacing(dim);
    guiDISTOR->addWidgetDown(new ofxUIToggle(dim*2, dim,true, "DISTORSION"));
    vector<string> distorModes;
    distorModes.push_back("ARC");
    distorModes.push_back("TAN");
    distorModes.push_back("HYP");
    ofxUIRadio* DistorModes = new ofxUIRadio("DISTORMODE",distorModes ,OFX_UI_ORIENTATION_HORIZONTAL, dim, dim);
    guiDISTOR->addWidgetDown(DistorModes);
    guiDISTOR->addSpacer(0,5);
    guiDISTOR->addWidgetDown(new ofxUIRotarySlider( dim*5, 1, 100, 4, "LEVEL", OFX_UI_FONT_SMALL));
    guiDISTOR->addWidgetRight(new ofxUIRotarySlider( dim*5, 0.f, 1.f, .9f, "GAIN", OFX_UI_FONT_SMALL));
    
    ofAddListener(guiDISTOR->newGUIEvent, this, &testApp::guiEvent);
    
    // GUI   L F O
    //----------------------------------------------------------------------------------------------------
    int widthLFO = 115;
    guiLFO1 = new ofxUICanvas(globalGuiWidth+10+ widthForENV1+ widthForENV2+ widthForReverb +  10*4 + widthForFILTER2,200+10*2,widthLFO,200);
    guiLFO1->setName("LFO");
    guiLFO1->setFontSize(OFX_UI_FONT_LARGE, textSize);
    guiLFO1->setFontSize(OFX_UI_FONT_MEDIUM, textSize);
    guiLFO1->setFontSize(OFX_UI_FONT_SMALL, textSize);
    guiLFO1->setWidgetSpacing(dim);
    guiLFO1->addWidgetDown(new ofxUIToggle(dim, dim,true, "LFO"));
    
    // LFO MODE
    vector<string> lfoModesList;
    lfoModesList.push_back("FIXED"); lfoModesList.push_back("MIDI");
    ofxUIRadio* lfoModes = new ofxUIRadio("MODE",lfoModesList ,OFX_UI_ORIENTATION_HORIZONTAL, dim, dim);
//    guiLFO1->addLabel("MODE");
//    guiLFO1->addSpacer();
    guiLFO1->addWidgetDown(lfoModes);
    
    // LFO WAVEFORM
    vector<string> lfoWaveformList;
    lfoWaveformList.push_back("SIN");
    lfoWaveformList.push_back("TRI");
    lfoWaveformList.push_back("SAW");
    lfoWaveformList.push_back("SQU");
    lfoWaveformList.push_back("PUL");
    lfoWaveformList.push_back("NOS");
    ofxUIRadio* lfoWaves = new ofxUIRadio("LFOWAVEFORM",lfoWaveformList ,OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    guiLFO1->addLabel("WAVEFORM");
    guiLFO1->addSpacer();
    guiLFO1->addWidgetDown(lfoWaves);
    
    guiLFO1->addWidgetDown(new ofxUISlider("FIXED FREQ",0.f,1.f,.3f, widthLFO-20, dim));
    
    
    
    ofAddListener(guiLFO1->newGUIEvent, this, &testApp::guiEvent);
    //gui->loadSettings("GUI/guiSettings.xml");
    
    /*====== END GUI ======*/
}

//----------------------------------------------------------------------------------------------------
void testApp::setupMIDI()
{
    
    
    
    /* ===== MIDI ===== */
    
    /*======  IN ======*/
    
    // print input ports to console
    midiIn.listPorts(); // via instance
    //ofxMidiIn::listPorts(); // via static as well
    
    // open port by number (you may need to change this)
    midiIn.openPort(0);
    //midiIn.openPort("IAC Pure Data In");	// by name
    //midiIn.openVirtualPort("ofxMidiIn Input");	// open a virtual port
    
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    
    // add testApp as a listener
    midiIn.addListener(this);
    
    // print received messages to the console
    midiIn.setVerbose(true);
    
    /*======  OUT ======*/
    
    // print the available output ports to the console
    midiOut.listPorts(); // via instance
    //ofxMidiOut::listPorts(); // via static too
    
    // connect
    midiOut.openPort(0);	// by number
    //midiOut.openPort("IAC Driver Pure Data In");	// by name
    //midiOut.openVirtualPort("ofxMidiOut");		// open a virtual port
    
    /* ===== END MIDI ===== */
}

//----------------------------------------------------------------------------------------------------
void testApp::setupAudio()
{
    audioOutL.assign(BUFFER_SIZE, 0.0);
	audioOutR.assign(BUFFER_SIZE, 0.0);
    
	audioInL.assign(BUFFER_SIZE, 0.0);
	audioInR.assign(BUFFER_SIZE, 0.0);
    
    Aux1L.assign(BUFFER_SIZE, 0.0);
    Aux1R.assign(BUFFER_SIZE, 0.0);
    
    synthOut = new float[2];
    synthOut[0]=synthOut[1]=0.;
    
    mysynth.init();
	
	soundStream.listDevices();
	
	//if you want to set the device id to be different than the default
	soundStream.setDeviceID(2); 	//note some devices are input only and some are output only
    
	soundStream.setup(this, 2, 0, SAMPLING_RATE, BUFFER_SIZE, 4);
    
    
}

//----------------------------------------------------------------------------------------------------
void testApp::synthmasterPower(bool & _value)
{
    mysynth.setParam(0, masterPower);
    ofxUIToggle * t = (ofxUIToggle*) guiGlobal->getWidget("ON-OFF"); // we get the widget into t pointer
    t->setValue(masterPower); //and bind widget value to changes in ofParameter
}
void testApp::synthisPlayingSAW(bool & _value){mysynth.setParam(2, isPlayingSAW);ofxUIToggle * t = (ofxUIToggle*) guiOSCSAW->getWidget("SAW"); t->setValue(isPlayingSAW);}
void testApp::synthisPlayingTRI(bool & _value){mysynth.setParam(3, isPlayingTRI);ofxUIToggle * t = (ofxUIToggle*) guiOSCTRI->getWidget("TRI"); t->setValue(isPlayingTRI);}
void testApp::synthisPlayingSQU(bool & _value){mysynth.setParam(4, isPlayingSQU);ofxUIToggle * t = (ofxUIToggle*) guiOSCSQU->getWidget("SQU"); t->setValue(isPlayingSQU);}
void testApp::synthisPlayingPULSE(bool & _value){mysynth.setParam(5, isPlayingPULSE);ofxUIToggle * t = (ofxUIToggle*) guiOSCPWM->getWidget("PWM"); t->setValue(isPlayingPULSE);}
void testApp::synthisPlayingNOISE(bool & _value){mysynth.setParam(6, isPlayingNOISE);ofxUIToggle * t = (ofxUIToggle*) guiOSCNOS->getWidget("NOS"); t->setValue(isPlayingNOISE);}
void testApp::synthisPlayingSINBUF(bool & _value){mysynth.setParam(7, isPlayingSINBUF);ofxUIToggle * t = (ofxUIToggle*) guiOSCSIN->getWidget("SIN"); t->setValue(isPlayingSINBUF);}
void testApp::synthkPWM(float & _value){mysynth.setParam(8, kPWM); ofxUIRotarySlider * r = (ofxUIRotarySlider*) guiOSCPWM->getWidget("WDT"); r->setValue(kPWM);}
void testApp::synthkPanSAW(float & _value){mysynth.setParam(10, kPanSAW); ofxUIRotarySlider * r = (ofxUIRotarySlider*) guiOSCSAW->getWidget("PAN"); r->setValue(kPanSAW);}
void testApp::synthkPanTRI(float & _value){mysynth.setParam(11, kPanTRI); ofxUIRotarySlider * r = (ofxUIRotarySlider*) guiOSCTRI->getWidget("PAN"); r->setValue(kPanTRI);}
void testApp::synthkPanSQU(float & _value){mysynth.setParam(12, kPanSQU); ofxUIRotarySlider * r = (ofxUIRotarySlider*) guiOSCSQU->getWidget("PAN"); r->setValue(kPanSQU);}
void testApp::synthkPanPULSE(float & _value){mysynth.setParam(13, kPanPULSE); ofxUIRotarySlider * r = (ofxUIRotarySlider*) guiOSCPWM->getWidget("PAN"); r->setValue(kPanPULSE);}
void testApp::synthkPanNOISE(float & _value){mysynth.setParam(14, kPanNOISE); ofxUIRotarySlider * r = (ofxUIRotarySlider*) guiOSCNOS->getWidget("PAN"); r->setValue(kPanNOISE);}
void testApp::synthkPanSINBUF(float & _value){mysynth.setParam(15, kPanSINBUF); ofxUISlider * r = (ofxUISlider*) guiOSCSIN->getWidget("PAN"); r->setValue(kPanSINBUF);}
void testApp::synthkVolSIN(float & _value){mysynth.setParam(16, kVolSIN); ofxUISlider * s = (ofxUISlider*) guiOSCSIN->getWidget("VOL"); s->setValue(kVolSIN); }
void testApp::synthkVolSAW(float & _value){mysynth.setParam(17, kVolSAW); ofxUISlider * s = (ofxUISlider*) guiOSCSIN->getWidget("VOL"); s->setValue(kVolSAW); }
void testApp::synthkVolTRI(float & _value){mysynth.setParam(18, kVolTRI); ofxUISlider * s = (ofxUISlider*) guiOSCTRI->getWidget("VOL"); s->setValue(kVolTRI); }
void testApp::synthkVolSQU(float & _value){mysynth.setParam(19, kVolSQU);ofxUISlider * s = (ofxUISlider*) guiOSCSQU->getWidget("VOL"); s->setValue(kVolSQU); }
void testApp::synthkVolPULSE(float & _value){mysynth.setParam(20, kVolPULSE);ofxUISlider * s = (ofxUISlider*) guiOSCPWM->getWidget("VOL"); s->setValue(kVolPULSE); }
void testApp::synthkVolNOISE(float & _value){mysynth.setParam(21, kVolNOISE); ofxUISlider * s = (ofxUISlider*) guiOSCNOS->getWidget("VOL"); s->setValue(kVolNOISE); }
void testApp::synthkHarmSIN(int & _value){mysynth.setParam(22, kHarmSIN); ofxUISlider * s = (ofxUISlider*) guiOSCSIN->getWidget("HRM"); s->setValue(kHarmSIN); }
void testApp::synthkHarmSAW(int & _value){mysynth.setParam(23, kHarmSAW); ofxUISlider * s = (ofxUISlider*) guiOSCSAW->getWidget("HRM"); s->setValue(kHarmSAW); }
void testApp::synthkHarmTRI(int & _value){mysynth.setParam(24, kHarmTRI); ofxUISlider * s = (ofxUISlider*) guiOSCTRI->getWidget("HRM"); s->setValue(kHarmTRI); }
void testApp::synthkHarmSQU(int & _value){mysynth.setParam(25, kHarmSQU); ofxUISlider * s = (ofxUISlider*) guiOSCSQU->getWidget("HRM"); s->setValue(kHarmSQU); }
void testApp::synthkHarmPULSE(int & _value){mysynth.setParam(26, kHarmPULSE); ofxUISlider * s = (ofxUISlider*) guiOSCPWM->getWidget("HRM"); s->setValue(kHarmPULSE); }

void testApp::synthklfoModPitchSINBUFamount(float & _value){mysynth.setParam(54, klfoModPitchSINBUFamount); ofxUISlider * s = (ofxUISlider*) guiOSCSIN->getWidget("MOD"); s->setValue(klfoModPitchSINBUFamount); }
void testApp::synthklfoModPitchTRIamount(float & _value){mysynth.setParam(55, klfoModPitchTRIamount); ofxUISlider * s = (ofxUISlider*) guiOSCTRI->getWidget("MOD"); s->setValue(klfoModPitchTRIamount); }
void testApp::synthklfoModPitchSAWamount(float & _value){mysynth.setParam(56, klfoModPitchSAWamount); ofxUISlider * s = (ofxUISlider*) guiOSCSAW->getWidget("MOD"); s->setValue(klfoModPitchSAWamount); }
void testApp::synthklfoModPitchSQUamount(float & _value){mysynth.setParam(57, klfoModPitchSQUamount); ofxUISlider * s = (ofxUISlider*) guiOSCSQU->getWidget("MOD"); s->setValue(klfoModPitchSQUamount); }
void testApp::synthklfoModPitchPWMamount(float & _value){mysynth.setParam(58, klfoModPitchPWMamount); ofxUISlider * s = (ofxUISlider*) guiOSCPWM->getWidget("M.1"); s->setValue(klfoModPitchPWMamount); }
void testApp::synthklfoModPulsePWMamount(float & _value){mysynth.setParam(59, klfoModPulsePWMamount); ofxUISlider * s = (ofxUISlider*) guiOSCPWM->getWidget("M.2"); s->setValue(klfoModPulsePWMamount); }

void testApp::synthA1(float & _value){mysynth.setParam(30, A1); ofxUISlider * s = (ofxUISlider*) guiENV1->getWidget("A"); s->setValue(A1); }
void testApp::synthD1(float & _value){mysynth.setParam(31, D1); ofxUISlider * s = (ofxUISlider*) guiENV1->getWidget("D"); s->setValue(D1);}
void testApp::synthS1(float & _value){mysynth.setParam(32, S1); ofxUISlider * s = (ofxUISlider*) guiENV1->getWidget("S"); s->setValue(S1);}
void testApp::synthR1(float & _value){mysynth.setParam(33, R1); ofxUISlider * s = (ofxUISlider*) guiENV1->getWidget("R"); s->setValue(R1);}
void testApp::synthA2(float & _value){mysynth.setParam(34, A2); ofxUISlider * s = (ofxUISlider*) guiENV2->getWidget("A"); s->setValue(A2);}
void testApp::synthD2(float & _value){mysynth.setParam(35, D2); ofxUISlider * s = (ofxUISlider*) guiENV2->getWidget("D"); s->setValue(D2);}
void testApp::synthS2(float & _value){mysynth.setParam(36, S2); ofxUISlider * s = (ofxUISlider*) guiENV2->getWidget("S"); s->setValue(S2);}
void testApp::synthR2(float & _value){mysynth.setParam(37, R2); ofxUISlider * s = (ofxUISlider*) guiENV2->getWidget("R"); s->setValue(R2);}
void testApp::synthkEnv2ToFilter(float & _value){mysynth.setParam(38, kEnv2ToFilter); ofxUISlider * s = (ofxUISlider*) guiENV2->getWidget("DEPTH"); s->setValue(kEnv2ToFilter); }
void testApp::synthisFilter1Playing(bool & _value){mysynth.setParam(40, isFilter1Playing); ofxUIToggle * t = (ofxUIToggle*) guiFILTER1->getWidget("LOWPASS"); t->setValue(isFilter1Playing); }
void testApp::synthkFilter1Frequency(float & _value){mysynth.setParam(41, kFilter1Frequency); ofxUIRotarySlider * r = (ofxUIRotarySlider*) guiFILTER1->getWidget("CUTOFF"); r->setValue(kFilter1Frequency); }
void testApp::synthkFilter1Resonance(float & _value){mysynth.setParam(42, kFilter1Resonance); ofxUIRotarySlider * r = (ofxUIRotarySlider*) guiFILTER1->getWidget("RES"); r->setValue(kFilter1Resonance); }
void testApp::synthisLfoPlaying(bool & _value){mysynth.setParam(50, isLfoPlaying);}
void testApp::synthkWaveformLfo(int & _value){mysynth.setParam(51, kWaveformLfo);}
void testApp::synthkLFOmode(int & _value){mysynth.setParam(52, kLFOmode);}
void testApp::synthkLFOfreq(float &_value){mysynth.setParam(53, kLFOfreq);}

//void testApp::synthsetPlaySAMPLER(bool & _value){mysynth.setParam(100, setPlaySAMPLER);}
//void testApp::synthLoopMode(int & _value){mysynth.setParam(101, LoopMode);}

void testApp::reverbkReverbRoomSize(float & _value){mysynth.setParam(200,kReverbRoomSize);}
void testApp::reverbkReverbDamp(float & _value){mysynth.setParam(201,kReverbDamp);}
void testApp::reverbkReverbWet(float & _value){mysynth.setParam(202,kReverbWet);}
void testApp::reverbisPlayingREVERB(bool & _value){mysynth.setParam(203,isPlayingREVERB);}
void testApp::reverbkReverbwidth(float & _value){mysynth.setParam(204,kReverbwidth);}

void testApp::fxsetPlayDistor(bool &_value){mysynth.setParam(210,setPlayDistor);}
void testApp::fxsetLevelDistor(int &_value){mysynth.setParam(211,setLevelDistor);}
void testApp::fxsetGainDistor(float &_value){mysynth.setParam(212,setGainDistor);}

void testApp::fxsetDistorMode(int &_value){
    mysynth.setParam(213,setDistorMode);
    ofxUIRadio *d = (ofxUIRadio *) guiDISTOR->getWidget("DISTORMODE"); setDistorMode=(d->whichIsTrue());
}


//----------------------------------------------------------------------------------------------------

// GUI EVENT

//----------------------------------------------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
     if(ofGetLogLevel()==OF_LOG_VERBOSE) cout <<"guiEvent ... name : " << e.widget->getName() << " parent : " << e.widget->getParent()->getName()<<endl;
    
    
    if(e.widget->getParent()->getName()=="GLOBALS")
    {
        if(e.widget->getName() == "ON-OFF") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;masterPower= (toggle->getValue());}
        else if(e.widget->getName() == "RANDOMIZE")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            if(toggle->getValue()==0){this->randomize();}
        }
        else if(e.widget->getName() == "SETTINGS")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            if (button->getValue())
            {
                guiGlobal->disable();
                
                guiOSCSIN->disable();
                guiOSCTRI->disable();
                guiOSCSAW->disable();
                guiOSCPWM->disable();
                guiOSCSQU->disable();
                guiOSCNOS->disable();
                
                guiDISTOR->disable();
                guiENV1->disable();
                guiENV2->disable();
                guiFILTER1->disable();
                guiREVERB->disable();
                guiLFO1->disable();
                
                
                guiSettings->enable();
                
            }
        }
        
    }
    else if((e.widget->getParent()->getName()=="CONFIG SETTINGS") || (e.widget->getParent()->getName()=="AUDIOOUTDEVICE"))
    {
        if(e.widget->getName() == "FULLSCREEN")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            ofSetFullscreen(toggle->getValue());
        }
        else if(e.widget->getName() == "BACK")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            if (button->getValue())
            {
                guiGlobal->enable();
                
                guiOSCSIN->enable();
                guiOSCTRI->enable();
                guiOSCSAW->enable();
                guiOSCPWM->enable();
                guiOSCSQU->enable();
                guiOSCNOS->enable();
                
                guiDISTOR->enable();
                guiENV1->enable();
                guiENV2->enable();
                guiFILTER1->enable();
                guiREVERB->enable();
                guiLFO1->enable();
                
                guiSettings->disable();
                
            }
        }
        else if(e.widget->getName() == "MIDIINDEVICE")
        {
            ofxUIRadio *midiInDevices = (ofxUIRadio*) e.widget->getParent();
            midiIn.closePort();
            int w = midiInDevices->whichIsTrue();
            midiIn.openPort(w-1);
        }
        else if(e.widget->getParent()->getName() == "AUDIOOUTDEVICE")
        {
            ofxUIRadio *audioOutDevice = (ofxUIRadio*) e.widget->getParent();
            
            int w = audioOutDevice->whichIsTrue();
            
            setupAudioDevice(w-1);
            
            
        }
        
    }
    else if(e.widget->getParent()->getName()=="SIN")
    {
        if(e.widget->getName() == "SIN") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;isPlayingSINBUF= (toggle->getValue());}
        else if(e.widget->getName() == "VOL") {ofxUISlider *slider = (ofxUISlider *) e.widget;kVolSIN= (slider->getScaledValue());}
        else if(e.widget->getName() == "HRM") {ofxUISlider *slider = (ofxUISlider *) e.widget;kHarmSIN= (slider->getScaledValue());}
        else if(e.widget->getName() == "PAN") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;kPanSINBUF= (rotary->getScaledValue());}
        else if(e.widget->getName() == "MOD") {ofxUISlider *slider = (ofxUISlider *) e.widget;klfoModPitchSINBUFamount= (slider->getScaledValue());}
        
    }
    else if(e.widget->getParent()->getName()=="TRI")
    {
        if(e.widget->getName() == "TRI") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;isPlayingTRI= (toggle->getValue());}
        else if(e.widget->getName() == "VOL") {ofxUISlider *slider = (ofxUISlider *) e.widget;kVolTRI= (slider->getScaledValue());}
        else if(e.widget->getName() == "HRM") {ofxUISlider *slider = (ofxUISlider *) e.widget;kHarmTRI= (slider->getScaledValue());}
        else if(e.widget->getName() == "PAN") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;kPanTRI= (rotary->getScaledValue());}
        else if(e.widget->getName() == "MOD") {ofxUISlider *slider = (ofxUISlider *) e.widget;klfoModPitchTRIamount= (slider->getScaledValue());}
        
    }
    else if(e.widget->getParent()->getName()=="SAW")
    {
        if(e.widget->getName() == "SAW") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;isPlayingSAW= (toggle->getValue());}
        else if(e.widget->getName() == "VOL") {ofxUISlider *slider = (ofxUISlider *) e.widget;kVolSAW= (slider->getScaledValue());}
        else if(e.widget->getName() == "HRM") {ofxUISlider *slider = (ofxUISlider *) e.widget;kHarmSAW= (slider->getScaledValue());}
        else if(e.widget->getName() == "PAN") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;kPanSAW= (rotary->getScaledValue());}
        else if(e.widget->getName() == "MOD") {ofxUISlider *slider = (ofxUISlider *) e.widget;klfoModPitchSAWamount= (slider->getScaledValue());}
        
    }
    else if(e.widget->getParent()->getName()=="SQU")
    {
        if(e.widget->getName() == "SQU") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;isPlayingSQU= (toggle->getValue());}
        else if(e.widget->getName() == "VOL") {ofxUISlider *slider = (ofxUISlider *) e.widget;kVolSQU= (slider->getScaledValue());}
        else if(e.widget->getName() == "HRM") {ofxUISlider *slider = (ofxUISlider *) e.widget;kHarmSQU= (slider->getScaledValue());}
        else if(e.widget->getName() == "PAN") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;kPanSQU= (rotary->getScaledValue());}
        else if(e.widget->getName() == "MOD") {ofxUISlider *slider = (ofxUISlider *) e.widget;klfoModPitchSQUamount= (slider->getScaledValue());}
        
    }
    else if(e.widget->getParent()->getName()=="PWM")
    {
        if(e.widget->getName() == "PWM") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;isPlayingPULSE= (toggle->getValue());}
        else if(e.widget->getName() == "VOL") {ofxUISlider *slider = (ofxUISlider *) e.widget;kVolPULSE= (slider->getScaledValue());}
        else if(e.widget->getName() == "HRM") {ofxUISlider *slider = (ofxUISlider *) e.widget;kHarmPULSE= (slider->getScaledValue());}
        else if(e.widget->getName() == "PAN") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;kPanPULSE= (rotary->getScaledValue());}
        else if(e.widget->getName() == "M.1") {ofxUISlider *slider = (ofxUISlider *) e.widget;klfoModPitchPWMamount= (slider->getScaledValue());}
        else if(e.widget->getName() == "M.2") {ofxUISlider *slider = (ofxUISlider *) e.widget;klfoModPulsePWMamount= (slider->getScaledValue());}
        
    }
    else if(e.widget->getParent()->getName()=="NOISE")
    {
        if(e.widget->getName() == "NOS") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;isPlayingNOISE= (toggle->getValue());}
        else if(e.widget->getName() == "VOL") {ofxUISlider *slider = (ofxUISlider *) e.widget;kVolNOISE= (slider->getScaledValue());}
        else if(e.widget->getName() == "PAN") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;kPanNOISE= (rotary->getScaledValue());}
        
    }
    else if(e.widget->getParent()->getName()=="ENV VOLUMEN")
    {
        if(e.widget->getName() == "A") {ofxUISlider *slider = (ofxUISlider *) e.widget;A1= (slider->getScaledValue());}
        else if(e.widget->getName() == "D") {ofxUISlider *slider = (ofxUISlider *) e.widget;D1= (slider->getScaledValue());}
        else if(e.widget->getName() == "S") {ofxUISlider *slider = (ofxUISlider *) e.widget;S1= (slider->getScaledValue());}
        else if(e.widget->getName() == "R") {ofxUISlider *slider = (ofxUISlider *) e.widget;R1= (slider->getScaledValue());}
        
    }
    else if(e.widget->getParent()->getName()=="ENV FILTER")
    {
        if(e.widget->getName() == "A") {ofxUISlider *slider = (ofxUISlider *) e.widget;A2= (slider->getScaledValue());}
        else if(e.widget->getName() == "D") {ofxUISlider *slider = (ofxUISlider *) e.widget;D2= (slider->getScaledValue());}
        else if(e.widget->getName() == "S") {ofxUISlider *slider = (ofxUISlider *) e.widget;S2= (slider->getScaledValue());}
        else if(e.widget->getName() == "R") {ofxUISlider *slider = (ofxUISlider *) e.widget;R2= (slider->getScaledValue());}
        else if(e.widget->getName() == "DEPTH") {ofxUISlider *slider = (ofxUISlider *) e.widget;kEnv2ToFilter= (slider->getScaledValue());}
        
        
    }
    else if(e.widget->getParent()->getName()=="REVERB")
    {
        if(e.widget->getName() == "SIZE") {ofxUISlider *slider = (ofxUISlider *) e.widget;kReverbRoomSize= (slider->getScaledValue());}
        else if(e.widget->getName() == "DAMP") {ofxUISlider *slider = (ofxUISlider *) e.widget;kReverbDamp= (slider->getScaledValue());}
        else if(e.widget->getName() == "WIDTH") {ofxUISlider *slider = (ofxUISlider *) e.widget;kReverbwidth= (slider->getScaledValue());}
        else if(e.widget->getName() == "MIX") {ofxUISlider *slider = (ofxUISlider *) e.widget;kReverbWet= (slider->getScaledValue());}
        else if(e.widget->getName() == "REVERB") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;isPlayingREVERB= (toggle->getValue());}
        
    }
    else if(e.widget->getParent()->getName()=="FILTER1")
    {
        if(e.widget->getName() == "LOWPASS") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;isFilter1Playing= (toggle->getValue());}
        else if(e.widget->getName() == "CUTOFF") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;kFilter1Frequency= (rotary->getValue());}
        else if(e.widget->getName() == "RES") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;kFilter1Resonance= (rotary->getValue());}
        
    }
    else if(e.widget->getParent()->getName()=="DISTOR")
    {
        if(e.widget->getName() == "DISTORSION") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;setPlayDistor= (toggle->getValue());}
        else if(e.widget->getName() == "LEVEL") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;setLevelDistor= (rotary->getScaledValue());}
        else if(e.widget->getName() == "GAIN") {ofxUIRotarySlider *rotary = (ofxUIRotarySlider *) e.widget;setGainDistor= (rotary->getValue());}
    }
    else if(e.widget->getParent()->getName()=="DISTORMODE")
    {
        ofxUIRadio *distorMode = (ofxUIRadio *) e.widget->getParent();
        int w = distorMode->whichIsTrue();
        setDistorMode= w;
        
    }
    else if(e.widget->getParent()->getName()=="LFO")
    {
        if(e.widget->getName() == "LFO") {ofxUIToggle *toggle = (ofxUIToggle *) e.widget;isLfoPlaying= (toggle->getValue());}
        
        else if(e.widget->getName() == "LFOMODE")
        {
            ofxUIRadio *lfoModeRadio = (ofxUIRadio *) e.widget;
            int w = lfoModeRadio->whichIsTrue();
            kLFOmode= w;
        }
        else if(e.widget->getName()== "FIXED FREQ"){ofxUISlider *s = (ofxUISlider *) e.widget; kLFOfreq = (s->getValue());}
        
    }
    else if(e.widget->getParent()->getName() == "LFOWAVEFORM")
    {
        ofxUIRadio *lfoWaveformRadio = (ofxUIRadio *) e.widget->getParent();
        int w = lfoWaveformRadio->whichIsTrue();
        kWaveformLfo= w;
    }

    
    /// GUI SETTINGS
    if(e.widget->getName() == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());
    }
    if(e.widget->getName() == "BACK")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue())
        {
            guiGlobal->enable();
            
            guiOSCSIN->enable();
            guiOSCTRI->enable();
            guiOSCSAW->enable();
            guiOSCPWM->enable();
            guiOSCSQU->enable();
            guiOSCNOS->enable();
            
            guiDISTOR->enable();
            guiENV1->enable();
            guiENV2->enable();
            guiFILTER1->enable();
            guiREVERB->enable();
            guiLFO1->enable();
            
            guiSettings->disable();
            
        }
    }
    
}

//----------------------------------------------------------------------------------------------------
void testApp::randomize(){
    
    
    
    srand(TIME_ABSOLUTE);
    
    /* OSC1 */
    
    isPlayingSAW = rand() % 2 ;//2
    isPlayingTRI = rand() % 2 ;//3
    isPlayingSQU = rand() % 2 ;//4
    isPlayingPULSE = rand() % 2 ;//5
    isPlayingNOISE = rand() % 2 ;//6
    isPlayingSINBUF = rand() % 2 ;//7
    kPWM = (float)rand()/ RAND_MAX;//8
    
    kPanSAW = (128.f * ((float)rand()/ RAND_MAX)) - 64.f;//10
    kPanTRI = (128.f * ((float)rand()/ RAND_MAX)) - 64.f ;//11
    kPanSQU = (128.f * ((float)rand()/ RAND_MAX)) - 64.f ;//12
    kPanPULSE = (128.f * ((float)rand()/ RAND_MAX)) - 64.f;//13
    kPanNOISE = (128.f * ((float)rand()/ RAND_MAX)) - 64.f;//14
    kPanSINBUF = (128.f * ((float)rand()/ RAND_MAX)) - 64.f ;//15
    
    kVolSIN = 127.f * ( 1.f - eFn.getParam(((float)rand()/ RAND_MAX)));//16
    kVolSAW = 127.f * ( 1.f - eFn.getParam(((float)rand()/ RAND_MAX)));//17
    kVolTRI = 127.f * ( 1.f - eFn.getParam(((float)rand()/ RAND_MAX)));//18
    kVolSQU = 127.f * ( 1.f - eFn.getParam(((float)rand()/ RAND_MAX)));//19
    kVolPULSE = 127.f * ( 1.f - eFn.getParam(((float)rand()/ RAND_MAX)));//20
    kVolNOISE = 127.f * ( 1.f - eFn.getParam(((float)rand()/ RAND_MAX)));//21
    
    kHarmSIN = 8.f * ((float)rand()/ RAND_MAX);//22
    kHarmSAW = 8.f * ((float)rand()/ RAND_MAX);//23
    kHarmTRI = 8.f * ((float)rand()/ RAND_MAX);//24
    kHarmSQU = 8.f * ((float)rand()/ RAND_MAX);//25
    kHarmPULSE = 8.f * ((float)rand()/ RAND_MAX);//26
    
    /* ENV 1 */
    A1 = eFn.getParam((float)rand()/ RAND_MAX);//30
    D1 = eFn.getParam((float)rand()/ RAND_MAX) ;//31
    S1 = (float)rand()/ RAND_MAX ;//32
    R1 = eFn.getParam((float)rand()/ RAND_MAX );//33
    
    /* ENV 2 */
    A2 = eFn.getParam((float)rand()/ RAND_MAX) ;//34
    D2 = eFn.getParam((float)rand()/ RAND_MAX );//35
    S2 = (float)rand()/ RAND_MAX ;//36
    R2 = eFn.getParam((float)rand()/ RAND_MAX ) ;//37
    kEnv2ToFilter = (2.f * ((float)rand()/ RAND_MAX )) - 1.f;//38
    
    /*FILTER 1 */
    
    isFilter1Playing = rand() % 2 ;//40
    kFilter1Frequency = (float)rand()/ RAND_MAX ;//41
    kFilter1Resonance = (float)rand()/ RAND_MAX ;//42
    
    /* LFO */
    isLfoPlaying = rand() % 2;//50
    //kWaveformLfo = 0;//51
    kLFOfreq=eFn.getParam((float)rand()/ RAND_MAX);//53
    klfoModPitchSINBUFamount=eFn.getParam((float)rand()/ RAND_MAX);//54
    klfoModPitchTRIamount=eFn.getParam((float)rand()/ RAND_MAX);//55
    klfoModPitchSAWamount=eFn.getParam((float)rand()/ RAND_MAX);//56
    klfoModPitchSQUamount=eFn.getParam((float)rand()/ RAND_MAX);//57
    klfoModPitchPWMamount=eFn.getParam((float)rand()/ RAND_MAX);//58
    klfoModPulsePWMamount=eFn.getParam((float)rand()/ RAND_MAX);//59
    klfoOffset = (2. * (float)rand()/RAND_MAX) - 1.f; //60
    
    
    
}
