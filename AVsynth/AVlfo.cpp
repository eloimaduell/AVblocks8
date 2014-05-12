//
//  AVlfo.cpp
//  ignasiAVblocks_003
//
//  Created by ignasi on 03/08/13.
//
//

#include "AVlfo.h"



avLFO::~avLFO(){
    
    delete declick_lfo;
}

avLFO::avLFO(){
    
    harmonicLFO=1;
    phaseLFO=PWMphaseLFO=0.;
    freqLFO =  midiFreqLFO = freeFreqLFO = 440.;
    smoothFreqLFO.setNewValue(440.);
    smoothMidiFreqLFO.setNewValue(mtofarray[62]);
    smoothOffsetLFO.setNewValue(0.);
    smoothPWMLFO.setNewValue(0.5);
    offsetLFO = 0.;
    waveformLFO=1;
    LFOmode=1;
    waveformOUT=0.;
    
    declick_lfo = new float[2];declick_lfo[0]=declick_lfo[1]=0.f;
    declickerLFO.setAttack(DECLICK_SHORT);
    declickerLFO.setRelease(DECLICK_SHORT);
    
    isPlayingLFO = false;
    
}

void avLFO::setFreqLFO(float _frequency){
    float frq = _frequency;
    frq = .001 + 100.*frq*frq; //parameter range 0 - 1 exponential mapped to 0.001 - 100.001 hz
    smoothFreqLFO.setNewValue( frq );}

// 1 = fixed frequency ; 2 = midi note ; 3 = sync to tempo
void avLFO::setLFOmode(int _mode){
    switch (_mode)
    {
        case 1:
            LFOmode=1;
            break;
        
        case 2:
            LFOmode=2;
            break;
        
        case 3:
            LFOmode=3;
            break;
        
        default:
            LFOmode=1;
            break;
    }
};

void avLFO::setMidiNoteFreqLFO(int _note_number){  smoothMidiFreqLFO.setNewValue(mtofarray[ _note_number ]);}
void  avLFO::setMidiNoteOffsetLFO (float _offset){ smoothOffsetLFO.setNewValue( _offset); } // param goes from -1. to 1. ...
double avLFO::getFreqLFO(){return (freqLFO);}
void avLFO::setHarmonicLFO(int _harmonic){ harmonicLFO = _harmonic;if(harmonicLFO<1 || harmonicLFO>32) harmonicLFO=1;}
void avLFO::setPWMphaseLFO(float _PWMphase){smoothPWMLFO.setNewValue(_PWMphase);}
void avLFO::setPlayLFO(bool _value){isPlayingLFO =_value;if(isPlayingLFO==true)  declickerLFO.trigger_on();if(isPlayingLFO==false) declickerLFO.trigger_off();}

void avLFO::setWaveformLFO(int _waveformLFO){
    
    // we have six different waveforms to choose from :
    //      1 = sin
    //      2 = triangle
    //      3 = saw
    //      4 = square
    //      5 = pulse
    //      6 = noise
    
    waveformLFO = _waveformLFO;
    if(waveformLFO<0 || waveformLFO>5)waveformLFO=1;
}

void avLFO::freq_calc()
{
    freeFreqLFO=smoothFreqLFO.getValue() * harmonicLFO;
   
    midiFreqLFO = smoothMidiFreqLFO.getValue() * harmonicLFO;
    
    offsetLFO=smoothOffsetLFO.getValue();
    //cout << offsetLFO << "\r";
   
    midiFreqLFO += midiFreqLFO * offsetLFO;
    
    switch(LFOmode)
    {
        case 1 : freqLFO = freeFreqLFO; break;
        case 2 : freqLFO = midiFreqLFO; break;
        //default : freqLFO = freeFreqLFO; break;
    }
    
    if(freqLFO>=HALF_SAMPLING_RATE)freqLFO=HALF_SAMPLING_RATE;
   
    if(freqLFO<=.001)freqLFO=.001; // extended low frequency range !!
    

    
}

float avLFO::getMonoAudioOut(){
    
    this->freq_calc();
    
    declick_lfo = declickerLFO.ar();

    if(declick_lfo[0]>0. )
    {
        
        switch (waveformLFO)
        {
          
            case 1: // sin
                //This is a sinewave oscillator that uses linear interpolation on a 514 point buffer
                //specify the frequency of the oscillator in Hz / cps etc.
                float remainder;
                phaseLFO += 512./(SAMPLING_RATE/freqLFO);
                if ( phaseLFO >= 511 ) phaseLFO -=512;
                remainder = phaseLFO - floor(phaseLFO);
                waveformOUT=declick_lfo[0]*(float) ((1-remainder) * sineBuffer[1+ (long) phaseLFO] + remainder * sineBuffer[2+(long) phaseLFO]);
                
                break;
          
            case 2: //
                //This is a triangle wave.
                if ( phaseLFO >= 1.0 ) phaseLFO -= 1.0;
                phaseLFO += (1./(SAMPLING_RATE/freqLFO));
                if (phaseLFO <= 0.5 ) {
                    waveformOUT=declick_lfo[0]*(phaseLFO - 0.25) * 4;
                 } else {
                    waveformOUT=declick_lfo[0]*((1.0-phaseLFO) - 0.25) * 4;
                }
                
                break;
                
                
            case 3: //saw
                //Sawtooth generator. This is like a phasor but goes between -1 and 1
                
                waveformOUT=declick_lfo[0]*phaseLFO;
                if ( phaseLFO >= 1.0 ) phaseLFO -= 2.0;
                phaseLFO += double(freqLFO/SAMPLING_RATE);
                
                break;
            
            case 4:
                //This is a square wave
                if (phaseLFO<0.5)
                {
                    waveformOUT=declick_lfo[0]*(-1.);
                }
                if (phaseLFO>0.5)
                {
                    waveformOUT=declick_lfo[0];
                }
                if ( phaseLFO >= 1.0 ) phaseLFO -= 1.0;
                phaseLFO += double(freqLFO/SAMPLING_RATE);
                
                break;
                
            case 5: // pulse
                
                PWMphaseLFO=smoothPWMLFO.getValue();
                
                //This is a pulse generator that creates a signal between -1 and 1.
                if (PWMphaseLFO<0.) PWMphaseLFO=0;
                if (PWMphaseLFO>1.) PWMphaseLFO=1;
                if ( phaseLFO >= 1.0 ) phaseLFO -= 1.0;
                phaseLFO += double(freqLFO/SAMPLING_RATE);
                if (phaseLFO<PWMphaseLFO)
                {
                    waveformOUT=declick_lfo[0]*(-1.);
                }
                if (phaseLFO>PWMphaseLFO) {
                    waveformOUT=declick_lfo[0];
                }
                
                break;
            
                           
            case 6: // noise
                //White Noise
                //always the same unless you seed it.
                float r = rand()/(float)RAND_MAX;
                waveformOUT=declick_lfo[0]*r*2-1;
                
                break;
            
            
        }
        
    }
    else if (declick_lfo[0]==0. )
    {
        waveformOUT=0.;
    }
        
    return(waveformOUT);
    
}

