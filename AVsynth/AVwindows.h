//
//  AVwindows.h
//  ignasiAVsynth_0001
//
//  Written by ignasi on 30/12/12.
//
//

#include "AVglobals.h"
#include "ofMath.h"

#ifndef ignasiAVsynth_0001_AVwindows_h
#define ignasiAVsynth_0001_AVwindows_h

/*
 
 a simple attack-release linear ramp window to be used as declicker
 
 */

class avWIN {
 
public:
    
    avWIN();
    ~avWIN();
    
    void setAttack(float _attack);
    void setRelease(float _release);
    
    float* ar(); // attack release
    
    
    void trigger_on();
	void trigger_off();
    
private:
    
    float amplitude;
    bool attackphase,releasephase;
    
    bool trigger;
    
    float attack; //in ms
    float release; //in ms
    float* output;

};









#endif
