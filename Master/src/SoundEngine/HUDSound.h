#ifndef HUDSound_h
#define HUDSound_h

#include "CommonSoundEvent.h"

class HUDSound : public CommonSoundEvent {
    public:
    HUDSound();
    ~HUDSound();
    
    protected:
    FMOD::Studio::EventInstance* soundInstance;

};

#endif /* HUDSound_h */