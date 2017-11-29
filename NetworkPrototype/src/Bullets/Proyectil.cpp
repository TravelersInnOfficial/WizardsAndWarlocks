#include "Proyectil.h"
#include "./../ControlProyectil.h"
#include "./../Player.h"

Proyectil::Proyectil(vector3df pos,vector3df dir, float r, float v){
    direction = new vector3df(dir.X, dir.Y, dir.Z);
    //NormalizeDir();
    radio = r;
    velocity = v;

    maxDistance = 10.0f;
    initPos = pos;

    CreateProyectil(pos);
    clase = EENUM_PROJECTILE;
}

Proyectil::~Proyectil(){
    delete direction;

    bt_body->Erase();
    m_proyectilNode->Erase();

    delete bt_body;
    delete m_proyectilNode;
}

void Proyectil::CreateProyectil(vector3df pos){
    //IRRLICHT
    GraphicEngine* engine = GraphicEngine::getInstance();

    // Cargamos el cubo
    m_proyectilNode = engine->addSphere2Scene(pos,vector3df(0,0,0),vector3df(1,1,1),radio, -1);
    m_proyectilNode->setScale(vector3df(1,1,1));

    // Aplicamos Material unlit y Textura
    if (m_proyectilNode) {
        m_proyectilNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
        m_proyectilNode->setMaterialTexture(0, "./../assets/textures/wall.bmp");
    }

    //BULLET    
    bt_body = new BT_Body();
    bt_body->CreateBox(pos, vector3df(0.7*radio,0.7*radio,0.7*radio), 50, 2.3);
    bt_body->AssignPointer(this);
    bt_body->SetGravity(vector3df(0,0,0));
    bt_body->SetCollisionFlags("no_contact");
    //bt_body->SetMass(0);    
}

void Proyectil::Update(){
    vector3df vel(velocity*direction->X, velocity*direction->Y, velocity*direction->Z);
    bt_body->SetLinearVelocity(vel);
    UpdatePosShape();

    //Comprobamos si ha llegado a la distancia maxima
    vector3df pos = bt_body->GetPosition();
    vector3df vectorDistance;
    vectorDistance.X = pos.X - initPos.X;
    vectorDistance.Y = pos.Y - initPos.Y;
    vectorDistance.Z = pos.Z - initPos.Z;

    float currentDistance = sqrt(pow(vectorDistance.X,2)+pow(vectorDistance.Y,2)+pow(vectorDistance.Z,2));

    if(currentDistance >= maxDistance){
        ControlProyectil* c = ControlProyectil::GetInstance();
        c->AddToDeleteProyecil(this);
    }
}

void Proyectil::UpdatePosShape(){
    bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_proyectilNode->setPosition(pos);
}

void Proyectil::Contact(void* punt, EntityEnum tipo){
    if(tipo==EENUM_PLAYER){
        Player* p = (Player*)punt;
        p->ChangeHP(-25.0f);
    }
    ControlProyectil* c = ControlProyectil::GetInstance();
    c->AddToDeleteProyecil(this);
}

BT_Body* Proyectil::GetBody(){
    return bt_body;
 }

GBody* Proyectil::GetShape(){
    return m_proyectilNode;
}

void Proyectil::NormalizeDir(){
    float length = sqrt(pow(direction->X, 2) + pow(direction->Y, 2) + pow(direction->Z,2));
    direction->X = direction->X/length;
    direction->Y = direction->Y/length;
    direction->Z = direction->Z/length;
}
