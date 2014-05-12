//
//  RECtrack.h
//  ignasiAVblocks
//
//  Created by ignasi on 02/02/13.
//
//  recTRACK objects are stereo audio tracks.
//

#ifndef ignasiAVblocks_RECtrack_h
#define ignasiAVblocks_RECtrack_h

#include "AVglobals.h"

class recTRACK: public audioBlock{
    
/*
 
KBD
'1'   PLAY sw0 |>--------------------< >---------------------(...)->| normal
'2'        sw1 |<--------------------< >---------------------(...)-<| reverse
'3'        sw2 |><-------------------< >--------------------(...)-<>| ping-pong
               |                      V                             |
'u'   Undo3 \--|---------------------------------------<v>-------\  |
'u'    Undo2 \-|---------------------------------------<v>--------\ |
'u'     Undo1 \|-------------------------------------<v>-----------\|
'w'    REC ovwr|-----------------------------------< 1 >-----(...)--|
'r'        owdb|-----------------------------------< 2 >-----(...)--|
               |                        rec position V              |
               |                                                    |
               |<+++++++++++++|<++++++++++++++>|+++++++++++++(...)+>|
 'i'= in       0             in               out                   end
 'o'= out
 'p'= reset in/out points
 'l'= looping
 'k'= no looping
 'c' CLEAR
 
 */
 
protected:
    
    // sampling memory arrays
    float* chL;
    float* chR;
    float* chL_undo1;
    float* chR_undo1;
    float* chL_undo2;
    float* chR_undo2;
    float* chL_undo3;
    float* chR_undo3;
    
    // memory array indexes
    unsigned long playPosition0; // where outgoing samples are read from in straight mode.
    unsigned long playPosition1; // where outgoing samples are read from in reverse mode.
    unsigned long playPosition2; // where outgoing samples are read from in ping-pong mode.
    
    unsigned long recPosition; // where incoming samples are written to.
    unsigned long inPoint; // starting point for reading / looping
    unsigned long outPoint; // end point for reading / looping.
    unsigned long endPoint; // absolute end of circular buffer.
    
    unsigned long shortDistance;
    unsigned long longDistance;
    unsigned long longDistance2;
    
    int recMode; // 0 = no record, 1 = overwrite, 2 = overdub
    bool isFirstPass; // first run-time a record mode is activated, it forces recPosition and playPosition to 0... also when clear is executed.
    int oldRecMode;
    int pingPongRecSense;
    
    int playMode; // 0 = normal, 1 = reverse, 2 = ping-pong
    int pingPongPlaySense;
  
    int clearMode; // 0 = clear all, 1 = clear inside in/out, 2 = clear ouside in/out
    bool loopSwitch; // 0 = no loop between in/out points, 1 = loop between in/out points ... anyway, always loops between endPoint and 0
    
    float pitch; // -1. ~ 0 ~ +1   ... 
    
    // patching for rec modes
    avWIN declickRec1, declickRec2, declickOwr;
    float* declickRecPatch1;
    float* declickRecPatch2;
    float* declickRecOwr;
    
    float* signalRec1;
    float* signalRec2;
    float* patchToOvdb;
    float* patchToOwr;

    Modulator modRec1, modRec2;
    
    // patching for play modes
    avWIN declickPlay0, declickPlay1, declickPlay2;
    float* declickPlayPatch0;
    float* declickPlayPatch1;
    float* declickPlayPatch2;
    
    float* signalPlay0;
    bool isPlaying0;
    float* signalPlay1;
    bool isPlaying1;
    float* signalPlay2;
    bool isPlaying2;
    float* signalPlayGet0;
    float* signalPlayGet1;
    float* signalPlayGet2;

    Modulator modPlay0, modPlay1, modPlay2;
    
    // patching to declick loops
    avWIN declickLoop0, declickLoop1, declickLoop2;
    Modulator modLoop0, modLoop1, modLoop2;
    
    // mix
    SignalClamper clampMixL, clampMixR;
    
public :
    
    recTRACK();
    ~recTRACK();
    
    void drawTrack(int _positionX, int _positionY, int _width, int _height);
    
    void setRecMode( int _recMode);
    void setClearMode(int _clearMode);
    void setPlayMode ( int _playMode);
    void playSw0();
    void playSw1();
    void playSw2();
    void loopSw(bool _isLooping);
    
    void setInPoint();
    void setOutPoint();
    void adjustInPoint(long int _adjustInPoint);
    void adjustOutPoint(long int _adjustOutPoint);
    void resetInOutPoints();
    
    void setPlayPosition();
    void setRecPosition();
   
    void incrPlayPosition();
    void incrRecPosition();

    unsigned long getInPoint();
    unsigned long getOutPoint();
    unsigned long getPlayPosition();
    unsigned long getRecPosition();
    
    void setPitch(float _pitch);
    float getPitch();
    
 

    
    void setAudioIn(float* _input);
    
    float* getAudioOut();
    
    void clear();
    
    void prepareUndo();
    void undo();
    

};


#endif

