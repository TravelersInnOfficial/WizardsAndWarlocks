#include "Door.h"

Door::Door(){
	CreateDoor();
    min = 0;
    max = 90;
    increment = -1;
    rotation.X = 0;
    rotation.Y = 0;
    rotation.Z = 0;
    working = false;
}

Door::~Door(){
    bt_body->Erase();
    m_doorNode->Erase();

    delete bt_body;
    delete m_doorNode;
}

void Door::CreateDoor(){
    //IRRLICHT
    GraphicEngine* engine = GraphicEngine::getInstance();

    // Cargamos el cubo
    m_doorNode = engine->addCube2Scene(vector3df(1,1,1));
    m_doorNode->setScale(vector3df(1,1,1));

    // Aplicamos Material unlit y Textura
    if (m_doorNode) {
        m_doorNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
        m_doorNode->setMaterialTexture(0, "./../assets/textures/door.jpg");
    }

    //BULLET
	bt_body = new BT_Body();
	bt_body->CreateBox(vector3df(0,0,0), vector3df(1*0.5,1*0.5,1*0.5), 0, 2.3);
}

void Door::Interact(){
    if(!working){
        working = true;
        increment = -increment;
    }
}

void Door::Update(){
    if(working){
       WorkDoor();
    }
	UpdatePosShape();
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
    vector3df* pos = bt_body->GetPosition();
    m_doorNode->setPosition(*pos);
    m_doorNode->setRotation(rotation);
}