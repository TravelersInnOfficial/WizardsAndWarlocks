#include "BT_GhostObject.h"
#include <iostream>
#include <btBulletDynamicsCommon.h>
#include "BulletEngine.h"
#include "./../Objects/Trap.h"

BT_GhostObject::BT_GhostObject(){
     m_body = new btGhostObject();
     m_position = new vector3df(0,0,0);
     m_dimensions = new vector3df(0,0,0);
     m_center = new vector3df(0,0,0);
}

BT_GhostObject::~BT_GhostObject(){}

void BT_GhostObject::CreateGhostBox(vector3df position,vector3df rotation, vector3df dimensions, vector3df center){
    m_body = new btGhostObject();
    m_position = new vector3df(position.X, position.Y, position.Z);
    m_dimensions = new vector3df(dimensions.X, dimensions.Y, dimensions.Z);
    m_rotation = new vector3df(rotation.X,rotation.Y, rotation.Z);
    m_center = new vector3df(center.X, center.Y, center.Z);

    const btVector3 m_ShapeInfo(m_dimensions->X, m_dimensions->Y, m_dimensions->Z);
    m_body->setCollisionShape(new btBoxShape(m_ShapeInfo));
    //m_body->setCollisionShape(new btSphereShape(0.5));
    Rotate(rotation);
    
    m_body->setCollisionFlags(m_body->getCollisionFlags() | btGhostObject::CF_NO_CONTACT_RESPONSE);

    BulletEngine::GetInstance()->AddGhostBody(m_body);
}

void BT_GhostObject::Rotate(vector3df rotation){
    //PASAMOS EL ANGULO A RADIANES
    rotation = rotation*M_PI/180;

    btVector3 TPosition(m_position->X,m_position->Y ,m_position->Z);
    btVector3 TCenter(m_center->X, m_center->Y, m_center->Z);

    btTransform tr;
    tr.setIdentity();
    tr.setOrigin(TCenter);

    btTransform tr1;
    btQuaternion quat;
    quat.setEuler(rotation.Y,rotation.X,rotation.Z);
    tr1.setRotation(quat);
    tr1.setOrigin(TPosition);


    tr = tr1*tr;

    //m_body->setWorldTransform(transform);  
    m_body->setWorldTransform(tr);
}

void BT_GhostObject::SetPosition(vector3df position){
    m_position->X = position.X;
    m_position->Y = position.Y;
    m_position->Z = position.Z;

    btVector3 TPosition(m_position->X,m_position->Y ,m_position->Z);

    btTransform transform;
    transform.setOrigin(TPosition);
    m_body->setWorldTransform(transform);
}

void BT_GhostObject::AssignPointer(void* pointer){
    m_body->setUserPointer(pointer);
}

void BT_GhostObject::SetScale(float ammount){
    m_body->getCollisionShape()->setLocalScaling(btVector3(ammount, ammount, ammount));
}

void BT_GhostObject::Erase(){
    BulletEngine::GetInstance()->RemoveGhostObject(m_body);
    //delete m_body->getMotionState();
    delete m_body->getCollisionShape();
    delete m_body;

    delete m_position;
    delete m_rotation;
    delete m_dimensions;
    delete m_center;
}