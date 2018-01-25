#include "Camera.h"

Camera::~Camera(){

}

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
	std::cout<<"accedo a camera: " << p_Camera << " de: " << this << std::endl;
	vector3df rotation = p_Camera->getRotation();
    std::cout<<"Acceso correcto"<< std::endl;
	return rotation;
	
}