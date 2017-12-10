#ifndef SpellSound_h
#define SpellSound_h

#include "CommonSoundEvent.h"

class SpellSound : public CommonSoundEvent {
    public:
    SpellSound();
    ~SpellSound();

    protected:
    FMOD::Studio::EventInstance* soundInstance;

};

#endif /* SpellSound_h */