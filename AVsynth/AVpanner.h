//
//  AVpanner.h
//  ignasiAVsynth_0003
//
//  Written by ignasi on 02/01/13.
//
//

#include "AVglobals.h"

#ifndef ignasiAVsynth_0003_AVpanner_h
#define ignasiAVsynth_0003_AVpanner_h

/*
 
 avPANNER objects deliver a two channel linear or exponential panned signal to be used as modulator.
 
 */

class avPANNER {

public :
    
    avPANNER();
    ~avPANNER();
    
    
    void setPan( float _pan); // from -64 to 64, C=0
    void setVolume( float _volume); //from 0. to 127.
    
    float* linearPan ();
    float* expPan ();
    
private:
    
    float* pan;
    float index, volume;
    ParamSmooth smoothedIndex, smoothedVolume;
    

};

#endif
