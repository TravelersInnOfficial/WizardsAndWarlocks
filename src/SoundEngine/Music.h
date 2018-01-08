#ifndef Music_h
#define Music_h

#include "CommonSoundEvent.h"

class Music : public CommonSoundEvent {
    public:

    /******************************************************
     *  Default constructor 
     ******************************************************/
    Music();

    /******************************************************
     *  Destructor
     ******************************************************/
    ~Music();

    protected:
    FMOD::Studio::EventInstance* soundInstance;
};

#endif /* CommonSoundSoundEvent_h */