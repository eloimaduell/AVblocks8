//
//  AVsynth.h
//  ignasiAVsynth_0001
//
//  Written by ignasi on 24/12/12.
//
//
#include "ofMain.h"
#include "Math.h"
#include "AVglobals.h"
#include "AVosc.h"
#include "AVpanner.h"
#include "AVenv.h"
#include "AVfilter.h"
#include "AVlfo.h"


#ifndef ignasiAVsynth_0001_AVvoice_h
#define ignasiAVsynth_0001_AVvoice_h

/*
 
 avVOICE objects define the structure for the synth.
 They are instantiated by the polySynhth, which then passes them notes and control parameters, from GUI or MIDI.
 The avVOICE object delivers an audio signal with all the synth features.
 
 */

class avVOICE: public audioBlock{
 
public:
    
    avVOICE();
    ~avVOICE();
    
    void setPlay(bool _isPlaying); // engage-disengage voice
    void setParam(int _index, int _value); // set int parameters
    void setParam(int _index, float _value); // set float parameters
    void setParam(int _index, bool _value); // set bool parameters
    void note_on(int _note_number, int _note_velocity); // trigger a note with velocity
    int get_midi_note_playing();
    void note_off(int _note_number); // un-trigger a note ... it's up to you to keep it coherent with note-on's
    float* getAudioOut();

private:
    
    bool    isPlaying;
    int     midi_note_playing;
    
    avOSC   osc1;
    avWIN   declicker1;
    avENV   env1;
    avENV   env2;
    avFILTER filter1;
    avLFO   lfo1;
    
    /* patches */
    Modulator env_to_osc;
    Modulator global_declicker;
    SignalClamper clampOut;
    
};



#endif
