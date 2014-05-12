//
//  AVquadratic.cpp
//  AVblocks_5
//
//  Created by ignasi on 06/12/13.
//
//

#include "AVquadratic.h"

avQUADRATIC::avQUADRATIC(){

    ax = ay = .1f;
    axnew = aynew = 0.f;
    kax = kay = 0.f;
    kcx =  kcy = 3.827529; // default
    offsetmin = .0f;
    offsetmax = .0001f;
    cmax = 3.999000f; 
    cmin = 3.570000f;
    this->setLevel(1.f);
    
    declickQuad.setAttack(DECLICK_LONG);
    declickQuad.setRelease(DECLICK_LONG);
    declicker=new float[2];
    declicker[0]=declicker[1]=0.;
    this -> setPlay (false);


};

avQUADRATIC::~avQUADRATIC(){};


// control parameters will range from 0. to 1.

void avQUADRATIC::setC (float _value){
    float value = cmin + ((cmax-cmin) *  _value);
    smoothC.setNewValue( value);
 
};

void avQUADRATIC::setOffset (float _value){
    float value = offsetmin + ((offsetmax-offsetmin) * _value);
    smoothOffset.setNewValue( value);
    
};

void avQUADRATIC::setLevel ( float _value){
    smoothLevel.setNewValue(_value);
};

void avQUADRATIC::setPlay (bool _value){
    isPlayingQuad = _value;
    if(isPlayingQuad==true)  declickQuad.trigger_on();
    if(isPlayingQuad==false) declickQuad.trigger_off();

};


float* avQUADRATIC::getAudioOut(){

    declicker=declickQuad.ar();
    
    if(declicker[0]>0. | declicker[1]>0. )
    {
    
        offset = smoothOffset.getValue();
        kcx = kcy = smoothC.getValue();
        kcy = kcx - offset;
        kcx += offset;
        
        axnew=kcx*ax*(1-ax);
        aynew=kcy*ay*(1-ay);

        AudioOut[0]= ax * smoothLevel.getValue() * declicker[0];
        AudioOut[1]= ay * smoothLevel.getValue() * declicker[1];

        ax=axnew;
        ay=aynew;
        
    }
    else
    {
        AudioOut[0]= 0.f;
        AudioOut[1]= 0.f;
    }
    
    return (AudioOut);
};