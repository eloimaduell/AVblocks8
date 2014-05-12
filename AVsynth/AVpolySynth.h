//
//  AVpolySynth.h
//  ignasiAVsynth_0003
//
//  Written by ignasi on 02/01/13.
//
//  inspired by  ofxMaximilian classes : http://www.maximilian.strangeloop.co.uk
//  ...and from musicdsp.org http://musicdsp.org/
//
//
#include "ofMain.h"
#include "Math.h"
#include "AVglobals.h"
#include "AVvoice.h"
#include "AVsamplerVoice.h"
#include "AVdistor.h"

#ifndef ignasiAVsynth_0003_AVpolySynth_h
#define ignasiAVsynth_0003_AVpolySynth_h

/*
 
 avPOLYSYNTH objects holds two arrays of a NUM_VOICES number of voices : synth and sampler simultaneously.
 Each voice is orderly triggered on-off by midi notes.
 Voice alocation is done by a straightforward oldest voice algorithm.
 Parameters are passed simultaneously to all voices.
 Audio out is a mix of both synth and sampler voices.
 
 Parameters are listed on :
 enum at AVparameters.h : the old vst style
 init method at AVpolySynth.cpp
 setParam methods at AVvoice.cpp
 
 */

class avPOLYSYNTH: public audioBlock {

public:
    
    avPOLYSYNTH();
    ~avPOLYSYNTH();
    
    void setParam(int _index, int _value);
    void setParam(int _index, float _value);
    void setParam(int _index, bool _value);
    void note_on(int _note_number, int _note_velocity);
    void note_off(int _note_number);
    void init();
  
    float* getAudioOut();
    
private:
    
    float* VoicesMixer;
    float* VoicesOut;
    float* polySamplerAudioOut;
   // float* SamplerMixer;
    
    int assigned_voice;
    
    avVOICE* voices;
 //   avSAMPLERVOICE* mysampler;
    DCremoval DCremover;
    
    SignalClamper clampMains;
    
    revmodel reverb;
    
    float* distorOut;
    
    avDISTOR distor;

    

};


#endif
