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
	delete banksPath;
	delete system;
	delete lowLevelSystem;
	delete masterBank;
	delete stringsBank;
	delete busMaster;
	delete listener;
	banks.clear();
	eventDescriptions.clear();
	soundEvents.clear();
}

/******************************************************
*  Constructor
*  @param soundBanksPath directory path where are located the FMOD Studio generated banks
******************************************************/
void SoundSystem::createSystem(std::string soundBanksPath){
   
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
	ERRCHECK(system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

	//Load the needed banks
	loadBanks();

	//Initialize other variables
	listener = new FMOD_3D_ATTRIBUTES();
	setPos(listener,vector3df(0.0f,0.0f,0.0f));
	setVel(listener,vector3df(0.0f,0.0f,0.0f));
	setForward(listener,vector3df(1.0f,0.0f,0.0f));
	setUp(listener,vector3df(0.0f,1.0f,0.0f));

	//Create event descriptions
	createEventDescriptionsNEvents();
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
*  Creates all the event descriptions and instances needed for the sound events
******************************************************/
void SoundSystem::createEventDescriptionsNEvents() {
	
	//Initialize the event descriptions
	FMOD::Studio::EventDescription * footstepDesc = NULL;
	FMOD::Studio::EventDescription * hitDesc = NULL;
	FMOD::Studio::EventDescription * drinkDesc = NULL;
	FMOD::Studio::EventDescription * dieDesc = NULL;

	//Initialize the event instances
	FMOD::Studio::EventInstance* footstepInst;
	FMOD::Studio::EventInstance* hitInst;
	FMOD::Studio::EventInstance* drinkInst;
	FMOD::Studio::EventInstance* dieInst;

	footstepDesc = createDescription("event:/Character/Hard/Footsteps", footstepDesc);  // Create and store the event in a map
	ERRCHECK(footstepDesc->createInstance(&footstepInst));                              // Create the event instance
	SoundEvent* footstepEvent = new CharacterSound();                                   // Initialize the event    
	footstepEvent->setInstance(footstepInst);                                           // Set the instance to the event
	soundEvents["event:/Character/Hard/Footsteps"] = footstepEvent;                     // Add the instance to the event instances map    

	hitDesc = createDescription("event:/Character/Hard/Hit", hitDesc);	// Create and store the event in a map          
	ERRCHECK(hitDesc->createInstance(&hitInst));						// Create the event instance  
	SoundEvent* hitEvent = new CharacterSound();						// Initialize the event      
	hitEvent->setInstance(hitInst);										// Set the instance to the event  
	soundEvents["event:/Character/Hard/Hit"] = hitEvent;				// Add the instance to the event instances map    
	
	drinkDesc = createDescription("event:/Character/Hard/Drink", drinkDesc);	// Create and store the event in a map          
	ERRCHECK(drinkDesc->createInstance(&drinkInst));							// Create the event instance
	SoundEvent* drinkEvent = new CharacterSound();								// Initialize the event    
	drinkEvent->setInstance(drinkInst);											// Set the instance to the event
	soundEvents["event:/Character/Hard/Drink"] = drinkEvent;					// Add the instance to the event instances map    

	dieDesc = createDescription("event:/Character/Hard/Die", dieDesc);	// Create and store the event in a map          
	ERRCHECK(dieDesc->createInstance(&dieInst));						// Create the event instance
	SoundEvent* dieEvent = new CharacterSound();						// Initialize the event    
	dieEvent->setInstance(dieInst);										// Set the instance to the event
	soundEvents["event:/Character/Hard/Die"] = dieEvent;				// Add the instance to the event instances map
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

	rot = rot * M_PI/180;		// Transformamos la rotacion
	rot.X = 0; rot.Z = 0;
	setPos(listener, pos);		// Ponemos la Posicion

	// Calculamos el FORWARD
	vector3df frwd = vector3df (sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X));
	setForward(listener, frwd);	// Ponemos el FORWARD

	// ########### CALCULATE UP VECTOR FROM FORWARD
	// RIGHT VECTOR X FORWARD VECTOR --> (FORWARD X ABSOLUTE UP) X FORWARD --> (FORWARD X [0,1,0]) X FORWARD
	vector3df A = frwd; vector3df B = vector3df(0, 1, 0);
	vector3df aux = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
	A = aux; B = frwd;
	vector3df up = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
	setUp(listener, up);
	// ###############################

	// Ponemos la VELOCIDAD
	setVel(listener, vector3df(0.0f,0.0f,0.0f));
	
	//std::cout<<"FORWARD: "<<frwd<<std::endl;
	//std::cout<<"UP: "<<up<<std::endl;
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
void SoundSystem::checkAndPlayEvent(std::string eventPath, vector3df playerPos, vector3df playerRot) {
	//Checks if the event exists and is being played
	if(getEvent(eventPath.c_str()) != NULL){
		if (!getEvent(eventPath.c_str())->isPlaying()) {
			playEvent(eventPath, playerPos, playerRot); //Plays the event
		}
	}
}

/******************************************************
* @brief Plays a sound event
* @param eventPath path of the event to play
* @param playerPos position where should play the event and/or of the listener
* @param playerRot rotation where should play the event and/or of the listener
******************************************************/
void SoundSystem::playEvent(std::string eventPath, vector3df playerPos, vector3df playerRot) {
	getEvent(eventPath.c_str())->setPosition(playerPos);	//Position the event
	getEvent(eventPath.c_str())->start();        			//Plays the event
}

/********************************************************************************************************
********************************************** Sound Event *********************************************
********************************************************************************************************/

/******************************************************
*  Constructor
******************************************************/
SoundEvent::SoundEvent() {}

/******************************************************
*  Destructor
******************************************************/
SoundEvent::~SoundEvent() {
	delete soundInstance;
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
	FMOD_3D_ATTRIBUTES* attributes = new FMOD_3D_ATTRIBUTES();

	SoundSystem::getInstance()->setPos(attributes, pos);					// Ponemos la Posicion
	SoundSystem::getInstance()->setVel(attributes, vector3df(0, 0, 0));		// Ponemos la Velocidad


	// ########### CALCULATE FORWARD VECTOR (LISTENER TO SOUND POSITION NORMALIZED)
		// Recibimos la posicion del listener
		//vector3df listenerPos = SoundSystem::getInstance()->getListenerPosition();
		vector3df frwd = vector3df(0,0,1);
		frwd.normalize();
		SoundSystem::getInstance()->setForward(attributes, frwd);
	// ###############################

	// ########### CALCULATE UP VECTOR FROM FORWARD
	// RIGHT VECTOR X FORWARD VECTOR --> (FORWARD X ABSOLUTE UP) X FORWARD --> (FORWARD X [0,1,0]) X FORWARD
		vector3df A = frwd; vector3df B = vector3df(0, 1, 0);
		vector3df aux = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
		A = aux; B = frwd;
		vector3df up = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
		SoundSystem::getInstance()->setUp(attributes, up);
	// ###############################
	
	// std::cout<<"##################################"<<std::endl;
	// std::cout<<"SOUND POS: "<<pos<<std::endl;
	// std::cout<<"FORWARD VECTOR: "<<frwd<<std::endl;
	// std::cout<<"UP VECTOR: "<<up<<std::endl;
	
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

FMOD::Studio::EventInstance* SoundEvent::getInstance() {
	return soundInstance;
}

void SoundEvent::setInstance(FMOD::Studio::EventInstance * instance) {
	soundInstance = instance;
}