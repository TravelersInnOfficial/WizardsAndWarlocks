#include "FPSCamera.h"
#include <GraphicEngine/GraphicEngine.h>

FPSCamera::FPSCamera(vector3df position, vector3df rotation){
	vector3df target = position;
	float max = 1;
	target.X = target.X + sin(rotation.Y)*cos(rotation.X)*max;
	target.Y = target.Y + sin(rotation.X)*max;
	target.Z = target.Z + cos(rotation.Y)*cos(rotation.X)*max;

    p_Camera = GraphicEngine::getInstance()->addCameraSceneNode(position, target);

    changeX = 0;
    changeY = 0;
    sensibility = 0.005;
}

FPSCamera::~FPSCamera(){
}

void FPSCamera::UpdateCamera(vector3df position){
    vector3df rotation = p_Camera->getRotation();
	p_Camera->setPosition(position);


	vector3df target = position;
	float max = 1;
	target.X = target.X + sin(rotation.Y)*cos(rotation.X)*max;
	target.Y = target.Y + sin(rotation.X)*max;
	target.Z = target.Z + cos(rotation.Y)*cos(rotation.X)*max;

	p_Camera->setTarget(target);

	CatchMouseInput();
	vector3df newRotation = GetNewRotation(rotation);

	p_Camera->setRotation(newRotation);
}


vector3df FPSCamera::GetNewRotation(vector3df rotation){
	rotation.Y += changeX * sensibility;
	rotation.X += changeY * sensibility;

	float variation = M_PI/16;
	float max = M_PI/2 - variation;
	float min = -M_PI/2 + variation;
	rotation.X = rotation.X>max? max: rotation.X<min? min: rotation.X;


	return rotation;
}

void FPSCamera::CatchMouseInput(){
	GraphicEngine* engine = GraphicEngine::getInstance();
	int screenW = engine->GetScreenWidth();
	int screenH = engine->GetScreenHeight();
	
	vector2di mousePos = engine->GetCursorPosition();
	vector2di dirMouse = lastPos - mousePos;

	lastPos = mousePos;
	if(lastPos.X<=0 || lastPos.Y<=0 || lastPos.X>=screenW || lastPos.Y>=screenH){
		lastPos = vector2di(screenW/2, screenH/2);
		engine->SetCursorPosition(lastPos);
	}

	// Update the Y angle
	changeY = dirMouse.Y;

	// Update the X angle
	changeX = -dirMouse.X;
}
