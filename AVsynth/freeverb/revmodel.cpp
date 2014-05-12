// Reverb model implementation
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#include "revmodel.hpp"

revmodel::revmodel()
{
	// Tie the components to their buffers
	combL[0].setbuffer(bufcombL1,combtuningL1);
	combR[0].setbuffer(bufcombR1,combtuningR1);
	combL[1].setbuffer(bufcombL2,combtuningL2);
	combR[1].setbuffer(bufcombR2,combtuningR2);
	combL[2].setbuffer(bufcombL3,combtuningL3);
	combR[2].setbuffer(bufcombR3,combtuningR3);
	combL[3].setbuffer(bufcombL4,combtuningL4);
	combR[3].setbuffer(bufcombR4,combtuningR4);
	combL[4].setbuffer(bufcombL5,combtuningL5);
	combR[4].setbuffer(bufcombR5,combtuningR5);
	combL[5].setbuffer(bufcombL6,combtuningL6);
	combR[5].setbuffer(bufcombR6,combtuningR6);
	combL[6].setbuffer(bufcombL7,combtuningL7);
	combR[6].setbuffer(bufcombR7,combtuningR7);
	combL[7].setbuffer(bufcombL8,combtuningL8);
	combR[7].setbuffer(bufcombR8,combtuningR8);
	allpassL[0].setbuffer(bufallpassL1,allpasstuningL1);
	allpassR[0].setbuffer(bufallpassR1,allpasstuningR1);
	allpassL[1].setbuffer(bufallpassL2,allpasstuningL2);
	allpassR[1].setbuffer(bufallpassR2,allpasstuningR2);
	allpassL[2].setbuffer(bufallpassL3,allpasstuningL3);
	allpassR[2].setbuffer(bufallpassR3,allpasstuningR3);
	allpassL[3].setbuffer(bufallpassL4,allpasstuningL4);
	allpassR[3].setbuffer(bufallpassR4,allpasstuningR4);

	// Set default values
	allpassL[0].setfeedback(0.5f);
	allpassR[0].setfeedback(0.5f);
	allpassL[1].setfeedback(0.5f);
	allpassR[1].setfeedback(0.5f);
	allpassL[2].setfeedback(0.5f);
	allpassR[2].setfeedback(0.5f);
	allpassL[3].setfeedback(0.5f);
	allpassR[3].setfeedback(0.5f);
	setwet(initialwet);
	setroomsize(initialroom);
	setdry(initialdry);
	setdamp(initialdamp);
	setwidth(initialwidth);
	setmode(initialmode);

	// Buffer will be full of rubbish - so we MUST mute them
	mute();
    
    
    declick_reverb = new float[2];
    declick_reverb[1]=declick_reverb[0]=0.;
    declickReverb.setAttack(DECLICK_SHORT);
    declickReverb.setRelease(DECLICK_SHORT);
    this->setPlayReverb(true);
}

void revmodel::mute()
{
	if (getmode() >= freezemode)
		return;

	for (int j=0;j<numcombs;j++)
	{
		combL[j].mute();
		combR[j].mute();
	}
	for (int k=0;k<numallpasses;k++)
	{
		allpassL[k].mute();
		allpassR[k].mute();
	}
}

void revmodel::setPlayReverb(bool _value){
    isPlayingReverb=_value;
    if(isPlayingReverb==true)  declickReverb.trigger_on();
    if(isPlayingReverb==false) declickReverb.trigger_off();
    
}

void revmodel::processreplace(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip)
{
	float outL,outR,input;
    
    //declicking and smoothing
    float smoothedWet1, smoothedWet2, smoothedDry;
    smoothedWet1 = smoothWet1.getValue();
    smoothedWet2 = smoothWet2.getValue();
    smoothedDry = smoothDry.getValue();
    declick_reverb=declickReverb.ar();

	while(numsamples-- > 0)
	{
		outL = outR = 0;
		input = (*inputL + *inputR) * gain;

		// Accumulate comb filters in parallel
		for(int i=0; i<numcombs; i++)
		{
			outL += combL[i].process(input);
			outR += combR[i].process(input);
		}

		// Feed through allpasses in series
		for(int j=0; j<numallpasses; j++)
		{
			outL = allpassL[j].process(outL);
			outR = allpassR[j].process(outR);
		}

        
        if(declick_reverb[0]>0. | declick_reverb[1]>0.)
        {
            // Calculate output REPLACING anything already there
            *outputL = (declick_reverb[0]*outL*smoothedWet1 + declick_reverb[0]*outL*smoothedWet2 + *inputL*smoothedDry)*.5f;
            *outputR = (declick_reverb[1]*outR*smoothedWet1 + declick_reverb[1]*outR*smoothedWet2 + *inputR*smoothedDry)*.5f;
            
        }
        else if (declick_reverb[0]==0. | declick_reverb[1]==0. )
        {
            *outputL = *inputL; // True bypass ;)
            *outputR = *inputR;
        }
		

		// Increment sample pointers, allowing for interleave (if any)
		inputL += skip;
		inputR += skip;
		outputL += skip;
		outputR += skip;
	}
}


void revmodel::processmix(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip)
{
	float outL,outR,input1, input2;
    float smoothedWet1, smoothedWet2, smoothedDry;
    smoothedWet1 = smoothWet1.getValue();
    smoothedWet2 = smoothWet2.getValue();
    smoothedDry = smoothDry.getValue();
    declick_reverb=declickReverb.ar();

	while(numsamples-- > 0)
	{
		outL = outR = 0;
		input1 = *inputL * gain;
        input2 = *inputR * gain;

		// Accumulate comb filters in parallel
		for(int i=0; i<numcombs; i++)
		{
			outL += combL[i].process(input1);
			outR += combR[i].process(input2);
		}

		// Feed through allpasses in series
		for(int j=0; j<numallpasses; j++)
		{
			outL = allpassL[j].process(outL);
			outR = allpassR[j].process(outR);
		}

		if(declick_reverb[0]>0. | declick_reverb[1]>0.)
        {
            // Calculate output REPLACING anything already there
            *outputL = (declick_reverb[0]*outL*smoothedWet1 + declick_reverb[0]*outL*smoothedWet2 + *inputL*smoothedDry)*.5f;
            *outputR = (declick_reverb[1]*outR*smoothedWet1 + declick_reverb[1]*outR*smoothedWet2 + *inputR*smoothedDry)*.5f;
            
        }
        else if (declick_reverb[0]==0. | declick_reverb[1]==0. )
        {
           *outputL = *inputL; // True bypass ;)
           *outputR = *inputR;
        }
		
        // Increment sample pointers, allowing for interleave (if any)
		inputL += skip;
		inputR += skip;
		outputL += skip;
		outputR += skip;
	}
}

void revmodel::update()
{
// Recalculate internal values after parameter change

	int i;

	smoothWet1.setNewValue(wet*(width/2 + 0.5f));
	smoothWet2.setNewValue(wet*((1-width)/2));

	if (mode >= freezemode)
	{
		roomsize1 = 1;
		damp1 = 0;
		gain = muted;
	}
	else
	{
		roomsize1 = roomsize;
		damp1 = damp;
		gain = fixedgain;
	}

	for(i=0; i<numcombs; i++)
	{
		combL[i].setfeedback(roomsize1);
		combR[i].setfeedback(roomsize1);
	}

	for(i=0; i<numcombs; i++)
	{
		combL[i].setdamp(damp1);
		combR[i].setdamp(damp1);
	}
}

// The following get/set functions are not inlined, because
// speed is never an issue when calling them, and also
// because as you develop the reverb model, you may
// wish to take dynamic action when they are called.

void revmodel::setroomsize(float value)
{
	roomsize = (value*scaleroom) + offsetroom;
	update();
}

float revmodel::getroomsize()
{
	return (roomsize-offsetroom)/scaleroom;
}

void revmodel::setdamp(float value)
{
	damp = value*scaledamp;
	update();
}

float revmodel::getdamp()
{
	return damp/scaledamp;
}

void revmodel::setwet(float value)
{
	wet = value*scalewet;
	update();
}

float revmodel::getwet()
{
	return wet/scalewet;
}

void revmodel::setdry(float value)
{
	smoothDry.setNewValue(value*scaledry);
}

float revmodel::getdry()
{
	return dry/scaledry;
}

void revmodel::setwidth(float value)
{
	width = value;
	update();
}

float revmodel::getwidth()
{
	return width;
}

void revmodel::setmode(float value)
{
	mode = value;
	update();
}

float revmodel::getmode()
{
	if (mode >= freezemode)
		return 1;
	else
		return 0;
}

//ends
