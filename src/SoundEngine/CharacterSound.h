#ifndef CharacterSound_h
#define CharacterSound_h

#include "CommonSoundEvent.h"

class CharacterSound : public CommonSoundEvent {
    public:
    CharacterSound();
    ~CharacterSound();

    protected:
    FMOD::Studio::EventInstance* soundInstance;
};

#endif /* CharacterSound_h */