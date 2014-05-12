
#include "ofMain.h"
#include "RtAudio.h"


#ifndef AVSoundStream_h
#define AVSoundStream_h

class AVSoundStream: public ofRtAudioSoundStream {

    public  :

        AVSoundStream();
        ~AVSoundStream();
    
    int getDeviceList();
    vector<RtAudio::DeviceInfo> infoList;

    string  getDeviceName(int i);
    int     getDeviceOutputChannels(int i);
    int     getDeviceInputChannels(int i);

    private :
    

};

#endif
