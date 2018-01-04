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

    /******************************************************
     *  Constructor 
     ******************************************************/
    CharacterSound(FMOD::Studio::EventInstance*);
};

#endif /* CommonSoundSoundEvent_h */