#ifndef FMODENGINE_H
#define FMODENGINE_H

#include <iostream>
#include <map>
#include <string>


//#include <mathvector.h>
#include <fmod.h>
#include <fmod_studio.h>
#include <fmod_studio.hpp>



using namespace std;

class SoundEvent;

class FmodEngine{
public:

    static FmodEngine* GetInstance(); 

    virtual ~FmodEngine();

private:

    FmodEngine();

    string banksPath;
	FMOD::Studio::System*	system = NULL;
	FMOD::System*			lowLevelSystem = NULL;
	FMOD::Studio::Bank* masterBank = NULL;
	FMOD::Studio::Bank* stringsBank = NULL;
    map<string, FMOD::Studio::Bank*> banks;
    map<string, FMOD::Studio::EventDescription*> eventDescriptions;
    map<string, SoundEvent*> soundEvents;

};

#endif
