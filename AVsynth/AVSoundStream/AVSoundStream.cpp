
#include "AVSoundStream.h"

#include "ofConstants.h"

#ifdef OF_SOUNDSTREAM_RTAUDIO
#include "ofSoundStream.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "RtAudio.h"

AVSoundStream::AVSoundStream()
{

};

AVSoundStream::~AVSoundStream(){};


int AVSoundStream::getDeviceList()
{
    ofPtr<RtAudio> audioTemp;
	try {
		audioTemp = ofPtr<RtAudio>(new RtAudio());
	} catch (RtError &error) {
		error.printMessage();
		return;
	}
 	int devices = audioTemp->getDeviceCount();
    
	RtAudio::DeviceInfo info;
	for (int i=0; i< devices; i++) {
		try {
			info = audioTemp->getDeviceInfo(i);
		} catch (RtError &error) {
			error.printMessage();
			break;
		}
        infoList.push_back(info);
	}
    return devices;
}


string  AVSoundStream::getDeviceName(int i)
{
    return(infoList[i].name);
}
int     AVSoundStream::getDeviceOutputChannels(int i)
{
    
}
int     AVSoundStream::getDeviceInputChannels(int i)
{

}
#endif

