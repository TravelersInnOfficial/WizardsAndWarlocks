#include "SpellManager.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "EffectManager.h"


SpellManager* SpellManager::instance = 0;

SpellManager::SpellManager(){
}

SpellManager::~SpellManager(){
	for(int i=0; i<numHechizos; i++){
		std::map<Player*, Hechizo*>::iterator it = hechizos[i].begin();
		for(; it!=hechizos[i].end(); ++it){		// Recorremos entre todos los hechizos
			Hechizo* h = it->second;			// Cargamos el hechizo
			delete h;
		}
	}
}

SpellManager* SpellManager::GetInstance(){
	if(instance==0) instance = new SpellManager();
	return instance;
}

/**
 * @brief [Asignacion de Hechizo y jugador]
 * @details [long description]
 * 
 * @param num 	[numero de hechizo para asignar]
 * @param p 	[jugador al que asignarle el hechizo]
 * 
 * @return 		[Se ha asignado correctamente el hechizo]
 */
bool SpellManager::AddHechizo(int num, Player* p, SPELLCODE type){
	if(num>=0 && num<numHechizos){				// Comprobamos si el numero de hechizo pasado es correcto
		Hechizo* h = hechizos[num][p];			// Nos guardamos el hechizo que habia antes guardado
		if(h!=NULL) delete h;					// En el caso de que ya existiese un Hechizo guardado lo eliminamos
		hechizos[num][p] = CrearHechizo(type);	// Anyadimos el nuevo hechizo
		return true;							// Hechizo asignado
	}
	return false;								// Hechizo no asignado
}

/**
 * @brief [Actualiza los valores de Cooldown de los hechizos]
 * @details [long description]
 */
void SpellManager::UpdateCooldown(float deltaTime){
	this->deltaTime = deltaTime;				// Hacemos update de nuestro deltaTime
	for(int i=0; i<numHechizos; i++){			// Recorremos todos los hashtables que tenemos
		std::map<Player*, Hechizo*>::iterator it = hechizos[i].begin();
		for(; it!=hechizos[i].end(); ++it){		// Recorremos entre todos los hechizos
			Hechizo* h = it->second;			// Cargamos el hechizo
			Player*  p = it->first;				//Load the player
			updateSoundEvents(h, p);			//Update the sound events
			if(h->GetCurrentCooldown()>0){	 	// Comprobamos si esta en cooldown
				h->DecCooldown(this->deltaTime);		// Le pasamos el tiempo que tiene que reducirse el cooldown
			}
			

		}
	}
}

void SpellManager::ResetCooldown(Player* p){
	for(int i=0; i<numHechizos; i++){
		if(hechizos[i].find(p) != hechizos[i].end()){
			Hechizo* h = hechizos[i][p];
			h->ResetCooldown();
		}
	}
}

bool SpellManager::LanzarHechizo(int num, Player* p){
	if(num>=0 && num<numHechizos){				// Comprobamos si el numero de hechizo pasado es correcto
		if(hechizos[num].find(p) != hechizos[num].end()){
			Hechizo* h = hechizos[num][p];			// Cargamos el hechizo en una variables
			if(h!=NULL){							// Comprobamos si realmente existe
				if(h->ComprobarCast(deltaTime)){	// Empezamos a Castearlo
					p->ChangeMP(h->GetMP());
					h->Lanzar(p);					// Lanzamos el hechizo
					return true;
				}
			}
		}
	}
	return false;
}

/**
 * @brief Comprueba si hay suficiente mana como para lanzarlo y lo resetea en caso afirmativo
 * 
 * @param num Numero de hechizo
 * @param p Player que lanza el hechizo
 */
bool SpellManager::StartHechizo(int num, Player* p){
	if(num>=0 && num<numHechizos){				// Comprobamos si el numero de hechizo pasado es correcto
		if(hechizos[num].find(p) != hechizos[num].end()){	// Comprobamos que la clave este
			Hechizo* h = hechizos[num][p];			// Cargamos el hechizo en una variables
			if(h!=NULL){							// Comprobamos si realmente existe
				float mana = p->GetMP();
				if(h->CheckMP(mana)){
					h->EmpezarCast();
					return true;			
				}
			}
		}
	}
	return false;
}

void SpellManager::ResetHechizo(int num, Player* p){
	if(num>=0 && num<numHechizos){
		if(hechizos[num].find(p) != hechizos[num].end()){
			Hechizo* h = hechizos[num][p];
			if(h!=NULL){
				h->ResetSpell();
			}
		}
	}
}

void SpellManager::ResetHechizo(Player* p){
	for(int i=0; i<numHechizos; i++){
		if(hechizos[i].find(p) != hechizos[i].end()){
			Hechizo* h = hechizos[i][p];
			if(h!=NULL){
				h->ResetSpell();
			}
		}
	}
}

float SpellManager::GetUtility(int num, Player* p){
	if(num>=0 && num<numHechizos){
		if(hechizos[num].find(p) != hechizos[num].end()){
			Hechizo* h = hechizos[num][p];
			if(h!=NULL){
				return h->GetUtility(p);
			}
		}
	}
	return 0;
}

Hechizo* SpellManager::CrearHechizo(SPELLCODE type){
	// COSTMP TIMECAST TIMECOOLDOWN OPTHP OPTMP
	Hechizo* h;
	switch(type){
		case SPELL_BASIC:		// Hechizo instantaneo
			h = new Hechizo(-70, 2.0f, 5.0f, SPELL_BASIC, "", 1, 1);
			break;
		case SPELL_PROJECTILE:	//Hechizo de ataque basico
			h = new SpellProjectile(-0, 0.0f, 0.5f, 100, 0);
			break;
		case SPELL_FIRE:
			h = new DragonBreath(-20, 1.0f, 5.0f, 100, 100);
			break;
		case SPELL_THUNDER:
			h = new OdinFury(-20, 1.0f, 5.0f, 100, 100);
			break;
		case SPELL_POISON:
			h = new OgreBelch(-20, 1.0f, 5.0f, 100, 100);
			break;
		case SPELL_WALL:		// Hechizo Invocacion Muro
			h = new DesperationWall(-5, 0.0f, 1.0f, 100, 50);
			break;
		case SPELL_BLIZZARD:		// Hechizo continuo hielo
			h = new GuivernoWind(-0.5, 0.0f, 0.0f, 100, 75);
			break;
		default:
			h = NULL;
	}
	return h;
}

//Update sound event spell position, otherwise will sound far as we move
void SpellManager::updateSoundEvents(Hechizo* h, Player* p) {
	if (h != NULL){
		if (p != NULL) {		
			if (h->getShotEvent() != NULL) {
				h->getShotEvent()->setPosition(p->GetHeadPos()); //Update the event position
			}
		}
	}
}

void SpellManager::DrawHUDSpells(Player* p){
	std::vector<std::string> spellHUDtextures;
	std::vector<float> spellCooldowns;
	std::vector<float> spellCastings;
	std::vector<float> totalCooldowns;
	std::vector<float> totalCastings;
	for(int i = 0; i<numHechizos;i++){
		if(hechizos[i].find(p) != hechizos[i].end()){
			Hechizo* h = hechizos[i][p];
			//get all spells texture
			spellHUDtextures.push_back(h->GetHUDTexturePath());
			spellCooldowns.push_back(h->GetCurrentCooldown());
			spellCastings.push_back(h->GetTimeCasting());
			totalCooldowns.push_back(h->GetTotalCooldown());
			totalCastings.push_back(h->GetTotalCasting());
		}
	}
	GraphicEngine::getInstance()->drawSpellSelector(spellHUDtextures, spellCooldowns,totalCooldowns, spellCastings, totalCastings,p->GetCurrentSpell());
}