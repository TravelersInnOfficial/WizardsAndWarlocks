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
    FMOD::Studio::EventInstance* soundInstance;
};

#endif /* CommonSoundSoundEvent_h */