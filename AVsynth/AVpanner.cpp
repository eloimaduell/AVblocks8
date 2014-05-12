//
//  AVpanner.cpp
//  ignasiAVsynth_0003
//
//  Written by ignasi on 02/01/13.
//
//

#include "AVpanner.h"

avPANNER::avPANNER(){
    pan= new float[2];
    pan[0]=pan[1]=0;
    index=.5;
    volume=.9;
}

avPANNER::~avPANNER(){
    delete pan;
}

void avPANNER::setPan(float _pan){
    smoothedIndex.setNewValue((1.+_pan/64.)*.5);
}

void avPANNER::setVolume ( float _volume)
{
    smoothedVolume.setNewValue(pow(float ( _volume/127.),2));
    
};

float* avPANNER::linearPan (){
    
    volume=smoothedVolume.getValue();
    index=smoothedIndex.getValue();
    
    if(volume > 1.) volume=1.;
    if(volume<=0.) volume=0.;
    
    pan[0]=float(1-index)*volume;
    pan[1]=float(index)*volume;
    return (pan);

}

float* avPANNER::expPan(){
    
    volume=smoothedVolume.getValue();
    index=smoothedIndex.getValue();
    
    if(volume > 1.) volume=1.;
    if(volume<=0.) volume=0.;
    
    pan[0]=float(sqrt(1.-index))*volume;
    pan[1]=float(sqrt(index))*volume;
    return (pan);
}
