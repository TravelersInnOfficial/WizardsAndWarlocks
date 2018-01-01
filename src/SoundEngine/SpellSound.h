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
   
    /******************************************************
     *  Constructor 
     ******************************************************/
    SpellSound(FMOD::Studio::EventInstance* instance);
};

#endif /* CommonSoundSoundEvent_h */