#ifndef SpellSound_h
#define SpellSound_h

#include "CommonSoundEvent.h"

class SpellSound : public CommonSoundEvent {
    public:     

    /******************************************************
     *  Default constructor 
     ******************************************************/
    SpellSound();

    /******************************************************
     *  Destructor
     ******************************************************/
    ~SpellSound();

    protected:
    FMOD_STUDIO_EVENTINSTANCE* soundInstance;
};

#endif /* CommonSoundSoundEvent_h */