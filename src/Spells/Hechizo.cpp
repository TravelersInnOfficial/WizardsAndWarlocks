#include "Hechizo.h"
#include "./../Managers/BulletManager.h"
#include <GraphicEngine/GraphicEngine.h>

Hechizo::Hechizo(float costPM, float tCast, float tCoolDown, SPELLCODE code, std::string HUDMiniature, float optHP, float optMP){
	costePM = costPM;
	lanzable = false;

	timeCasting = 0.0f;
	casting = tCast;

	currentCooldown = 0.0f;
	cooldown = tCoolDown;

	optimHP = optHP;
	optimMP = optMP;

	type = code;
	HUDTexturePath = HUDMiniature;
	shotEvent = NULL;
	voiceEvent = NULL;
}

Hechizo::~Hechizo(){
	if(shotEvent!=NULL){
		shotEvent->release();
		delete shotEvent;
	}
	if(voiceEvent!=NULL){
		voiceEvent->release();
		delete voiceEvent;
	}
}

void Hechizo::DieReset(){
	timeCasting = 0.0f;
	currentCooldown = 0.0f;
	lanzable = false;
}

void Hechizo::ResetSpell(){
	timeCasting = 0.0f;
	lanzable = false;
}

void Hechizo::ResetCooldown(){
	currentCooldown = 0.0f;
	ResetSpell();
}

void Hechizo::WasteMana(Player* p){
	p->ChangeMP(costePM);
}

/**
 * @brief Pone las variables aptas para empezar el casteo
 */
void Hechizo::EmpezarCast(){
	lanzable = true;
}
 
/**
 * @brief Comprueba si el casteo del hechizo se ha completado
 * @return True->El casteo del hechizo se ha completado
 */
bool Hechizo::ComprobarCast(float deltaTime){
	if(lanzable && currentCooldown<=0){				// Comprobamos que realmente se pueda lanzar el hechizo
		timeCasting += deltaTime;					// Le sumamos la diferencia entre la ultima vez que se llamo al metodo
		if(timeCasting>=casting){					// Comprobamos si el tiempo de casteo se ha completado
			timeCasting = 0.0f;						// Reseteamos el tiempo de casteo
			currentCooldown = cooldown;				// Ponemos el tiempo de cooldown
			return true;
		}
	}
	return false;
}

/**
 * @brief Efecto del hechizo
 * 
 * @param p Jugador que lanza el hechizo
 */
void Hechizo::Lanzar(Player* p){
	if(!CheckMP(p->GetMP())){
		lanzable = false;
	}
}

/**
 * @brief Decrementa el tiempo del cooldown
 * 
 * @param time Deltatime a reducir del cooldown
 */
void Hechizo::DecCooldown(float time){
	currentCooldown -= time;
}

/**
 * @brief Devuelve el tiempo de cooldown que lleva
 */
float Hechizo::GetCurrentCooldown(){
	return currentCooldown;
}

float Hechizo::GetTotalCooldown(){
	return cooldown;
}

/**
 * @brief Comprueba si hay suficiente mana para lanzarlo
 * 
 * @param MP Mana con el que comparar
 */
bool Hechizo::CheckMP(float MP){
	return -costePM<=MP;
}

/**
 * @brief Devuelve el coste del hechizo
 */
float Hechizo::GetMP(){
	return costePM;
}

float Hechizo::CalculateUtility(float value, float optim){ 	// 100 1

	float base = optim;
	if(signbit(value - optim) == 0){	 // 0 = positivo / 0 != negativo
		base = 100 - optim;
	}

	float pendiente = 0;
	if(base!=0){
		pendiente = 100/base;
	} 
	float utility = pendiente * abs(value - optim);
	utility = 100 - utility;

	return utility;
}

float Hechizo::GetUtility(Player* p){
	if(currentCooldown>0.5) return 0;

	float HP = p->GetHP();		// Valores entre [0, 100]
	float MP = p->GetMP();		// Valores entre [0, 100]

	float utilityHP = CalculateUtility(HP, optimHP);
	float utilityMP = CalculateUtility(MP, optimMP);

	// Situacion optima 100HP && 100MP
	float utility = utilityHP + utilityMP;
	utility = utility / 2;
	return utility;
}

SPELLCODE Hechizo::GetType(){
	return type;
}

void Hechizo::createSoundEvent(){}

void Hechizo::playSoundEvent(SoundEvent* event, vector3df pos) {
	//SoundSystem::getInstance()->playEvent(event, pos);
}

SoundEvent* Hechizo::getShotEvent() {
	return shotEvent;
}

SoundEvent* Hechizo::getVoiceEvent() {
	return voiceEvent;
}

std::string Hechizo::GetHUDTexturePath(){
	return HUDTexturePath;
}

float Hechizo::GetTimeCasting(){
	return timeCasting;
}

float Hechizo::GetTotalCasting(){
	return casting;
}

void Hechizo::DrawHUD(float initX, float initY, float size, float outline, bool current){
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	g_engine->draw2DRectangle(vector3df(0,0,0), 
								initX, 
								initY, 
								initX + size, 
								initY + size);
	if(current){
	g_engine->draw2DRectangle(vector3df(255,255,0), 
								initX, 
								initY, 
								initX + size, 
								initY + size);
	}	
	if(timeCasting>0){
		g_engine->draw2DRectangle(vector3df(0,0,255), 
									initX, 
									(initY + size) - size * (timeCasting/casting), 
									initX + size, 
									initY + size);
	}
	else if(currentCooldown>0){
		g_engine->draw2DRectangle(vector3df(255,0,0), 
									initX, 
									(initY) + size * (1-(currentCooldown/cooldown)) , 
									initX + size, 
									initY + size);
	}
	vector4df sizeImage(initX+outline, initY+outline, initX+size-outline, initY+size-outline);
	g_engine->draw2DImage(HUDTexturePath, sizeImage);
}
