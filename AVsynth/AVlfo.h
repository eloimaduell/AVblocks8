//
//  AVlfo.h
//  ignasiAVblocks_003
//
//  Created by ignasi on 16/12/13. 
// Derived form Maximilian classes for oscillators.
//

#include "AVglobals.h"
#include "AVpanner.h"

#ifndef ignasiAVblocks_003_AVlfo_h
#define ignasiAVblocks_003_AVlfo_h


class avLFO : public audioBlock {


public:
	
    avLFO();
    ~avLFO();
    
    void    setFreqLFO (float _frequency);
    void    setMidiNoteFreqLFO (int _note_number);
    void    setMidiNoteOffsetLFO (float _offset);
    void    setLFOmode (int _mode);
    void    setHarmonicLFO (int _harmonic);
    void    setWaveformLFO (int _waveformLFO);
    double  getFreqLFO ();
    void    setPWMphaseLFO (float _PWMphase);
    void    setPlayLFO(bool _value);
   void    setVolumeLFO(int _value);
    void    setVolumeLFO(float _value);
    float  getMonoAudioOut ();
    
private:
    
    int LFOmode; // 1 = fixed frequency ; 2 = midi note ; 3 = sync to tempo
	double freqLFO, freeFreqLFO, midiFreqLFO, offsetLFO;
    ParamSmooth smoothFreqLFO, smoothMidiFreqLFO, smoothOffsetLFO, smoothPWMLFO;
    int harmonicLFO;
    double phaseLFO, PWMphaseLFO;
    int waveformLFO;
    bool isPlayingLFO;
    
    avWIN declickerLFO;
        
    /* patches */
    float* declick_lfo;
    
    void freq_calc();
   
    float waveformOUT;		
    
};


#endif
