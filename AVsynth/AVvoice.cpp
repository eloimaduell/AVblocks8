//
//  AVsynth.cpp
//  ignasiAVsynth_0001
//
//  Written by ignasi on 24/12/12.
//
//

#include "AVvoice.h"


/* exit */

avVOICE::~avVOICE()
{}

/* init */
 
 avVOICE::avVOICE(){
 
    /* on-off */
    isPlaying = false;
    midi_note_playing=1;

   
    /* declick ramps */
    declicker1.setAttack(DECLICK_LONG);
    declicker1.setRelease(DECLICK_LONG);
    
    /* env1 inits */
    env1.setAttack(100);
    env1.setDecay(50);
    env1.setSustain(0.1);
    env1.setRelease(1000);
     
     /* env2 inits */
     env2.setAttack(100);
     env2.setDecay(50);
     env2.setSustain(0.1);
     env2.setRelease(1000);
    
    /* mute all osc waveforms before starting */
    osc1.setPlaySAW ( false);
    osc1.setPlayTRI ( false);
    osc1.setPlaySQU ( false);
    osc1.setPlayPULSE ( false);
    osc1.setPlayNOISE ( false);
    osc1.setPlaySINBUF ( false);
    
    lfo1.setPlayLFO(true);
             
}


void avVOICE::setPlay(bool _isPlaying){
    isPlaying = _isPlaying;
    if(isPlaying==true)  declicker1.trigger_on();
    if(isPlaying==false) declicker1.trigger_off();
}


void avVOICE::setParam(int _index, float _value)
{
    switch (_index)
    {
        case kPanSINBUF : osc1.setPanSINBUF (_value); break;
        case kVolSIN : osc1.setVolumeSINBUF (_value); break;
               
        case kPanSAW : osc1.setPanSAW (_value); break;
        case kVolSAW : osc1.setVolumeSAW (_value); break;
            
        case kPanTRI : osc1.setPanTRI (_value); break;
        case kVolTRI : osc1.setVolumeTRI (_value); break;
            
        case kPanSQU : osc1.setPanSQU (_value); break;
        case kVolSQU : osc1.setVolumeSQU (_value); break;
            
        case kPanNOISE : osc1.setPanNOISE (_value); break;
        case kVolNOISE : osc1.setVolumeNOISE (_value); break;
            
        case kPanPULSE : osc1.setPanPULSE (_value); break;
        case kVolPULSE : osc1.setVolumePULSE (_value); break;
        case kPWM : osc1.setPWMphase ( _value); break;

        case A1 : env1.setAttack(_value); break;
        case D1 : env1.setDecay(_value); break;
        case S1 : env1.setSustain(_value); break;
        case R1 : env1.setRelease(_value); break;
        
        case A2 : env2.setAttack(_value); break;
        case D2 : env2.setDecay(_value); break;
        case S2 : env2.setSustain(_value); break;
        case R2 : env2.setRelease(_value); break;
        
        case kEnv2ToFilter : filter1.setCutoffModDepth(_value);break;

        case kFilter1Frequency : filter1.setFrequency(_value); break;
        case kFilter1Resonance : filter1.setResonance(_value); break;
        
        case kLFOfreq : lfo1.setFreqLFO(_value);break;
             
        case klfoModPitchSINBUFamount :  osc1.setLfoModPitchSINBUF(_value);break;//54
        case klfoModPitchTRIamount : osc1.setLfoModPitchTRI(_value);break;//55
        case klfoModPitchSAWamount :  osc1.setLfoModPitchSAW(_value);break;//56
        case klfoModPitchSQUamount : osc1.setLfoModPitchSQU(_value);break;
        case klfoModPitchPWMamount : osc1.setLfoModPitchPWM(_value);break;
        case klfoModPulsePWMamount : osc1.setLfoModPulsePWM(_value);break;//59
        case klfoOffset: lfo1.setMidiNoteOffsetLFO ( _value);break;//60
            
            
    }
    
}

void avVOICE::setParam(int _index, int _value)
{
    switch (_index)
    {
        case kHarmSIN : osc1.setHarmSINBUF(_value);break;
        case kHarmSAW : osc1.setHarmSAW(_value);break;
        case kHarmTRI : osc1.setHarmTRI(_value);break;
        case kHarmSQU : osc1.setHarmSQU(_value);break;
        case kHarmPULSE : osc1.setHarmPULSE(_value);break;
        case kWaveformLfo : lfo1.setWaveformLFO(_value);break;
        case kLFOmode: lfo1.setLFOmode (_value);break;//52
            
    }
}

void avVOICE::setParam(int _index, bool _value)
{
    switch (_index)
    {
        case masterPower : this->setPlay(_value); break;
            
        case isPlayingSINBUF : osc1.setPlaySINBUF ( _value); break;
        case isPlayingSAW : osc1.setPlaySAW ( _value); break;
        case isPlayingTRI : osc1.setPlayTRI ( _value); break;
        case isPlayingSQU : osc1.setPlaySQU ( _value); break;
        case isPlayingPULSE : osc1.setPlayPULSE ( _value); break;
        case isPlayingNOISE : osc1.setPlayNOISE ( _value); break;
        case isFilter1Playing : filter1.setPlay (_value);break;
        case isLfoPlaying : lfo1.setPlayLFO (_value);break;
       
            
    }
    
}

void avVOICE::note_on(int _note_number, int _note_velocity)
{
    osc1.setMidiNoteFreq(_note_number);
    lfo1.setMidiNoteFreqLFO( _note_number);
    env1.trigger_on();
    env2.trigger_on();
    midi_note_playing= _note_number;
    //cout << "voice note-on : pitch : " << midi_note_playing << "\r" ;
}

int avVOICE::get_midi_note_playing(){return(midi_note_playing);}

void avVOICE::note_off(int _note_number){env1.trigger_off();env2.trigger_off();}

float* avVOICE::getAudioOut()
{
    
    if(isPlaying==false){
        AudioOut[0]=AudioOut[1]=0.;
    }
    else if(isPlaying==true){
        
/* The synth : ======= */
    
    filter1.patchCutoffModSignal(env2.adsr());
    
    osc1.patchLfoModPitch(lfo1.getMonoAudioOut());
        
    AudioOut = clampOut.ClampSignal(global_declicker.modulate(filter1.getAudioOut(env_to_osc.modulate( env1.adsr(),osc1.getAudioOut())),declicker1.ar()));
    
/* end synth ========== */
    
    }
   
    return (AudioOut);
}

