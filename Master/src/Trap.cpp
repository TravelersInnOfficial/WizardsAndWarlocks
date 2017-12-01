#include "Trap.h"
#include "./ManagerTrap.h"

Trap::Trap(){
    m_position = new vector3df(0,0,0);
    m_dimensions = new vector3df(0,0,0);
    m_body = new BT_GhostObject();
    m_deactivated = false;
    m_texturePath = "";
    m_trapData.damage = 0;
    m_trapData.effect = "";
    m_trapData.duration = 0;
}

Trap::Trap(vector3df position, vector3df dimensions, TrapEnum trapType){
    m_position = &position;
    m_dimensions = &dimensions;
    m_body = new BT_GhostObject();
    m_deactivated = false;
    m_trapType = trapType;
    InitializeTrapData();
    m_texturePath = "../assets/textures/decal.png";
    clase = EENUM_TRAP;

    m_body->CreateGhostBox(m_position, m_dimensions);
    m_body->SetObjectPtr(this);

    g_body = GraphicEngine::getInstance()->addCube2Scene(*m_position, vector3df(0,0,0), vector3df(m_dimensions->X,0.1,m_dimensions->Z));
    g_body->setMaterialTexture(0,m_texturePath);
    g_body->setMaterialFlag(EMF_LIGHTING,false);
     
}

Trap::~Trap(){
    Erase();
}

void Trap::InitializeTrapData(){

        switch(m_trapType){
        case TENUM_DEATH_CLAWS:
            std::cout<<"IM TENUM_DEATH_CLAWS"<<std::endl;
            SetTrapData(-3,"Inmovilizacion mortal",7);
        break;

        case TENUM_SPIRITS:
            std::cout<<"IM TENUM_SPIRITS"<<std::endl;
            SetTrapData(0,"Distorsion fantasmal",5);
        break;

        case TENUM_SILENCE: 
            std::cout<<"IM TENUM_SILENCE"<<std::endl;
            SetTrapData(0,"Silenciar",5);
        break;

        case TENUM_TAXES:
            std::cout<<"IM TENUM_TAXES"<<std::endl;
            SetTrapData(0,"Te quita la poción que lleves encima",1);
        break;
        
        case TENUM_DISTURBANCE: 
            std::cout<<"IM TENUM_DISTURBANCE"<<std::endl;
            SetTrapData(0,"Locura",5);
        break;

    }
}

void Trap::SetTrapData(float damage, std::string effect, float duration){
    m_trapData.damage = damage;
    m_trapData.effect = effect;
    m_trapData.duration = duration;
}

void Trap::Contact(void* punt, EntityEnum tipo){
    if(tipo == EENUM_PLAYER){
        Player* player = (Player*)(punt);
        Activate(player);
    }
}

void Trap::Activate(Player* player ){
    //std::cout<<"Something stepped on me!!!"<<std::endl;
    //player->ChangeHP(-1);
    //std::cout<<player->GetHP()<<std::endl;

    std::cout<<"Aplico: "<<m_trapData.effect<<" con "<<m_trapData.damage<<" de daño durante "<<m_trapData.duration<<" segundos."<<std::endl;

    player->Respawn();
    ManagerTrap::GetInstance()->DeleteTrap(this);
    
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

void Trap::Erase(){
    m_body->Erase();
    g_body->Erase();
}