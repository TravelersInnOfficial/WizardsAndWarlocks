#include "GRect.h"
#include "GraphicEngine.h"

GRect::GRect(TFRect* rect){
	m_rect = rect;
}

GRect::~GRect(){
	GraphicEngine::getInstance()->privateSManager->Delete2Delement(m_rect);
}

void GRect::SetColor(float r, float g, float b){
	m_rect->SetColor(r, g, b);
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

