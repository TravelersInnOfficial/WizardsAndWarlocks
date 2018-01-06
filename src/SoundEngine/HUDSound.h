#ifndef HUDSound_h
#define HUDSound_h

#include "CommonSoundEvent.h"

class HUDSound : public CommonSoundEvent {
    public:

    /******************************************************
     *  Default constructor 
     ******************************************************/
    HUDSound();

    /******************************************************
     *  Destructor
     ******************************************************/
    ~HUDSound();

    protected:
    FMOD::Studio::EventInstance* soundInstance;
};

#endif /* CommonSoundSoundEvent_h */