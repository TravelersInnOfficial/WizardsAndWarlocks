#include "SoundSystem.h"

//Function that shows an error when something goes wrong
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
    if (result != FMOD_OK)
    {
        std::cerr << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

static SoundSystem* instance;
/********************************************************************************************************
********************************************** Sound System *********************************************
*********************************************************************************************************/

/******************************************************
 *  Constructor
 *  \param soundBanksPath directory path where are located the FMOD Studio generated banks
 ******************************************************/
SoundSystem* SoundSystem::getInstance() {
    if (instance == NULL) instance = new SoundSystem();
    return (instance);
}

SoundSystem::SoundSystem() {
    const char * banksPath          =               NULL;
	FMOD::Studio::System* system    =               NULL;
	FMOD::System* lowLevelSystem    =               NULL;
	FMOD::Studio::Bank* masterBank  =               NULL;
	FMOD::Studio::Bank* stringsBank =               NULL;
    FMOD::Studio::Bus* busMaster    =               NULL;
}

void SoundSystem::createSystem(string soundBanksPath){
   
    //Variables needed for the banks' filename
    banksPath = soundBanksPath.c_str();
    string path;
    const char * finalPath;
    //Initializing the fmod studio api
    //Create the object Studio System
    ERRCHECK(FMOD::Studio::System::create(&system));
    
    //Initializing the fmod low level api
    FMOD::System* lowLevelSystem = NULL;
    system->getLowLevelSystem(&lowLevelSystem);
    ERRCHECK(lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(lowLevelSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT));

    //Initialize the system
    system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);

    //Load a generated fmod bank
    FMOD::Studio::Bank* masterBank = NULL;
    FMOD::Studio::Bank* stringsBank = NULL;
    FMOD::Studio::Bank* characterBank = NULL;

    //The loadBankFile function needs a const char*, so we have to modify our string every time we load a new bank
    path = banksPath + string("Master Bank.bank");     //Make a string with the filename
    finalPath = path.c_str();                           //Convert to const char*
    ERRCHECK(system->loadBankFile(finalPath, FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));   //Load the bank

    path = banksPath + string("Master Bank.strings.bank"); //Make a string with the filename
    finalPath = path.c_str();                               //Convert to const char*
    ERRCHECK(system->loadBankFile(finalPath, FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));  //Load the bank
    
    path = banksPath + string("Character.bank");           //Make a string witht the filename
    finalPath = path.c_str();                               //Convert to const char*
    ERRCHECK(system->loadBankFile(finalPath, FMOD_STUDIO_LOAD_BANK_NORMAL, &characterBank));//Load the bank

    //Insert the banks at the banks map
    banks.insert(std::pair<string, FMOD::Studio::Bank*>("masterBank", masterBank));
    banks.insert(std::pair<string, FMOD::Studio::Bank*>("stringsBank", stringsBank));
    banks.insert(std::pair<string, FMOD::Studio::Bank*>("characterBank", characterBank));

    //Initialize other variables
}

/******************************************************
 * Factory method that constucts a SoundEvent by the name of the FMOD Studio event 
 *  f. ex. "event:/Ambience/Country"
 *  \param eventPath name of the event according FMOD Studio nomenclature
 ******************************************************/
SoundEvent* SoundSystem::getEvent(const char * eventPath) {

    std::cout << eventPath << std::endl;
    //Create event description
    FMOD::Studio::EventDescription * eventDesc;
    ERRCHECK(system->getEvent(eventPath, &eventDesc) );

    //Create event instance
    FMOD::Studio::EventInstance* eventInstance = NULL;
    eventDesc->createInstance(&eventInstance);
    
    //Create the sound event
    SoundEvent* newEvent = new SoundEvent();
    newEvent->newSoundEvent(eventInstance);
    
    //Insert the new elements on its respecting map
    eventDescriptions.insert(std::pair<string, FMOD::Studio::EventDescription *>(string(eventPath), eventDesc));
    soundEvents.insert(std::pair<string, SoundEvent *>(string(eventPath), newEvent));

    //Return the event
    return newEvent;
}

/******************************************************
 *  Modifies the general volume of the engine
 ******************************************************/
void SoundSystem::setVolume(float vol) {

    ERRCHECK(busMaster->setVolume(vol));
}

/******************************************************
 *  Modifies the position of the listening point
 ******************************************************/
void SoundSystem::setListenerPosition(Vector3 pos) {
    FMOD_3D_ATTRIBUTES* listener;
    FMOD_VECTOR fmodVec = {pos.x, pos.y, pos.z};
    listener->position = fmodVec;
    ERRCHECK(system->setListenerAttributes(0, listener));
}

/******************************************************
 *  Updates the audio engine
 *  \param paused tells if has to pause the engine or not
 ******************************************************/
void SoundSystem::update() {
    ERRCHECK(system->update());
}

/********************************************************************************************************
 ********************************************** Sound Event *********************************************
 ********************************************************************************************************/

SoundEvent::SoundEvent() {
    soundInstance = NULL;
}

SoundEvent* SoundEvent::newSoundEvent(FMOD::Studio::EventInstance* eventInstance) {
    soundInstance = eventInstance;
    soundInstance->setParameterValue("Surface", 3.0f);
    
    return this;
}

/******************************************************
 *  Starts to reproduce the event
 ******************************************************/
void SoundEvent::start() {
    ERRCHECK(soundInstance->start());   
}

/******************************************************
 *  Stops the event reproduction inmediately
 ******************************************************/
void SoundEvent::stop() {
    ERRCHECK(soundInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE));
}

/******************************************************
 *  Pauses the event reproduction
 ******************************************************/
void SoundEvent::pause() {
    bool paused = NULL;
    //Check if is paused or not
    if (soundInstance->getPaused(&paused))
        ERRCHECK(soundInstance->setPaused(false));
    else
        ERRCHECK(soundInstance->setPaused(true));
}


/******************************************************
 *  Modifies the sound event volume
 *  \param vol event volume, 0 = silence, 1 = maximun volume
 ******************************************************/
void SoundEvent::setVolume(float vol) {
    ERRCHECK(soundInstance->setVolume(vol));
}

/******************************************************
 *  Modifies the event volume multiplying it with a gain factor
 *  \param gain factor, 0 = silence, 1 = keep volume
 ******************************************************/
void SoundEvent::setGain(float gain) {
    ERRCHECK(soundInstance->setPitch(gain));

}
/******************************************************
 *  Modifies the 3D position of the sound event
 *  \param x, y, and z, new 3D position
 ******************************************************/
void SoundEvent::setPosition(Vector3 pos) {
    FMOD_3D_ATTRIBUTES* attributes;
    FMOD_VECTOR fmodVec = {pos.x, pos.y, pos.z};
    attributes->position = fmodVec;
    ERRCHECK(soundInstance->set3DAttributes(attributes));
}

/*******************************************************
 * Checks if the sound is playing
 *  \return returns true if the sound is playing
 *******************************************************/
bool SoundEvent::isPlaying() {
    bool res = false;
    FMOD_STUDIO_PLAYBACK_STATE state;

    ERRCHECK(soundInstance->getPlaybackState(&state));

    if (state == 0) {
        res = true;
    }

    return res;
}

/*******************************************************
 * Releases the event and destroys it after it has stop ped
 *******************************************************/
void SoundEvent::release() {
    ERRCHECK(soundInstance->release());
}



