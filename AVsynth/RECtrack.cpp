//  RECtrack.cpp
//  ignasiAVblocks
//  Created by ignasi on 02/02/13.

#include "RECtrack.h"

void recTRACK::drawTrack(int _positionX, int _positionY, int _width, int _height)
{    };

recTRACK::recTRACK()
{
    endPoint = MAX_REC_TIME * SAMPLING_RATE ;
    
    shortDistance = long( float(SAMPLING_RATE*( (.5 + pow(32. * DECLICK_SHORT, 2))/1000.)));
    longDistance =long( float(SAMPLING_RATE*( (.5 + pow(32. * DECLICK_LONG, 2))/1000.)));
    longDistance2=2*longDistance;
    
    cout << shortDistance;
    cout << "\r";
    cout << longDistance;
    cout << "\r";
    cout << longDistance2;
    cout << "\r";
    
    
    chL = new float [endPoint];
    chR = new float [endPoint];
    chL_undo1 = new float [endPoint];
    chR_undo1 = new float [endPoint];
    chL_undo2 = new float [endPoint];
    chR_undo2 = new float [endPoint];
    chL_undo3 = new float [endPoint];
    chR_undo3 = new float [endPoint];
    
    playPosition0=playPosition1=playPosition2=0; // where outgoing samples are  read
    recPosition=0; // where incoming samples are written
    inPoint=0; // starting point for reading / looping
    outPoint=endPoint-shortDistance; // end point for reading / looping
    
    recMode=0; // 0 = no record, 1 = overwrite, 2 = overdub
    oldRecMode=0;
    isFirstPass=true;
    
    declickRec1.setAttack(DECLICK_SHORT); declickRec1.setRelease(DECLICK_SHORT); // warning : it should be clamped : MAX_REC_TIME/2 >= x/SAMPLING_RATE >= 1
    declickRec2.setAttack(DECLICK_SHORT); declickRec2.setRelease(DECLICK_SHORT);
    declickOwr.setAttack(DECLICK_SHORT); declickOwr.setRelease(DECLICK_SHORT);
    
    signalRec1 = new float[2]; signalRec1[0]=signalRec1[1]=0.;
    signalRec2 = new float[2]; signalRec2[0]=signalRec2[1]=0.;
    patchToOvdb = new float[2]; patchToOvdb[0]=patchToOvdb[1]=0.;
    patchToOwr = new float[2]; patchToOwr[0]=patchToOwr[1]=0.;
    declickRecPatch1 = new float[2]; declickRecPatch1[0]=declickRecPatch1[1]=0.;
    declickRecPatch2 = new float[2]; declickRecPatch2[0]=declickRecPatch2[1]=0.;
    declickRecOwr = new float[2]; declickRecOwr[0]=declickRecOwr[1]=0.;
    
    declickPlay0.setAttack(DECLICK_LONG); declickPlay0.setRelease(DECLICK_LONG);
    declickPlay1.setAttack(DECLICK_LONG); declickPlay1.setRelease(DECLICK_LONG);
    declickPlay2.setAttack(DECLICK_LONG); declickPlay2.setRelease(DECLICK_LONG);
    signalPlay0 = new float[2]; signalPlay0[0]=signalPlay0[1]=0.;
    signalPlay1 = new float[2]; signalPlay1[0]=signalPlay1[1]=0.;
    signalPlay2 = new float[2]; signalPlay2[0]=signalPlay2[1]=0.;
    declickPlayPatch0 = new float[2]; declickPlayPatch0[0]=declickPlayPatch0[1]=0.;
    declickPlayPatch1 = new float[2]; declickPlayPatch1[0]=declickPlayPatch1[1]=0.;
    declickPlayPatch2 = new float[2]; declickPlayPatch2[0]=declickPlayPatch2[1]=0.;
    signalPlayGet0 = new float[2]; signalPlayGet0[0]=signalPlayGet0[1]=0.;
    signalPlayGet1 = new float[2]; signalPlayGet1[0]=signalPlayGet1[1]=0.;
    signalPlayGet2 = new float[2]; signalPlayGet2[0]=signalPlayGet2[1]=0.;
    
    this->playSw0();
    
    
    playMode=0; // 0 = normal, 1 = reverse, 2 = ping-pong
    isPlaying0=isPlaying1=isPlaying2=false;
    pingPongPlaySense=pingPongRecSense=1;
   
    clearMode=0; // 0 = clear all, 1 = clear inside in/out, 2 = clear outside in/out
    this->clear(); this->prepareUndo(); this->prepareUndo(); // set channels to zeroes, then copy to undo twice (both undo arrays to zeroes)
    
    loopSwitch=false; // 0 = no loop between in/out points, 1 = loop between in/out points ... anyway, always loops between endPoint and 0
    declickLoop0.setAttack(DECLICK_LONG);declickLoop0.setRelease(DECLICK_LONG);
    declickLoop1.setAttack(DECLICK_LONG);declickLoop1.setRelease(DECLICK_LONG);
    declickLoop2.setAttack(DECLICK_LONG);declickLoop2.setRelease(DECLICK_LONG);
    
    pitch=1; // -1. ~ 0 ~ +1  ...
    
   // cout << "track setup ok\r";

};

recTRACK::~recTRACK(){delete chL, chR,chL_undo1, chR_undo1 ,chL_undo2, chR_undo2, chL_undo3, chR_undo3, signalPlay0, signalPlay1,signalPlay2, signalPlayGet0, signalPlayGet1, signalPlayGet2,signalRec1,signalRec2;};


void recTRACK::prepareUndo()
{
    for(unsigned long i=0; i<=endPoint; i++)
    {
        chL_undo3[i]=chL_undo2[i];
        chL_undo2[i]=chL_undo1[i];
        chL_undo1[i]=chL[i];
        
        chR_undo3[i]=chR_undo2[i];
        chR_undo2[i]=chR_undo1[i];
        chR_undo1[i]=chR[i];
    }
};

void recTRACK::undo()
{

     for(unsigned long i=0; i<=endPoint; i++)
    {
        chL_undo3[i]=chL[i];
        chL[i]=chL_undo1[i];
        chL_undo1[i]=chL_undo2[i];
        chL_undo2[i]=chL_undo3[i];
        
        chR_undo3[i]=chR[i];
        chR[i]=chR_undo1[i];
        chR_undo1[i]=chR_undo2[i];
        chR_undo2[i]=chR_undo3[i];

    }
    
    
    /* looks nicer but is slower ??!! 
   
    std::swap(chL_undo3, chL);
    std::swap(chL,chL_undo1);
    std::swap(chL_undo1,chL_undo2);
    std::swap(chL_undo2,chL_undo3);
    
    std::swap(chR_undo3, chR);
    std::swap(chR,chR_undo1);
    std::swap(chR_undo1,chR_undo2);
    std::swap(chR_undo2,chR_undo3);
     */
    
   
};

void recTRACK::clear(){
    switch (clearMode){
        case 0: this->prepareUndo();
            for(unsigned long i=0; i<=endPoint; i++) chL[i]=chR[i]=0.;isFirstPass=true;break;
        case 1: this->prepareUndo();
                for(unsigned long i=inPoint; i<=outPoint; i++) chL[i]=chR[i]=0.;isFirstPass=true;break;
        case 2: this->prepareUndo();
                for(unsigned long i=0; i<inPoint; i++) chL[i]=chR[i]=0.; for(unsigned long i=outPoint+1; i<=endPoint; i++) chL[i]=chR[i]=0.;isFirstPass=true;break;}
};

void recTRACK::setRecMode(int _recMode)
{
    oldRecMode=recMode;
    recMode=_recMode;
    
    if(isFirstPass==true)
    {
        if (loopSwitch==false)
        {
            recPosition=0;
            playPosition0=playPosition1=playPosition2=0;
        }
        else
        {
            recPosition=inPoint;
            playPosition0=playPosition1=playPosition2=inPoint;
        }
    
        isFirstPass=false;
    };
    
    switch (oldRecMode)
    {
        case 0 :
        {
            switch(recMode)
            {
                case 0: ;break;
               
                case 1:
                    this->prepareUndo();
                    declickRec1.trigger_on();
                    declickOwr.trigger_off(); break;
                
                case 2:
                    this->prepareUndo();
                    declickRec2.trigger_on(); break;
            }
        };break;
            
        case 1 :
        {
            switch(recMode)
            {
                case 0:
                    declickRec1.trigger_off();
                    declickOwr.trigger_on() ;break;
                case 1: ;break;
                case 2:
                    this->prepareUndo();
                    declickRec1.trigger_off();
                    declickOwr.trigger_on();
                    declickRec2.trigger_on(); break;
            }
        };break;
            
        case 2 :
        {
            switch(recMode)
            {
                case 0: declickRec2.trigger_off() ;break;
                case 1:
                    this->prepareUndo();
                    declickRec1.trigger_on();
                    declickOwr.trigger_off();
                    declickRec2.trigger_off(); break;
                case 2: ;break;}
        };break;
            
    }
    
};
void recTRACK::setClearMode(int _clearMode){clearMode=_clearMode;};

void recTRACK::playSw0()
{
    if (isPlaying0==true)
    {
        declickPlay0.trigger_off();
        isPlaying0=false;
    }
    else
    {
        declickPlay0.trigger_on();
        isPlaying0=true;
    }
};

void recTRACK::playSw1()
{
    if (isPlaying1==true)
    {
        declickPlay1.trigger_off();
        isPlaying1=false;
    }
    else
    {
        declickPlay1.trigger_on();
        isPlaying1=true;
    }
};

void recTRACK::playSw2()
{
    if (isPlaying2==true)
    {
        declickPlay2.trigger_off();
        isPlaying2=false;
    }
    else
    {
        declickPlay2.trigger_on();
        isPlaying2=true;
    }
};

void recTRACK::loopSw(bool _isLooping)
{
    loopSwitch=_isLooping;
    
    //only to be sure that loops are triggered ...
    if(loopSwitch==true)
    {
        recPosition=playPosition0=playPosition1=playPosition2=inPoint;
        
        if(isPlaying0==true)declickLoop0.trigger_on();
        if(isPlaying1==true)declickLoop1.trigger_on();
        if(isPlaying2==true)declickLoop2.trigger_on();
    }
};

void recTRACK::setInPoint()
{
    inPoint=recPosition;
    
    if(inPoint>=outPoint) outPoint=endPoint;
    
    if(outPoint-inPoint<longDistance2)
    {
        inPoint=outPoint-longDistance2;
        if(inPoint==0) outPoint=longDistance2;
        
    }
    //only to be sure that loops are triggered ...
    if(loopSwitch==true)
    {
        if(isPlaying0==true)declickLoop0.trigger_on();
        if(isPlaying1==true)declickLoop1.trigger_on();
        if(isPlaying2==true)declickLoop2.trigger_on();
    }

};
void recTRACK::setOutPoint()
{
    outPoint=recPosition;
    
    if(outPoint==0)outPoint=longDistance2;
    if(outPoint>endPoint)outPoint=endPoint;
    if(outPoint<=inPoint) inPoint=0;
  
    if(outPoint-inPoint<longDistance2)
    {
        outPoint=inPoint+longDistance2;
        if(outPoint>endPoint)
        {
            outPoint=endPoint;
            inPoint=endPoint-longDistance2;
        }
    }
    //only to be sure that loops are triggered ...
    if(loopSwitch==true)
    {
        if(isPlaying0==true)declickLoop0.trigger_on();
        if(isPlaying1==true)declickLoop1.trigger_on();
        if(isPlaying2==true)declickLoop2.trigger_on();
    }

};

void recTRACK::adjustInPoint(long _adjustInPoint){};
void recTRACK::adjustOutPoint(long _adjustOutPoint){};

void recTRACK::resetInOutPoints()
{
    inPoint=0; outPoint=endPoint;
 
    //only to be sure that loops are triggered ...
    if(loopSwitch==true)
    {
        if(isPlaying0==true)declickLoop0.trigger_on();
        if(isPlaying1==true)declickLoop1.trigger_on();
        if(isPlaying2==true)declickLoop2.trigger_on();
    }
};

void recTRACK::setPlayPosition(){};

void recTRACK::incrPlayPosition()
{
    
    if(loopSwitch==false)
    {
        //normal play
            playPosition0+=1;
            if(playPosition0>endPoint)playPosition0=0;
    
        //reverse play
            if(playPosition1==0)
            {
                playPosition1=endPoint;
            }
            else
            {
                playPosition1-=1;
            };
    
        //ping-pong play
          if (pingPongPlaySense==1)
            {
                playPosition2+=1;
               
                if(playPosition2>endPoint)
                {
                    playPosition2=endPoint-1;
                    pingPongPlaySense=-1;
                };
            }
            else
            {
                if(playPosition2==0)
                {
                    playPosition2=1;
                    pingPongPlaySense=1;
                }
                else
                {
                    playPosition2-=1;
                };
            };
    };
    
    if(loopSwitch==true)
    {
        if(playPosition0<inPoint)playPosition0=inPoint;
        if(playPosition1<inPoint)playPosition1=inPoint;
        if(playPosition2<inPoint)playPosition2=inPoint;
        if(playPosition0>outPoint)playPosition0=outPoint;
        if(playPosition1>outPoint)playPosition1=outPoint;
        if(playPosition2>outPoint)playPosition2=outPoint;
        
        //normal play
        
        if(playPosition0==outPoint-longDistance) declickLoop0.trigger_off();
        if(playPosition0==inPoint) declickLoop0.trigger_on();
        
        playPosition0+=1;
        
        if(playPosition0>outPoint)playPosition0=inPoint;
        
       
        //reverse play
        
        if(playPosition1<=inPoint)
        {
            playPosition1=outPoint;  declickLoop1.trigger_on();
        }
        else
        {
            if(playPosition1==inPoint+longDistance) declickLoop1.trigger_off();
            playPosition1-=1;
        };
        
        //ping-pong play
        if (pingPongPlaySense==1)
        {
            if(playPosition2==outPoint-longDistance) declickLoop2.trigger_off();
            if(playPosition2==inPoint) declickLoop2.trigger_on();
            
            playPosition2+=1;
            
            if(playPosition2>=outPoint)
            {
                playPosition2=outPoint;
                pingPongPlaySense=-1;
                declickLoop2.trigger_on();
            };
        }
        else
        {
            if(playPosition2==inPoint+longDistance) declickLoop2.trigger_off();
            if(playPosition2==outPoint) declickLoop2.trigger_on();
            
            playPosition2-=1;
            
            if(playPosition2<=inPoint)
            {
                playPosition2=inPoint;
                pingPongPlaySense=1;
                declickLoop2.trigger_on();
            };
        };

    };
};

void recTRACK::setRecPosition(){};

void recTRACK::incrRecPosition()
{
    recPosition+=1;
    
    if (loopSwitch==false)if(recPosition>endPoint)recPosition=0;
    
    if (loopSwitch==true)
    {
        if(recPosition>outPoint)recPosition=inPoint;
        if(recPosition<inPoint)recPosition=inPoint;
    }
};

void recTRACK::setPitch(float _pitch){pitch=_pitch;};

unsigned long recTRACK::getInPoint(){return(inPoint);};
unsigned long recTRACK::getOutPoint(){return(outPoint);};
unsigned long recTRACK::getPlayPosition(){/*return(playPosition);*/};
unsigned long recTRACK::getRecPosition(){return(recPosition);};
float recTRACK::getPitch(){return(pitch);};

void recTRACK::setAudioIn(float* _input)
{
    AudioIn=_input;
    
    declickRecPatch1=declickRec1.ar();
    declickRecPatch2=declickRec2.ar();
    declickRecOwr=declickOwr.ar();
    
    if(declickRecPatch1[0]>0.)
    {
    // overwrite
        patchToOwr=modRec1.modulate(AudioIn,declickRecPatch1);
        signalRec1[0]=patchToOwr[0]+chL[recPosition]*declickRecOwr[0];
        signalRec1[1]=patchToOwr[1]+chL[recPosition]*declickRecOwr[1];
    } else { signalRec1[0]=signalRec1[1]=0.; }
    
    if(declickRecPatch2[0]>0.)
    {
    //overdub
        patchToOvdb=modRec2.modulate(AudioIn, declickRecPatch2);
        signalRec2[0]=chL[recPosition]+patchToOvdb[0];
        signalRec2[1]=chR[recPosition]+patchToOvdb[1];
    } else { signalRec2[0]=signalRec2[1]=0.; }
    
    if(declickRecPatch1[0]>0. || declickRecPatch2[0]>0.)
    {
     
    // mix
        chL[recPosition]=clampMixL.ClampSignal(signalRec1[0]+signalRec2[0]);
        chR[recPosition]=clampMixR.ClampSignal(signalRec1[1]+signalRec2[1]);
    }     
        this->incrRecPosition();
    
};

float* recTRACK::getAudioOut()
{
    declickPlayPatch0=declickPlay0.ar();
    declickPlayPatch1=declickPlay1.ar();
    declickPlayPatch2=declickPlay2.ar();
   
    signalPlayGet0[0]=chL[playPosition0];signalPlayGet0[1]=chR[playPosition0];
    signalPlayGet1[0]=chL[playPosition1];signalPlayGet1[1]=chR[playPosition1];
    signalPlayGet2[0]=chL[playPosition2];signalPlayGet2[1]=chR[playPosition2];
 
    // normal play
    if(declickPlayPatch0[0]>0.)
    {
        if(loopSwitch==false)
        {
            signalPlay0=modPlay0.modulate(declickPlayPatch0,signalPlayGet0);
        }
        else
        {
            signalPlay0=modLoop0.modulate(declickLoop0.ar(),modPlay0.modulate(declickPlayPatch0,signalPlayGet0));
        }
    }
    else{signalPlay0[0]=signalPlay0[1]=0.;};
    
    // reverse play
    if(declickPlayPatch1[1]>0.)
    {
        if(loopSwitch==false)
        {
            signalPlay1=modPlay1.modulate(declickPlayPatch1,signalPlayGet1);
        }
        else
        {
            signalPlay1=modLoop1.modulate(declickLoop1.ar(),modPlay1.modulate(declickPlayPatch1,signalPlayGet1));
        }
    }
    else{signalPlay1[0]=signalPlay1[1]=0.;};
    
    // ping-pong play
    if(declickPlayPatch2[0]>0.)
    {
        if(loopSwitch==false)
        {
            signalPlay2=modPlay2.modulate(declickPlayPatch2,signalPlayGet2);
        }
        else
        {
            signalPlay2=modLoop2.modulate(declickLoop2.ar(),modPlay2.modulate(declickPlayPatch2,signalPlayGet2));
        }
    }
    else{signalPlay2[0]=signalPlay2[1]=0.;};
    
    // mix
    AudioOut[0]=signalPlay0[0]+signalPlay1[0]+signalPlay2[0];
    AudioOut[1]=signalPlay0[1]+signalPlay1[1]+signalPlay2[1];
    
    this->incrPlayPosition();
    
    return (AudioOut);
    
};

