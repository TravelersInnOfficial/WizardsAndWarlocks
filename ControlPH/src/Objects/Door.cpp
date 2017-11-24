#include "Door.h"

Door::Door(){
	CreateDoor();
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
	std::cout<<"Open the Door"<<std::endl;
}

void Door::Update(){
	UpdatePosShape();
}

void Door::UpdatePosShape(){
	bt_body->Update();
    vector3df* pos = bt_body->GetPosition();
    m_doorNode->setPosition(*pos);
}