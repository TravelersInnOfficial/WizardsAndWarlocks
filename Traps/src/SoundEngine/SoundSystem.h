#ifndef SoundSystem_h
#define SoundSystem_h

#include <map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_errors.h>

struct Vector3 {
	float x;
	float y;
	float z;
};

class SoundEvent;

class SoundSystem {
	
public:
	SoundSystem();    
	~SoundSystem();

    static SoundSystem* getInstance();

	/******************************************************
     *  Creates the system, loads the banks and insert them on SoundSystem maps
     *  \param soundBanksPath directory path where are located the FMOD Studio generated banks
     ******************************************************/
    void createSystem(string soundBanksPath);
    /******************************************************
     * Factory method that constucts a SoundEvent by the name of the FMOD Studio event 
     *  f. ex. "event:/Ambience/Country"
     *  \param eventPath name of the event according FMOD Studio nomenclature
     ******************************************************/
    SoundEvent* getEvent(const char * eventPath);
    
    /******************************************************
     *  Modifies the general volume of the engine
     ******************************************************/
    void setVolume(float vol);
    
    /******************************************************
     *  Modifies the position of the listening point
     ******************************************************/
    void setListenerPosition(Vector3 pos);
    
    /******************************************************
     *  Updates the audio engine
     *  \param paused tells if has to pause the engine or not
     ******************************************************/
    void update();
    
private:
    const char * banksPath;
	FMOD::Studio::System* system;
	FMOD::System* lowLevelSystem;
	FMOD::Studio::Bank* masterBank;
	FMOD::Studio::Bank* stringsBank;
    FMOD::Studio::Bus* busMaster;
    map<string, FMOD::Studio::Bank*> banks;
    map<string, FMOD::Studio::EventDescription*> eventDescriptions;
    map<string, SoundEvent*> soundEvents;  
};

class SoundEvent {
public:
    SoundEvent();
     ~SoundEvent();
    
    /******************************************************
     *  Starts to reproduce the event
     ******************************************************/
    void start();
    
    /******************************************************
     *  Stops the event reproduction inmediately
     ******************************************************/
    void stop();
    
    /******************************************************
     *  Pauses the event reproduction
     ******************************************************/
    void pause();
    
    /******************************************************
     *  Modifies the sound event volume
     *  \param vol event volume, 0 = silence, 1 = maximun volume
     ******************************************************/
    void setVolume(float vol);
    
    /******************************************************
     *  Modifies the event volume multiplying it with a gain factor
     *  \param gain factor, 0 = silence, 1 = keep volume
     ******************************************************/
    void setGain(float  gain);
    
    /******************************************************
     *  Modifies the 3D position of the sound event
     *  \param x, y, and z, new 3D position
     ******************************************************/
    void setPosition(Vector3 pos);
    
    /*******************************************************
     * Checks if the sound is playing
     *  \return returns true if the sound is playing
     *******************************************************/
    bool isPlaying();

    /*******************************************************
     * Releases the event and destroys it after it has stopped
     *******************************************************/
    void release();

    /******************************************************
     * This method creates a SoundEvent corresponding to the event received by argument
     ******************************************************/
    SoundEvent* newSoundEvent(FMOD::Studio::EventInstance* eventInstance);

    
protected:
    FMOD::Studio::EventInstance* soundInstance;
    
};

#endif /* SoundSystem_h */
