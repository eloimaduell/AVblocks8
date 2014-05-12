//
//  AVdistor.h
//  AVblocks_4
//
//  Created by ignasi on 09/11/13.
//
//  Direct form implementation of :
//  http://sgm-audio.com/research/dist/dist.html
//
//

#include "AVglobals.h"
#include "ofMath.h"


#ifndef AVblocks_4_AVdistor_h
#define AVblocks_4_AVdistor_h

class avDISTOR: public audioBlock {

    public  :

        avDISTOR();
        ~avDISTOR();
    
    void    setGain(float _gain);
    float   getGain();
    void    setLevel(int level);
    int     getLevel();
    void    setPlay(bool _value);
    void    setDistorMode ( int _distorMode);
    
    float* getAudioOut(float* audioIn);
    
    
    private :
    
        float* phase;
        int LevelStage;
        int distorMode;
    
        bool isPlayingDistor;
        avWIN declickDistor;
        float* declicked_Distor;
        ParamSmooth smoothGain;
        float smoothedGain;
        DCremoval DCremover;;
    
    
        float *distortedOut;

};

#endif
