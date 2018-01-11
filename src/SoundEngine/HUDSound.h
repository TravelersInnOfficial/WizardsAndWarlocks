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
    FMOD_STUDIO_EVENTINSTANCE* soundInstance;
};

#endif /* CommonSoundSoundEvent_h */