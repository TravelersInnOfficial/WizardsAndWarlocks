#include "LevelLoader.h"
#include "Objects/Block.h"
#include "Managers/ObjectManager.h"
#include <vector3d.h>
#include <map>
#include <json.hpp>
#include <fstream>

LevelLoader::LevelLoader(){
    
}

bool LevelLoader::LoadLevel(std::string jsonPath){
    ObjectManager* objManager = ObjectManager::GetInstance();
    std::map<int, Door*> doors;
    
    //Takes path from binary location (/bin)
    std::ifstream i(jsonPath);
    //std::cout<<"Encuentra el json"<<std::endl;
    nlohmann::json j;
    i >> j;

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
        //unity transform
        position = vector3df(ptr["Position"][0], ptr["Position"][1], ptr["Position"][2]);
        rotation = vector3df(ptr["Rotation"][0], ptr["Rotation"][1], ptr["Rotation"][2]);
        size     = vector3df(ptr["Scale"][0], ptr["Scale"][1], ptr["Scale"][2]);

        //Textures and objects
        texture = ptr["Texture"];
        model = ptr["3DModel"];

        //aditional variables
        axis = ptr["AxisCoord"].empty()? vector3df() : 
        vector3df(ptr["AxisCoord"][0], ptr["AxisCoord"][1], ptr["AxisCoord"][2]);
        
        //create object
        if(j["Objects"][i]["Type"] == "Block"){
            objManager->AddBlock(position, size, rotation, texture);           
        }
        else if(j["Objects"][i]["Type"] == "Switch"){
            // do nothing here
        }
        else if(j["Objects"][i]["Type"] == "Door"){
            int idDoor = j["Objects"][i]["ID"];
            doors[idDoor] = objManager->AddDoor(position, size, rotation, axis);
        }
        else if(j["Objects"][i]["Type"] == "PotionSpawner"){
            objManager->AddBlock(position, size, rotation, "../assets/textures/red.jpg");
            position.Y += 1;
            objManager->AddPotion(position, vector3df(0.25,0.25,0.25), rotation);
        }
		else if(j["Objects"][i]["Type"] == "Potion"){
            objManager->AddFountain(position, vector3df(1,1,1), rotation);
        }
        else if(j["Objects"][i]["Type"] == "PlayerSpawner"){
            objManager->AddBlock(position, size, rotation, "../assets/textures/yellow.jpg");
        }
        else if(j["Objects"][i]["Type"] == "Grail"){
            objManager->AddGrail(position, vector3df(1,1,1), rotation);
        }
        else if(j["Objects"][i]["Type"] == "Fountain"){
            objManager->AddFountain(position, vector3df(1,1,1), rotation);
        }
        else if(j["Objects"][i]["Type"] == "NPC1"){
            //objManager->AddNPC(position, vector3df(1,1,1), rotation, 1);
        }
        else if(j["Objects"][i]["Type"] == "NPC2"){
            //objManager->AddNPC(position, vector3df(1,1,1), rotation, 2);
        }
        else if(j["Objects"][i]["Type"] == "NPC3"){
            //objManager->AddNPC(position, vector3df(1,1,1), rotation, 3);
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
    return true;
}