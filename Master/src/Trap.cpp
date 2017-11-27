#include "Trap.h"

Trap::Trap(){
    m_position = new vector3df(0,0,0);
    m_dimensions = new vector3df(0,0,0);
    m_body = new BT_GhostObject();
    m_deactivated = false;
    m_texturePath = "";
}

Trap::Trap(vector3df position, vector3df dimensions, TrapEnum trapType){
    m_position = &position;
    m_dimensions = &dimensions;
    m_body = new BT_GhostObject();
    m_deactivated = false;
    m_trapType = trapType;
    m_texturePath = "../assets/textures/decal.png";
    clase = EENUM_TRAP;

    m_body->CreateGhostBox(m_position, m_dimensions);
    m_body->SetObjectPtr(this);

    g_body = GraphicEngine::getInstance()->addCube2Scene(*m_position, vector3df(0,0,0), vector3df(m_dimensions->X,0.1,m_dimensions->Z));
    g_body->setMaterialTexture(0,m_texturePath);
    g_body->setMaterialFlag(EMF_LIGHTING,false);
     
}

void Trap::Contact(void* punt, EntityEnum tipo){
    if(tipo == EENUM_PLAYER){
        Player* player = (Player*)(punt);
        Activate(player);
    }
}

EntityEnum Trap::GetClase(){
    return clase;

}

void Trap::Activate(Player* player ){
    std::cout<<"Something stepped on me!!!"<<std::endl;
    player->ChangeHP(-1);
    std::cout<<player->GetHP()<<std::endl;

    switch(m_trapType){
        case TENUM_DEATH_CLAWS:
            std::cout<<"HI HOOMAMN IM TENUM_DEATH_CLAWS"<<std::endl;
        break;

        case TENUM_SPIRITS:
            std::cout<<"HI HOOMAMN IM TENUM_SPIRITS"<<std::endl;
        break;

        case TENUM_SILENCE: 
            std::cout<<"HI HOOMAMN IM TENUM_SILENCE"<<std::endl;
        break;

        case TENUM_TAXES:
            std::cout<<"HI HOOMAMN IM TENUM_TAXES"<<std::endl;
        break;
        
        case TENUM_DISTURBANCE: 
            std::cout<<"HI HOOMAMN IM TENUM_DISTURBANCE"<<std::endl;
        break;
    }
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

TrapEnum Trap::GetTrapType(){
    return m_trapType;
}