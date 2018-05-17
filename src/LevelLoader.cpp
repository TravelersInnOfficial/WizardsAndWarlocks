#include "LevelLoader.h"

#include <map>
#include <fstream>
#include <json.hpp>
#include <Alliance.h>
#include <vector3d.h>
#include <NPCTypes.h>
#include <PotionTypes.h>

#include <GraphicEngine/GraphicEngine.h>

// Managers
#include "Managers/ObjectManager.h"
#include "Objects/Door.h"
#include "Objects/Switch.h"

class GPortal;

std::string LevelLoader::m_mapPath = "";
float LevelLoader::m_centerX = 0.0f;
float LevelLoader::m_centerY = 0.0f;
float LevelLoader::m_sizeMap = 0.0f;
float LevelLoader::m_zoom = 0.0f;

LevelLoader::LevelLoader(){

}

bool SpawnPotion(std::string objectType, vector3df position, vector3df size, vector3df rotation){
	POTIONTYPE pot = POTION_DEFENSE;

	if(objectType == "LifePotion") pot = POTION_LIFE;
	else if(objectType == "ManaPotion") pot = POTION_MANA;
	else if(objectType == "FirePotion") pot = POTION_FIRE;
	else if(objectType == "IcePotion") pot = POTION_ICE;
	else if(objectType == "ElectricPotion") pot = POTION_ELECTRIC;
	else if(objectType == "PoisonPotion") pot = POTION_POISON;
	else if(objectType == "DefensePotion") pot = POTION_DEFENSE;
	else return false;

	ObjectManager* objManager = ObjectManager::GetInstance();
	return objManager->AddPotion(position, size, rotation, pot) != nullptr ? true : false;
}

bool LevelLoader::LoadLevel(std::string jsonPath){
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	g_engine->AddDome();

	// Limpiamos los objetos
	ObjectManager* objManager = ObjectManager::GetInstance();

	// Takes path from binary location (/bin)
	std::ifstream i(jsonPath);
	nlohmann::json j;
	i >> j;

	std::map<int, Door*> doors;
	std::map<Switch*, int> switchs;

	std::string type;
	int id;
	vector3df position;
	vector3df rotation;
	vector3df size;
	std::string texture;
	std::string model;
	vector3df axis;
	GRoom* room ;

	m_mapPath = j["Map"][0]["Path"];

	float value = j["Map"][0]["Center_X"];
	m_centerX = roundf(value * 100.0f) / 100.0f;

	value = j["Map"][0]["Center_Y"];
	m_centerY = roundf(value * 100.0f) / 100.0f;

	value = j["Map"][0]["Size"];
	m_sizeMap = roundf(value * 100.0f) / 100.0f;

	value = j["Map"][0]["Zoom"];
	m_zoom 	= roundf(value * 100.0f) / 100.0f;

	for(int k=0; !j["Rooms"][k].is_null(); k++){
		room = nullptr;
		auto ptr = j["Rooms"][k];

		//Unity transform
		position = vector3df(ptr["POSITION_X"], ptr["POSITION_Y"], ptr["POSITION_Z"]);
		rotation = vector3df(ptr["ROTATION_X"], ptr["ROTATION_Y"], ptr["ROTATION_Z"]);
		size = vector3df(ptr["SCALE_X"], ptr["SCALE_Y"], ptr["SCALE_Z"]);
		id = ptr["ID"];

		// HABITACION CREADA
		if(id>=0) room = g_engine->AddRoom(id, position, rotation, size);
		g_engine->SetCurrentRoom(room);

		// Iterates objects
		for(int i = 0; !j["Rooms"][k]["Objects"][i].is_null(); i++){

			// Pointer to object body
			type = j["Rooms"][k]["Objects"][i]["Type"];
			id = j["Rooms"][k]["Objects"][i]["ID"];
			ptr = j["Rooms"][k]["Objects"][i]["Body"];

			// Unity transform
			position = vector3df(ptr["Position"][0], 	ptr["Position"][1], ptr["Position"][2]);
			if(ptr["Rotation"][0] != nullptr) rotation = vector3df(ptr["Rotation"][0], ptr["Rotation"][1], ptr["Rotation"][2]);
			if(ptr["Scale"][0] != nullptr) size = vector3df(ptr["Scale"][0], ptr["Scale"][1], ptr["Scale"][2]);

			// Textures and objects
			if(ptr["Texture"] != nullptr) texture = ptr["Texture"];
			if(ptr["3DModel"] != nullptr) model = ptr["3DModel"];

			// Aditional variables
			axis = ptr["AxisCoord"].empty()? vector3df() : vector3df(ptr["AxisCoord"][0], ptr["AxisCoord"][1], ptr["AxisCoord"][2]);


			// Create object
			if(type == "Block") {
				objManager->AddBlock(position, size, rotation, texture);
			}
			else if(type == "Prop") objManager->AddProp(position, size, rotation, model, texture);
			else if(type == "Switch"){
				Switch* swi = objManager->AddSwitch(position, size, rotation, vector3df(0,0,0));
				if(!ptr["TargetDoor"].is_null()){
					switchs[swi] = ptr["TargetDoor"];
				}
			}
			else if( SpawnPotion(type, position, size, rotation) == true ){} //stuff is made in the function
			else if(type == "Grail") objManager->AddGrail(position, size, rotation);
			else if(type == "Fountain") objManager->AddFountain(position, size, rotation);
			else if(type == "ReadyPoint") objManager->AddReadyPoint(position);
			else if(type == "NpcSelector") objManager->AddNpc(position, size, rotation, NPC_SELECTOR);
			else if(type == "NpcSeller") objManager->AddNpc(position, size, rotation, NPC_SELLER);
			else if(type == "NpcPowerUp") objManager->AddNpc(position, size, rotation, NPC_POWERUP);
			
			else if(type == "WizardSpawn"){
				position.Y += 1;
				objManager->AddSpawner(ALLIANCE_WIZARD, position, rotation);
			}

			else if(type == "WarlockSpawn"){
				position.Y += 1;
				objManager->AddSpawner(ALLIANCE_WARLOCK, position, rotation);
			}

			else if(type == "Door"){
				int idDoor = id;
				doors[idDoor] = objManager->AddDoor(position, size, rotation, axis, model);
			}

			else if(type == "DoorBlocked"){
				int idDoor = id;
				doors[idDoor] = objManager->AddDoor(position, size, rotation, axis, model);
				doors[idDoor]->SetBlock(true);
			}

			else if(type == "PointLight"){
				float range = ptr["Range"];
				vector3df color = vector3df(ptr["Color"][0], ptr["Color"][1], ptr["Color"][2]);
				objManager->AddLight(position, color, range);

			}
		}
	}

	// Juntamos cada switch con su puerta
	std::map<Switch*, int >::iterator it = switchs.begin();
	for(; it != switchs.end(); ++it){				// Recorremos entre todos los efectos
		Switch* currentSwitch = it->first;
		int assignedDoorID = it->second;
		currentSwitch->SetDoor(doors[assignedDoorID]);
	}

	// Cargamos todos los portales
	for(int i=0; !j["Connections"][i].is_null(); i++){
		auto ptr = j["Connections"][i];

		//Unity transform
		position = vector3df(ptr["POSITION_X"], ptr["POSITION_Y"], ptr["POSITION_Z"]);
		rotation = vector3df(ptr["ROTATION_X"], ptr["ROTATION_Y"], ptr["ROTATION_Z"]);
		size = vector3df(ptr["SCALE_X"], ptr["SCALE_Y"], ptr["SCALE_Z"]);

		for(int k=0; !ptr["Rooms"][k].is_null(); k++){
			int firstID = ptr["Rooms"][k]["First"];
			int secondID = ptr["Rooms"][k]["Second"];

			//Anyadir Portal, Dimensiones, ID1, ID2
			GPortal* currentPortal = g_engine->AddConnection(firstID, secondID, position, rotation, size);
			for(int l=0; !j["Connections"][i]["Door"][l].is_null(); l++){
				int assignedID = j["Connections"][i]["Door"][l];
				Door* idDoor = doors[assignedID];
				idDoor->AddPortal(currentPortal);
			}
		}

	}



	// Ponemos de vuelta la habitacion actual a null
	g_engine->SetCurrentRoom(nullptr);

	return true;
}