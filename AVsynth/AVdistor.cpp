//
//  AVdistor.cpp
//  AVblocks_4
//
//  Created by ignasi on 09/11/13.
//
//

#include "AVdistor.h"


avDISTOR::avDISTOR(){

    declickDistor.setAttack(DECLICK_SHORT);
    declickDistor.setRelease(DECLICK_SHORT);
    declicked_Distor = new float[2];
    declicked_Distor[0]=declicked_Distor[1]=0.f;
    phase = new float[2];
    phase[0]=phase[1]=0.f;
    distortedOut= new float[2];
    distortedOut[0]=distortedOut[1]=0.f;
    smoothedGain = 0.f;
    this->setLevel(1);
    this->setGain(1.f);
    this->setPlay(true);
    this->setDistorMode(1);
    
    
    

};

avDISTOR::~avDISTOR(){};


void avDISTOR::setPlay(bool _value){
    isPlayingDistor = _value;
    if (isPlayingDistor==true){declickDistor.trigger_on();}    
    if (isPlayingDistor==false){declickDistor.trigger_off();}
};

void avDISTOR::setGain(float _gain){smoothGain.setNewValue(_gain);};
float avDISTOR::getGain(){return smoothGain.getValue();}

void avDISTOR::setLevel(int _level){LevelStage=abs(_level);};
int avDISTOR::getLevel(){return LevelStage;};

void avDISTOR::setDistorMode(int _distorMode){distorMode=_distorMode;};


float* avDISTOR::getAudioOut(float *audioIN){
    
    declicked_Distor=declickDistor.ar();
    smoothedGain=smoothGain.getValue();
    
  /* === linear interpolated lookup  arcsin table to optimize asin function calls :   */
  // ... instead of : 
  // phase[0]=asin(audioIN[0]);
  // phase[1]=asin(audioIN[1]);
  // ... we do that : 
    
    double idxremainder1;
    double readIdx1 = 256. * ( 1. + audioIN[0]); // rectify input to 0 ~ +2 and map to 512 table values for lookup
    if ( readIdx1>=511 ) readIdx1 -=512; // make it warp arround
    idxremainder1 = readIdx1 - floor(readIdx1); // see where it falls in between
  
    if(distorMode==3){
        phase[0]=(float)((1-idxremainder1) * arcsinXXBuffer[1+ (long) readIdx1] + idxremainder1 * arcsinXXBuffer[2+(long) readIdx1]); //compute the linear interpolation
    } else if(distorMode==2){
    phase[0]=(float)((1-idxremainder1) * arcsinXBuffer[1+ (long) readIdx1] + idxremainder1 * arcsinXBuffer[2+(long) readIdx1]); //compute the linear interpolation
    }
    else{
    phase[0]=(float)((1-idxremainder1) * arcsinBuffer[1+ (long) readIdx1] + idxremainder1 * arcsinBuffer[2+(long) readIdx1]); //compute the linear interpolation
    }
    
    double idxremainder2;
    double readIdx2 = 256. * ( 1. + audioIN[1]);
    if ( readIdx2>=511 ) readIdx2 -=512;
    idxremainder2 = readIdx2 - floor(readIdx2);
   
    if(distorMode==3){
        phase[1]=(float)((1-idxremainder2) * arcsinXXBuffer[1+ (long) readIdx2] + idxremainder2 * arcsinXXBuffer[2+(long) readIdx2]);
    }
    else if(distorMode==2){
    phase[1]=(float)((1-idxremainder2) * arcsinXBuffer[1+ (long) readIdx2] + idxremainder2 * arcsinXBuffer[2+(long) readIdx2]);
    }
    else{
    phase[1]=(float)((1-idxremainder2) * arcsinBuffer[1+ (long) readIdx2] + idxremainder2 * arcsinBuffer[2+(long) readIdx2]);
        
    }
    /*==== END interpolated lookup arcsin ==*/
    
    double idxremainder3, idxremainder4, readIdx3, readIdx4; // prepare variables for sin lookup inside distortion loop
    
    if(declicked_Distor[0]>0. | declicked_Distor[1]>0. )
    {
        for (int n=1; n<=LevelStage; n+=2)
        {
        
        // intstead of calling sin function ...
        //   distortedOut[0]+=float((2./(PInValue[n]))*sin(float(n)*phase[0]));
        //   distortedOut[1]+=float((2./(PInValue[n]))*sin(float(n)*phase[1]));
        // ... we interpolate :
            
            readIdx3 = fmod((float(n))*(256. + (256. *phase[0])),512);
            readIdx4 = fmod((float(n))*(256. + (256. *phase[1])),512);
           
            idxremainder3 = readIdx3 - floor(readIdx3);
            idxremainder4 = readIdx4 - floor(readIdx4);
            
            distortedOut[0]+=float((2./(PInValue[n]))* (float)((1-idxremainder3) * sineBuffer[1+ (long) readIdx3] + idxremainder3 * sineBuffer[2+(long) readIdx3]));
            distortedOut[1]+=float((2./(PInValue[n]))* (float)((1-idxremainder4) * sineBuffer[1+ (long) readIdx4] + idxremainder4 * sineBuffer[2+(long) readIdx4]));
            
        }
        
        AudioOut[0]=(declicked_Distor[0]*smoothedGain*distortedOut[0])  +  ((1.f-declicked_Distor[0])*audioIN[0]);
        AudioOut[1]=(declicked_Distor[1]*smoothedGain*distortedOut[1])  +  ((1.f-declicked_Distor[1])*audioIN[1]);
       
        AudioOut=DCremover.removeDC(AudioOut);
        
        distortedOut[0]=distortedOut[1]=0.f;
        
        return AudioOut;
   
    }
    else if (declicked_Distor[0]==0. | declicked_Distor[1]==0. )
    {
        // true bypass
        return audioIN;    }
    
    
};