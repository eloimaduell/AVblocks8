//
//  AVsampler.h
//  ignasiAVsynth_0005
//
//  Adopted/Adapted by ignasi on 13/01/13
//  from : http://www.maximilian.strangeloop.co.uk
//
/*  maximilian.h
 *  platform independent synthesis library using portaudio or rtaudio
 *
 *  Created by Mick Grierson on 29/12/2009.
 *  Copyright 2009 Mick Grierson & Strangeloop Limited. All rights reserved.
 *	Thanks to the Goldsmiths Creative Computing Team.
 *	Special thanks to Arturo Castro for the PortAudio implementation.
 *
 *	Permission is hereby granted, free of charge, to any person
 *	obtaining a copy of this software and associated documentation
 *	files (the "Software"), to deal in the Software without
 *	restriction, including without limitation the rights to use,
 *	copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the
 *	Software is furnished to do so, subject to the following
 *	conditions:
 *
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *	OTHER DEALINGS IN THE SOFTWARE.
 *
 */

//
//

#ifndef ignasiAVsynth_0005_ofxMaxiSampler_h
#define ignasiAVsynth_0005_ofxMaxiSampler_h

#include "AVglobals.h"

//lagging with an exponential moving average
//a lower alpha value gives a slower lag
template <class T>
class maxiLagExp {
public:
	T alpha, alphaReciprocal;
	T val;
	
	maxiLagExp() {
		init(0.5, 0.0);
	};
	
	maxiLagExp(T initAlpha, T initVal) {
		init(initAlpha, initVal);
	}
	
	void init(T initAlpha, T initVal) {
		alpha = initAlpha;
		alphaReciprocal = 1.0 - alpha;
		val = initVal;
	}
	
	inline void addSample(T newVal) {
		val = (alpha * newVal) + (alphaReciprocal * val);
	}
	
	inline T value() {
		return val;
	}
};



class maxiSample  {
	
private:
	string 	myPath;
	int 	myChunkSize;
	int	mySubChunk1Size;
	int		readChannel;
	short 	myFormat;
	int   	myByteRate;
	short 	myBlockAlign;
	short 	myBitsPerSample;
	float speed;
	float output;
    maxiLagExp<float> loopRecordLag;
	
public:
	float position, recordPosition;
	int	myDataSize;
	short 	myChannels;
	int   	mySampleRate;
	long length;
	void getLength();
    void setLength(unsigned long numSamples);
    //	char* 	myData;
    short* temp;
	
	~maxiSample()
	{
        //		if (myData) free(myData);
        if (temp) free(temp);
	}
	
	maxiSample():temp(NULL),position(0), recordPosition(0), myChannels(1), mySampleRate(SAMPLING_RATE) {};
    
    maxiSample& operator=(const maxiSample &source)
    {
        if (this == &source)
            return *this;
        position=0;
        recordPosition = 0;
        myChannels = source.myChannels;
        mySampleRate = SAMPLING_RATE;
        free(temp);
        myDataSize = source.myDataSize;
        temp = (short*) malloc(myDataSize * sizeof(char));
        memcpy(temp, source.temp, myDataSize * sizeof(char));
        length = source.length;
        return *this;
    }
	
    void loopRecord(double newSample, const bool recordEnabled, const double recordMix) {
        loopRecordLag.addSample(recordEnabled);
        if(recordEnabled) {
            double currentSample = temp[(unsigned long)recordPosition] / 32767.0;
            newSample = (recordMix * currentSample) + ((1.0 - recordMix) * newSample);
            newSample *= loopRecordLag.value();
            temp[(unsigned long)recordPosition] = newSample * 32767;
        }
        ++recordPosition;
        if (recordPosition == length)
            recordPosition=0;
    }
    
    
	void trigger();
    void clear();
    void reset();
	
	float play(float frequency, float start, float end);
	
// read a wav file into this class
    bool load(string fileName, int channel=0);
	bool read();

    bool save() {return save(myPath);}
    
	bool save(string filename)
	{
		fstream myFile (filename.c_str(), ios::out | ios::binary);
		
		// write the wav file per the wav file format
		myFile.seekp (0, ios::beg);
		myFile.write ("RIFF", 4);
		myFile.write ((char*) &myChunkSize, 4);
		myFile.write ("WAVE", 4);
		myFile.write ("fmt ", 4);
		myFile.write ((char*) &mySubChunk1Size, 4);
		myFile.write ((char*) &myFormat, 2);
		myFile.write ((char*) &myChannels, 2);
		myFile.write ((char*) &mySampleRate, 4);
		myFile.write ((char*) &myByteRate, 4);
		myFile.write ((char*) &myBlockAlign, 2);
		myFile.write ((char*) &myBitsPerSample, 2);
		myFile.write ("data", 4);
		myFile.write ((char*) &myDataSize, 4);
		myFile.write ((char*) temp, myDataSize);
		
		return true;
	}
	
// return a printable summary of the wav file
	char *getSummary()
	{
		char *summary = new char[250];
		sprintf(summary, " Format: %d\n Channels: %d\n SampleRate: %d\n ByteRate: %d\n BlockAlign: %d\n BitsPerSample: %d\n DataSize: %d\n", myFormat, myChannels, mySampleRate, myByteRate, myBlockAlign, myBitsPerSample, myDataSize);
		return summary;
	}
};



#endif
