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

void GRect::SetWidth(float width){
	m_rect->SetWidth(width);
}

void GRect::SetHeight(float height){
	m_rect->SetHeight(height);
}

float GRect::GetWidth(){
	return m_rect->GetWidth();
}

