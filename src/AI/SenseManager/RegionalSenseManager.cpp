#include "RegionalSenseManager.h"
#include "./../../GraphicEngine/GraphicEngine.h"

GraphicEngine* g_engine = GraphicEngine::getInstance(); 

RegionalSenseManager* RegionalSenseManager::instance = 0;

RegionalSenseManager* RegionalSenseManager::GetInstance(){
	if(instance==0){
		instance = new RegionalSenseManager();
	}
	return instance;
}

RegionalSenseManager::RegionalSenseManager(){}

RegionalSenseManager::~RegionalSenseManager(){
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
}

Sensor* RegionalSenseManager::AddSensor(int id, vector3df* cPos, vector3df* cOri, float thresh, Blackboard* inf){
	Sensor* s = new Sensor(id, cPos, cOri, thresh, inf);
	sensors.push_back(s);
	return s;
}

void RegionalSenseManager::AddSignal(int id, AI_code name, float str, Kinematic kin, AI_modalities mod){
	Signal* s = new Signal(id, name, str, kin, mod);
	AddSignal(s);
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
	for(int i=0; i<size; i++){
		Notification* n = notificationQueue[i];
		//Check if the notification is due
		if(n->time <= currentTime){
			n->sensor->Notify(n->signal, currentTime);
			notificationQueue.erase(notificationQueue.begin()+i);
			delete n;
		}
		else{
			break;
		}
	}
}