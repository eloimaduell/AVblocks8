//
//  AVsynthGlobals.h
//  ignasiAVsynth_0003
//
//  Written by ignasi on 02/01/13.
//
//


#ifndef ignasiAVsynth_0003_AVsynthGlobals_h
#define ignasiAVsynth_0003_AVsynthGlobals_h


#ifndef NUM_CHANNELS
#define NUM_CHANNELS 2
#endif

#ifndef SAMPLING_RATE
#define SAMPLING_RATE 44100
#endif

#ifndef HALF_SAMPLING_RATE
#define HALF_SAMPLING_RATE 22050
#endif

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 128 // very low latency !!
#endif

#ifndef NUMBER_OF_VOICES
#define NUMBER_OF_VOICES 6
#endif

#ifndef VOICE_ATTENUATOR
#define VOICE_ATTENUATOR 5.6
#endif

#ifndef MAX_ENV_ATTACK
#define MAX_ENV_ATTACK 5 //seconds
#endif

#ifndef MAX_ENV_DECAY
#define MAX_ENV_DECAY 5
#endif

#ifndef MAX_ENV_RELEASE
#define MAX_ENV_RELEASE 10
#endif

#ifndef MAX_REC_TIME
#define MAX_REC_TIME 2 // seconds
#endif

#ifndef DECLICK_SHORT
#define DECLICK_SHORT 0.010
#endif

#ifndef DECLICK_LONG
#define DECLICK_LONG  0.150
#endif

#ifndef DECLICK_EXTRA_LONG
#define DECLICK_EXTRA_LONG  0.500
#endif




#include "AVtools.h"
#include "AVparameters.h"
#include "AVblocks.h"
#include "AVwindows.h"
#include "revmodel.hpp"


#endif
