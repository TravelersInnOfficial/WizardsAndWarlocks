#ifndef CommonSoundEvent_h
#define CommonSoundEvent_h
#include "SoundSystem.h"

class CommonSoundEvent : public SoundEvent {
    public:
    /******************************************************
     *  Default constructor 
     ******************************************************/
    CommonSoundEvent();

    /******************************************************
     *  Destructor
     ******************************************************/
    ~CommonSoundEvent();

    protected:
    FMOD_STUDIO_EVENTINSTANCE* soundInstance;
};

#endif /* CommonSoundSoundEvent_h */