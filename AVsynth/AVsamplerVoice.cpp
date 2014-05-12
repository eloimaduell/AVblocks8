//
//  AVsamplerVoice.cpp
//  ignasiAVsynth_0006_sampler
//
//  Created by ignasi on 13/01/13.
//
//

#include "AVsamplerVoice.h"

/* exit */

avSAMPLERVOICE::~avSAMPLERVOICE()
{
    delete sampleOut;
    
}

/* init */

avSAMPLERVOICE::avSAMPLERVOICE(){
    
    /* patching */
    
    sampleOut = new float[2];
    sampleOut[0]=sampleOut[1]=0.;
    
    sample1L.load("data/samples/piano/L/0.wav",1);
    sample1R.load("data/samples/piano/R/0.wav",1);
    
    /* on-off */
    isPlaying = false;
    midi_note_playing=1;
    
    /* default no looping */
    loopMode=false;
    
    /* declick ramps */
    declicker1.setAttack(DECLICK_SHORT);
    declicker1.setRelease(DECLICK_SHORT);
    
    /* env1 inits */
    env1.setAttack(100);
    env1.setDecay(50);
    env1.setSustain(0.1);
    env1.setRelease(1000);
    
    /* mute all osc waveforms before starting 
    osc1.setPlaySAW ( false);
    osc1.setPlayTRI ( false);
    osc1.setPlaySQU ( false);
    osc1.setPlayPULSE ( false);
    osc1.setPlayNOISE ( false);
    osc1.setPlaySINBUF ( false); */
    
    
}


void avSAMPLERVOICE::setPlay(bool _isPlaying){
    isPlaying = _isPlaying;
    if(isPlaying==true)  declicker1.trigger_on();
    if(isPlaying==false) declicker1.trigger_off();
}

void avSAMPLERVOICE::setLoopMode(bool _loopMode){
    loopMode= _loopMode;
}

void avSAMPLERVOICE::setParam(int _index, float _value)
{
    switch (_index)
    {
        /*case kPanSINBUF : osc1.setPanSINBUF (_value); break;
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
         */
            
        case A1 : env1.setAttack(_value); break;
        case D1 : env1.setDecay(_value); break;
        case S1 : env1.setSustain(_value); break;
        case R1 : env1.setRelease(_value); break;
            
        case kFilter1Frequency : filter1.setFrequency(_value); break;
        case kFilter1Resonance : filter1.setResonance(_value); break;
            
    }
    
}


void avSAMPLERVOICE::setParam(int _index, bool _value)
{
    switch (_index)
    {
        case setPlaySAMPLER : this->setPlay(_value); break;
        case LoopMode : this->setLoopMode( _value); break;;
        /*case isPlayingSAW : osc1.setPlaySAW ( _value); break;
        case isPlayingTRI : osc1.setPlayTRI ( _value); break;
        case isPlayingSQU : osc1.setPlaySQU ( _value); break;
        case isPlayingPULSE : osc1.setPlayPULSE ( _value); break;
        case isPlayingNOISE : osc1.setPlayNOISE ( _value); break;
             */
        case isFilter1Playing : filter1.setPlay (_value);break;
    }
    
}

void avSAMPLERVOICE::note_on(int _note_number, int _note_velocity)
{
    //osc1.setMidiNoteFreq(_note_number);
    sample1L.trigger();
    sample1R.trigger();
    env1.trigger_on();
    midi_note_playing= _note_number;
}

int avSAMPLERVOICE::get_midi_note_playing(){return(midi_note_playing);}

void avSAMPLERVOICE::note_off(int _note_number){
    env1.trigger_off();
}

float* avSAMPLERVOICE::getAudioOut()
{
    
    if(isPlaying==false){
        AudioOut[0]=AudioOut[1]=0.;
    }
    else if(isPlaying==true){
        
        /* The sampler : =============================================================================================================== */
        
        
        sampleOut[0]=sample1L.play(1.,0.,44100);
        sampleOut[1]=sample1R.play(1.,0.,44100);
        
            
            AudioOut = clampOut.ClampSignal(
                                             global_declicker.modulate(
                                                                       filter1.getAudioOut(
                                                                                           env_to_osc.modulate(env1.adsr(),sampleOut)
                                                                                           ),
                                                                       declicker1.ar()
                                                                       )
                                             );
        
        /* end ======================================================================================================================= */
        
    }
    
    return (AudioOut);
}

