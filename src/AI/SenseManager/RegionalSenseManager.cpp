#include "RegionalSenseManager.h"
// Engines
#include <GraphicEngine/GraphicEngine.h>
// Managers
#include "./../../Managers/TrapManager.h"
#include "./../../Managers/PlayerManager.h"
#include "./../../Managers/ObjectManager.h"

static RegionalSenseManager* instance = nullptr;

RegionalSenseManager* RegionalSenseManager::GetInstance(){
	static RegionalSenseManager localInstance;
	if(instance == nullptr){
		localInstance.InitObject();
		instance = &localInstance;
	}
	return instance;
}

RegionalSenseManager::RegionalSenseManager(){
}

RegionalSenseManager::~RegionalSenseManager(){
	EmptyObject();
}

void RegionalSenseManager::InitObject(){
	g_engine = GraphicEngine::getInstance();
}

void RegionalSenseManager::EmptyObject(){
	int size = sensors.size();
	for(int i=0; i<size; i++){
		Sensor* s = sensors[i];
		delete s;
	}
	sensors.clear();

	size = notificationQueue.size();
	for(int i=0; i<size; i++){
		Notification* n = notificationQueue[i];
		delete n;
	}
	notificationQueue.clear();
	instance = nullptr;
}

Sensor* RegionalSenseManager::AddSensor(int id, vector3df* cPos, vector3df* cOri, float thresh, Blackboard* inf){
	Sensor* s = new Sensor(id, cPos, cOri, thresh, inf);
	sensors.push_back(s);
	return s;
}

/**
 * @brief Anyade una senyal al manager de sentidos
 * 
 * @param id 	- Id del emisor
 * @param punt	- Puntero del emisor
 * @param temp	- La senyal perdurara en memoria? Si/No
 * @param name	- Codigo de IA que define el tipo de objeto
 * @param str 	- Fuera de la senyal
 * @param kin 	- Objeto kinematico del objeo
 * @param mod 	- Modalidad de la senyal
 */
void RegionalSenseManager::AddSignal(int id, void* punt, bool temp, AI_code name, float str, Kinematic kin, AI_modalities mod){
	Signal* s = new Signal(id, punt, temp, name, str, kin, mod);
	AddSignal(s);
}

void RegionalSenseManager::CreateAllSignals(){
	PlayerManager::GetInstance()->SendVisualSignal();
	ObjectManager::GetInstance()->SendAllSignal();
	TrapManager::GetInstance()->SendAllSignal();
}

void RegionalSenseManager::AddSignal(Signal* sig){
	//Aggregation phase
	// validSensor = [];
	int size = sensors.size();
	for(int i=0; i<size; i++){
		Sensor* sensor = sensors[i];
		// Testing phase
		// Check if is the sensor who emmit the signal
		if(sensor->GetId() == sig->GetId()){
			continue;
		}
		// Check the modality first
		if(!sensor->DetectsModality(sig->GetModality())){
			continue; //Pasar al siguiente elemento
		}
		// Find te distance of the signal and check range
		vector3df direction = sig->GetPosition() - sensor->GetPosition();
		float distance = direction.length();
		if(sig->GetModality()->GetMaximumRange() < distance){
			continue;
		}
		// Find the intensity of the signal and check threshold
		float intensity = sig->GetStrength() * pow(sig->GetModality()->GetAttenuation(), distance);
		if (intensity < sensor->GetThreshold()){
			continue;
		}
		// Perform additional modaliy specific checks
		if (!sig->GetModality()->ExtraChecks(sig, sensor)){
			continue;
		}
		// We are goint to notify the sensor, work out when
		float time = g_engine->getTime() + distance * sig->GetModality()->GetInverseSpeed();
		// Create a Notification
		Notification* notification = new Notification();
		notification->time = time;
		notification->sensor = sensor;
		notification->signal = new Signal(sig);
		notificationQueue.push_back(notification);
	}
	delete sig;
	//Send signals, in case the current signal is ready to notify immediately
	SendSignals();
}

void RegionalSenseManager::SendSignals(){
	//Notification Phase
	float currentTime = g_engine->getTime() +  0;
	int size = notificationQueue.size();
	for(int i=size-1; i>=0; i--){
		Notification* n = notificationQueue[i];
		//Check if the notification is due
		if(n->time <= currentTime){
			n->sensor->Notify(n->signal, currentTime);
			notificationQueue.erase(notificationQueue.begin()+i);
			delete n;
		}
	}
}

void RegionalSenseManager::ResetSensor(Sensor* s){
	int size = notificationQueue.size();
	for(int i=size-1; i>=0; i--){
		Notification* n = notificationQueue[i];
		if(n->sensor == s){
			notificationQueue.erase(notificationQueue.begin() + i);
			delete n;
		}
	}
}