//
//  AVsamplerVoice.h
//  ignasiAVsynth_0006_sampler
//
//  Created by ignasi on 13/01/13.
//
//

#ifndef ignasiAVsynth_0006_sampler_AVsamplerVoice_h
#define ignasiAVsynth_0006_sampler_AVsamplerVoice_h

#include "AVglobals.h"
#include "AVenv.h"
#include "AVfilter.h"
#include "ofxMaxiSampler.h"


class avSAMPLERVOICE: public audioBlock{

public:
    
    avSAMPLERVOICE();
    ~avSAMPLERVOICE();
    
    void setPlay(bool _isPlaying); // ON-OFF
    void setLoopMode(bool _loopMode);
    void setParam(int _index, float _value); // set parameters
    void setParam(int _index, bool _value); // set parameters
    void note_on(int _note_number, int _note_velocity);
    int get_midi_note_playing();
    void note_off(int _note_number);
    float* getAudioOut();
    
private:
    
    bool    isPlaying;
    int midi_note_playing;
    bool loopMode;
    
    maxiSample   sample1L, sample1R;
    float* sampleOut;
    
    avWIN   declicker1;
    avENV   env1;
    avFILTER filter1;
    
    /* patches */
    Modulator env_to_osc;
    Modulator global_declicker;
    SignalClamper clampOut;
    
    
    
};



#endif
