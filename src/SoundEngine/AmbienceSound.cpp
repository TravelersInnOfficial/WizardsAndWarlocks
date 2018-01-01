#include "AmbienceSound.h"

/******************************************************
 *  Destructor
 ******************************************************/
AmbienceSound::~AmbienceSound() {}

/******************************************************
 *  Constructor 
 ******************************************************/
AmbienceSound::AmbienceSound(FMOD::Studio::EventInstance* instance) {
    soundInstance = instance;
}