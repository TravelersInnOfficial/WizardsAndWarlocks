#include "BT_Body.h"

//=====================================================================================================//
//=======================================\\ PUBLIC FUNCTIONS //========================================//
//=====================================================================================================//

//CONSTRUCTOR
BT_Body::BT_Body(){
	m_position = new vector3df(0,0,0);
	m_dimensions = new vector3df(0,0,0);
	m_center = new vector3df(0,0,0);
	m_Mass = 0;
	m_Friction = 0;

	m_Shape = NULL;
	m_MotionState = NULL;
	m_RigidBody = NULL;
}

void BT_Body::CreateBox(vector3df position, vector3df dimensions, float mass, float friction, vector3df center, CollisionTypes mask, int collideWith){

	//ASSIGN VALUES TO LOCAL VARIABLES
	m_position->X = position.X;
	m_position->Y = position.Y;
	m_position->Z = position.Z;

	m_dimensions->X = dimensions.X; 
	m_dimensions->Y = dimensions.Y; 
	m_dimensions->Z = dimensions.Z;

	m_center->X = center.X;
	m_center->Y = center.Y;
	m_center->Z = center.Z;

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
    BulletEngine::GetInstance()->AddRigidBody(m_RigidBody, mask, collideWith);
}

void BT_Body::ApplyTorque(vector3df force){
	m_RigidBody->setAngularVelocity(btVector3(force.X, force.Y, force.Z));
	//std::cout<<m_RigidBody->getTotalTorque().getZ()<<std::endl;
}

void BT_Body::SetAngularFactor(vector3df v){
	m_RigidBody->setAngularFactor(btVector3(v.X, v.Y, v.Z));
}

void BT_Body::Update(){
	m_position->X = GetBodyTrans().getOrigin().getX();
	m_position->Y  = GetBodyTrans().getOrigin().getY();
	m_position->Z  = GetBodyTrans().getOrigin().getZ();
}

void BT_Body::ApplyCentralImpulse(vector3df force){
	m_RigidBody->applyCentralImpulse(btVector3(force.X,force.Y,force.Z));
}

void BT_Body::ApplyCentralForce(vector3df force){
	m_RigidBody->applyCentralForce(btVector3(force.X, force.Y, force.Z));
}

void BT_Body::Rotate(vector3df rotation){
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

	m_RigidBody->setCenterOfMassTransform(tr);
}

void BT_Body::SetCenter(vector3df center){
	m_center->X = center.X;
	m_center->Y = center.Y;
	m_center->Z = center.Z;
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

void BT_Body::SetGravity(vector3df gravity){
	m_RigidBody->setGravity(btVector3(gravity.X, gravity.Y, gravity.Z));
}

vector3df BT_Body::GetPosition(){
	vector3df pos(m_position->X, m_position->Y, m_position->Z);
	return pos;
}

vector3df* BT_Body::GetDimensions(){
	return m_dimensions;
}

vector3df BT_Body::GetLinearVelocity(){
	vector3df LVel(m_RigidBody->getLinearVelocity().getX(),
					m_RigidBody->getLinearVelocity().getY(), 
					m_RigidBody->getLinearVelocity().getZ());
	return LVel;
}

vector3df BT_Body::GetAngularVelocity(){
	btVector3 temp = m_RigidBody->getAngularVelocity();
	vector3df Angu(temp.getX(), temp.getY(), temp.getZ());
	return Angu;
}

vector3df BT_Body::GetRotation(){

	btQuaternion rot = m_RigidBody->getOrientation();

	float eulerX, eulerY, eulerZ;
 	
	rot = btQuaternion(rot.y(), rot.x(), rot.z(), rot.w());


 	double sqw = rot.w() * rot.w();

 	double sqx = rot.x() * rot.x();
 	double sqy = rot.y() * rot.y();
 	double sqz = rot.z() * rot.z();

 	eulerY = (float)atan2l((2.0 * (rot.y()*rot.z() + rot.x()*rot.w())), (-sqx - sqy + sqz + sqw));
 	eulerX = (float)asinl(-2.0 * (rot.x()*rot.z() - rot.y()*rot.w() ));
 	eulerZ = (float)atan2l((2.0 * (rot.x()*rot.y() + rot.z()*rot.w())), (sqx - sqy - sqz + sqw));

	return vector3df(eulerX, eulerY, eulerZ);

}

void BT_Body::AssignPointer(void* pointer){
	m_RigidBody->setUserPointer(pointer);
}

void BT_Body::SetCollisionFlags(std::string s){
	if(s.compare("no_contact")==0){
		m_RigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

void BT_Body::Erase(){
	BulletEngine::GetInstance()->RemoveRigidBody(m_RigidBody);
    delete m_RigidBody->getMotionState();
    delete m_RigidBody;

    delete m_Shape;

    delete m_position;
	delete m_dimensions; 
	delete m_center;
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