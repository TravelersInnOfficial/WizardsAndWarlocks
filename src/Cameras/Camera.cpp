#include "Camera.h"

Camera::Camera(){
	m_working = true;
}

Camera::~Camera(){
	// La camara la elimina el propio motor grafico
	/*if(p_Camera!=nullptr){
		delete p_Camera;
		p_Camera = nullptr;
	}*/
	
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

void Camera::SetWorking(bool work){
	m_working = work;
}