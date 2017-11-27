#include "Switch.h"

Switch::Switch(){
    open = false;
    clase = EENUM_SWITCH;   
	CreateSwitch();
}

Switch::~Switch(){
    bt_body->Erase();
    m_switchNode->Erase();

    delete bt_body;
    delete m_switchNode;
}

void Switch::CreateSwitch(){
    //IRRLICHT
    GraphicEngine* engine = GraphicEngine::getInstance();

    // Cargamos el cubo
    m_switchNode = engine->addCube2Scene(vector3df(1,1,1));
    m_switchNode->setScale(vector3df(1,1,1));

    // Aplicamos Material unlit y Textura
    if (m_switchNode) {
        m_switchNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
        m_switchNode->setMaterialTexture(0, "./../assets/textures/switch.png");
    }

    //BULLET
	bt_body = new BT_Body();
	bt_body->CreateBox(vector3df(-1,1,-1), vector3df(1*0.5,1*0.5,1*0.5), 0, 2.3);
}

void Switch::SetDoor(Door* d){
    bt_body->AssignPointer(this);   // Solamente activamos el Contact cuando tiene una puerta
    actualDoor = d;
}

void Switch::Update(){
	UpdatePosShape();
}

void Switch::UpdatePosShape(){
	bt_body->Update();
    vector3df* pos = bt_body->GetPosition();
    m_switchNode->setPosition(*pos);
}

void Switch::Contact(void* punt, EntityEnum tipo){
    if(tipo == EENUM_PROJECTILE){
        open = true;
        actualDoor->Interact();
    }
}