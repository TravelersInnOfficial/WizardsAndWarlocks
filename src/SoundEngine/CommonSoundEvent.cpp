#include "CommonSoundEvent.h"

/******************************************************
 *  Default constructor 
 ******************************************************/
CommonSoundEvent::CommonSoundEvent() {}

/******************************************************
 *  Destructor
******************************************************/
CommonSoundEvent::~CommonSoundEvent() {}

/******************************************************
 *  Constructor 
 ******************************************************/
CommonSoundEvent::CommonSoundEvent(FMOD::Studio::EventInstance* instance) {
    soundInstance = instance;
}
