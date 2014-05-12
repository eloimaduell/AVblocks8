//
//  AVpolySynth.cpp
//  ignasiAVsynth_0003
//
//  Written by ignasi on 02/01/13.
//
//

#include "AVpolySynth.h"

avPOLYSYNTH::avPOLYSYNTH()
{
    VoicesMixer = new float[2];
    VoicesMixer[0]=VoicesMixer[1]=0.;
    VoicesOut = new float[2];
    VoicesOut[0]=VoicesOut[1]=0.;
  //  polySamplerAudioOut = new float[2];
  //  polySamplerAudioOut[0]=polySamplerAudioOut[1]=0.;
  //  SamplerMixer = new float[2];
  //  SamplerMixer[0]=SamplerMixer[1]=0.;
    
    assigned_voice=0;
 
    voices = new avVOICE[NUMBER_OF_VOICES];
   // mysampler = new avSAMPLERVOICE[NUMBER_OF_VOICES];
    
    distorOut = new float[2];
    distorOut[0]=distorOut[1]=0.;
    
        
        
}

avPOLYSYNTH::~avPOLYSYNTH(){
   
   // delete voices;
    delete polySamplerAudioOut;
   // delete mysampler;
    
}

void avPOLYSYNTH::setParam(int _index, int _value)
{
//parameters numbered higher than 200 are forwarded to effects
    if(_index>=200)
    {
        switch (_index)
        {
            case setLevelDistor : distor.setLevel(_value);  break;
            case setDistorMode :  distor.setDistorMode(_value); break;

        }
    }
    else // forward parameters to all voices
    {
        for(int voices_count=0; voices_count<NUMBER_OF_VOICES; voices_count++)
        {
            voices[voices_count].setParam(_index,_value);
            //   mysampler[voices_count].setParam(_index,_value);
            
        }
    }
}



void avPOLYSYNTH::setParam(int _index, float _value)
{
    if(_index>=200){
        switch (_index) {
            case kReverbRoomSize : reverb.setroomsize(_value);break;
            case kReverbDamp : reverb.setdamp(_value); break;
            case kReverbWet : {reverb.setwet(_value); reverb.setdry(1.-_value); } break;
            case kReverbwidth : reverb.setwidth(_value);  break;
            case setGainDistor :   distor.setGain(_value); break;
                    }
    }
    else
    {
        for(int voices_count=0; voices_count<NUMBER_OF_VOICES; voices_count++)
        {
            voices[voices_count].setParam(_index,_value);
            //   mysampler[voices_count].setParam(_index,_value);
        }
    }
}

void avPOLYSYNTH::setParam(int _index, bool _value)
{
    
        
    if(_index>=200){
        switch (_index) {
            case isPlayingREVERB :
                reverb.setPlayReverb(_value);
                break;
            case setPlayDistor : //210
                distor.setPlay(_value); break;
            }
    }
    else
    {
        for(int voices_count=0; voices_count<NUMBER_OF_VOICES; voices_count++)
        {
            voices[voices_count].setParam(_index,_value);
            //    mysampler[voices_count].setParam(_index,_value);
        }
    }
}

void avPOLYSYNTH::note_on(int _note_number, int _note_velocity)
{
    voices[assigned_voice].note_on(_note_number,_note_velocity);
  //  mysampler[assigned_voice].note_on(_note_number,_note_velocity);
    
    assigned_voice++;
    if(assigned_voice+1==NUMBER_OF_VOICES) assigned_voice=0;
}

void avPOLYSYNTH::note_off(int _note_number)
{
 
    for(int voices_count=0; voices_count<NUMBER_OF_VOICES; voices_count++)
    {
        if( _note_number == voices[voices_count].get_midi_note_playing() ) voices[voices_count].note_off(_note_number);
  //      if( _note_number == mysampler[voices_count].get_midi_note_playing()) mysampler[voices_count].note_off(_note_number);
    }
    
    
}

void avPOLYSYNTH::init(){
    
    
    /*Master power on */
    this->setParam(masterPower,true);//0
    
    /* OSC1 */
    
    this->setParam(isPlayingSAW, true );//2
    this->setParam(isPlayingTRI, true );//3
    this->setParam(isPlayingSQU, true );//4
    this->setParam(isPlayingPULSE, true );//5
    this->setParam(isPlayingNOISE, false );//6
    this->setParam(isPlayingSINBUF, true );//7
    this->setParam(kPWM, 0.5f );//8
    
    this->setParam(kPanSAW, 64.f );//10
    this->setParam(kPanTRI, 1.f );//11
    this->setParam(kPanSQU, -64.f );//12
    this->setParam(kPanPULSE, 1.f);//13
    this->setParam(kPanNOISE, 1.f );//14
    this->setParam(kPanSINBUF, 1.f );//15
    
    this->setParam(kVolSIN, 110.f );//16
    this->setParam(kVolSAW, 110.f );//17
    this->setParam(kVolTRI, 110.f );//18
    this->setParam(kVolSQU, 110.f );//19
    this->setParam(kVolPULSE, 110.f );//20
    this->setParam(kVolNOISE, 110.f );//21
    
    this->setParam(kHarmSIN, 1.f );//22
    this->setParam(kHarmSAW, 2.f );//23
    this->setParam(kHarmTRI, 3.f );//24
    this->setParam(kHarmSQU, 4.f );//25
    this->setParam(kHarmPULSE, 5.f );//26
    
    /* ENV 1 */
    this->setParam(A1, .0f );//30
    this->setParam(D1, .1f );//31
    this->setParam(S1, 1.f );//32
    this->setParam(R1, .5f );//33
    
    /* ENV 2 */
    this->setParam(A2, .1f );//34
    this->setParam(D2, .5f );//35
    this->setParam(S2, .5f );//36
    this->setParam(R2, .5f );//37
    this->setParam(kEnv2ToFilter, .5f );//38
    
    /*FILTER 1 */
    
    this->setParam(isFilter1Playing, true );//40
    this->setParam(kFilter1Frequency, 0.2f );//41
    this->setParam(kFilter1Resonance, 0.2f );//42
    
    /* LFO */
    this->setParam(isLfoPlaying,true);//50
    this->setParam(kWaveformLfo,1);//51
    this->setParam(kLFOmode,1);//52
    this->setParam(kLFOfreq,.5f);//53
    this->setParam(klfoModPitchSINBUFamount,.1f);//54
    this->setParam(klfoModPitchTRIamount,0.f);//55
    this->setParam(klfoModPitchSAWamount,0.f);//56
    this->setParam(klfoModPitchSQUamount,0.f);//57
    this->setParam(klfoModPitchPWMamount,0.f);//58
    this->setParam(klfoModPulsePWMamount,0.f);//59
    this->setParam(klfoOffset,0.f);//60

    
    /* SAMPLER */
    this->setParam(setPlaySAMPLER, false );//100
    this->setParam(LoopMode, 1.f );//101
    
    
    /* Reverb */
    
    this->setParam(kReverbRoomSize,.65f);//200
    this->setParam(kReverbDamp,.1f);//201
    this->setParam(kReverbWet,.3f);//202
    this->setParam(isPlayingREVERB, true);//203
    this->setParam(kReverbwidth,.99f);//204
    
    /* Distor */
    this->setParam(setPlayDistor, true);
    this->setParam(setLevelDistor,2); // absolute max level is 1024 (no more PInVals stored) ... computational sense dictates 100 is ok as max
    this->setParam(setGainDistor,.8f);
    this->setParam(setDistorMode, 0);
                   
}


float* avPOLYSYNTH::getAudioOut(){
    
    for(int voices_count=0; voices_count<NUMBER_OF_VOICES; voices_count++)
    {
        VoicesMixer=voices[voices_count].getAudioOut();
    //    SamplerMixer=mysampler[voices_count].getAudioOut();
        VoicesOut[0]+=VoicesMixer[0];
        VoicesOut[1]+=VoicesMixer[1];
    //    VoicesOut[0]+=SamplerMixer[0];
    //    VoicesOut[1]+=SamplerMixer[1];
    }
    
    VoicesOut[0]=VoicesOut[0] / VOICE_ATTENUATOR;// if sampler is activated : multiply by 2. because of doubled voices by the sampler
    VoicesOut[1]=VoicesOut[1] / VOICE_ATTENUATOR;

    
    distorOut = distor.getAudioOut(VoicesOut);
    
    reverb.processmix(&distorOut[0], &distorOut[1], &AudioOut[0], &AudioOut[1],1,1);

    
    return ( clampMains.ClampSignal(AudioOut) );
    
}