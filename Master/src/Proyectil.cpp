#include "Proyectil.h"
#include "ControlProyectil.h"

Proyectil::Proyectil(float dirX, float dirY, float dirZ, float r, float v){
    direction.push_back(dirX);
    direction.push_back(dirY);
    direction.push_back(dirZ);
    NormalizeDir();
    radio = r;
    velocity = v;
    clase = "proyectil";
}

Proyectil::~Proyectil(){
    bt_body->Erase();
    m_proyectilNode->Erase();

    delete bt_body;
    delete m_proyectilNode;
}

void Proyectil::CreateProyectil(){
    //IRRLICHT
    GraphicEngine* engine = GraphicEngine::getInstance();

    // Cargamos el cubo
    m_proyectilNode = engine->addSphere2Scene(vector3df(0,0,0),vector3df(0,0,0),vector3df(1,1,1),radio, -1);
    m_proyectilNode->setScale(vector3df(1,1,1));

    // Aplicamos Material unlit y Textura
    if (m_proyectilNode) {
        m_proyectilNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
        m_proyectilNode->setMaterialTexture(0, "./../assets/textures/wall.bmp");
    }

    //BULLET    
    bt_body = new BT_Body();
    bt_body->CreateBox(vector3df(0,0,0), vector3df(0.7*radio,0.7*radio,0.7*radio), 50, 2.3);
    bt_body->AssignPointer(this);
    bt_body->SetGravity(vector3df(0,0,0));
}

void Proyectil::Update(){
    vector3df vel(direction[0], direction[1], direction[2]);
    vel = vel * velocity;
    bt_body->SetLinearVelocity(vel);
    
    UpdatePosShape();
}

void Proyectil::UpdatePosShape(){
    bt_body->Update();
    vector3df* pos = bt_body->GetPosition();
    m_proyectilNode->setPosition(*pos);
}

void Proyectil::Contact(void* punt, std::string tipo){
    if(tipo.compare("player")==0){
        ControlProyectil* c = ControlProyectil::GetInstance();
        c->AddToDeleteProyecil(this);
    }
}

BT_Body* Proyectil::GetBody(){
    return bt_body;
 }

GBody* Proyectil::GetShape(){
    return m_proyectilNode;
}

void Proyectil::NormalizeDir(){
    float length = sqrt(pow(direction[0], 2) + pow(direction[1], 2) + pow(direction[2],2));
    direction[0] = direction[0]/length;
    direction[1] = direction[1]/length;
    direction[2] = direction[2]/length;
}
