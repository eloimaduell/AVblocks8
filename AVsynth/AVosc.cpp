//
//  AVosc.cpp
//  ignasiAVsynth_0001
//
//  Adapted by ignasi on 27/12/12 from ofxMaximilian classes.
//
//


#include "AVosc.h"



avOSC::~avOSC(){

    delete declick_saw;
    delete declick_tri;
    delete declick_squ;
    delete declick_pulse;
    delete declick_noise;
    delete declick_sinbuf;

    delete sawwave;
    delete triangle;
    delete square;
    delete pulse;
    delete noise;
    delete sinebuf;
    
    

}

avOSC::avOSC(){
  
    phaseSAW=phaseTRI=phaseSQU=phasePULSE=phaseNOISE=phaseSINBUF= 0.0;
    harmSAW=harmTRI=harmSQU=harmPULSE=harmSINBUF=1;
    PWMphase=0.5;
    frequency=freqSAW=freqSQU=freqSINBUF=freqPULSE=freqTRI=440.;
    freq_min= 1.f;
    freq_max= HALF_SAMPLING_RATE;
    
    declick_saw = new float[2];
    declick_tri = new float[2];
    declick_squ = new float[2];
    declick_pulse = new float[2];
    declick_noise = new float[2];
    declick_sinbuf = new float[2];
    
    declickSAW.setAttack(DECLICK_SHORT);
    declickSAW.setRelease(DECLICK_SHORT);
    
    declickTRI.setAttack(DECLICK_SHORT);
    declickTRI.setRelease(DECLICK_SHORT);
    declickSQU.setAttack(DECLICK_SHORT);
    declickSQU.setRelease(DECLICK_SHORT);
    declickPULSE.setAttack(DECLICK_SHORT);
    declickPULSE.setRelease(DECLICK_SHORT);
    declickNOISE.setAttack(DECLICK_SHORT);
    declickNOISE.setRelease(DECLICK_SHORT);
    declickSINBUF.setAttack(DECLICK_SHORT);
    declickSINBUF.setRelease(DECLICK_SHORT);
    
    
    panSAW.setPan(64);
    panTRI.setPan(64);
    panSQU.setPan(64);
    panPULSE.setPan(64);
    panNOISE.setPan(64);
    panSINBUF.setPan(64);
    
    sawwave = new float[2];
    sawwave[0]=sawwave[1]=0.;
    triangle = new float[2];
    triangle[0]=triangle[1]=0.;
    square = new float[2];
    square[0]=square[1]=0.;
    pulse = new float[2];
    pulse[0]=pulse[1]=0.;
    noise = new float[2];
    noise[0]=noise[1]=0.;
    sinebuf = new float[2];
    sinebuf[0]=sinebuf[1]=0.;
    
    isPlayingSAW = false;
    isPlayingTRI = false;
    isPlayingSQU = false;
    isPlayingPULSE = false;
    isPlayingNOISE = false;
    isPlayingSINBUF = false;
    
    lfoModPitch=0.f;
    lfoModPitchSINBUFamount=lfoModPitchTRIamount=lfoModPitchSAWamount=lfoModPitchSQUamount=lfoModPitchPWMamount=lfoModPulsePWMamount=0.f; //
    
    this->freq_calc();
    
    
  }

void avOSC::setFreq (float _frequency)
{ smoothFreq.setNewValue( _frequency );}

void avOSC::setMidiNoteFreq(int _note_number)
{
    smoothFreq.setNewValue(mtofarray[ _note_number ]);
   
}

float avOSC::getFreq ()
{return (frequency);}

void avOSC::setPWMphase(double _PWMphase){smoothPWM.setNewValue(_PWMphase);}

void avOSC::setPlaySAW(bool _value){
    isPlayingSAW =_value;
    if(isPlayingSAW==true)  declickSAW.trigger_on();
    if(isPlayingSAW==false) declickSAW.trigger_off();
}
void avOSC::setPlayTRI(bool _value){
    isPlayingTRI=_value;
    if(isPlayingTRI==true)  declickTRI.trigger_on();
    if(isPlayingTRI==false) declickTRI.trigger_off();

}
void avOSC::setPlaySQU(bool _value){
    isPlayingSQU=_value;
    if(isPlayingSQU==true)  declickSQU.trigger_on();
    if(isPlayingSQU==false) declickSQU.trigger_off();

}
void avOSC::setPlayPULSE(bool _value){
    isPlayingPULSE=_value;
    if(isPlayingPULSE==true)  declickPULSE.trigger_on();
    if(isPlayingPULSE==false) declickPULSE.trigger_off();

}
void avOSC::setPlayNOISE(bool _value){
    isPlayingNOISE=_value;
    if(isPlayingNOISE==true)  declickNOISE.trigger_on();
    if(isPlayingNOISE==false) declickNOISE.trigger_off();

}
void avOSC::setPlaySINBUF(bool _value){
    isPlayingSINBUF=_value;
    if(isPlayingSINBUF==true)  declickSINBUF.trigger_on();
    if(isPlayingSINBUF==false) declickSINBUF.trigger_off();

}

void avOSC::setPanSAW(int _value){ panSAW.setPan( _value); }
void avOSC::setPanTRI(int _value){ panTRI.setPan( _value); }
void avOSC::setPanSQU(int _value){ panSQU.setPan( _value); }
void avOSC::setPanPULSE(int _value){ panPULSE.setPan( _value); }
void avOSC::setPanNOISE(int _value){ panNOISE.setPan( _value); }
void avOSC::setPanSINBUF(int _value){ panSINBUF.setPan( _value); }

void avOSC::setPanSAW(float _value){ panSAW.setPan( _value); }
void avOSC::setPanTRI(float _value){ panTRI.setPan( _value); }
void avOSC::setPanSQU(float _value){ panSQU.setPan( _value); }
void avOSC::setPanPULSE(float _value){ panPULSE.setPan( _value); }
void avOSC::setPanNOISE(float _value){ panNOISE.setPan( _value); }
void avOSC::setPanSINBUF(float _value){ panSINBUF.setPan( _value); }

void avOSC::setVolumeSAW(int _value){ panSAW.setVolume( _value); }
void avOSC::setVolumeTRI(int _value){ panTRI.setVolume( _value); }
void avOSC::setVolumeSQU(int _value){ panSQU.setVolume( _value); }
void avOSC::setVolumePULSE(int _value){ panPULSE.setVolume( _value); }
void avOSC::setVolumeNOISE(int _value){ panNOISE.setVolume( _value); }
void avOSC::setVolumeSINBUF(int _value){ panSINBUF.setVolume( _value); }

void avOSC::setVolumeSAW(float _value){ panSAW.setVolume( _value); }
void avOSC::setVolumeTRI(float _value){ panTRI.setVolume( _value); }
void avOSC::setVolumeSQU(float _value){ panSQU.setVolume( _value); }
void avOSC::setVolumePULSE(float _value){ panPULSE.setVolume( _value); }
void avOSC::setVolumeNOISE(float _value){ panNOISE.setVolume( _value); }
void avOSC::setVolumeSINBUF(float _value){ panSINBUF.setVolume( _value); }

void avOSC::setHarmSAW(int _harmonic){harmSAW= _harmonic;};
void avOSC::setHarmTRI(int _harmonic){harmTRI= _harmonic;};
void avOSC::setHarmSQU(int _harmonic){harmSQU= _harmonic;};
void avOSC::setHarmPULSE(int _harmonic){harmPULSE= _harmonic;};
void avOSC::setHarmSINBUF(int _harmonic){harmSINBUF= _harmonic;};


void avOSC::patchLfoModPitch(float _lfoAudioOut){lfoModPitch = _lfoAudioOut;}; //

void avOSC::setLfoModPitchSINBUF(float _amount){ lfoModPitchSINBUFamount = _amount* _amount;}
void avOSC::setLfoModPitchTRI(float _amount){    lfoModPitchTRIamount = _amount* _amount;}
void avOSC::setLfoModPitchSAW(float _amount){    lfoModPitchSAWamount = _amount* _amount;}
void avOSC::setLfoModPitchSQU(float _amount){    lfoModPitchSQUamount = _amount* _amount;}
void avOSC::setLfoModPitchPWM(float _amount){    lfoModPitchPWMamount = _amount* _amount;}
void avOSC::setLfoModPulsePWM(float _amount){    lfoModPulsePWMamount = _amount* _amount;}



void avOSC::freq_calc(){
  
    frequency=smoothFreq.getValue();
    
    freqSINBUF=frequency*harmSINBUF;
    freqSINBUF+= freqSINBUF*lfoModPitch*lfoModPitchSINBUFamount;
    if(freqSINBUF>= freq_max)freqSINBUF= freq_max;
    if(freqSINBUF<=freq_min)freqSINBUF=freq_min;
    
    freqSQU=frequency*harmSQU; freqSQU += freqSQU*lfoModPitch*lfoModPitchSQUamount;
    if(freqSQU>= freq_max)freqSQU= freq_max;if(freqSQU<=freq_min)freqSQU=freq_min;
    
    freqPULSE=frequency*harmPULSE; freqPULSE += freqPULSE*lfoModPitch*lfoModPitchPWMamount;
    if(freqPULSE>= freq_max)freqPULSE= freq_max;if(freqPULSE<=freq_min)freqPULSE=freq_min;
    
    freqSAW=frequency*harmSAW; freqSAW += freqSAW *lfoModPitch*lfoModPitchSAWamount;
    if(freqSAW>= freq_max)freqSAW= freq_max;if(freqSAW<=freq_min)freqSAW=freq_min;
    
    freqTRI=frequency*harmTRI; freqTRI += freqTRI*lfoModPitch*lfoModPitchTRIamount;
    if(freqTRI>= freq_max)freqTRI= freq_max;if(freqTRI<=freq_min)freqTRI=freq_min;

    
    PWMphase=smoothPWM.getValue();
    PWMphase += PWMphase*lfoModPitch*lfoModPulsePWMamount;
    
};

float* avOSC::getAudioOut(){
    
    this->freq_calc();

    declick_noise = declickNOISE.ar();
    declick_tri = declickTRI.ar();
    declick_pulse = declickPULSE.ar();
    declick_saw = declickSAW.ar();
    declick_squ = declickSQU.ar();
    declick_sinbuf = declickSINBUF.ar();
    
    
    if(declick_noise[0]>0. | declick_noise[1]>0. )
    {
        //White Noise
        //always the same unless you seed it.
        float r = rand()/(float)RAND_MAX;
        noise[0]=declick_noise[0]*r*2-1;
       noise[1]=declick_noise[1]*r*2-1;
    }
    else if (declick_noise[0]==0. | declick_noise[1]==0. )
    {
        noise[0]=noise[1]=0.;
    }
    
    if(declick_sinbuf[0]>0. | declick_sinbuf[1]>0. )
    {
        //This is a sinewave oscillator that uses linear interpolation on a 514 point buffer
        //specify the frequency of the oscillator in Hz / cps etc.
        float remainder;
        phaseSINBUF += 512./(SAMPLING_RATE/freqSINBUF);
        if ( phaseSINBUF >= 511 ) phaseSINBUF -=512;
        remainder = phaseSINBUF - floor(phaseSINBUF);
        sinebuf[0]=declick_sinbuf[0]*(float) ((1-remainder) * sineBuffer[1+ (long) phaseSINBUF] + remainder * sineBuffer[2+(long) phaseSINBUF]);
        sinebuf[1]=declick_sinbuf[1]*(float) ((1-remainder) * sineBuffer[1+ (long) phaseSINBUF] + remainder * sineBuffer[2+(long) phaseSINBUF]);
    }
    else if(declick_sinbuf[0]==0. | declick_sinbuf[1]==0.)
    {
        sinebuf[0]=sinebuf[1]=0.;
    }
        
    if(declick_squ[0]>0. | declick_squ[1]>0. )
    {
        //This is a square wave
        if (phaseSQU<0.5)
        {
            square[0]=declick_squ[0]*(-1.);
            square[1]=declick_squ[1]*(-1.);
        }
        if (phaseSQU>0.5)
        {
            square[0]=declick_squ[0];
            square[1]=declick_squ[1];
            }
        if ( phaseSQU >= 1.0 ) phaseSQU -= 1.0;
        phaseSQU += double(freqSQU/SAMPLING_RATE);
        //
    }
    else if (declick_squ[0]==0 | declick_squ[1]==0)
    {
        square[0]=square[1]=0.;
    }
    
   
    if(declick_pulse[0]>0. | declick_pulse[1]>0. )
    {
       
        
        //This is a pulse generator that creates a signal between -1 and 1.
        if (PWMphase<0.) PWMphase=0;
        if (PWMphase>1.) PWMphase=1;
        if ( phasePULSE >= 1.0 ) phasePULSE -= 1.0;
        phasePULSE += double(freqPULSE/SAMPLING_RATE);
        if (phasePULSE<PWMphase)
        {
            pulse[0]=declick_pulse[0]*(-1.);
            pulse[1]=declick_pulse[1]*(-1.);
        }
        if (phasePULSE>PWMphase) {
            pulse[0]=declick_pulse[0];
            pulse[1]=declick_pulse[1];
        }

    }
    else if (declick_pulse[0]==0 | declick_pulse[1]==0)
    {
        pulse[0]=pulse[1]=0.;
    }
    
    
    if(declick_saw[0]>0. | declick_saw[1]>0.)
    {
    //Sawtooth generator. This is like a phasor but goes between -1 and 1
    
        sawwave[0]=declick_saw[0]*phaseSAW;
        sawwave[1]=declick_saw[1]*phaseSAW;
        if ( phaseSAW >= 1.0 ) phaseSAW -= 2.0;
        phaseSAW += double(freqSAW/SAMPLING_RATE);
     
    }
    else if (declick_saw[0]==0. | declick_saw[1]==0. )
    {
        sawwave[0]=sawwave[1]=0.;
    }
    
    
    if(declick_tri[0]>0. | declick_tri[1]>0.)
    {
        //This is a triangle wave.
        if ( phaseTRI >= 1.0 ) phaseTRI -= 1.0;
        phaseTRI += (1./(SAMPLING_RATE/freqTRI));
        if (phaseTRI <= 0.5 ) {
            triangle[0]=declick_tri[0]*(phaseTRI - 0.25) * 4;
            triangle[1]=declick_tri[1]*(phaseTRI - 0.25) * 4;
        } else {
            triangle[0]=declick_tri[0]*((1.0-phaseTRI) - 0.25) * 4;
            triangle[1]=declick_tri[1]*((1.0-phaseTRI) - 0.25) * 4;
        }
    }
    else if (declick_tri[0]==0. | declick_tri[1]==0.)
    {
        triangle[0]=triangle[1]=0.;
    }
    
    
/* final sum */
    
    
      AudioOut=     clampOutput.ClampSignal(
                                   mixOut.mix6(
                                               pan_noise.modulate(panNOISE.expPan(),noise),
                                               pan_saw.modulate(panSAW.expPan(),sawwave),
                                               pan_squ.modulate(panSQU.expPan(),square),
                                               pan_tri.modulate(panTRI.expPan(),triangle),
                                               pan_sinbuf.modulate(panSINBUF.expPan(),sinebuf),
                                               pan_pulse.modulate(panPULSE.expPan(),pulse)
                                               )
                                   );
    return(AudioOut);
  
}

