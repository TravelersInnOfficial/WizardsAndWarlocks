#include "IncludeEventChilds.h"
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)
static SoundSystem* instance;

//Function that shows an error when something goes wrong
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line){
	if (result != FMOD_OK) {
		std::cerr << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
}

/********************************************************************************************************
********************************************** Sound System *********************************************
*********************************************************************************************************/

SoundSystem* SoundSystem::getInstance() {
	if (instance == NULL) instance = new SoundSystem();
	return (instance);
}

/******************************************************
*  Constructor
******************************************************/
SoundSystem::SoundSystem() {}

/******************************************************
*  Destructor
******************************************************/
SoundSystem::~SoundSystem() {

	//Delete the sound event instances
	std::map<std::string, SoundEvent*>::iterator itSe = soundEvents.begin();
	for(; itSe!=soundEvents.end(); itSe++){
		SoundEvent* even = itSe->second;
		delete even;		//Delete the events
	}
	soundEvents.clear();	//Clear the sound events map

	//Deete the event descriptions
	std::map<std::string, FMOD::Studio::EventDescription*>::iterator iteven = eventDescriptions.begin();
	for(; iteven!=eventDescriptions.end(); iteven++){
		FMOD::Studio::EventDescription* even = iteven->second;
		ERRCHECK(even->releaseAllInstances());		//Delette all the  event instances of the event description
	}
	eventDescriptions.clear();		//Clear the event descriptions map

	//Delete the banks
	std::map<std::string, FMOD::Studio::Bank*>::iterator itBanks = banks.begin();
	for(; itBanks!=banks.end(); itBanks++){
		FMOD::Studio::Bank* ba = itBanks->second;
		if(ba!=NULL) ERRCHECK(ba->unload());  //Delete every map
	}
	banks.clear();		//Clear the banks map
	
	delete listener;	//Delete the sound listener

	ERRCHECK(system->release());	//Delete the sound system
}

/******************************************************
*  Constructor
*  @param soundBanksPath directory path where are located the FMOD Studio generated banks
******************************************************/
void SoundSystem::createSystem(std::string soundBanksPath){
   
	// Init Variables
	system = NULL;
	lowLevelSystem = NULL;
	masterBank = NULL;
	stringsBank = NULL;
    busMaster = NULL;

	//Variables needed for the banks' filename
	banksPath = soundBanksPath.c_str();

	//Initializing the fmod studio api
	//Create the object Studio System
	ERRCHECK(FMOD::Studio::System::create(&system));
	
	//Initializing the fmod low level api
	FMOD::System* lowLevelSystem = NULL;
	ERRCHECK(system->getLowLevelSystem(&lowLevelSystem));
	ERRCHECK(lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
	ERRCHECK(lowLevelSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT));

	//Initialize the system
	ERRCHECK(system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

	//Load the needed banks
	loadBanks();

	//Initialize other variables
	listener = new FMOD_3D_ATTRIBUTES();
	setPos(listener,vector3df(0.0f,0.0f,0.0f));
	setVel(listener,vector3df(0.0f,0.0f,0.0f));
	setForward(listener,vector3df(1.0f,0.0f,0.0f));
	setUp(listener,vector3df(0.0f,1.0f,0.0f));
}

/******************************************************
* Factory method that constucts a SoundEvent by the name of the FMOD Studio event 
*  f. ex. "event:/Ambience/Country"
*  \param eventPath name of the event according FMOD Studio nomenclature
******************************************************/
SoundEvent* SoundSystem::getEvent(const char * eventPath) {
	return soundEvents.find(eventPath)->second;
}

/******************************************************
* @brief Loads an specific bank
* @param string path of the bank to load
* @param FMOD::Studio::Bank* bank where will be loaded the 
******************************************************/
void SoundSystem::loadBank(std::string filePath, FMOD::Studio::Bank* bank) {
	std::string path = "";
	const char * finalPath = "";

	path = banksPath + filePath;     //Make a string with the filename
	finalPath = path.c_str();        //Convert to const char*
	
	ERRCHECK(system->loadBankFile(finalPath, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));   //Load the bank
}

/******************************************************
*  Loads all the needed banks
******************************************************/
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
	banks["characterBank"] = characterBank;
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
*  Modifies the position and rotation of the listening point
******************************************************/
void SoundSystem::setListenerPosRot(vector3df pos, vector3df rot) {

	rot = rot * M_PI/180;		//Transform to degrees
	rot.X = 0; rot.Z = 0;
	setPos(listener, pos);		//Set the listener position

	//Calculate the forward vector
	vector3df frwd = vector3df (sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X));
	setForward(listener, frwd);	// Ponemos el FORWARD

	//Calculate the up vector from the forward vector
	//Right vector x forward vector --> (forward x absolute up) x forward --> (forward x [0,1,0]) x forward
	vector3df A = frwd; vector3df B = vector3df(0, 1, 0);
	vector3df aux = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
	A = aux; B = frwd;
	vector3df up = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
	setUp(listener, up);
	
	//Finally, set the attributes
	ERRCHECK(system->setListenerAttributes(0, listener));
}

/******************************************************
*  Return the position of the listener
******************************************************/
vector3df SoundSystem::getListenerPosition() {
	vector3df toRet;
	toRet = vector3df(listener->position.x, listener->position.y, listener->position.z);
	return toRet;
}

/******************************************************
*  Updates the audio engine
*  \param paused tells if has to pause the engine or not
******************************************************/
void SoundSystem::Update(vector3df headPos, vector3df headRot) {
	setListenerPosRot(headPos, headRot);	//Position and rotation of the listener
	ERRCHECK(system->update());
}

void SoundSystem::Update(){
	ERRCHECK(system->update());
}

/******************************************************
*  @brief Creates the a FMOD eventDescription
*  @param string name path of the description
*  @param FMOD::Studio::EventDescription* event description pointer
******************************************************/
FMOD::Studio::EventDescription* SoundSystem::createDescription(const char* path, FMOD::Studio::EventDescription* desc){
	ERRCHECK(system->getEvent(path, &desc));	//Create the event
	eventDescriptions[path] = desc;				//Add the descriptions to the event descriptions map
	return desc;
}

/******************************************************
 * @brief Creates a sound event
 * @param std::string path of the event
 ******************************************************/
SoundEvent* SoundSystem::createEvent(std::string eventPath) {
	FMOD::Studio::EventDescription* eventDesc  = NULL;					//Initialize the event description
	FMOD::Studio::EventInstance* eventInst     = NULL;					//Initialize the event instance
	SoundEvent* newEvent					   = NULL; 					//Initialize the event
	
	//Search the description to know if it's already created
	if (eventDescriptions[eventPath] != NULL) 
		eventDesc = eventDescriptions[eventPath];					 //Set it to the eventDesc var
	else {
		eventDesc = createDescription(eventPath.c_str(), eventDesc); //Else set a new event description
		eventDescriptions[eventPath] = eventDesc;					 //And store it at the descriptions map
	}

	ERRCHECK(eventDesc->createInstance(&eventInst));				//Set the event instance

	//Dertermine wich type of sound event will create
	if (eventPath.find("Character") != -1)  		newEvent = new CharacterSound();	//Set the event
	else if (eventPath.find("Spells") != -1)   		newEvent = new SpellSound();		//Set the event		
	else if (eventPath.find("Ambience") != -1) 		newEvent = new AmbienceSound();		//Set the event
	else if (eventPath.find("HUD") != -1) 	  		newEvent = new HUDSound();			//Set the event
	else if (eventPath.find("Music") != -1)   		newEvent = new Music();				//Set the event
	else if (eventPath.find("CommonSound") != -1)	newEvent = new CommonSoundEvent();	//Set the event
	
	newEvent->setInstance(eventInst);	//Set the event instance
	soundEvents[eventPath] = newEvent;  //Store the event in the sound events map
	
	return newEvent;
}

/******************************************************
*  Sets the property for the listener
*  \param vec that has to be assigned
******************************************************/
void SoundSystem::setPos(FMOD_3D_ATTRIBUTES * var,vector3df vec) {   
	var->position.x = vec.X;
	var->position.y = vec.Y;
	var->position.z = vec.Z;
}

void SoundSystem::setVel(FMOD_3D_ATTRIBUTES * var,vector3df vec) {
	var->velocity.x = vec.X;
	var->velocity.y = vec.Y;
	var->velocity.z = vec.Z;
}

void SoundSystem::setForward(FMOD_3D_ATTRIBUTES * var,vector3df vec) {
	var->forward.x = vec.X;
	var->forward.y = vec.Y;
	var->forward.z = vec.Z;

}

void SoundSystem::setUp(FMOD_3D_ATTRIBUTES * var,vector3df vec) {
	var->up.x = vec.X;
	var->up.y = vec.Y;
	var->up.z = vec.Z;
}

/******************************************************
* @brief Checks if an event is playing and plays it
* @param eventPath path of the event to play
* @param playerPos position where should play the event and/or of the listener
* @param playerRot rotation where should play the event and/or of the listener
******************************************************/
void SoundSystem::checkAndPlayEvent(SoundEvent* event, vector3df playerPos) {
	
	//Checks if the event exists and is being played
	if (!event->isPlaying()) {
		playEvent(event, playerPos); //Plays the event
	}
}

/******************************************************
* @brief Plays a sound event
* @param eventPath path of the event to play
* @param playerPos position where should play the event and/or of the listener
* @param playerRot rotation where should play the event and/or of the listener
******************************************************/
void SoundSystem::playEvent(SoundEvent* event, vector3df playerPos) {
	event->setPosition(playerPos);	//Position the event
	event->start();					//Start the event
}

//Used for events without position as the events from the menu
void SoundSystem::playEvent(SoundEvent* event) {
	event->start();					//Start the event
}

/******************************************************
 * @brief Stops an event that is being played
 * @param eventPath path of the event to stop
 ******************************************************/
void SoundSystem::stopEvent(SoundEvent* event) {
    
    event->stop();
}

/******************************************************
 * @brief Stops an event if it's being played
 * @param eventPath path of the event to stop
 ******************************************************/
void SoundSystem::checkAndStopEvent(SoundEvent* event) {
    if (event->isPlaying()) event->stop();
}


/********************************************************************************************************
********************************************** Sound Event *********************************************
********************************************************************************************************/

/******************************************************
*  Constructor
******************************************************/
SoundEvent::SoundEvent() {
	soundInstance = NULL;
}

/******************************************************
*  Destructor
******************************************************/
SoundEvent::~SoundEvent() {
	//ERRCHECK(soundInstance->release());
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
*  @param x, y, and z, new 3D position
******************************************************/
void SoundEvent::setPosition(vector3df pos) {
	if(!isnan(pos.X)){	// Comprobamos que el valor no sea NaN
		FMOD_3D_ATTRIBUTES* attributes = new FMOD_3D_ATTRIBUTES();

		vector3df newPos(pos.X, pos.Y+0.5, pos.Z);

		SoundSystem::getInstance()->setPos(attributes, newPos);	//Set the position

		//Set the forward vector
		vector3df frwd = newPos - pos;
		frwd.normalize();
		SoundSystem::getInstance()->setForward(attributes, frwd);
		
		//Calculate the up vector from the forward vector
		//Right vector x forward vector --> (forward x absolute up) x forward --> (forward x [0,1,0]) x forward
		vector3df A = frwd; vector3df B = vector3df(0, 0, 1);
		vector3df aux = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
		A = aux; B = frwd;
		vector3df up = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
		SoundSystem::getInstance()->setUp(attributes, up);

		//Finally, set the attributes
		if (soundInstance != NULL) ERRCHECK(soundInstance->set3DAttributes(attributes));
	}
	else{
		std::cout<<"ERROR: NaN Value, SoundSystem::setPosition"<<std::endl;
	}
}

/*******************************************************
* Checks if the sound is playing
*  @return returns true if the sound is playing
*******************************************************/
bool SoundEvent::isPlaying() {
	bool res = false;
	FMOD_STUDIO_PLAYBACK_STATE state;
	ERRCHECK(soundInstance->getPlaybackState(&state)); //Checks the state (0 = true, 3 = false)
	if (state == 0) res = true;
	return res;
}

/*******************************************************
 * Releases the event and destroys it after it has stop ped
 *******************************************************/
void SoundEvent::release() {
	ERRCHECK(soundInstance->release());
}

/*******************************************************
* Sets the value of the pararmeter called as the parameter name
* @param string name of the parameter to modify
* @param float value of the parameter to modify
*******************************************************/
void SoundEvent::setParamValue(std::string name, float value) {
	ERRCHECK(soundInstance->setParameterValue(name.c_str(), value));
}   

/*******************************************************
 * @brief Returns the event instance
 * @return FMOD::Studio::EventInstance* 
 *******************************************************/
FMOD::Studio::EventInstance* SoundEvent::getInstance() {
	return soundInstance;
}

/*******************************************************
 * @brief Sets the event instance
 * @return FMOD::Studio::EventInstance* 
 *******************************************************/
void SoundEvent::setInstance(FMOD::Studio::EventInstance * instance) {
	soundInstance = instance;
}