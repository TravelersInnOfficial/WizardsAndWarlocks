#ifndef CommonSoundEvent_h
#define CommonSoundEvent_h

#include "SoundSystem.h"


class CommonSoundEvent : public SoundEvent {
    public:
    CommonSoundEvent();
    ~CommonSoundEvent();
    void start() {SoundEvent::start();};

    protected:
    FMOD::Studio::EventInstance* soundInstance;

    SoundEvent* newSoundEvent(FMOD::Studio::EventInstance* eventInstance);
};

#endif /* CommonSoundSoundEvent_h */