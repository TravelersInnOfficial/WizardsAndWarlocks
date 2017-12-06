#include "Trap.h"
#include "./Managers/TrapManager.h"
#include "Game.h"

Trap::Trap(){
   // m_trapType = 0;

    m_position = new vector3df(0,0,0);
    m_dimensions = new vector3df(0,0,0);

    m_body = new BT_GhostObject();
    m_rigidBody = new BT_Body();

    m_texturePath = "";
    m_effect = "";

    m_current_time = 0;
    m_deactivation_time = 5;
    m_world_time = 0;
}

Trap::Trap(vector3df position, TrapEnum trapType){
    clase = EENUM_TRAP;
    m_position = &position;
    m_trapType = trapType;
    InitializeTrapData();
 
    m_body = new BT_GhostObject();
    m_rigidBody = new BT_Body();
    
    m_current_time = 0;
    m_deactivation_time = 3;
    m_world_time = 0;

    vector3df aux_dimensions(m_dimensions->X*0.5,m_dimensions->Y*0.5,m_dimensions->Z*0.5);
    m_body->CreateGhostBox(m_position, &aux_dimensions);
    m_body->SetObjectPtr(this);

    std::cout<<"m_dimensionsX: "<<m_dimensions->X<<" m_dimensionsY: "<<m_dimensions->Y<<" m_dimensionsZ: "<<m_dimensions->Z<<std::endl;
    m_rigidBody->CreateBox(*m_position, (*m_dimensions)*0.5,0,0);
    m_rigidBody->AssignPointer(this);

    std::cout<<"m_dimensionsX: "<<m_dimensions->X<<" m_dimensionsY: "<<m_dimensions->Y<<" m_dimensionsZ: "<<m_dimensions->Z<<std::endl;
    g_body = GraphicEngine::getInstance()->addCube2Scene(*m_position, vector3df(0,0,0), vector3df(m_dimensions->X,m_dimensions->Y,m_dimensions->Z));
    g_body->setMaterialTexture(0,m_texturePath);
    g_body->setMaterialFlag(EMF_LIGHTING,false);

}

Trap::~Trap(){
    Erase();
}

void Trap::InitializeTrapData(){

        switch(m_trapType){
        case TENUM_DEATH_CLAWS:
            //std::cout<<"IM TENUM_DEATH_CLAWS"<<std::endl;
            SetTrapData(vector3df(0.5,0.5,0.5),"","Inmovilizacion mortal");
        break;

        case TENUM_SPIRITS:
            //std::cout<<"IM TENUM_SPIRITS"<<std::endl;
            SetTrapData(vector3df(0.5,0.5,0.5),"","Distorsion fantasmal");
        break;

        case TENUM_SILENCE: 
            //std::cout<<"IM TENUM_SILENCE"<<std::endl;
            SetTrapData(vector3df(0.5,0.5,0.5),"","Silenciar");
        break;

        case TENUM_TAXES:
            //std::cout<<"IM TENUM_TAXES"<<std::endl;
            SetTrapData(vector3df(0.5,0.5,0.5),"","Te quita la poción que lleves encima");
        break;
        
        case TENUM_DISTURBANCE: 
            //std::cout<<"IM TENUM_DISTURBANCE"<<std::endl;
            SetTrapData(vector3df(0.5,0.5,0.5),"","Locura");
        break;

    }
}

void Trap::SetTrapData(vector3df dimensions, std::string texturePath, std::string effect){
        m_dimensions = new vector3df(0.5,0,0.5);
        m_texturePath = "../assets/textures/decal.png";
        m_effect = effect;
}

void Trap::Contact(void* punt, EntityEnum tipo){
    std::cout<<tipo<<std::endl;

    if(tipo == EENUM_PLAYER){
        Player* player = (Player*)(punt);
        Activate(player);
    }
    if(tipo == EENUM_PROJECTILE){
        TrapManager::GetInstance()->DeleteTrap(this);
    }
}

void Trap::Interact(Player* p){
    if(m_world_time - Game::GetInstance()->GetTotalTime()*0.001 < -0.1) m_current_time = 0;
    //std::cout<<m_world_time - Game::GetInstance()->GetTotalTime()*0.001<<std::endl;
    Deactivate();
}

void Trap::Activate(Player* player ){

    std::cout<<"Aplico: "<<m_effect<<std::endl;

    player->Respawn();
    TrapManager::GetInstance()->DeleteTrap(this);
    
}

void Trap::Deactivate(){
    std::cout<<"INNER TIME: "<<m_world_time<<std::endl;
    m_world_time = Game::GetInstance()->GetTotalTime()*0.001;
    std::cout<<"TOTAL TIME: "<<Game::GetInstance()->GetTotalTime()*0.001<<std::endl;
    
    std::cout<<"DELTA TIME: "<<Game::GetInstance()->GetDeltaTime()<<std::endl;
	m_current_time += Game::GetInstance()->GetDeltaTime();
    std::cout<<"CURRENT TIME: "<<m_current_time<<std::endl;

	if(m_current_time>=m_deactivation_time){
		TrapManager::GetInstance()->DeleteTrap(this);
		m_current_time=0.0f;
	}
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
    m_rigidBody->Erase();
    m_body->Erase();
    g_body->Erase();
}