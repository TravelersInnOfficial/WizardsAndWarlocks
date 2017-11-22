#include "BT_Body.h"

//=====================================================================================================//
//=======================================\\ PUBLIC FUNCTIONS //========================================//
//=====================================================================================================//

//CONSTRUCTOR
BT_Body::BT_Body(){
	m_position = new vector3df(0,0,0);
	m_dimensions = new vector3df(0,0,0);
	m_Mass = 0;
	m_Friction = 0;

	m_Shape = NULL;
	m_MotionState = NULL;
	m_RigidBody = NULL;
}

void BT_Body::CreateBox(vector3df position, vector3df dimensions, float mass, float friction){

	//ASSIGN VALUES TO LOCAL VARIABLES
	m_position->X = position.X;
	m_position->Y = position.Y;
	m_position->Z = position.Z;

	m_dimensions->X = dimensions.X; 
	m_dimensions->Y = dimensions.Y; 
	m_dimensions->Z = dimensions.Z; 

	m_Mass = mass;
	m_Friction = friction;

	//CREATES THE SHAPE (A BOX IN THIS CASE)
	const btVector3 m_ShapeInfo(m_dimensions->X, m_dimensions->Y, m_dimensions->Z);
	m_Shape = new btBoxShape(m_ShapeInfo);

	//CREATE INITIAL MOTION STATE OF THE BOX
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btVector3(m_position->X, m_position->Y, m_position->Z));
	
	// Give it a default MotionState
	m_MotionState = new btDefaultMotionState(Transform);

    //CREATE BOX INFO
    btVector3 m_Inertia(0, 0, 0);
   	m_Shape->calculateLocalInertia(m_Mass, m_Inertia);
   	btRigidBody::btRigidBodyConstructionInfo m_ConstructionInfo(m_Mass, m_MotionState, m_Shape, m_Inertia);
	m_ConstructionInfo.m_friction = m_Friction;

    //ADD PLAYER TO THE WORLD
    m_RigidBody = new btRigidBody(m_ConstructionInfo);
    m_RigidBody->setActivationState(DISABLE_DEACTIVATION); //IMPORTANT: FOR BULLET DO NOT DEACTIVATE MOVEMENT IF STAND STILL
	m_RigidBody->setAngularFactor(btVector3(0,0,0));
    BulletEngine::GetInstance()->AddRigidBody(m_RigidBody);
}

void BT_Body::Update(){
	m_position->X = GetBodyTrans().getOrigin().getX();
	m_position->Y  = GetBodyTrans().getOrigin().getY();
	m_position->Z  = GetBodyTrans().getOrigin().getZ();
}

void BT_Body::ApplyCentralImpulse(float x, float y, float z){
	m_RigidBody->applyCentralImpulse(btVector3(x,y,z));
}

void BT_Body::Rotate(vector3df rotation){
	//PASAMOS EL ANGULO A RADIANES
	float RADX = rotation.X*M_PI/180;
	float RADY = rotation.Y*M_PI/180;
	float RADZ = rotation.Z*M_PI/180;

	btVector3 TPosition(m_position->X,m_position->Y,m_position->Z);

	btTransform tr;
	tr.setIdentity();
	btQuaternion quat;
	quat.setEuler(RADX,RADY,RADZ);
	tr.setRotation(quat);
	tr.setOrigin(TPosition);
	m_RigidBody->setCenterOfMassTransform(tr);
}

void BT_Body::SetPosition(vector3df position){
	m_position->X = position.X;
	m_position->Y = position.Y;
	m_position->Z = position.Z;

	btTransform transform;
    m_RigidBody->getMotionState()->getWorldTransform(transform);
    transform.setOrigin(btVector3(m_position->X, m_position->Y, m_position->Z));
    m_RigidBody->getMotionState()->setWorldTransform(transform);
    m_RigidBody->setCenterOfMassTransform(transform);
	m_RigidBody->setLinearVelocity(btVector3(0,0,0));
}

void BT_Body::SetDimensions(vector3df dimensions){
	m_dimensions->X = dimensions.X;
	m_dimensions->Y = dimensions.Y;
	m_dimensions->Z = dimensions.Z;

}

void BT_Body::SetLinearVelocity(vector3df velocity){
	m_RigidBody->setLinearVelocity(btVector3(velocity.X, velocity.Y, velocity.Z));
}

void BT_Body::SetMass(float mass){
	m_Mass = mass;
}

vector3df* BT_Body::GetPosition(){
	return m_position;
}

vector3df* BT_Body::GetDimensions(){
	return m_dimensions;
}

vector3df* BT_Body::GetLinearVelocity(){
	vector3df* LVel = new vector3df(m_RigidBody->getLinearVelocity().getX(),
									m_RigidBody->getLinearVelocity().getY(), 
									m_RigidBody->getLinearVelocity().getZ());
	return LVel;
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