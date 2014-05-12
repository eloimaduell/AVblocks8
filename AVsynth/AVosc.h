//
//  AVosc.h
//  ignasiAVsynth_0001
//
//  Adapted by ignasi on 27/12/12 from ofxMaximilian classes. http://www.maximilian.strangeloop.co.uk
//

#include "AVglobals.h"
#include "AVpanner.h"

#ifndef ignasiAVsynth_0001_AVosc_h
#define ignasiAVsynth_0001_AVosc_h

/*

 Derived form Maximilian classes for oscillators.
 
 -- Should be easy to break it into simpler, less cost, single wave generators --
 
*/


class avOSC: public audioBlock {
    
public:
	
    avOSC();
	~avOSC();
    
    void    setFreq (float _frequency);
    void    setMidiNoteFreq (int _note_number);
    float  getFreq ();
    
    void    setPWMphase (double _PWMphase);
    
    void setPlaySAW(bool _value);
    void setPlayTRI(bool _value);
    void setPlaySQU(bool _value);
    void setPlayPULSE(bool _value);
    void setPlayNOISE(bool _value);
    void setPlaySINBUF(bool _value);
    
    void setPanSAW(int _value);
    void setPanTRI(int _value);
    void setPanSQU(int _value);
    void setPanPULSE(int _value);
    void setPanNOISE(int _value);
    void setPanSINBUF(int _value);
    
    void setPanSAW(float _value);
    void setPanTRI(float _value);
    void setPanSQU(float _value);
    void setPanPULSE(float _value);
    void setPanNOISE(float _value);
    void setPanSINBUF(float _value);
    
    void setVolumeSAW(int _value);
    void setVolumeTRI(int _value);
    void setVolumeSQU(int _value);
    void setVolumePULSE(int _value);
    void setVolumeNOISE(int _value);
    void setVolumeSINBUF(int _value);
    
    void setVolumeSAW(float _value);
    void setVolumeTRI(float _value);
    void setVolumeSQU(float _value);
    void setVolumePULSE(float _value);
    void setVolumeNOISE(float _value);
    void setVolumeSINBUF(float _value);
    
    void setHarmSAW(int _harmonic); // 1,2,3 ...
    void setHarmTRI(int _harmonic);
    void setHarmSQU(int _harmonic);
    void setHarmPULSE(int _harmonic);
    void setHarmSINBUF(int _harmonic);
    
    void patchLfoModPitch(float _lfoAudioOut); //
   
    void setLfoModPitchSINBUF(float _amount);
    void setLfoModPitchTRI(float _amount);
    void setLfoModPitchSAW(float _amount);
    void setLfoModPitchSQU(float _amount);
    void setLfoModPitchPWM(float _amount);
    void setLfoModPulsePWM(float _amount);

    
    float* getAudioOut ();

private:
	
	float frequency;
    float freq_min, freq_max;
    ParamSmooth smoothFreq;
 	float freqSAW, freqTRI, freqSQU, freqPULSE, freqSINBUF;
 	float phaseSAW, phaseTRI, phaseSQU, phasePULSE, phaseNOISE, phaseSINBUF;
    int harmSAW, harmTRI, harmSQU, harmPULSE, harmSINBUF;
    float PWMphase;
    ParamSmooth smoothPWM;
    
    void freq_calc();
	
    bool isPlayingSAW;
    bool isPlayingTRI;
    bool isPlayingSQU;
    bool isPlayingPULSE;
    bool isPlayingNOISE;
    bool isPlayingSINBUF;
    
    avWIN declickSAW;
    avWIN declickTRI;
    avWIN declickSQU;
    avWIN declickPULSE;
    avWIN declickNOISE;
    avWIN declickSINBUF;
    
    avPANNER panSAW;
    avPANNER panTRI;
    avPANNER panSQU;
    avPANNER panPULSE;
    avPANNER panNOISE;
    avPANNER panSINBUF;
    
    /* patches */
    
    float* declick_saw;
    float* declick_tri;
    float* declick_squ;
    float* declick_pulse;
    float* declick_noise;
    float* declick_sinbuf;
    
    Modulator pan_saw;
    Modulator pan_tri;
    Modulator pan_squ;
    Modulator pan_pulse;
    Modulator pan_noise;
    Modulator pan_sinbuf;
    
	float* phasor;
	float* sawwave;
	float* triangle;
	float* square;
	float* pulse;
	float* noise;
	float* sinebuf;
    
    float lfoModPitch; //
    float lfoModPitchSINBUFamount,lfoModPitchTRIamount, lfoModPitchSAWamount, lfoModPitchSQUamount, lfoModPitchPWMamount, lfoModPulsePWMamount; //
    
    Mix6 mixOut;
    SignalClamper clampOutput;
	
	
    
};


#endif
