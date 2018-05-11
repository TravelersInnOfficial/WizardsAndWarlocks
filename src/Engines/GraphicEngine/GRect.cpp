#include "GRect.h"

#include "GraphicEngine.h"
#include <TravelersOcularEngine/src/TOcularEngine/Elements/2DElements/TFRect.h>
#include <TravelersOcularEngine/src/TOcularEngine/SceneManager.h>

GRect::GRect(TFRect* rect){
	m_rect = rect;
}

GRect::~GRect(){
	GraphicEngine::getInstance()->privateSManager->Delete2Delement(m_rect);
}

void GRect::SetColor(float r, float g, float b, float a){
	m_rect->SetColor(r, g, b, a);
}

void GRect::SetMask(std::string path){
	m_rect->SetMask(path);
}

void GRect::SetRotation(float rot){
	m_rect->SetRotation(rot);
}

void GRect::SetWidth(float width){
	m_rect->SetWidth(width);
}

void GRect::SetHeight(float height){
	m_rect->SetHeight(height);
}

void GRect::SetPosition(float x, float y){
	m_rect->SetPosition(x,y);
}

void GRect::SetXPos(float x){
	m_rect->SetPosX(x);
}

void GRect::SetYPos(float y){
	m_rect->SetPosY(y);
}

float GRect::GetWidth(){
	return m_rect->GetWidth();
}

float GRect::GetHeight(){
	return m_rect->GetHeight();
}

