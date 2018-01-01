#include "SpellSound.h"

/******************************************************
 *  Default constructor
 ******************************************************/
SpellSound::SpellSound() {}

/******************************************************
 *  Destructor
 ******************************************************/
SpellSound::~SpellSound() {}

/******************************************************
 *  Constructor 
 ******************************************************/
SpellSound::SpellSound(FMOD::Studio::EventInstance* instance) {
    soundInstance = instance;
}