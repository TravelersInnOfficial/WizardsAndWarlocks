#include "BT_GhostObject.h"
#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>
#include "BulletEngine.h"
#include "./../Objects/Trap.h"

BT_GhostObject::BT_GhostObject(){
     m_body = new btGhostObject();
     m_position = new vector3df(0,0,0);
     m_dimensions = new vector3df(0,0,0);
}

void BT_GhostObject::CreateGhostBox(vector3df* position,vector3df* rotation, vector3df* dimensions){
    m_body = new btGhostObject();
    m_position = position;
    m_dimensions = dimensions;
    m_rotation = new vector3df(rotation->X*M_PI/180,rotation->Y*M_PI/180,rotation->Z*M_PI/180);


    m_body->setCollisionShape(new btBoxShape(btVector3(m_dimensions->X, m_dimensions->Y, m_dimensions->Z)));
    //m_body->setCollisionShape(new btSphereShape(0.5));
    btTransform transform;
    transform.setIdentity();
    btQuaternion quat;
	quat.setEuler(m_rotation->Y,m_rotation->X,m_rotation->Z);
	transform.setRotation(quat);
    transform.setOrigin(btVector3(m_position->X,m_position->Y,m_position->Z));
    

    m_body->setWorldTransform(transform);  
    m_body->setCollisionFlags(m_body->getCollisionFlags() | btGhostObject::CF_NO_CONTACT_RESPONSE);

    BulletEngine::GetInstance()->AddGhostBody(m_body);
}

void BT_GhostObject::SetObjectPtr(void* pointer){
    m_body->setUserPointer(pointer);
}

void BT_GhostObject::Erase(){

    BulletEngine::GetInstance()->RemoveGhostObject(m_body);
    //delete m_body->getMotionState();
    delete m_body->getCollisionShape();

    delete m_body;
}

BT_GhostObject::~BT_GhostObject(){}