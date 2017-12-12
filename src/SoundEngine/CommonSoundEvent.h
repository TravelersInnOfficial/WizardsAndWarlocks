#ifndef CommonSoundEvent_h
#define CommonSoundEvent_h

#include "SoundSystem.h"

class CommonSoundEvent : public SoundEvent {
    public:
    CommonSoundEvent();
    ~CommonSoundEvent();
    SoundEvent* newSoundEvent(FMOD::Studio::EventInstance*);

    protected:
    FMOD::Studio::EventInstance* soundInstance;
};

#endif /* CommonSoundSoundEvent_h */