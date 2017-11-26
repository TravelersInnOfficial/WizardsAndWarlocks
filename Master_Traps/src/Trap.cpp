#include "Trap.h"

Trap::Trap(){
    m_position = new vector3df(0,0,0);
    m_dimensions = new vector3df(0,0,0);
    m_body = new BT_GhostObject();
    m_deactivated = false;
}

Trap::Trap(vector3df position, vector3df dimensions, TrapEnum trapType){
    m_position = &position;
    m_dimensions = &dimensions;
    m_body = new BT_GhostObject();
    m_deactivated = false;
    m_trapType = trapType;

    m_body->CreateGhostBox(m_position, m_dimensions);
}

void Trap::Activate(){

}

void Trap::Deactivate(){

}

void Trap::SetPosition(vector3df position){
    m_position = &position;
}

void Trap::SetDimensions(vector3df dimensions){
    m_dimensions = &dimensions;
}

void Trap::SetType(TrapEnum trapType){
    m_trapType = trapType;
}

vector3df* Trap::GetPosition(){
    return m_position;
}

vector3df* Trap::GetDimensions(){
    return m_dimensions;
}

TrapEnum Trap::GetType(){
    return m_trapType;
}