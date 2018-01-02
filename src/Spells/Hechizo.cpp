#include "Hechizo.h"
#include "./../Managers/BulletManager.h"
#include "./../GraphicEngine/GraphicEngine.h"

Hechizo::Hechizo(int costPM, float tCast, float tCoolDown, SPELLCODE code, float optHP, float optMP){
	costePM = costPM;
	lanzable = false;

	timeCasting = 0.0f;
	casting = tCast;

	currentCooldown = 0.0f;
	cooldown = tCoolDown;

	optimHP = optHP;
	optimMP = optMP;

	type = code;
}

Hechizo::~Hechizo(){
	
}

void Hechizo::ResetSpell(){
	timeCasting = 0.0f;
	lanzable = false;
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

/**
 * @brief Comprueba si hay suficiente mana para lanzarlo
 * 
 * @param MP Mana con el que comparar
 */
bool Hechizo::CheckMP(int MP){
	return -costePM<=MP;
}

/**
 * @brief Devuelve el coste del hechizo
 */
int Hechizo::GetMP(){
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