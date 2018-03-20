#include "LevelLoader.h"
#include <map>
#include <json.hpp>
#include <fstream>
#include <Alliance.h>
#include <vector3d.h>
#include <NPCTypes.h>
#include <PotionTypes.h>
#include <iostream>

#include <GraphicEngine/GraphicEngine.h>

// Managers
#include "Managers/TrapManager.h"
#include "Managers/SpellManager.h"
#include "Managers/BulletManager.h"
#include "Managers/ObjectManager.h"
#include "Managers/PlayerManager.h"


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
	
	GraphicEngine::getInstance()->AddDome();

	// Limpiamos los objetos
	ObjectManager* objManager = ObjectManager::GetInstance();

	// Takes path from binary location (/bin)
	std::ifstream i(jsonPath);
	nlohmann::json j;
	i >> j;

	std::map<int, Door*> doors;
	std::string type;
	vector3df position;
	vector3df rotation;
	vector3df size;
	std::string texture;
	std::string model;
	vector3df axis;

	// Iterates objects
	for(int i = 0; !j["Objects"][i].is_null(); i++){
		
		// Pointer to object body
		auto ptr = j["Objects"][i]["Body"];
		type = j["Objects"][i]["Type"];

		// Unity transform
		position = vector3df(ptr["Position"][0], 	ptr["Position"][1], ptr["Position"][2]);
		if(ptr["Rotation"][0] != nullptr) rotation = vector3df(ptr["Rotation"][0], ptr["Rotation"][1], ptr["Rotation"][2]);
		if(ptr["Scale"][0] != nullptr) size = vector3df(ptr["Scale"][0], ptr["Scale"][1], ptr["Scale"][2]);

		// Textures and objects
		if(ptr["Texture"] != nullptr) texture = ptr["Texture"];
		if(ptr["3DModel"] != nullptr) model = ptr["3DModel"];

		// Aditional variables
		axis = ptr["AxisCoord"].empty()? vector3df() :
		vector3df(ptr["AxisCoord"][0], ptr["AxisCoord"][1], ptr["AxisCoord"][2]);

		// Create object
		if(type == "Block") objManager->AddBlock(position, size, rotation, texture);
		else if(type == "Prop") objManager->AddProp(position, size, rotation, model, texture);
		else if(type == "Switch"){}//do nothing here
		else if( SpawnPotion(type, position, size, rotation) == true ){} //stuff is made in the function
		else if(type == "Grail") objManager->AddGrail(position, size, rotation);
		else if(type == "Fountain") objManager->AddFountain(position, size, rotation);
		else if(type == "ReadyPoint") objManager->AddReadyPoint(position);
		else if(type == "NpcSelector") objManager->AddNpc(position, size, rotation, NPC_SELECTOR);
		else if(type == "NpcSeller") objManager->AddNpc(position, size, rotation, NPC_SELLER);
		else if(type == "NpcPowerUp") objManager->AddNpc(position, size, rotation, NPC_POWERUP);
		
		else if(type == "WizardSpawn"){
			objManager->AddProp(position, size, rotation, model, texture);
			position.Y += 1;
			objManager->AddSpawner(ALLIANCE_WIZARD, position);
		}

		else if(type == "WarlockSpawn"){
			objManager->AddProp(position, size, rotation, model, texture);
			position.Y += 1;
			objManager->AddSpawner(ALLIANCE_WARLOCK, position);
		}

		else if(type == "Door"){
			int idDoor = j["Objects"][i]["ID"];
			doors[idDoor] = objManager->AddDoor(position, size, rotation, axis);
		}

		else if(type == "DoorBlocked"){
			int idDoor = j["Objects"][i]["ID"];
			doors[idDoor] = objManager->AddDoor(position, size, rotation, axis);
			doors[idDoor]->SetBlock(true);
		}

		else if(type == "PointLight"){
			float range = ptr["Range"];
			vector3df color = vector3df(ptr["Color"][0], ptr["Color"][1], ptr["Color"][2]);
			objManager->AddLight(position, color, range);
		}

	}

	// Loop to iterate switch objects and assign it to a door
	for(int i = 0; !j["Objects"][i].is_null(); i++){
		if(j["Objects"][i]["Type"] == "Switch" && !j["Objects"][i]["TargetDoor"].is_null()){
			// Pointer to object body
			auto ptr = j["Objects"][i]["Body"];
			// Unity transform
			position = vector3df(ptr["Position"][0], ptr["Position"][1], ptr["Position"][2]);
			rotation = vector3df(ptr["Rotation"][0], ptr["Rotation"][1], ptr["Rotation"][2]);
			size     = vector3df(ptr["Scale"][0], ptr["Scale"][1], ptr["Scale"][2]);

			//axis = ptr["AxisCoord"].empty()? vector3df() :
			//vector3df(ptr["AxisCoord"][0], ptr["AxisCoord"][1], ptr["AxisCoord"][2]);

			int assignedDoorID = j["Objects"][i]["TargetDoor"];
			objManager->AddSwitch(doors[assignedDoorID], position, vector3df(1,1,1), rotation, vector3df(0,0,0));
		}
	}

	return true;
}