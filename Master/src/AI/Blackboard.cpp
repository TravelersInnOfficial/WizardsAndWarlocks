#include "Blackboard.h"

Blackboard::Blackboard(){

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
        return 0; //Numero que devuelve en caso negativo
    }
}

void Blackboard::SetPuntero(AI_code name, void* value){
    dataPuntero[name] = value;
}

void Blackboard::SetInt(AI_code name, int value){
    dataInt[name] = value;
}

