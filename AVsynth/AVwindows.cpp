//
//  AVwindows.cpp
//  ignasiAVsynth_0001
//
//  Written by ignasi on 30/12/12.
//
//

#include "AVwindows.h"

avWIN::avWIN(){
    amplitude=0.;
    attackphase=releasephase=false;

    trigger=false;
    attack=0.001;
    release=0.001;
    output=new float[2];
    output[0]=output[1]=0.;
};

avWIN::~avWIN(){
    delete output;
}

void avWIN::setAttack(float _attack){
    attack=1/(SAMPLING_RATE*((.5 + pow(32. * _attack, 2))/1000.0));
}

void avWIN::setRelease(float _release){
    release=1/(SAMPLING_RATE*((.5 + pow(32. * _release, 2))/1000.));
}

float* avWIN::ar() {
	
	if ( trigger==true && attackphase==false )
    {
        releasephase=false;
        attackphase=true;
    }
    
    if (amplitude>=1.0)
    {
        amplitude=1.0;
    }
    
    if (attackphase==true)
    {
        if(attack>0)
        {
        output[0]=output[1]=( amplitude += attack );
        }
        else
        {
        output[0]=output[1]=1.0;
		}
        
	}
    
    if (trigger==false && releasephase==false )
    {
        attackphase=false;
        releasephase=true;
    }
    
    if (releasephase==true && amplitude>0.)
    {
        output[0]=output[1]=( amplitude -= release );
    }
    
    if (amplitude<=0.)
    {
        amplitude=0.;
        output[0]=output[1]=0.;
    }
    
    return output;
    
};



void avWIN::trigger_on(){
    trigger=true;
};


void avWIN::trigger_off(){
    trigger=false;
};


