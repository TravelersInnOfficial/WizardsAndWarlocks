#include "Camera.h"

Camera::Camera(){}

Camera::~Camera(){
	if(p_Camera!=NULL){
		p_Camera->Erase();
		delete p_Camera;
		p_Camera = NULL;
	}
}

void Camera::Contact(void* punt, EntityEnum tipo){}

void Camera::SetPosition(vector3df position){
	p_Camera->setPosition(position);
}

vector3df Camera::GetPosition(){
	return p_Camera->getPosition();
}

void Camera::SetTarget(vector3df lookat){
	p_Camera->setTarget(lookat);
}

vector3df Camera::GetTarget(){
	return p_Camera->getTarget();
}

void Camera::SetRotation(vector3df rotation){
	p_Camera->setRotation(rotation);
}

vector3df Camera::GetRotation(){
	vector3df rotation = p_Camera->getRotation();
	return rotation;
	
}