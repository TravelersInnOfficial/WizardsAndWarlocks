#include "FPSCamera.h"
#include "../GraphicEngine/GraphicEngine.h"

FPSCamera::FPSCamera(float rotateSpeed, float moveSpeed){
    p_Camera = GraphicEngine::getInstance()->addCameraSceneNodeFPS(rotateSpeed, moveSpeed);
}

FPSCamera::~FPSCamera(){
}

void FPSCamera::UpdateCamera(vector3df position){
    vector3df newRot = p_Camera->getRotation();
	p_Camera->setPosition(position);
	p_Camera->updateAbsolutePosition();
	p_Camera->setRotation(newRot);
}
