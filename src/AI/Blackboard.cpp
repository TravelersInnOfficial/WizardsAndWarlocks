#include "Blackboard.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../Players/AIPlayer.h"

Sense_struct::~Sense_struct(){
}

Blackboard::Blackboard(){
    blackboardParent = NULL;
    currentPlayer = NULL;
    
    masterMovement = 0;
    masterAction = 0;
}

Blackboard::~Blackboard(){
    CleanSense();
}

void Blackboard::SetPlayer(AIPlayer* p){
    currentPlayer = p;
}

AIPlayer* Blackboard::GetPlayer(){
    if(currentPlayer!= NULL){
        return currentPlayer;
    }else if(blackboardParent != NULL){
        return blackboardParent->GetPlayer();
    }else{
        return NULL;
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
    else if(blackboardParent != NULL){
        return blackboardParent->GetPuntero(name);
    }
    else{
        return NULL;
    }
}

int Blackboard::GetInt(AI_code name){
    std::map<AI_code, int>::iterator it;
    it = dataInt.find(name);
    if(it != dataInt.end()){
        return dataInt[name];
    }
    else if(blackboardParent != NULL){
        return blackboardParent->GetInt(name);
    }
    else{
        return -1; //Numero que devuelve en caso negativo
    }
}

void Blackboard::CleanPuntero(AI_code name){
    dataPuntero[name] = NULL;
}

void Blackboard::SetPuntero(AI_code name, void* value){
    dataPuntero[name] = value;
}

void Blackboard::SetInt(AI_code name, int value){
    dataInt[name] = value;
}

void Blackboard::SetSound(int id, void* punt, AI_code name, Kinematic kin, float len, float dur){
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
    s->length = len;
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

void Blackboard::SetSight(int id, void* punt, AI_code name, Kinematic kin, float len, float dur){
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
    s->length = len;
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
    void* em = NULL;
    float d = -1;
    int size = sightSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = sightSense[i];
        if(s->code == name){
            if(d == -1){
                d = s->length;
                em = s;
            }else{
                if(s->length < d){
                    d = s->length;
                    em = s;
                }
            }
        }
    }
    if(em != NULL){
        SetPuntero(where, em);
    }
}

void Blackboard::SetTargetSound(AI_code name, AI_code where){
    void* em = NULL;
    float d = -1;
    int size = soundSense.size();
    for(int i=0; i<size; i++){
        Sense_struct* s = soundSense[i];
        if(s->code == name){
            if(d == -1){
                d = s->length;
                em = s;
            }else{
                if(s->length < d){
                    d = s->length;
                    em = s;
                }
            }
        }
    }
     if(em != NULL){
        SetPuntero(where, em);
    }
}

void Blackboard::SetMasterMovement(AI_code name){
    masterMovement = name - AI_MOVE_DEFAULT;
}

void Blackboard::SetMasterAction(AI_code name){
    masterAction = name - AI_TASK_DEFAULT;
}