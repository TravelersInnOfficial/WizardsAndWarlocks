#include "Effect.h"
#include "./../Managers/PlayerManager.h"

/**
 * @brief Constructor basico de hechizo
 * 
 * @param time - tiempo de duracion del efecto
 */
Effect::Effect(float time, EFFECTCODE cod){
	maxTime = time;
	currentTime = time;
	code = cod;
	myPlayer = NULL;
	createSoundEvent();
}

Effect::~Effect(){}

/**
 * @brief Compruebo si ha llegado el final del efecto
 * 
 * @param deltaTime Tiempo pasado desde la ultima vez que se llamo al metodo
 * @return True->El efecto ha terminado
 */
bool Effect::CheckFinish(float deltaTime){
	currentTime -= deltaTime;

	if(currentTime<=0){
		return true;
	}
	return false;
}

/**
 * @brief Vuelve a poner el tiempo del hechizo, se usa para volver a aplicar tiempos
 * @details [long description]
 * 
 * @param time [description]
 */
void Effect::ResetTime(){
	currentTime = maxTime;
}

/**
 * @brief Aplico por primera vez el efecto
 * 
 * @param p Jugador al que se le aplicara
 */
void Effect::ApplyEffect(Player* p){
	myPlayer = p;
}

/**
 * @brief Aplico el efecto continuo
 * 
 * @param p Jugador al que se le aplicara
 */
void Effect::UpdateEffect(Player* p){
	myPlayer = p;
}

/**
 * @brief Quito el efecto al jugador
 * 
 * @param p Jugador al que se le quita el efecto
 */
void Effect::RemoveEffect(Player* p){
	myPlayer = p;
}

/**
 * @brief Codigo que identifica al efecto
 * 
 * @return EFFECTCODE codigo del efecto
 */
EFFECTCODE Effect::getCode(){
	return code;
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void Effect::createSoundEvent() {

}

void Effect::playEffectEvent() {
	vector3df pos = vector3df(0,0,0);
	Player* playerOne = PlayerManager::GetInstance()->GetPlayerOne(); 
	Player* p = NULL;

	if(myPlayer != NULL) p = myPlayer;
	else if (playerOne != NULL) p = playerOne;
	if(p != NULL) pos = p->GetPos();

	SoundSystem::getInstance()->playEvent(effectEvent, pos);
}
