//
//  AVsampler.cpp
//  ignasiAVsynth_0005
//
//  Created by ignasi on 13/01/13.
//
//

#include "ofxMaxiSampler.h"

/*
 //This is a lookup table for converting midi to frequency
double mtofarray[129]={0, 8.661957, 9.177024, 9.722718, 10.3, 10.913383, 11.562325, 12.25, 12.978271, 13.75, 14.567617, 15.433853, 16.351599, 17.323914, 18.354048, 19.445436, 20.601723, 21.826765, 23.124651, 24.5, 25.956543, 27.5, 29.135235, 30.867706, 32.703197, 34.647827, 36.708096, 38.890873, 41.203445, 43.65353, 46.249302, 49., 51.913086, 55., 58.27047, 61.735413, 65.406395, 69.295654, 73.416191, 77.781746, 82.406891, 87.30706, 92.498604, 97.998856, 103.826172, 110., 116.540939, 123.470825, 130.81279, 138.591309, 146.832382, 155.563492, 164.813782, 174.61412, 184.997208, 195.997711, 207.652344, 220., 233.081879, 246.94165, 261.62558, 277.182617,293.664764, 311.126984, 329.627563, 349.228241, 369.994415, 391.995422, 415.304688, 440., 466.163757, 493.883301, 523.25116, 554.365234, 587.329529, 622.253967, 659.255127, 698.456482, 739.988831, 783.990845, 830.609375, 880., 932.327515, 987.766602, 1046.502319, 1108.730469, 1174.659058, 1244.507935, 1318.510254, 1396.912964, 1479.977661, 1567.981689, 1661.21875, 1760., 1864.655029, 1975.533203, 2093.004639, 2217.460938, 2349.318115, 2489.015869, 2637.020508, 2793.825928, 2959.955322, 3135.963379, 3322.4375, 3520., 3729.31, 3951.066406, 4186.009277, 4434.921875, 4698.63623, 4978.031738, 5274.041016, 5587.651855, 5919.910645, 6271.926758, 6644.875, 7040., 7458.620117, 7902.132812, 8372.018555, 8869.84375, 9397.272461, 9956.063477, 10548.082031, 11175.303711, 11839.821289, 12543.853516, 13289.75};
*/


bool maxiSample::load(string fileName, int channel) {
	myPath = fileName;
	readChannel=channel;
	return read();
}

bool maxiSample::read()
{
	bool result;
	ifstream inFile( myPath.c_str(), ios::in | ios::binary);
	result = inFile;
    
	if (inFile) {
		bool datafound = false;
		inFile.seekg(4, ios::beg);
		inFile.read( (char*) &myChunkSize, 4 ); // read the ChunkSize
		
		inFile.seekg(16, ios::beg);
		inFile.read( (char*) &mySubChunk1Size, 4 ); // read the SubChunk1Size
		
		//inFile.seekg(20, ios::beg);
		inFile.read( (char*) &myFormat, sizeof(short) ); // read the file format.  This should be 1 for PCM
		
		//inFile.seekg(22, ios::beg);
		inFile.read( (char*) &myChannels, sizeof(short) ); // read the # of channels (1 or 2)
		
		//inFile.seekg(24, ios::beg);
		inFile.read( (char*) &mySampleRate, sizeof(int) ); // read the samplerate
		
		//inFile.seekg(28, ios::beg);
		inFile.read( (char*) &myByteRate, sizeof(int) ); // read the byterate
		
		//inFile.seekg(32, ios::beg);
		inFile.read( (char*) &myBlockAlign, sizeof(short) ); // read the blockalign
		
		//inFile.seekg(34, ios::beg);
		inFile.read( (char*) &myBitsPerSample, sizeof(short) ); // read the bitspersample
		
		//ignore any extra chunks
		char chunkID[5]="";
		chunkID[4] = 0;
		int filePos = 36;
		while(!datafound && !inFile.eof()) {
			inFile.seekg(filePos, ios::beg);
			inFile.read((char*) &chunkID, sizeof(char) * 4);
			inFile.seekg(filePos + 4, ios::beg);
			inFile.read( (char*) &myDataSize, sizeof(int) ); // read the size of the data
			filePos += 8;
			if (strcmp(chunkID,"data") == 0) {
				datafound = true;
			}else{
				filePos += myDataSize;
			}
		}
		
		// read the data chunk
		char *myData = (char*) malloc(myDataSize * sizeof(char));
		inFile.seekg(filePos, ios::beg);
		inFile.read(myData, myDataSize);
		length=myDataSize*(0.5/myChannels);
		inFile.close(); // close the input file
		
        cout << "Ch: " << myChannels << ", len: " << length << endl;
		if (myChannels>1) {
			int position=0;
			int channel=readChannel*2;
			for (int i=channel;i<myDataSize+6;i+=(myChannels*2)) {
				myData[position]=myData[i];
				myData[position+1]=myData[i+1];
				position+=2;
			}
		}
        temp = (short*) malloc(myDataSize * sizeof(char));
        memcpy(temp, myData, myDataSize * sizeof(char));
        
        free(myData);
		
	}else {printf("ERROR: Could not load sample.");}

	return result; // this should probably be something more descriptive
}

void maxiSample::trigger() { position = 0;}

//better cubic inerpolation. Cobbled together from various (pd externals, yehar, other places).
float maxiSample::play(float frequency, float start, float end) {
	
    float remainder;
	float a,b,c,d,a1,a2,a3;
	
    if (frequency >0.)
    {
        if (position  < start) { position = start;}
		if ( position >= end ) { position = start;}
		
        position += ((end-start)/(SAMPLING_RATE/frequency));
		
        remainder = position - floor(position);
		
        if (position>0){ a=temp[(int)(floor(position))-1];} else {a=temp[0];}
		
		b=temp[(long) position];
		
        if (position<end-2) { c=temp[(long) position+1];} else { c=temp[0];}
		if (position<end-3) { d=temp[(long) position+2];} else { d=temp[0];}
		
        a1 = 0.5f * (c - a);
		a2 = a - 2.5 * b + 2.f * c - 0.5f * d;
		a3 = 0.5f * (d - a) + 1.5f * (b - c);
		
        output = (float) (((a3 * remainder + a2) * remainder + a1) * remainder + b) / 32767.;
		
	}
    else
    {
		frequency=frequency-(frequency+frequency);
		
        if ( position <= start ) position = end;
		
        position -= ((end-start)/(SAMPLING_RATE/frequency));
		
        remainder = position - floor(position);
		
        if (position>start && position < end-1) {a=temp[(long) position+1];} else {a=temp[0];}
		
		b=temp[(long) position];
		
        if (position>start) {c=temp[(long) position-1];} else {c=temp[0];}
		if (position>start+1) { d=temp[(long) position-2];} else {d=temp[0];}
        
		a1 = 0.5f * (c - a);
		a2 = a - 2.5 * b + 2.f * c - 0.5f * d;
		a3 = 0.5f * (d - a) + 1.5f * (b - c);
		
        output = (float) (((a3 * remainder + a2) * -remainder + a1) * -remainder + b) / 32767.;
		
	}
	
	return(output);
}



void maxiSample::getLength() {
	length=myDataSize*0.5;
}

void maxiSample::setLength(unsigned long numSamples) {
    temp = (short*) realloc(temp, sizeof(short) * numSamples);
    //    short *newData = (short*) malloc(sizeof(short) * numSamples);
    //    if (NULL!=temp) {
    //        unsigned long copyLength = min((unsigned long)length, numSamples);
    //        memcpy(newData, temp, sizeof(short) * copyLength);
    //        free(temp);
    //    }
    //    temp = newData;
    myDataSize = numSamples * 2;
    length=numSamples;
    position=0;
    recordPosition=0;
}

void maxiSample::clear() {
    memset(temp, 0, myDataSize);
}

void maxiSample::reset() {
    position=0;
}


