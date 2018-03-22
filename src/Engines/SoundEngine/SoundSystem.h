#ifndef SoundSystem_h
#define SoundSystem_h

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "../Includes/vector3d.h"
#include <Constants.h>

#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_errors.h>

class SoundEvent;
/********************************************************************************************************
********************************************** Sound System *********************************************
*********************************************************************************************************/
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
     *  @brief Modifies the vca volume
     *  @param string name of the vca
     *  @param vol Volume required
     ******************************************************/
    void setVolumeVCA(std::string name, float vol);

    /******************************************************
     * @brief Get the volume of the vca
     * @param string name of the vca
     * @return volume of the required vca
     ******************************************************/
    float getVolumeVCA(std::string name);

    /******************************************************
     *  Modifies the position and orientation of the listener point
     ******************************************************/
    void Update(vector3df headPos, vector3df headRot);

    /******************************************************
     *  Modifies the position and rotation of the listening point
     ******************************************************/
    void setListenerPosRot(vector3df pos, vector3df rot);

    /******************************************************
     *  Return the position of the listener
     ******************************************************/
    vector3df getListenerPosition();
    
    /******************************************************
     *  Updates the audio engine
     *  \param paused tells if has to pause the engine or not
     ******************************************************/
    void Update();

    /******************************************************
     *  @brief Sets the property for the var
     *  @param FMOD_3D_ATTRIBUTES var to set the vec
     *  @param vec that has to be assigned
     ******************************************************/
    void setPos(FMOD_3D_ATTRIBUTES* var,vector3df vec);
    void setVel(FMOD_3D_ATTRIBUTES* var,vector3df vec);
    void setForward(FMOD_3D_ATTRIBUTES* var,vector3df vec);
    void setUp(FMOD_3D_ATTRIBUTES* var,vector3df vec);    

    /******************************************************
     *  Loads all the needed banks
     ******************************************************/
    void loadBanks();

    /******************************************************
     *  Load all the avc
     ******************************************************/
    void loadAvc();

    /******************************************************
     * @brief Loads an specific vca
     * @param string path of the vca to load
     ******************************************************/
    void loadAvc(std::string path);

    /******************************************************
     * @brief Loads an specific bank
     * @param string path of the bank to load
     * @param string name of the bank to save it in the map 
     ******************************************************/
    void loadBank(std::string path, std::string name);

    /******************************************************
     *  @brief Creates the a FMOD eventDescription
     *  @param const char* name path of the description
     *  @param FMOD_STUDIO_EVENTDESCRIPTION* event description pointer
     ******************************************************/
    FMOD_STUDIO_EVENTDESCRIPTION* createDescription(const char* name, FMOD_STUDIO_EVENTDESCRIPTION*);

    /******************************************************
     * @brief Creates a sound event
     * @param std::string path of the event
     ******************************************************/
    SoundEvent* createEvent(std::string);

    /******************************************************
     * @brief Checks if an event is playing and plays it
     * @param string eventPath path of the event to play
     * @param playerPos position where should play the event and/or of the listener
     * @param playerRot rotation where should play the event and/or of the listener
     ******************************************************/
    void checkAndPlayEvent(SoundEvent* event, vector3df playerPos);

    /******************************************************
     * @brief Plays a sound event
     * @param string eventPath path of the event to play
     * @param vector3df playerPos position where should play the event and/or of the listener
     * @param vector3df playerRot rotation where should play the event and/or of the listener
     ******************************************************/
    void playEvent(SoundEvent* event, vector3df playerPos);
    void playEvent(SoundEvent* event); //Used for events without position as the events from the menu
    /******************************************************
     * @brief Stops an event
     * @param eventPath path of the event to stop
     ******************************************************/
    void stopEvent(SoundEvent* event);

    /******************************************************
     * @brief Stops an event if it's being played
     * @param eventPath path of the event to stop
     ******************************************************/
    void checkAndStopEvent(SoundEvent* event);

    /******************************************************
     * @brief Erase an soundEvent from the map
     * @param event to erase
     ******************************************************/
    void eraseSoundEvent(SoundEvent* event);

private:
    const char * banksPath;
	FMOD_STUDIO_SYSTEM* system;
	FMOD_SYSTEM* lowLevelSystem;
	FMOD_STUDIO_BANK* masterBank;
	FMOD_STUDIO_BANK* stringsBank;
    FMOD_STUDIO_BUS* busMaster;
    FMOD_3D_ATTRIBUTES* listener;
    std::map<std::string, FMOD_STUDIO_BANK*> banks;
    std::map<std::string, FMOD_STUDIO_VCA*> vcas;
    std::map<std::string, FMOD_STUDIO_EVENTDESCRIPTION*> eventDescriptions;
    std::map<std::string, SoundEvent*> soundEvents;
};

/********************************************************************************************************
 ********************************************** Sound Event *********************************************
 ********************************************************************************************************/

class SoundEvent {
    //Friend method so it can access the protected functions
    friend SoundEvent* SoundSystem::createEvent(std::string);
public:
    /******************************************************
     * @brief Default constructor
     ******************************************************/
    SoundEvent();

    /******************************************************
     * Destructor, makes the class pure abstract (= 0)
     * Pure abstract function
     ******************************************************/
     virtual ~SoundEvent() = 0;
    
    /******************************************************
     * Starts to reproduce the event
     * Abstract function
     ******************************************************/
    virtual void start();
    
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
     *  @param vol event volume, 0 = silence, 1 = maximun volume
     ******************************************************/
    void setVolume(float vol);
    
    /******************************************************
     *  Modifies the event volume multiplying it with a gain factor
     *  @param gain factor, 0 = silence, 1 = keep volume
     ******************************************************/
    void setGain(float  gain);
    
    /******************************************************
     *  Modifies the 3D position of the sound event
     *  @param x, y, and z, new 3D position
     ******************************************************/
    void setPosition(vector3df pos);
    
    /*******************************************************
     * Checks if the sound is playing
     *  @return returns true if the sound is playing
     *******************************************************/
    bool isPlaying();

    /*******************************************************
     * Releases the event and destroys it after it has stopped
     *******************************************************/
    void release();

    /*******************************************************
     * Sets the value of the pararmeter called as the parameter name
     * @param string name of the parameter to modify
     * @param float value of the parameter to modify
     *******************************************************/
    void setParamValue(std::string name, float value);
    
    /*******************************************************
     * @brief Returns the event instance
     * @return FMOD_STUDIO_EVENTINSTANCE* 
     *******************************************************/
    FMOD_STUDIO_EVENTINSTANCE* getInstance();

protected:
    FMOD_STUDIO_EVENTDESCRIPTION* soundDescription;
    FMOD_STUDIO_EVENTINSTANCE* soundInstance;
    /*******************************************************
     * @brief Sets the event instance
     * @return FMOD_STUDIO_EVENTINSTANCE* 
     *******************************************************/
    void setInstance(FMOD_STUDIO_EVENTINSTANCE * instance);
    void setDescription(FMOD_STUDIO_EVENTDESCRIPTION* description);
};

#endif /* SoundSystem_h */
