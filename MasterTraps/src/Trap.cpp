#include "Trap.h"
#include "./ControlTrap.h"
#include "Game.h"

Trap::Trap(){
    m_position = new vector3df(0,0,0);
    m_dimensions = new vector3df(0,0,0);
    m_body = new BT_GhostObject();
    m_deactivated = false;
    m_texturePath = "";
    m_trapData.damage = 0;
    m_trapData.effect = "";
    m_trapData.duration = 0;
    m_current_time = 0;
    m_deactivation_time = 5;
    m_world_time = 0;
}

Trap::Trap(vector3df position, vector3df dimensions, TrapEnum trapType){
    m_position = &position;
    m_dimensions = &dimensions;
    m_body = new BT_GhostObject();
    m_deactivated = false;
    m_trapType = trapType;
    m_current_time = 0;
    m_deactivation_time = 5;
    m_world_time = 0;
    InitializeTrapData();
    m_texturePath = "../assets/textures/decal.png";
    clase = EENUM_TRAP;

    m_body->CreateGhostBox(m_position, m_dimensions);
    m_body->SetObjectPtr(this);

    m_rigidBody = new BT_Body();
    m_rigidBody->CreateBox(*m_position, *m_dimensions*0.5,0,0);
    m_rigidBody->AssignPointer(this);

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
            SetTrapData(-3,"Inmovilizacion mortal",7);
        break;

        case TENUM_SPIRITS:
            //std::cout<<"IM TENUM_SPIRITS"<<std::endl;
            SetTrapData(0,"Distorsion fantasmal",5);
        break;

        case TENUM_SILENCE: 
            //std::cout<<"IM TENUM_SILENCE"<<std::endl;
            SetTrapData(0,"Silenciar",5);
        break;

        case TENUM_TAXES:
            //std::cout<<"IM TENUM_TAXES"<<std::endl;
            SetTrapData(0,"Te quita la poción que lleves encima",1);
        break;
        
        case TENUM_DISTURBANCE: 
            //std::cout<<"IM TENUM_DISTURBANCE"<<std::endl;
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
    if(tipo == EENUM_PROJECTILE){
        ControlTrap::GetInstance()->DeleteTrap(this);
    }
}

void Trap::Interact(Player* p){
    Deactivate();
}

void Trap::Activate(Player* player ){

    std::cout<<"Aplico: "<<m_trapData.effect<<" con "<<m_trapData.damage<<" de daño durante "<<m_trapData.duration<<" segundos."<<std::endl;

    player->Respawn();
    ControlTrap::GetInstance()->DeleteTrap(this);
    
}

void Trap::Deactivate(){
     std::cout<<"INNER TIME: "<<m_world_time<<std::endl;
    float TotalTime = Game::GetInstance()->GetTotalTime();
    std::cout<<"TOTAL TIME: "<<TotalTime<<std::endl;
    
    float deltaTime = Game::GetInstance()->GetDeltaTime();

    std::cout<<"DELTA TIME: "<<deltaTime<<std::endl;
	m_current_time += deltaTime;

    std::cout<<"CURRENT TIME: "<<m_current_time<<std::endl;

	if(m_current_time>=m_deactivation_time){
		ControlTrap::GetInstance()->DeleteTrap(this);
		m_current_time=0.0f;
	}
    //m_deactivated = true;
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