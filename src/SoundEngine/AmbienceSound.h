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
    FMOD_STUDIO_EVENTINSTANCE* soundInstance;
};
#endif /* CommonSoundSoundEvent_h */