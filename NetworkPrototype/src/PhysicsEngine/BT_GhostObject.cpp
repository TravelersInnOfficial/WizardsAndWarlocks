#include "BT_GhostObject.h"
#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>
#include "BulletEngine.h"
#include "../Trap.h"

BT_GhostObject::BT_GhostObject(){
     m_body = new btGhostObject();
     m_position = new vector3df(0,0,0);
     m_dimensions = new vector3df(0,0,0);
}

void BT_GhostObject::CreateGhostBox(vector3df* position, vector3df* dimensions){
    m_position = position;
    m_dimensions = dimensions;
    std::cout<<m_position->X<<position->X<<m_position->Y<<position->Y<<m_position->Z<<position->Z<<std::endl;

    m_body->setCollisionShape(new btBoxShape(btVector3(m_dimensions->X, m_dimensions->Y,m_dimensions->Z)));
    btTransform transform;
	//transform.setIdentity();
    transform.setOrigin(btVector3(m_position->X,m_position->Y,m_position->Z));
    m_body->setWorldTransform(transform);  
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