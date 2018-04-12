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
	shotEvent = nullptr;
	voiceEvent = nullptr;

	m_rect = nullptr;
	m_cast_cd = nullptr;
	m_sprite = nullptr;
}

Hechizo::~Hechizo(){
	if (voiceEvent != nullptr) {
		if (voiceEvent->isPlaying()) voiceEvent->stop();	//Stop the sound if its playing
		voiceEvent->release();		//Release the sound
		delete voiceEvent;
	}

	if(shotEvent!=nullptr) {
		if (shotEvent->isPlaying()) shotEvent->stop();		//Stop the sound if its playing
		shotEvent->release();			//Release the sound
		delete shotEvent;
	}

	if(m_sprite != nullptr){
		delete m_sprite;
		m_sprite = nullptr;
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

void Hechizo::WasteMana(Player* p, float deltaTime){
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
	if(base != 0){
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
	SoundSystem::getInstance()->playEvent(event, pos);
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

void Hechizo::EraseHUD(){

	if(m_rect 		!= nullptr)	delete m_rect;
	if(m_cast_cd 	!= nullptr)	delete m_cast_cd;
	if(m_sprite 	!= nullptr) delete m_sprite;
	
	m_rect = nullptr;
	m_cast_cd = nullptr;
	m_sprite = nullptr;
}

void Hechizo::DrawHUD(float initX, float initY, float size, float outline, bool current){
	if(m_rect!=nullptr && m_cast_cd != nullptr && m_sprite != nullptr){
		if(current) m_rect->SetColor(1,1,0);
		else m_rect->SetColor(0,0,0);

		if(timeCasting>0){ 
			m_cast_cd->SetColor(0,0,1);
			m_cast_cd->SetHeight((size + outline)*(timeCasting/casting)); //(initY + size) - size * (timeCasting/casting), 
		}
		else if(currentCooldown>0){ 
			m_cast_cd->SetColor(1,0,0);
			m_cast_cd->SetHeight((size + outline)*(currentCooldown/cooldown)); //(initY) + size * (1-(currentCooldown/cooldown))
		}
		else{
			m_cast_cd->SetHeight(0);
		}
	}
	else{
		m_rect 		= GraphicEngine::getInstance()->add2DRect(vector2df(initX, initY), vector2df(size+outline, size+outline));
		m_cast_cd 	= GraphicEngine::getInstance()->add2DRect(vector2df(initX,initY), vector2df(size+outline,0));
		m_sprite 	= GraphicEngine::getInstance()->addSprite(HUDTexturePath, vector2df(initX+outline, initY+outline),vector2df(size-outline, size-outline));
	}
}
