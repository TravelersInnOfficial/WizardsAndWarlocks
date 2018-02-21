#include "Blackboard.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./../Managers/ObjectManager.h"
#include "./../Players/AIPlayer.h"

Sense_struct::~Sense_struct(){
}

Blackboard::Blackboard(){
    blackboardParent = nullptr;
    currentPlayer = nullptr;
    roomGraph = nullptr;
    
    masterMovement = 0;
    masterAction = 0;
}

Blackboard::~Blackboard(){
    CleanSense();
    delete roomGraph;
}

void Blackboard::UnloadRoomGraph(){
    if(roomGraph!=nullptr){
        delete roomGraph;
        roomGraph = nullptr;
    }
}

void Blackboard::LoadRoomGraph(){
    if(roomGraph!=nullptr){
        delete roomGraph;
    }
    roomGraph = new RoomGraph();
    ObjectManager::GetInstance()->CopyRoomGraph(roomGraph);
    UpdateRoomGraph(0);
}

void Blackboard::UpdateRoomGraph(float deltaTime){
    if(roomGraph!=nullptr){
        vector3df pos = currentPlayer->GetPos();
        roomGraph->InitRoom(pos, deltaTime);
        roomGraph->UpdateExplore(currentPlayer->GetPos());
    }
}

void Blackboard::SetPlayer(AIPlayer* p){
    currentPlayer = p;
}

AIPlayer* Blackboard::GetPlayer(){
    if(currentPlayer!= nullptr){
        return currentPlayer;
    }else if(blackboardParent != nullptr){
        return blackboardParent->GetPlayer();
    }else{
        return nullptr;
    }
}

RoomGraph* Blackboard::GetRoomGraph(){
    if(roomGraph != nullptr){
        return roomGraph;
    }else if(blackboardParent != nullptr){
        return blackboardParent->GetRoomGraph();
    }else{
        return nullptr;
    }
}

void Blackboard::SaveParent(Blackboard* parent){
    blackboardParent = parent;
}

void* Blackboard::GetPuntero(AI_code name){
    std::map<AI_code, void*>::iterator it;
    it = dataPuntero.find(name);
    if(it != dataPuntero.end()){
        return dataPuntero[name];
    }
    else if(blackboardParent != nullptr){
        return blackboardParent->GetPuntero(name);
    }
    else{
        return nullptr;
    }
}

float Blackboard::GetFloat(AI_code name){
    std::map<AI_code, float>::iterator it;
    it = dataFloat.find(name);
    if(it != dataFloat.end()){
        return dataFloat[name];
    }
    else if(blackboardParent != nullptr){
        return blackboardParent->GetFloat(name);
    }
    else{
        return -1; //Numero que devuelve en caso negativo
    }
}

void Blackboard::CleanPuntero(AI_code name){
    dataPuntero[name] = nullptr;
}

void Blackboard::SetPuntero(AI_code name, void* value){
    dataPuntero[name] = value;
}

void Blackboard::SetFloat(AI_code name, float value){
    dataFloat[name] = value;
}

void Blackboard::SetSound(int id, void* punt, AI_code name, Kinematic kin, float dur){
    int size = soundSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = soundSense[i];
        if(s->id == id){
            s->code = name;
            s->kinematic = kin;
            s->duration = dur;
            return;
        }
    }

    Sense_struct* s = new Sense_struct();
    s->id = id;
    s->pointer = punt;
    s->code = name;
    s->kinematic = kin;
    s->duration = dur;
    soundSense.push_back(s);
}

/**
 * @brief Vacia por completo los sentidos de la IA, resetea la informacion
 */
void Blackboard::CleanSense(){
    int size = soundSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = soundSense[i];
        delete s;
    }
    soundSense.clear();

    size = sightSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = sightSense[i];
        delete s;
    }
    sightSense.clear();
}

void Blackboard::CleanSense(int id){
    int size = soundSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = soundSense[i];
        if(s->id == id){
            soundSense.erase(soundSense.begin()+i);
            delete s;
            break;
        }
    }

    size = sightSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = sightSense[i];
        if(s->id == id){
            sightSense.erase(sightSense.begin()+i);
            delete s;
            break;
        }
    }
}

void Blackboard::SetSight(int id, void* punt, AI_code name, Kinematic kin, float dur){
    int size = sightSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = sightSense[i];
        if(s->id == id){
            s->code = name;
            s->kinematic = kin;
            s->duration = dur;
            return;
        }
    }

    Sense_struct* s = new Sense_struct();
    s->id = id;
    s->pointer = punt;
    s->code = name;
    s->kinematic = kin;
    s->duration = dur;
    sightSense.push_back(s);
}

/**
 * Dice el numero de Sonios escuchados por la IA con ese tag
 * 
 * @name Tag del emisor del que se quiere ver
 * @return Numero de veces que aparece
 */
int Blackboard::GetNumberSound(AI_code name){
    GraphicEngine* g_engine = GraphicEngine::getInstance();
    float time = g_engine->getTime();        // Cogemos el tiempo actual del juego
    int output = 0;                         // Output que devolvera el numero de Sonidos con ese TAG
    int size = soundSense.size();           
    for(int i=size-1; i>=0; i--){           // Recorremos todos los sonidos del final al principio para poder eliminar
        Sense_struct* s = soundSense[i];
        if(s->duration < time && s->duration != 0){             // Si la duracion del sonido ya ha pasado lo eliminamos
            soundSense.erase(soundSense.begin() + i);
            delete s;
        }
        else{
            if(s->code == name){          // Si el tag es correcto anyadimos 1 al output
                output++;
            }
        }
    }
    return output;
}

/**
 * Dice el numero de visiones vistas por la IA con ese Tag
 * 
 * @name Tag del emisor del que se quiere ver
 * @return Numero de veces que aparece
 */
int Blackboard::GetNumberSight(AI_code name){
    GraphicEngine* g_engine = GraphicEngine::getInstance();
    float time = g_engine->getTime();        // Cogemos el tiempo actual del juego
    int output = 0;                         // Output que devolvera el numero de Visiones con ese TAG
    int size = sightSense.size();           
    for(int i=size-1; i>=0; i--){           // Recorremos todos los sonidos del final al principio para poder eliminar
        Sense_struct* s = sightSense[i];
        if(s->duration < time && s->duration != 0){             // Si la duracion de la vision ya ha pasado lo eliminamos
            sightSense.erase(sightSense.begin() + i);
            delete s;
        }
        else{
            if(s->code == name){          // Si el tag es correcto anyadimos 1 al output
                output++;
            }
        }
    }
    return output;
}

/**
 * Guarda como Target de la IA el elemento más cercano
 * 
 * @name tipo del elemento a poner en target
 * @where donde poner la variable
 */
void Blackboard::SetTargetSight(AI_code name, AI_code where){
    vector3df playerPos;
    if(currentPlayer!=nullptr) playerPos = currentPlayer->GetPos();

    void* em = nullptr;
    float d = std::numeric_limits<float>::max();

    int size = sightSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = sightSense[i];
        if(s->code == name){
            float length = (playerPos - sightSense[i]->kinematic.position).length();
            if(length < d){
                d = length;
                em = s;
            }
        }
    }
    if(em != nullptr){
        SetPuntero(where, em);
    }
}

void Blackboard::SetTargetSound(AI_code name, AI_code where){
    vector3df playerPos;
    if(currentPlayer!=nullptr) playerPos = currentPlayer->GetPos();

    void* em = nullptr;
    float d = std::numeric_limits<float>::max();

    int size = soundSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = soundSense[i];
        if(s->code == name){
            float length = (playerPos - soundSense[i]->kinematic.position).length();
            if(length < d){
                d = length;
                em = s;
            }
        }
    }
     if(em != nullptr){
        SetPuntero(where, em);
    }
}

void Blackboard::SetMasterMovement(AI_code name){
    masterMovement = (int)name;
}

void Blackboard::SetMasterAction(AI_code name){
    // En el caso de que se cambie uno de los hechizos reseteamos estos
    if(masterAction >= AI_MOVE_SPELL00 && masterAction <= AI_MOVE_SPELL03){
        currentPlayer->SetController(ACTION_SHOOT, RELEASED);   
    }
    masterAction = (int)name;
}