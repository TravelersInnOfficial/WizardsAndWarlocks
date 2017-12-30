#ifndef SoundSystem_h
#define SoundSystem_h

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <vector3d.h>
#include <math.h>

#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_errors.h>

using namespace std;

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
    void createSystem(std::string soundBanksPath);
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
     *  Modifies the position and orientation of the listener point
     ******************************************************/
    void setListenerPosRot(vector3df pos, vector3df rot);
    
    /******************************************************
     *  Updates the audio engine
     *  \param paused tells if has to pause the engine or not
     ******************************************************/
    void update();

    /******************************************************
     *  Sets the property for the listener
     *  \param vec that has to be assigned
     ******************************************************/
    void setListenerPos(vector3df vec);
    void setListenerVel(vector3df vec);
    void setListenerForward(vector3df vec);
    void setListenerUp(vector3df vec);    
   
    /******************************************************
     *  Loads all the needed banks
     ******************************************************/
    void loadBanks();

    /******************************************************
     * Loads an specific bank
     * @param string path of the bank to load
     * @param FMOD::Studio::Bank* bank where will be loaded the bank
     ******************************************************/
    void loadBank(std::string path, FMOD::Studio::Bank* bank);

private:
    const char * banksPath;
	FMOD::Studio::System* system;
	FMOD::System* lowLevelSystem;
	FMOD::Studio::Bank* masterBank;
	FMOD::Studio::Bank* stringsBank;
    FMOD::Studio::Bus* busMaster;
    FMOD_3D_ATTRIBUTES* listener;
    std::map<std::string, FMOD::Studio::Bank*> banks;
    std::map<std::string, FMOD::Studio::EventDescription*> eventDescriptions;
    std::map<std::string, SoundEvent*> soundEvents;
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
    void setPosition(vector3df pos);
    
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

    void setParamValue(std::string name, float value);

    
protected:
    FMOD::Studio::EventInstance* soundInstance;
    
};

#endif /* SoundSystem_h */
