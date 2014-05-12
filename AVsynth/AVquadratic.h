//
//  AVquadratic.h
//  AVblocks_5
//
//  Created by ignasi on 06/12/13.
//
//  Logistic map (quadratic iterator) as an audio source.
//

#include "AVglobals.h"

#ifndef AVblocks_5_AVquadratic_h
#define AVblocks_5_AVquadratic_h

class avQUADRATIC : public audioBlock   {
    
public :

    avQUADRATIC();
    virtual ~avQUADRATIC();
    
    // C range : 3.570000 to 3.999000, default 3.827529
    // control parameter for C must range from 0. to 1.
    
    void setC (float _value);
    void setLevel ( float _value);
    void setOffset (float _value); // offset control parameter must range from 0. to 1.
    void setPlay (bool _value);
    
    float* getAudioOut();
    
private :
    
    // iterator function variables
    float ax, ay, axnew, aynew, kax, kay, kcx, kcy;
    float cmax, cmin, offsetmin, offsetmax, offset;
    
    // declicking
    bool isPlayingQuad;
    ParamSmooth smoothC, smoothLevel, smoothOffset;
    avWIN declickQuad;
    float* declicker;
    
};

#endif
