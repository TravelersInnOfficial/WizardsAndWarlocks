#include "Trap.h"
#include "./../Managers/TrapManager.h"
#include "./../Managers/EffectManager.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../NetworkEngine/NetworkEngine.h"

Trap::Trap(vector3df TPosition, vector3df normal, TrapEnum trapType){
    clase = EENUM_TRAP;
    m_position = new vector3df(TPosition.X, TPosition.Y, TPosition.Z);
    m_rotation = new vector3df(normal.X,normal.Y,normal.Z);
    m_trapType = trapType;
    InitializeTrapData();

    m_position->Y +=0.01; 

    if(m_rotation->X >= 270) m_rotation->X = m_rotation->X - 270;
    if(m_rotation->Y >= 270) m_rotation->Y = m_rotation->Y - 270;
    if(m_rotation->Z >= 270) m_rotation->Z = m_rotation->Z - 270;

    m_body = new BT_GhostObject();
    m_rigidBody = new BT_Body();
    
    m_current_time = 0;
    m_deactivation_time = 3;
    m_world_time = 0;

    m_rigidBody->CreateBox(TPosition,(*m_dimensions)*0.5,0,0);
    m_rigidBody->AssignPointer(this);
    m_rigidBody->Rotate(*m_rotation);

    g_body = GraphicEngine::getInstance()->addCube2Scene(TPosition, *m_rotation, vector3df(m_dimensions->X,m_dimensions->Y,m_dimensions->Z));
    g_body->setMaterialTexture(0,m_texturePath);
    g_body->setMaterialFlag(EMF_LIGHTING,false);

    vector3df aux_dimensions(m_dimensions->X*0.5,m_dimensions->Y*0.5+0.25,m_dimensions->Z*0.5);
    m_body->CreateGhostBox(*m_position, *m_rotation, aux_dimensions, vector3df(0,aux_dimensions.Y, 0));
    m_body->AssignPointer(this);
}

void Trap::SetTrapData(vector3df dimensions, std::string texturePath, std::string effect){
    m_dimensions = new vector3df(dimensions.X, dimensions.Y, dimensions.Z);
    m_texturePath = "../assets/textures/decal.png";
    m_effect = effect;
    trapId = -1;
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
            SetTrapData(vector3df(0.5,0.001,0.5),"","Inmovilizacion mortal");
        break;

        case TENUM_SPIRITS:
            SetTrapData(vector3df(0.5,0.001,0.5),"","Distorsion fantasmal");
        break;

        case TENUM_SILENCE:
            SetTrapData(vector3df(0.5,0.001,0.5),"","Silenciar");
        break;

        case TENUM_TAXES:
            SetTrapData(vector3df(0.5,0.001,0.5),"","Te quita la poción que lleves encima");
        break;
        
        case TENUM_DISTURBANCE: 
            SetTrapData(vector3df(0.5,0.001,0.5),"","Locura");
        break;

        case TENUM_EXPLOSIVE: 
            SetTrapData(vector3df(0.5,0.001,0.5),"","Explosion");
        break;

        default:
            SetTrapData(vector3df(0.5,0.001,0.5),"","DEFAULT TRAP");
        break;
    }
}

void Trap::Contact(void* punt, EntityEnum tipo){
    NetworkEngine* n_engine = NetworkEngine::GetInstance();
	if(!n_engine->IsClientInit()){

        bool contacted = false;

        if(tipo == EENUM_PLAYER){
            Player* player = (Player*)(punt);
            Activate(player);
            contacted = true;
        }
        else if(tipo == EENUM_PROJECTILE){
            TrapManager::GetInstance()->DeleteTrap(this);
            contacted = true;
        }

        if(contacted && n_engine->IsServerInit()){
            Server* myServer = n_engine->GetServer();
            if(myServer != NULL) myServer->EraseTrap(trapId);
        }

    }    
}

void Trap::Interact(Player* p){
    NetworkEngine* n_engine = NetworkEngine::GetInstance();
	if(!n_engine->IsClientInit()){
        if(m_world_time - deltaTime*0.001 < -0.1) m_current_time = 0;
        Deactivate(deltaTime);
        if(n_engine->IsServerInit()){
            Server* myServer = n_engine->GetServer();
            if(myServer != NULL) myServer->EraseTrap(trapId);
        }
    }
}

void Trap::Activate(Player* player){

    switch(m_trapType){
        case TENUM_DEATH_CLAWS:
            EffectManager::GetInstance()->AddEffect(player, WEAK_PARALYZED);
            player->ChangeHP(-10);
        break;

        case TENUM_SPIRITS:
            EffectManager::GetInstance()->AddEffect(player, WEAK_MADNESS);
            player->ChangeHP(-10);
        break;

        case TENUM_SILENCE:
            EffectManager::GetInstance()->AddEffect(player, WEAK_SILENCED);
            player->ChangeHP(-10);
        break;

        case TENUM_TAXES:
            player->LosePotion();
            player->ChangeHP(-10);
        break;
        
        case TENUM_DISTURBANCE:
            player->ApplyFuzyEffect();
            player->ChangeHP(-10);
        break;

        case TENUM_EXPLOSIVE:
            player->ChangeHP(-50);
        break;

        default:
        break;
    }

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
    m_position->X = position.X;
    m_position->Y = position.Y;
    m_position->Z = position.Z;
}

void Trap::SetDimensions(vector3df dimensions){
    m_dimensions->X = dimensions.X;
    m_dimensions->Y = dimensions.Y;
    m_dimensions->Z = dimensions.Z;
}

void Trap::SetType(TrapEnum trapType){
    m_trapType = trapType;
}

vector3df Trap::GetPosition(){
    vector3df pos(m_position->X, m_position->Y, m_position->Z);
    return pos;
}

vector3df Trap::GetDimensions(){
    vector3df dim(m_dimensions->X, m_dimensions->Y, m_dimensions->Z);
    return dim;
}

TrapEnum Trap::GetTrapType(){
    return m_trapType;
}

void Trap::Erase(){
    m_rigidBody->Erase();
    m_body->Erase();
    g_body->Erase();

    delete m_rigidBody;
    delete m_body;
    delete g_body;

    delete m_position;
    delete m_rotation;
    delete m_dimensions;
}

void Trap::SendSignal(){
    RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
    
    // id, AI_code name, float str, Kinematic kin, AI_modalities mod
    sense->AddSignal(id, this, true, AI_TRAP, 5.0f, GetKinematic(), AI_SIGHT);
}

Kinematic Trap::GetKinematic(){
    Kinematic cKin;
    cKin.position = m_rigidBody->GetPosition();
    cKin.orientation =  vector2df(0,0);
    cKin.velocity = m_rigidBody->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}

void Trap::SetTrapId(int id){
    this->trapId = id;
}

int Trap::GetTrapId(){
    return(trapId);
}