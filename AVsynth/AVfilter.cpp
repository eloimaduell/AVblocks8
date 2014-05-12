
#include "AVfilter.h"



avFILTER::avFILTER()
{
    /* declick ramps */
    declick.setAttack(DECLICK_LONG);
    declick.setRelease(DECLICK_LONG);
    declicker=new float[2];
    declicker[0]=declicker[1]=0.;
    
    cutoffModSignal=new float[2];
    cutoffModSignal[0]=cutoffModSignal[1]=0.;
    cutoffModDepth = 0.;
    
    freq_max=SAMPLING_RATE/6.;
    freq_min=40.;
    ln_freq_max=log(freq_max);
    ln_freq_min=log(freq_min);
    frequency=exp_freq=CUTOFF=440.;
    
    resonance=QFACTOR=1.;
    x1=y1=z1=c1=x2=y2=z2=c2=r1=r2=0.;
    isPlaying=true;

};

avFILTER::~avFILTER()
{
    delete declicker, exp_freq, cutoffModSignal;
};

void    avFILTER::setPlay(bool _isPlaying)
{   // ON-OFF (bypass)
    isPlaying = _isPlaying;
    if(isPlaying==true)  declick.trigger_on();
    if(isPlaying==false) declick.trigger_off();
}; 


void avFILTER::setFrequency(double _value)
{
    // _value is in 0-1 range.
    exp_freq = pow(3., (_value * (ln_freq_max-ln_freq_min)) + ln_freq_min ) - 17.5497;
    
    if (exp_freq < freq_min){exp_freq = freq_min;}
    if (exp_freq > freq_max){exp_freq = freq_max;}
   
    smoothFreq.setNewValue(exp_freq);
        
}; 


void avFILTER::setResonance(double _value)
{
   //_value from 0. to 1.
    resonance=pow(7.*_value,2);
    if (resonance<1.) resonance = 1.;
    smoothResonance.setNewValue(resonance);

};

void avFILTER::patchCutoffModSignal(float* _cutoffModSignal)
{
   cutoffModSignal=_cutoffModSignal ;
}

void avFILTER::setCutoffModDepth(float _cutoffModDepth)
{
    cutoffModDepth = _cutoffModDepth;
    smoothCutoffModDepth.setNewValue(cutoffModDepth);
}

float* avFILTER::getAudioOut(float* _input){
    
    AudioIn = _input;
    declicker = declick.ar();
    frequency=smoothFreq.getValue();
    cutoffModSignal[0]=cutoffModSignal[0]*smoothCutoffModDepth.getValue();
   
    if(cutoffModSignal[0]>=0)
    {
        CUTOFF=frequency+(freq_max-frequency)*cutoffModSignal[0];
    } else {
        CUTOFF=frequency+(frequency-freq_min)*cutoffModSignal[0];
    }
    
    QFACTOR=smoothResonance.getValue();
    
    /* ch1 */
   // z1=cos(TWOPI*CUTOFF/SAMPLING_RATE);

    double phase, remainder;
    phase=fmod(512.*CUTOFF/SAMPLING_RATE, 512.);
    remainder= phase - floor(phase);
    z1 = (float) ((1-remainder) * cosineBuffer[1+ (long) phase] + remainder * cosineBuffer[2+(long) phase]);
    
    c1=2-2*z1;
    r1=(TWOSQRT*sqrt(-pow((z1-1.0),3.0))+QFACTOR*(z1-1))/(QFACTOR*(z1-1));
    x1=x1+(AudioIn[0]-y1)*c1;
    y1=y1+x1;
    x1=x1*r1;
    AudioOut[0]=float(-y1);
    
    /*ch2*/
    //z2=cos(TWOPI*CUTOFF/SAMPLING_RATE);
    
    double phase2, remainder2;
    phase2=fmod(512.*CUTOFF/SAMPLING_RATE, 512.);
    remainder2= phase2 - floor(phase2);
    z2 = (float) ((1-remainder2) * cosineBuffer[1+ (long) phase2] + remainder2 * cosineBuffer[2+(long) phase2]);
    
    c2=2-2*z2;
    r2=(TWOSQRT*sqrt(-pow((z2-1.0),3.0))+QFACTOR*(z2-1))/(QFACTOR*(z2-1));
    x2=x2+(AudioIn[1]-y2)*c2;
    y2=y2+x2;
    x2=x2*r2;
    AudioOut[1]=float(-y2);
    
    /* crossfade  */
    AudioOut[0]=AudioIn[0]*(1.-declicker[0]) + AudioOut[0]*declicker[0];
    AudioOut[1]=AudioIn[1]*(1.-declicker[1]) + AudioOut[1]*declicker[1];
    
    return (AudioOut);

};
