//
//  AVenv.cpp
//  ignasiAVsynth_0003
//
//  Written by ignasi on 03/01/13.
//
//

#include "AVenv.h"

avENV::avENV(){
    
    amplitude=releaselevel=0.;
    counter=0;
    trigger=false;
    attack= 0.1 * SAMPLING_RATE * MAX_ENV_ATTACK;
    decay= 0.2 * SAMPLING_RATE * MAX_ENV_DECAY;
    sustain=.5;
    release= .2 * SAMPLING_RATE * MAX_ENV_RELEASE;
    output=new float[2];
    output[0]=output[1]=0.;
 
};

avENV::~avENV(){
    delete output;
};

void avENV::setAttack(float _attack){
    attackSmoother.setNewValue(int(pow(_attack,2) * SAMPLING_RATE * MAX_ENV_ATTACK));
}
void avENV::setDecay(float _decay){decaySmoother.setNewValue(int(pow(_decay,2) * SAMPLING_RATE * MAX_ENV_DECAY));}
void avENV::setSustain(float _sustain){sustainSmoother.setNewValue( pow(_sustain,2) );}
void avENV::setRelease(float _release){releaseSmoother.setNewValue( int(pow(_release,2) * SAMPLING_RATE * MAX_ENV_RELEASE)) ;}

float* avENV::adsr(){
    
    // this is a complete naïve aproach : sure the logic is incomplete, but it seems to work ... :)
    
    attack=attackSmoother.getValue();
    decay=decaySmoother.getValue();
    sustain=sustainSmoother.getValue();
    release=releaseSmoother.getValue();
    
   if (trigger==false)
   {
       if (amplitude>0)
       {
           if(release>0)
           {
               amplitude-=float(releaselevel/release);
           }
           else
           {
               amplitude=0.; counter=0.;
           }
       }
       else
       {
           amplitude=0.;counter=0;
       }
   }
   
   if(trigger==true)
   {
    
       if ( counter<=attack)
       {
           if(attack>0)
           {
               amplitude+=1./attack;
           }
           else
           {
               amplitude=1.;
           }
       }
       
       if ( amplitude>=1.)
       {
           amplitude=1.;
       }
       
       if ( counter>attack && counter<=(attack+decay))
       {
           if (decay>0)
           {
               amplitude-=(1.-sustain)/decay;
           }
           else
           {
               amplitude=sustain;
           }
        }
       
       if ( counter>(attack+decay))
       {
           amplitude=sustain;
       }
           
    
    counter++;
      
    
    }
    
    amplitude=clampAmplitude.ClampSignal(amplitude);
    
    output[0]=output[1]=amplitude;
   
    return output;
    
};

void avENV::trigger_on(){
    trigger=true;
    counter=0;
    amplitude=0.;  
};


void avENV::trigger_off(){
    trigger=false;
    releaselevel=amplitude;
   
  
};
