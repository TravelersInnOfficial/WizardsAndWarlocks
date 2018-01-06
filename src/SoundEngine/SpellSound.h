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
    FMOD::Studio::EventInstance* soundInstance;
};

#endif /* CommonSoundSoundEvent_h */