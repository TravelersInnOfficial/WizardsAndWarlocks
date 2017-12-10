#ifndef Music_h
#define Music_h

#include "CommonSoundEvent.h"

class Music : public CommonSoundEvent {
    public:
    Music();
    ~Music();

    protected:
    FMOD::Studio::EventInstance* soundInstance;

};

#endif /* Music_h */