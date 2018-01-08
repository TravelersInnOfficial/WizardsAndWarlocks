#ifndef AmbienceSound_h
#define AmbienceSound_h

#include "CommonSoundEvent.h"

class AmbienceSound : public CommonSoundEvent {
    public:

    /******************************************************
     *  Defautl Constructor 
     ******************************************************/
    AmbienceSound();

    /******************************************************
     *  Destructor
     ******************************************************/
    ~AmbienceSound();

    protected:
    FMOD::Studio::EventInstance* soundInstance;
};
#endif /* CommonSoundSoundEvent_h */