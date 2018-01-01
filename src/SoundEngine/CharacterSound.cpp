#include "CharacterSound.h"

/******************************************************
 *  Default constructor
 ******************************************************/
CharacterSound::CharacterSound() {}

/******************************************************
 *  Destructor
 ******************************************************/
CharacterSound::~CharacterSound() {}

/******************************************************
 *  Constructor 
 ******************************************************/
CharacterSound::CharacterSound(FMOD::Studio::EventInstance* instance) {
    soundInstance = instance;
    if (soundInstance != NULL) std::cout << "CHAR: NO ES NULL" << std::endl; else std::cout << "CHAR: ES NULL" << std::endl;
}