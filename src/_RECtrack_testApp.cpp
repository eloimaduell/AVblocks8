#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
/* ===== MIDI ===== */
    
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
	//midiIn.setVerbose(true);
    
/*=====END midi ====*/
    
    
/*==== AUDIO ====*/
    
    /* Audio "devices" init. */
    
    audioOutL.assign(BUFFER_SIZE, 0.0);
	audioOutR.assign(BUFFER_SIZE, 0.0);
    
	audioInL.assign(BUFFER_SIZE, 0.0);
	audioInR.assign(BUFFER_SIZE, 0.0);
    
    Aux1L.assign(BUFFER_SIZE, 0.0);
    Aux1R.assign(BUFFER_SIZE, 0.0);
    
    //   synthOut = new float[2];
    //   synthOut[0]=synthOut[1]=0.;
    trackOut = new float[2];
    trackOut[0]=trackOut[1]=0.;
    trackIn = new float[2];
    trackIn[0]=trackIn[1]=0.;
    
    
    /* Soundstream init. */
        
    soundStream.setDeviceID(3); //   <--- remember to select device here !!!
    soundStream.setup(this, NUM_CHANNELS, NUM_CHANNELS, SAMPLING_RATE, BUFFER_SIZE, 2); /* MACKIE Onyx Blackjack */
    // soundStream.setup(this, 2, 0, SAMPLING_RATE, BUFFER_SIZE, 4);  /*Built-in*/
    soundStream.listDevices();

/*=====END audio =========*/
    
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    mytrack.drawTrack(0,0,500,100);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key)
    {
        case 'c' : mytrack.clear();break;
        case 'u' : mytrack.undo();break;
        case 'r' : mytrack.setRecMode(1);break;
        case 'w' : mytrack.setRecMode(2);break;
      //  case 'i' : mytrack.setInPoint();break;
      //  case 'o' : mytrack.setOutPoint();break;
        case '1' : mytrack.playSw0();break;
        case '2' : mytrack.playSw1();break;
        case '3' : mytrack.playSw2();break;
        case 'i' : mytrack.setInPoint();break;
        case 'o' : mytrack.setOutPoint();break;
        case 'l' : mytrack.loopSw(true);break;
        case 'k' : mytrack.loopSw(false);break;
        case 'p' : mytrack.resetInOutPoints();break;
    }
    ofSleepMillis(10);
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key)
    {
        case 'r' : mytrack.setRecMode(0);break;
        case 'w' : mytrack.setRecMode(0);break;
    }
    ofSleepMillis(10);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
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

//- MIDI --------------------------------------------------------------


void testApp::newMidiMessage(ofxMidiMessage& msg) {
    
 	// make a copy of the latest message
	midiMessage = msg;
    
    switch (midiMessage.status)
    {
            
            // channel voice messages
        case MIDI_NOTE_OFF :
            //  mysynth.note_off(midiMessage.pitch);
            break;
        case MIDI_NOTE_ON :
            //  mysynth.note_on(midiMessage.pitch, midiMessage.velocity);
            break;
        case MIDI_CONTROL_CHANGE : {/* */}; break;
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





//- AUDIO -------------------------------------------------------------

void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){
	
	for (int i = 0; i < bufferSize; i++){
        
        audioInL[i]	= input[i*nChannels  ];
		audioInR[i]	= input[i*nChannels+1];
        trackIn[0]=audioInL[i];
        trackIn[1]=audioInR[i];
        mytrack.setAudioIn(trackIn);
    }
	
}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
    
    for (int i = 0; i < bufferSize; i++){
        
        trackOut=mytrack.getAudioOut();
       
        //  synthOut = mysynth.getAudioOut();
        
        audioOutL[i] = output[i*nChannels    ] = trackOut[0];
        audioOutR[i] = output[i*nChannels + 1] = trackOut[1];
        
    }
    
}

