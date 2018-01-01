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
    
    /******************************************************
     *  Constructor 
     ******************************************************/
    Music(FMOD::Studio::EventInstance* instance);
};

#endif /* CommonSoundSoundEvent_h */