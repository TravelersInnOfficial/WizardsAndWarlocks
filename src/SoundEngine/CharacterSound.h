#ifndef CharacterSound_h
#define CharacterSound_h

#include "CommonSoundEvent.h"

class CharacterSound : public CommonSoundEvent {
    public:

    /******************************************************
     *  Default constructor
     ******************************************************/
    CharacterSound();    

    /******************************************************
     *  Destructor
     ******************************************************/
    ~CharacterSound();
    
    
    protected:
    FMOD_STUDIO_EVENTINSTANCE* soundInstance;
};

#endif /* CommonSoundSoundEvent_h */