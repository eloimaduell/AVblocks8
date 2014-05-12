//
//  AVenv.h
//  ignasiAVsynth_0003
//
//  Written by ignasi on 03/01/13.
//
//

#include "AVglobals.h"
#include "ofMath.h"

#ifndef ignasiAVsynth_0003_AVenv_h
#define ignasiAVsynth_0003_AVenv_h

/* 

 avENV objects are ADSR generators with linear slopes and max time for the ramps set in globals.h
 Output value ranges form 0. to 1. 
 Paramteres are set in normalized 0.~1. ranges 

*/

class avENV {
    
public:
    
    avENV();
    ~avENV();
    
    void setAttack(float _attack); // parameter control value 0.~ 1. related to global variable MAX_ENV_ATTACK
    void setDecay(float _decay);  // parameter control value 0.~ 1. related to global variable MAX_ENV_DECAY
    void setSustain(float _sustain); // parameter control value 0.~ 1.
    void setRelease(float _release); // parameter control value 0.~ 1. related to global variable MAX_ENV_RELEASE
    
    float* adsr(); // returns a 2chan audio signal from 0. to 1. : to be used as modulator
    
    void trigger_on();
	void trigger_off();

private:
    
    float amplitude, releaselevel;
    long unsigned int counter;
    
    bool trigger;
    
    long unsigned int attack; // in samples for internal calc.
	long unsigned int decay; // in samples for internal calc.
    float sustain; // in samples for internal calc.
    long unsigned int release; // in samples for internal calc.
    float* output;
    
    ParamSmooth attackSmoother;
    ParamSmooth decaySmoother;
    ParamSmooth sustainSmoother;
    ParamSmooth releaseSmoother;
    
    SignalClamper clampAmplitude;

};


#endif
