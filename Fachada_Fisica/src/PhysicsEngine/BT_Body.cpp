#include "BT_Body.h"

//=====================================================================================================//
//=======================================\\ PUBLIC FUNCTIONS //========================================//
//=====================================================================================================//

//CONSTRUCTOR
BT_Body::BT_Body(){

	m_posX = 0.f;
	m_posY = 0.f;
	m_posZ = 0.f;
	m_width = 0.f;
	m_height = 0.f;
	m_length = 0.f;

	m_Shape = NULL;
	m_MotionState = NULL;
	m_Mass = 0;
	m_RigidBody = NULL;
}

void BT_Body::CreateBox(float posX, float posY, float posZ, float width, float height, float lenght, float mass){

	//ASSIGN VALUES TO LOCAL VARIABLES
	m_posX = posX;
	m_posY = posY;
	m_posZ = posZ;
	m_width = width;
	m_height = height;
	m_length = lenght;
	m_Mass = mass;

	//CREATES THE SHAPE (A BOX IN THIS CASE)
	const btVector3 m_ShapeInfo(m_width,m_height,m_length);
	m_Shape = new btBoxShape(m_ShapeInfo);

	//CREATE INITIAL MOTION STATE OF THE BOX
    m_MotionState = new btDefaultMotionState(btTransform(btQuaternion(m_posX, m_posY, m_posZ, 1), btVector3(0, 1, 0))); //con el 1 en offset contrarrestamos el -1 del suelo

    //CREATE BOX INFO
    btVector3 m_Inertia(0, 0, 0);
   	m_Shape->calculateLocalInertia(m_Mass, m_Inertia);
   	btRigidBody::btRigidBodyConstructionInfo m_ConstructionInfo(m_Mass, m_MotionState, m_Shape, m_Inertia);

    //ADD PLAYER TO THE WORLD
    m_RigidBody = new btRigidBody(m_ConstructionInfo);
    m_RigidBody->setActivationState(DISABLE_DEACTIVATION); //IMPORTANT: FOR BULLET DO NOT DEACTIVATE MOVEMENT IF STAND STILL
    BulletEngine::GetInstance()->AddRigidBody(m_RigidBody);
}

void BT_Body::Update(){

	m_posX = GetBodyTrans().getOrigin().getX();
	m_posY = GetBodyTrans().getOrigin().getY();
	m_posZ = GetBodyTrans().getOrigin().getZ();

	btVector3 velocity = m_RigidBody->getLinearVelocity();
    btScalar speed = velocity.length();
    if(speed > 20) {
        velocity *= 20/speed;
        m_RigidBody->setLinearVelocity(velocity);
    }
}

void BT_Body::ApplyCentralImpulse(float x, float y ,float z){

	m_RigidBody->applyCentralImpulse(btVector3(x,y,z));
}

void BT_Body::SetPosition(float x, float y, float z){
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}

void BT_Body::SetPosX(float x){
	m_posX = x;
	GetBodyTrans().getOrigin().setX(m_posX);
}

void BT_Body::SetPosY(float y){
	m_posY = y;
	GetBodyTrans().getOrigin().setY(m_posY);
}

void BT_Body::SetPosZ(float z){
	m_posZ = z;
	GetBodyTrans().getOrigin().setX(m_posZ);
}

float BT_Body::GetPosX(){
	m_posX = GetBodyTrans().getOrigin().getX();
	return m_posX;
}

float BT_Body::GetPosY(){
	m_posY = GetBodyTrans().getOrigin().getY();
	return m_posY;
}


float BT_Body::GetPosZ(){
	m_posZ = GetBodyTrans().getOrigin().getZ();
	return m_posZ;
}


void BT_Body::Erase(){

	BulletEngine::GetInstance()->RemoveRigidBody(m_RigidBody);
    delete m_RigidBody->getMotionState();
    delete m_RigidBody;

    delete m_Shape;
}

//DESTRUCTOR
BT_Body::~BT_Body(){

}

//=====================================================================================================//
//=======================================\\ PRIVATE FUNCTIONS //=======================================//
//=====================================================================================================//
btTransform BT_Body::GetBodyTrans(){

	btTransform trans;
    m_RigidBody->getMotionState()->getWorldTransform(trans);
    return trans;
}