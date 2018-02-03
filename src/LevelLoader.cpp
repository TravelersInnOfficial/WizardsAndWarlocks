#include "LevelLoader.h"
#include "Objects/Block.h"
#include "Managers/SpellManager.h"
#include "Managers/BulletManager.h"
#include "Managers/ObjectManager.h"
#include "Managers/PlayerManager.h"
#include "Managers/TrapManager.h"
#include <NetworkEngine/NetworkEngine.h>
#include <map>
#include <json.hpp>
#include <fstream>
#include <Alliance.h>
#include <vector3d.h>
#include <NPCTypes.h>
#include <PotionTypes.h>

#include <GraphicEngine/GraphicEngine.h>

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
	return objManager->AddPotion(position, size, rotation, pot) != NULL ? true : false;
}

bool LevelLoader::LoadLevel(std::string jsonPath){

	// Primero de todo reseteamos los hechizos para que limpien los objetos que toqen
	//zSpellManager::GetInstance()->ResetAllDieHechizo();

	// Limpiamos los objetos
	ObjectManager* objManager = ObjectManager::GetInstance();
	//objManager->ClearMap();
	//BulletManager::GetInstance()->DeleteAllProyectiles();
	//TrapManager::GetInstance()->ClearTraps();

	// Reseteamos los hechizos
	//PlayerManager::GetInstance()->ResetAllSpells();

	std::map<int, Door*> doors;
	
	//Takes path from binary location (/bin)
	std::ifstream i(jsonPath);
	nlohmann::json j;
	i >> j;

	std::string type;
	vector3df position;
	vector3df rotation;
	vector3df size;
	std::string texture;
	std::string model;
	vector3df axis;

	//iterates objects
	for(int i = 0; !j["Objects"][i].is_null(); i++){
		//pointer to object body
		auto ptr = j["Objects"][i]["Body"];
		type = j["Objects"][i]["Type"];
		
		//unity transform
		position = vector3df(ptr["Position"][0], 	ptr["Position"][1], ptr["Position"][2]);
		rotation = vector3df(ptr["Rotation"][0], 	ptr["Rotation"][1], ptr["Rotation"][2]);
		size     = vector3df(ptr["Scale"][0], 		ptr["Scale"][1], 	ptr["Scale"][2]);

		//Textures and objects
		texture = ptr["Texture"];
		model = ptr["3DModel"];

		//aditional variables
		axis = ptr["AxisCoord"].empty()? vector3df() : 
		vector3df(ptr["AxisCoord"][0], ptr["AxisCoord"][1], ptr["AxisCoord"][2]);
		
		//create object
		if(type == "Block"){
			objManager->AddBlock(position, size, rotation, texture);           
		}
		else if(type == "Prop"){
			objManager->AddProp(position, size, rotation, model, texture); 
		}
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
		else if(type == "Switch"){
			// do nothing here
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
		else if( SpawnPotion(type, position, size, rotation) == true ){
			// stuff is made in the function
		}
		else if(type == "Grail"){
			objManager->AddGrail(position, size, rotation);
		}
		else if(type == "Fountain"){
			objManager->AddFountain(position, size, rotation);
		}
		else if(type == "ReadyPoint"){
			objManager->AddReadyPoint(position);
		}
		else if(type == "NpcSelector"){
			objManager->AddNpc(position, size, rotation, NPC_SELECTOR);
		}
		else if(type == "NpcSeller"){
			objManager->AddNpc(position, size, rotation, NPC_SELLER);
		}
		else if(type == "NpcPowerUp"){
			objManager->AddNpc(position, size, rotation, NPC_POWERUP);
		}
		else{
			//std::cout<<"No se controla el tipo: "<<j["Objects"][i]["Type"]<<std::endl;
		}

	}

	// loop to iterate switch objects and assign it to a door
	for(int i = 0; !j["Objects"][i].is_null(); i++){
		if(j["Objects"][i]["Type"] == "Switch" && !j["Objects"][i]["TargetDoor"].is_null()){
			//pointer to object body
			auto ptr = j["Objects"][i]["Body"];
			//unity transform
			position = vector3df(ptr["Position"][0], ptr["Position"][1], ptr["Position"][2]);
			rotation = vector3df(ptr["Rotation"][0], ptr["Rotation"][1], ptr["Rotation"][2]);
			size     = vector3df(ptr["Scale"][0], ptr["Scale"][1], ptr["Scale"][2]);

			//axis = ptr["AxisCoord"].empty()? vector3df() : 
			//vector3df(ptr["AxisCoord"][0], ptr["AxisCoord"][1], ptr["AxisCoord"][2]);
						
			int assignedDoorID = j["Objects"][i]["TargetDoor"];
			objManager->AddSwitch(doors[assignedDoorID], position, vector3df(1,1,1), rotation, vector3df(0,0,0));
		}
	}

	PlayerManager* plyManager = PlayerManager::GetInstance();
	plyManager->RestartMatchStatus();

	return true;
}