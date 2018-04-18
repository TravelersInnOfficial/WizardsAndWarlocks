#include "Door.h"
#include "./../Managers/ObjectManager.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

#include <GraphicEngine/GPortal.h>

Door::Door(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter, std::string model3d){
	TRotation.Y += 180;
    CreateDoor(TPosition, TScale, TRotation, TCenter, model3d);

    min = TRotation.Y;
    max = TRotation.Y + 90;
    increment = -5;
    rotation = TRotation;
    working = false;
    isOpen  = false;
    clase = EENUM_DOOR;
    block = false;
}

Door::~Door(){
    std::map<std::string, SoundEvent*>::iterator itSe = soundEvents.begin();
    for(; itSe!=soundEvents.end(); itSe++){
        SoundEvent* even = itSe->second;
        even->release();
        delete even;
    }
    soundEvents.clear();

    delete bt_body;
    delete m_doorNode;
}

void Door::CreateDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter, std::string model3d){

    //Dimensions of the box
    TScale.X *= 0.05;
    TScale.Y *= 1;
    TScale.Z *= 0.5;

    TCenter.X *= TScale.X;
    TCenter.Y *= TScale.Y;
    TCenter.Z *= TScale.Z;

    // Graphic Engine
    GraphicEngine* g_engine = GraphicEngine::getInstance();

    // Cargamos el modelo
    m_doorNode = g_engine->addObjMeshSceneNode("./../assets/modelos/door2.obj");
    m_doorNode->setPosition(TPosition);
    m_doorNode->setRotation(TRotation);
    m_doorNode->setScale(vector3df(1,1,1));

    // Aplicamos Material unlit y Textura
    if (m_doorNode) {
        m_doorNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
        m_doorNode->setMaterialTexture(0, "./../assets/textures/door.jpg");
    }

    //BULLET
    vector3df HalfExtents(TScale.X, TScale.Y, TScale.Z);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents, 0, 0, TCenter, C_DOOR, doorCW);
    bt_body->Rotate(TRotation);
    bt_body->AssignPointer(this);

    //Sound
    createSoundEvents();
}

void Door::Interact(Player* p){
    if(!block) Interact();
}

void Door::Interact(){
    NetworkEngine* n_engine = NetworkEngine::GetInstance();
	
    if(!n_engine->IsClientInit()){
        if(!working){
            if (n_engine->IsServerInit()){
                Server* server = n_engine->GetServer();
                if(server != nullptr){
                    int pos = ObjectManager::GetInstance()->GetDoorVecPos(this);
                    server->NotifyDoorInteracted(pos);
                }
            }
            
            working = true;
            increment = -increment;
            if (isOpen) playClose();
            else {
                playOpen();
                SetVisible(true);
            }
        }
    }
}

void Door::WorkDoor(){
    rotation.Y += increment;
    bt_body->Rotate(rotation);

    if(rotation.Y <= min || rotation.Y >= max){ 
        working = false;
        // Comprobamos si se ha cerrado por completo
        if(rotation.Y <= min){
            SetVisible(false);
        }
    }
}

void Door::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_doorNode->setPosition(pos);
    m_doorNode->setRotation(rotation);
}

void Door::Update(){
    if(working){
       WorkDoor();
    }
	UpdatePosShape();
}

void Door::SendSignal(){
    RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
    // id, AI_code name, float str, Kinematic kin, AI_modalities mod
    sense->AddSignal(id, this, true, AI_DOOR, 5.0f, GetKinematic(), AI_SIGHT);
}

void Door::SetBlock(bool bl){
    block = bl;
    m_doorNode->setMaterialTexture(0, "./../assets/textures/doorblocked.jpg");
}

Kinematic Door::GetKinematic(){
    Kinematic cKin;
    cKin.position = bt_body->GetPosition();
    cKin.orientation =  vector2df(0,0);
    cKin.velocity = bt_body->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/

void Door::createSoundEvents() {
    //Create the events
    SoundEvent* close = SoundSystem::getInstance()->createEvent("event:/CommonSounds/Doors/Close");
    SoundEvent* open  = SoundSystem::getInstance()->createEvent("event:/CommonSounds/Doors/Open");
    
    //Store them at the player's sounds map
    soundEvents["close"] = close;
    soundEvents["open"]  = open;
}

void Door::playClose() {
    SoundSystem::getInstance()->checkAndPlayEvent(soundEvents["close"], bt_body->GetPosition());
    isOpen = false;
}

void Door::playOpen() {
    SoundSystem::getInstance()->checkAndPlayEvent(soundEvents["open"], bt_body->GetPosition());
    isOpen = true;
}

void Door::NetInteract(){
    NetworkEngine* n_engine = NetworkEngine::GetInstance();
	if(n_engine->IsClientInit()){
        if(!working){
            working = true;
            increment = -increment;
            if (isOpen) playClose();
            else{ 
                playOpen();
                SetVisible(true);
            }
        }
    }
}

void Door::ForceOpen(){
    NetworkEngine* n_engine = NetworkEngine::GetInstance();
	if(true || n_engine->IsClientInit()){
        increment *= -1;
        rotation.Y = max;
        bt_body->Rotate(rotation);
        isOpen = true;
    }
}

bool Door::GetOpenState(){
    return(isOpen);
}

void Door::AddPortal(GPortal* portal){
    portal->SetVisible(isOpen);
    m_portals.push_back(portal);
}

void Door::SetVisible(bool visibility){
    int size = m_portals.size();
    for(int i=0; i<size; i++){
        GPortal* portal = m_portals[i];
        portal->SetVisible(visibility);
    }
}
