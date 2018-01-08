#include "Door.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

Door::Door(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter){
	CreateDoor(TPosition, TScale, TRotation, TCenter);
    min = TRotation.Y;
    max = TRotation.Y + 90;
    increment = -5;
    rotation = TRotation;
    working = false;
    clase = EENUM_DOOR;
}

Door::~Door(){
    bt_body->Erase();
    m_doorNode->Erase();

    delete bt_body;
    delete m_doorNode;
}

void Door::CreateDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter){

    //Dimensions of the box
    TScale.X *= 0.05;
    TScale.Y *= 1;
    TScale.Z *= 0.5;

    TCenter.X *= TScale.X;
    TCenter.Y *= TScale.Y;
    TCenter.Z *= TScale.Z;

    //IRRLICHT
    GraphicEngine* engine = GraphicEngine::getInstance();

    // Cargamos el modelo
    m_doorNode = engine->addObjMeshSceneNode("./../assets/modelos/door.obj");
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
}

void Door::Interact(Player* p){
    Interact();
}

void Door::Interact(){
    if(!working){
        working = true;
        increment = -increment;
    }
}

void Door::WorkDoor(){
    rotation.Y += increment;
    bt_body->Rotate(rotation);

    if(rotation.Y<=min || rotation.Y>=max){
        working = false;
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
    sense->AddSignal(id, true, AI_DOOR, 5.0f, GetKinematic(), AI_SIGHT);
}

Kinematic Door::GetKinematic(){
    Kinematic cKin;
    cKin.position = bt_body->GetPosition();
    cKin.orientation =  vector2df(0,0);
    cKin.velocity = bt_body->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}