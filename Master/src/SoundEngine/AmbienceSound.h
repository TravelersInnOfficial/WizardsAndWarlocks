#ifndef AmbienceSound_h
#define AmbienceSound_h

#include "CommonSoundEvent.h"

class AmbienceSound : public CommonSoundEvent {
    public:
    AmbienceSound();
    ~AmbienceSound();
    
    protected:
    FMOD::Studio::EventInstance* soundInstance;
};

#endif /* AmbienceSound_h */