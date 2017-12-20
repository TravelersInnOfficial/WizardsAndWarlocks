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
}

void SoundSystem::createSystem(string soundBanksPath){
   
    //Variables needed for the banks' filename
    banksPath = soundBanksPath.c_str();

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

    //Load the needed banks
    loadBanks();

    //Initialize other variables
    listener = new FMOD_3D_ATTRIBUTES();
    setListenerPos(vector3df(0,0,0));
    setListenerVel(vector3df(0,0,0));
    setListenerForward(vector3df(0,1,0));
    setListenerUp(vector3df(0,1,0));
}

/******************************************************
 * Factory method that constucts a SoundEvent by the name of the FMOD Studio event 
 *  f. ex. "event:/Ambience/Country"
 *  \param eventPath name of the event according FMOD Studio nomenclature
 ******************************************************/
SoundEvent* SoundSystem::getEvent(const char * eventPath) {

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
    eventDescriptions[eventPath] = eventDesc;
    soundEvents[eventPath] = newEvent;

    //Return the event
    return newEvent;
}

void SoundSystem::loadBank(string filePath, FMOD::Studio::Bank* bank) {
    string path = "";
    const char * finalPath = "";

    path = banksPath + filePath;     //Make a string with the filename
    finalPath = path.c_str();        //Convert to const char*
    ERRCHECK(system->loadBankFile(finalPath, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));   //Load the bank
}

void SoundSystem::loadBanks() {

    //Initialize the banks
    FMOD::Studio::Bank* masterBank      = NULL;
    FMOD::Studio::Bank* stringsBank     = NULL;
    FMOD::Studio::Bank* characterBank   = NULL;
    FMOD::Studio::Bank* spellsBank      = NULL;
    FMOD::Studio::Bank* ambienceBank    = NULL;
    FMOD::Studio::Bank* HUDBank         = NULL;
    FMOD::Studio::Bank* MusicBank       = NULL;
    FMOD::Studio::Bank* commonSoundBank = NULL;

    //Load the banks
    loadBank("Master Bank.bank", masterBank);
    loadBank("Master Bank.strings.bank", stringsBank);
    loadBank("Character.bank", characterBank);
    loadBank("Spells.bank", spellsBank);
    loadBank("Ambience.bank", ambienceBank);
    loadBank("HUD.bank", HUDBank);
    loadBank("Music.bank", MusicBank);
    loadBank("CommonSound.bank", commonSoundBank);
    
    //Insert the banks at the banks map
    banks["masterBank"] = masterBank;
    banks["stringsBank"] = stringsBank;
    banks["characterBank"] = stringsBank;
    banks["spellsBank"] = spellsBank;
    banks["AmbienceBank"] = ambienceBank;
    banks["HUDBank"] = HUDBank;
    banks["MusicBank"] = MusicBank;
    banks["CommonSoundBank"] = commonSoundBank;
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
void SoundSystem::setListenerPosRot(vector3df pos, vector3df rot) {
    //Sets the position and orientation of the listener
    setListenerPos(pos);        //Position vector = player position vector
    //wsetListenerForward(rot);    //Orientation vector = player rotation vector    
    //setListenerUp(vector3df(-rot.Y, rot.X, rot.Z)); //Orientation normal vector
    
    ERRCHECK(system->setListenerAttributes(0, listener));
}

/******************************************************
 *  Updates the audio engine
 *  \param paused tells if has to pause the engine or not
 ******************************************************/
void SoundSystem::update() {
    ERRCHECK(system->update());
}

/******************************************************
 *  Sets the property for the listener
 *  \param vec that has to be assigned
 ******************************************************/
void SoundSystem::setListenerPos(vector3df vec) {   
        listener->position.x = vec.X;
        listener->position.y = vec.Y;
        listener->position.z = vec.Z;
}
void SoundSystem::setListenerVel(vector3df vec) {

        listener->velocity.x = vec.X;
        listener->velocity.y = vec.Y;
        listener->velocity.z = vec.Z;
}
void SoundSystem::setListenerForward(vector3df vec) {
        listener->forward.x = vec.X;
        listener->forward.y = vec.Y;
        listener->forward.z = vec.Z;

}
void SoundSystem::setListenerUp(vector3df vec) {
        listener->up.x = vec.X;
        listener->up.y = vec.Y;
        listener->up.z = vec.Z;
}



/********************************************************************************************************
 ********************************************** Sound Event *********************************************
 ********************************************************************************************************/

SoundEvent::SoundEvent() {
    soundInstance = NULL;
}

SoundEvent* SoundEvent::newSoundEvent(FMOD::Studio::EventInstance* eventInstance) {
    soundInstance = eventInstance;    
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
    bool paused = soundInstance->getPaused(&paused);
    ERRCHECK(soundInstance->setPaused(!paused));
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
void SoundEvent::setPosition(vector3df pos) {
    FMOD_3D_ATTRIBUTES* attributes = new FMOD_3D_ATTRIBUTES();
    FMOD_VECTOR fmodVec = {pos.X, pos.Y, pos.Z};
    attributes->position = fmodVec;
    fmodVec = {0,0,0};
    attributes->velocity = fmodVec;
    fmodVec = {0,1,0};
    attributes->forward = fmodVec;
    attributes->up = fmodVec;
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

void SoundEvent::setParamValue(string name, float value) {
    ERRCHECK(soundInstance->setParameterValue(name.c_str(), value));
}   

/*******************************************************
 * Releases the event and destroys it after it has stop ped
 *******************************************************/
void SoundEvent::release() {
    ERRCHECK(soundInstance->release());
}



