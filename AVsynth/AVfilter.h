//
//  AVfilter.cpp
//  ignasiAVsynth_0003
//
//  Written by ignasi on 04/01/13
//
//  avFILTER derives from ofxMaximilian classes : http://www.maximilian.strangeloop.co.uk
//
//  Exp freq trick from musicdsp.org http://musicdsp.org/
//

#include "AVglobals.h"
#include "ofMath.h"



#ifndef ignasiAVsynth_0003_AVfilter_h
#define ignasiAVsynth_0003_AVfilter_h

/*
 
 This is the Maximilian Low pass filter with resonance.
 avFILTER objects deliver filtered signal received thru an input.
 Bypass button smoothly (?) crossfades between dry and wet signals.
 -- seems to have a dirty point at 0.5 cutoff --
 
 */

class avFILTER: public audioBlock {

public :
    
    avFILTER();
    ~avFILTER();
    
    void    setPlay(bool _isPlaying); // ON-OFF (bypass)
    void setFrequency(double _value); // _value is in Hz.
    void setResonance(double _value); //_value from 0. to 1.
    
    void patchCutoffModSignal(float* _cutoffModSignal);
    void setCutoffModDepth(float _cutoffModDepth);
    
    float* getAudioOut(float* _input);
    
private:
    
    double frequency, exp_freq, freq_max, ln_freq_max, freq_min, ln_freq_min;
    double resonance;
    double CUTOFF, QFACTOR;
    
    bool isPlaying;
    
    avWIN declick;
    ParamSmooth smoothFreq;
    ParamSmooth smoothResonance;
    ParamSmooth smoothCutoffModDepth;
   
    double x1,x2;//speed
	double y1,y2;//pos
	double z1,z2;//pole
	double c1,c2,r1,r2;//filter coefficient
    
    /* patches */
    float* declicker;
    float* cutoffModSignal;
    float cutoffModDepth;
    
   
    
};



#endif
