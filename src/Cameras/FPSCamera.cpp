#include "FPSCamera.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GCamera.h>
#include <cmath>

FPSCamera::FPSCamera(vector3df position, vector3df rotation):Camera(){
	vector3df target = position;
	float max = 1;
	target.X = target.X + sin(rotation.Y)*cos(rotation.X)*max;
	target.Y = target.Y + sin(rotation.X)*max;
	target.Z = target.Z + cos(rotation.Y)*cos(rotation.X)*max;

    p_Camera = GraphicEngine::getInstance()->addCameraSceneNode(position, rotation);

    changeX = 0;
    changeY = 0;
    sensibility = 0.5f;

	GraphicEngine* g_engine = GraphicEngine::getInstance();
	int screenW = g_engine->GetScreenWidth();
	int screenH = g_engine->GetScreenHeight();
	lastPos = vector2di(screenW/2, screenH/2);
	g_engine->SetCursorPosition(vector2di(screenW/2, screenH/2));
}

FPSCamera::~FPSCamera(){
}

void FPSCamera::UpdateCamera(vector3df position){
	if(m_working){
    	vector3df rotation = p_Camera->getRotation();
    	rotation.Z = 0;
		p_Camera->setPosition(position);

		/*
		vector3df target = position;
		float max = 1;
		target.X = target.X + sin(rotation.Y)*cos(rotation.X)*max;
		target.Y = target.Y + sin(rotation.X)*max;
		target.Z = target.Z + cos(rotation.Y)*cos(rotation.X)*max;

		p_Camera->setTarget(target);
		*/

		CatchMouseInput();
		GetNewRotation(&rotation);
		p_Camera->setRotation(rotation);
	}
}


void FPSCamera::GetNewRotation(vector3df* rotation){
	rotation->Y += changeX * sensibility;
	rotation->X += changeY * sensibility;

	float variation = 180/10;
	float max = 180/2 - variation;
	float min = -180/2 + variation;

	rotation->X = rotation->X>max? max: rotation->X<min? min: rotation->X;
}

void FPSCamera::CatchMouseInput(){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	int screenW = g_engine->GetScreenWidth();
	int screenH = g_engine->GetScreenHeight();
	
	vector2di mousePos = g_engine->GetCursorPosition();
	vector2di dirMouse = lastPos - mousePos;

	lastPos = vector2di(screenW/2, screenH/2);
	g_engine->SetCursorPosition(lastPos);

	// Update the Y angle
	changeY = -dirMouse.Y;

	// Update the X angle
	changeX = -dirMouse.X;
}

void FPSCamera::SetWorking(bool work){
	if(work && !m_working) GraphicEngine::getInstance()->SetCursorPosition(lastPos);
	m_working = work;
}
