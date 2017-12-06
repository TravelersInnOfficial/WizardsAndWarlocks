#include "LevelLoader.h"
#include "Objects/Block.h"
#include "Managers/ObjectManager.h"
#include <vector3d.h>
#include <json.hpp>
#include <fstream>

LevelLoader::LevelLoader(){

}

bool LevelLoader::loadLobby()
{   
    return readJson("../assets/json/map.json");
}

bool LevelLoader::readJson(std::string jsonPath){
    ObjectManager* objManager = ObjectManager::GetInstance();
    
    //Takes path from binary location (/bin)
    std::ifstream i(jsonPath);
    nlohmann::json j;
    i >> j;

    //iterates objects
    for(int i = 0; !j["Objects"][i].is_null(); i++){
        //pointer to object body
        auto ptr = j["Objects"][i]["Body"];
        //unity transform
        vector3df position = vector3df(ptr["Position"][0], ptr["Position"][1], ptr["Position"][2]);
        vector3df rotation = vector3df(ptr["Rotation"][0], ptr["Rotation"][1], ptr["Rotation"][2]);
        vector3df size =     vector3df(ptr["Scale"][0], ptr["Scale"][1], ptr["Scale"][2]);

        //Textures and objects
        std::string texture = ptr["Texture"];
        std::string model = ptr["3DModel"];

        //aditional variables
        vector3df axis = ptr["AxisCoord"].empty()? vector3df() : 
        vector3df(ptr["AxisCoord"][0], ptr["AxisCoord"][1], ptr["AxisCoord"][2]);
        
        bool interact = ptr["Interact"];

        //create object
        if(j["Objects"][i]["Type"] == "Block"){
            objManager->AddBlock(position, size, rotation, texture);
        }
        else if(j["Objects"][i]["Type"] == "Door"){
            //objManager->AddDoor(position, size, rotation);
        }
        else if(j["Objects"][i]["Type"] == "Switch"){
            //objManager->AddSwitch(door, position, size, rotation);
        }
        else if(j["Objects"][i]["Type"] == "Grail"){
            //objManager->AddGrail(position, size, rotation);
        }
        else if(j["Objects"][i]["Type"] == "Fountain"){
            //objManager->AddFountain(position, size, rotation);
        }
    }

    return true;
}