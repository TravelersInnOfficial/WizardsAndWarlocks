#include "Hechizo.h"
#include "./../Managers/BulletManager.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../Game.h"

Hechizo::Hechizo(int costPM, float tCast, float tCoolDown){
	costePM = costPM;
	lanzable = false;

	timeCasting = 0.0f;
	casting = tCast;

	currentCooldown = 0.0f;
	cooldown = tCoolDown;
}

Hechizo::~Hechizo(){
	
}

/**
 * @brief Pone las variables aptas para empezar el casteo
 */
void Hechizo::EmpezarCast(){
	timeCasting = 0.0f;
	lanzable = true;
}
 
/**
 * @brief Comprueba si el casteo del hechizo se ha completado
 * @return True->El casteo del hechizo se ha completado
 */
bool Hechizo::ComprobarCast(){
	if(lanzable && currentCooldown<=0){				// Comprobamos que realmente se pueda lanzar el hechizo
		float deltaTime = Game::GetInstance()->GetDeltaTime();
		timeCasting += deltaTime;					// Le sumamos la diferencia entre la ultima vez que se llamo al metodo

		if(timeCasting>=casting){					// Comprobamos si el tiempo de casteo se ha completado
			timeCasting = 0.0f;						// Reseteamos el tiempo de casteo
			currentCooldown = cooldown;				// Ponemos el tiempo de cooldown
			lanzable = false;
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
	std::cout<<"Hey Lanzaste un hechizo normal"<<std::endl;
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
	return costePM<=MP;
}

/**
 * @brief Devuelve el coste del hechizo
 */
int Hechizo::GetMP(){
	return costePM;
}

