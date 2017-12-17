#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#pragma clang diagnostic pop

#include "Trap.h"
#include "./../Managers/TrapManager.h"

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

Trap::Trap(vector3df position, vector3df normal, TrapEnum trapType){
    clase = EENUM_TRAP;
    m_position = &position;
    m_rotation = new vector3df(normal.X,normal.Y,normal.Z);
    m_trapType = trapType;
    InitializeTrapData();

    m_position->Y +=0.01; 

    if(m_rotation->X >= 270) m_rotation->X = m_rotation->X - 270;
    if(m_rotation->Y >= 270) m_rotation->Y = m_rotation->Y - 270;
    if(m_rotation->Z >= 270) m_rotation->Z = m_rotation->Z - 270;
    //m_rotation->X = 360 - m_rotation->X;

    m_body = new BT_GhostObject();
    m_rigidBody = new BT_Body();
    
    m_current_time = 0;
    m_deactivation_time = 3;
    m_world_time = 0;

    m_rigidBody->CreateBox(*m_position,(*m_dimensions)*0.5,0,0);
    m_rigidBody->AssignPointer(this);
    m_rigidBody->Rotate(*m_rotation);

    g_body = GraphicEngine::getInstance()->addCube2Scene(*m_position, *m_rotation, vector3df(m_dimensions->X,m_dimensions->Y,m_dimensions->Z));
    g_body->setMaterialTexture(0,m_texturePath);
    g_body->setMaterialFlag(EMF_LIGHTING,false);

    vector3df aux_dimensions(m_dimensions->X*0.5,m_dimensions->Y*0.5,m_dimensions->Z*0.5);
    m_body->CreateGhostBox(m_position,m_rotation,&aux_dimensions);
    m_body->SetObjectPtr(this);
}

Trap::~Trap(){
    Erase();
}

void Trap::Update(float deltaTime){
    this->deltaTime = deltaTime;
}

void Trap::Update(){
	std::cout<<"NO USAR ESTE UPDATE"<<std::endl;
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

        default:
        break;

    }
}

void Trap::SetTrapData(vector3df dimensions, std::string texturePath, std::string effect){
        m_dimensions = new vector3df(0.5,0,0.5);
        m_texturePath = "../assets/textures/decal.png";
        m_effect = effect;
}

void Trap::Contact(void* punt, EntityEnum tipo){
    //std::cout<<tipo<<std::endl;

    if(tipo == EENUM_PLAYER){
        Player* player = (Player*)(punt);
        Activate(player);
    }
    if(tipo == EENUM_PROJECTILE){
        TrapManager::GetInstance()->DeleteTrap(this);
    }
}

void Trap::Interact(Player* p){
    if(m_world_time - deltaTime*0.001 < -0.1) m_current_time = 0;
    Deactivate(deltaTime);
}

void Trap::Activate(Player* player ){
    player->ChangeHP(-50);
    TrapManager::GetInstance()->DeleteTrap(this);    
}

void Trap::Deactivate(float deltaTime){
    m_world_time = deltaTime*0.001;
	m_current_time += deltaTime;
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