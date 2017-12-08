#include "CommonSoundEvent.h"


CommonSoundEvent::CommonSoundEvent() {
    soundInstance = NULL;
}

SoundEvent* CommonSoundEvent::newSoundEvent(FMOD::Studio::EventInstance* eventInstance) {
    soundInstance = eventInstance;

    return this;
}