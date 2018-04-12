#include "GSprite.h"
#include "GraphicEngine.h"

GSprite::GSprite(TFSprite* sprite){
	m_sprite = sprite;
}

GSprite::~GSprite(){
	GraphicEngine::getInstance()->privateSManager->Delete2Delement(m_sprite);
}

void GSprite::ToFront(){
	m_sprite->ToFront();
}

void GSprite::ToBkg(){
	m_sprite->ToBkg();
}

vector2df GSprite::GetPosition(){
	toe::core::TOEvector2df pos = m_sprite->GetPosition();
	vector2df output(pos.X, pos.Y);
	return output;
}

float GSprite::GetPosX(){
	return m_sprite->GetPosX();
}

float GSprite::GetPosY(){
	return m_sprite->GetPosY();
}

float GSprite::GetHeight(){
	return m_sprite->GetHeight();
}

float GSprite::GetWidth(){
	return m_sprite->GetWidth();
}

float GSprite::GetTextureHeight(){
	return m_sprite->GetTextureHeight();
}

std::string GSprite::GetTexture(){
	return m_sprite->GetTexture();
}

void GSprite::SetRect(float x, float y, float w, float h){
	m_sprite->SetRect(x, y, w, h);
}

void GSprite::SetPosition(float X, float Y){
	m_sprite->SetPosition(X, Y);
}

void GSprite::ScrollV(float vel){
	m_sprite->ScrollV(vel);
}

void GSprite::ScrollH(float vel){
	m_sprite->ScrollH(vel);
}

void GSprite::SetHeight(float h){
	m_sprite->SetHeight(h);
}

void GSprite::SetWidth(float w){
	m_sprite->SetWidth(w);
}

void GSprite::SetColor(float r, float g, float b, float a){
	m_sprite->SetColor(r, g, b, a);
}

void GSprite::SetTexture(std::string path){
	m_sprite->SetTexture(path);
}

void GSprite::SetMask(std::string path){
	m_sprite->SetMask(path);
}

