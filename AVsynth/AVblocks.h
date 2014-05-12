//
//  AVblocks.h
//  ignasiAVblocks
//
//  Created by ignasi on 01/02/13.
//
//  Base class for audio processing objects or "blocks".
//
//

#ifndef ignasiAVblocks_AVblocks_h
#define ignasiAVblocks_AVblocks_h


#include "AVglobals.h"



class audioBlock {
    
protected:
	float* AudioIn;
	float* AudioOut;
    float* ModIn; // !!
    float* ModOut;
	
public:
    
    /* defaults to global number of channels, and 2 mod channels ... */
	
	audioBlock()
	{
        AudioIn  = new float [NUM_CHANNELS];
        AudioOut = new float [NUM_CHANNELS];
        ModIn = new float [2];
        ModOut = new float [2];
        
        
        for(int i=0; i<NUM_CHANNELS; i++)
	  	{
	  		AudioIn[i]=AudioOut[i]=0.;
	  	}
	}
	
    /* ... and can also be initialized with an arbitrary number of equal input and output channels */
	
	audioBlock (int channels)
	{
        AudioIn  = new float [channels];
        AudioOut = new float [channels];
        
        for(int i=0; i<channels; i++)
	  	{
	  		AudioIn[i]=AudioOut[i]=0.;
	  	}
	}
	
    /* ... or with different number of input and output channels */
	
	audioBlock (int input_channels, int output_channels)
	{
        AudioIn  = new float [input_channels];
        AudioOut = new float [output_channels];
	  	
        for(int i=0; i<input_channels; i++)
	  	{
            AudioIn[i]=0.;
	  	}
        
        for(int i=0; i<output_channels; i++)
	  	{
            AudioOut[i]=0.;
	  	}
        
	}
	
	~audioBlock()
	{
	 	delete AudioIn, AudioOut, ModIn, ModOut;
	}
	
    	
	virtual void setAudioIn (float* _input)
	{
		AudioIn = _input; /* care must be taken that _input has the same dimension (number of channels) as AudioIN */
	
	}
    
	
	virtual float* getAudioOut ()
	{
        return (AudioOut);  /* output defaults to 0. */
	}
    
    
    virtual float* getAudioOut (float* _audioIn)
	{
        return (_audioIn);  /* output defaults to bypass */
	}
	
};



#endif
