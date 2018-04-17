#include "SpellManager.h"
#include <GraphicEngine/GraphicEngine.h>
#include "EffectManager.h"
#include "PlayerManager.h"
#include "./../Spells/SpellsInclude.h"
#include <limits>
#include <fstream>
#include <json.hpp>

static SpellManager* instance = nullptr;

SpellManager* SpellManager::GetInstance(){
	static SpellManager localInstance;
	if(instance == nullptr){
		localInstance.InitObject();
		instance = &localInstance;
	}
	return instance;
}

SpellManager::SpellManager(){}

SpellManager::~SpellManager(){
	EmptyObject();
}

void SpellManager::InitObject(){
	loadSpellsData();
}

void SpellManager::loadSpellsData(){
 	//Takes path from binary location (/bin)
	std::string jsonPath = "./../assets/json/game_data.json";
	std::ifstream i(jsonPath);
	nlohmann::json j;
	i >> j;

	//spell data
	SPELLCODE ID;

	std::map<std::string,SPELLCODE> codesMap = GetSPELLCODE_StrMap();
	std::map<SPELLCODE,std::string> s_info;
	std::map<SPELLCODE,float> s_props;

	for(int i = 0; !j["spells_data"][i].is_null(); i++){
		ID = codesMap[ j["spells_data"][i]["ID"] ];

		spell_names.insert(std::pair<SPELLCODE,std::string>(ID, j["spells_data"][i]["name"]));
		spell_descriptions.insert(std::pair<SPELLCODE,std::string>(ID, j["spells_data"][i]["description"]));
		spell_PROJECTILE.insert(std::pair<SPELLCODE,std::string>(ID, j["spells_data"][i]["PROJECTILE_ID"]));
		spell_EFFECT.insert(std::pair<SPELLCODE,std::string>(ID, j["spells_data"][i]["EFFECT_ID"]));

		spell_costMP.insert(std::pair<SPELLCODE,float>(ID, j["spells_data"][i]["costMP"]));
		spell_tCast.insert(std::pair<SPELLCODE,float>(ID, j["spells_data"][i]["tCast"]));
		spell_tCooldown.insert(std::pair<SPELLCODE,float>(ID, j["spells_data"][i]["tCooldown"]));
		spell_optHP.insert(std::pair<SPELLCODE,float>(ID, j["spells_data"][i]["optHP"]));
		spell_optMP.insert(std::pair<SPELLCODE,float>(ID, j["spells_data"][i]["optMP"]));
	}
}

void SpellManager::EmptyObject(){
	for(int i=0; i<numHechizos; i++){
		hechizos[i].clear();
	}
	instance = nullptr;
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
bool SpellManager::AddHechizo(int num, Player* p, SPELLCODE type, bool broadcast){
	bool toRet = false;
	bool alreadyHas = false;

	if(num >=0 && num < numHechizos){			// Comprobamos si el numero de hechizo pasado es correcto

		for(int i = 1; i < numHechizos && !alreadyHas; i++){
			Hechizo* hAux = hechizos[i][p];
			if(hAux != nullptr){
				SPELLCODE currentSpell = hAux->GetType();
				if(currentSpell == type) alreadyHas = true;	
			}
		}

		if(!alreadyHas){
			Hechizo* h = hechizos[num][p];			// Nos guardamos el hechizo que habia antes guardado
			if(h!=nullptr) delete h;					// En el caso de que ya existiese un Hechizo guardado lo eliminamos
			hechizos[num][p] = CrearHechizo(type);	// Anyadimos el nuevo hechizo
			toRet = true;
		}
	}

	if(broadcast){
		// Si somos cliente y player one, sincronizarlo
		NetworkEngine* n_engine = NetworkEngine::GetInstance();
		if(toRet && p->IsPlayerOne() && n_engine->IsClientInit()){
			Client* client = n_engine->GetClient();
			if(client != nullptr){
				NetworkObject* nObject = p->GetNetworkObject();
				if(nObject != nullptr){
					int netPlayerId = nObject->GetObjId();
					client->SetPlayerSpell(netPlayerId, num, type);
				}
			}
		}
	}

	return toRet;
}

/**
 * @brief [Actualiza los valores de Cooldown de los hechizos]
 * @details [long description]
 */
void SpellManager::UpdateCooldown(float deltaTime){
	m_deltaTime = deltaTime;				// Hacemos update de nuestro deltaTime
	for(int i=0; i<numHechizos; i++){			// Recorremos todos los hashtables que tenemos
		std::map<Player*, Hechizo*>::iterator it = hechizos[i].begin();
		for(; it!=hechizos[i].end(); ++it){		// Recorremos entre todos los hechizos
			Hechizo* h = it->second;			// Cargamos el hechizo
			Player*  p = it->first;				//Load the player
			updateSoundEvents(h, p);			//Update the sound events
			if(h->GetCurrentCooldown()>0){	 	// Comprobamos si esta en cooldown
				h->DecCooldown(m_deltaTime);		// Le pasamos el tiempo que tiene que reducirse el cooldown
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
			if(h!=nullptr){							// Comprobamos si realmente existe
				if(h->ComprobarCast(m_deltaTime)){	// Empezamos a Castearlo
					h->WasteMana(p, m_deltaTime);
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
			if(h!=nullptr){							// Comprobamos si realmente existe
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
			if(h!=nullptr){
				h->ResetSpell();
			}
		}
	}
}

void SpellManager::ResetHechizo(Player* p){
	for(int i=0; i<numHechizos; i++){
		if(hechizos[i].find(p) != hechizos[i].end()){
			Hechizo* h = hechizos[i][p];
			if(h!=nullptr){
				h->ResetSpell();
			}
		}
	}
}

void SpellManager::ResetDieHechizo(Player* p){
	for(int i=0; i<numHechizos; i++){
		if(hechizos[i].find(p) != hechizos[i].end()){
			Hechizo* h = hechizos[i][p];
			if(h!=nullptr){
				h->DieReset();
			}
		}
	}
}

void SpellManager::ResetAllDieHechizo(){
	std::map<Player*, Hechizo*>::iterator it;
	for(int i=0; i<numHechizos; i++){
		it = hechizos[i].begin();
		for(;it!=hechizos[i].end(); it++){
			Hechizo* h = it->second;
			if(h!=nullptr){
				h->DieReset();
			}
		}
	}
}

SPELLCODE SpellManager::GetSpellCode(int num, Player* p){
	if(num>=0 && num<numHechizos){
		if(hechizos[num].find(p) != hechizos[num].end()){
			Hechizo* h = hechizos[num][p];
			if(h!=nullptr){
				return h->GetType();
			}
		}
	}
	return NO_SPELL;
}

float SpellManager::GetUtility(int num, Player* p){
	if(num>=0 && num<numHechizos){
		if(hechizos[num].find(p) != hechizos[num].end()){
			Hechizo* h = hechizos[num][p];
			if(h!=nullptr){
				return h->GetUtility(p);
			}
		}
	}
	return 0;
}

float SpellManager::GetMinCostPM(Player* p){
	float min = std::numeric_limits<float>::max();;

	for(int i=0; i<numHechizos; i++){
		if(hechizos[i].find(p) != hechizos[i].end()){
			Hechizo* h = hechizos[i][p];
			if(h != nullptr){
				float cost = h->GetMP();
				if(cost<min){
					min = cost;
				}
			}
		}
	}

	return min;
}

Hechizo* SpellManager::CrearHechizo(SPELLCODE type){
	Hechizo* h;
	switch(type){
		default:				// Para los que no existan
		case SPELL_PROJECTILE:	// Hechizo de ataque basico
			h = new SpellProjectile(spell_costMP[SPELL_PROJECTILE], spell_tCast[SPELL_PROJECTILE], spell_tCooldown[SPELL_PROJECTILE], spell_optHP[SPELL_PROJECTILE], spell_optMP[SPELL_PROJECTILE]);
		break;
		
		case SPELL_FIRE:		// Hechizo bola fuego
			h = new DragonBreath(spell_costMP[SPELL_FIRE], spell_tCast[SPELL_FIRE], spell_tCooldown[SPELL_FIRE], spell_optHP[SPELL_FIRE], spell_optMP[SPELL_FIRE]);
			//h = new DragonBreath(-0, 0.0f, 0.2f, 100, 100);
		break;
		
		case SPELL_THUNDER:		// Hechizo paralizador
			h = new OdinFury(spell_costMP[SPELL_THUNDER], spell_tCast[SPELL_THUNDER], spell_tCooldown[SPELL_THUNDER], spell_optHP[SPELL_THUNDER], spell_optMP[SPELL_THUNDER]);
		break;
		
		case SPELL_POISON:		// Bomba de veneno
			h = new OgreBelch(spell_costMP[SPELL_POISON], spell_tCast[SPELL_POISON], spell_tCooldown[SPELL_POISON], spell_optHP[SPELL_POISON], spell_optMP[SPELL_POISON]);
		break;
		
		case SPELL_WALL:		// Hechizo Invocacion Muro
			h = new DesperationWall(spell_costMP[SPELL_WALL], spell_tCast[SPELL_WALL], spell_tCooldown[SPELL_WALL], spell_optHP[SPELL_WALL], spell_optMP[SPELL_WALL]);
		break;
		
		case SPELL_BLIZZARD:	// Hechizo continuo hielo
			h = new GuivernoWind(spell_costMP[SPELL_BLIZZARD], spell_tCast[SPELL_BLIZZARD], spell_tCooldown[SPELL_BLIZZARD], spell_optHP[SPELL_BLIZZARD], spell_optMP[SPELL_BLIZZARD]);
			//h = new GuivernoWind(-2, 0.0f, 0.0f, 100, 75);
		break;

		case SPELL_TELEPORT:	// Hechizo de teleport
			h = new Teleport(spell_costMP[SPELL_TELEPORT], spell_tCast[SPELL_TELEPORT], spell_tCooldown[SPELL_TELEPORT], spell_optHP[SPELL_TELEPORT], spell_optMP[SPELL_TELEPORT]);
		break;

		case SPELL_INVISIBILITY:	// Hechizo de invisibilidad
			h = new InvisibilityCape(spell_costMP[SPELL_INVISIBILITY], spell_tCast[SPELL_INVISIBILITY], spell_tCooldown[SPELL_INVISIBILITY], spell_optHP[SPELL_INVISIBILITY], spell_optMP[SPELL_INVISIBILITY]);
		break;

		case SPELL_SPEED:
			h = new Superspeed(spell_costMP[SPELL_SPEED], spell_tCast[SPELL_SPEED], spell_tCooldown[SPELL_SPEED], spell_optHP[SPELL_SPEED], spell_optMP[SPELL_SPEED]);
		break;

		case SPELL_DEFENSE:
			h = new OhmnioProtection(spell_costMP[SPELL_DEFENSE], spell_tCast[SPELL_DEFENSE], spell_tCooldown[SPELL_DEFENSE], spell_optHP[SPELL_DEFENSE], spell_optMP[SPELL_DEFENSE]);
		break;

		case SPELL_UNTARGET:
			h = new DivinePoncho(spell_costMP[SPELL_UNTARGET], spell_tCast[SPELL_UNTARGET], spell_tCooldown[SPELL_UNTARGET], spell_optHP[SPELL_UNTARGET], spell_optMP[SPELL_UNTARGET]);
		break;

		case SPELL_CLEANSE:
			h = new GaiaCleanse(spell_costMP[SPELL_CLEANSE], spell_tCast[SPELL_CLEANSE], spell_tCooldown[SPELL_CLEANSE], spell_optHP[SPELL_CLEANSE], spell_optMP[SPELL_CLEANSE]);
		break;

		case SPELL_DUMMY:
			h = new SpellDummy(spell_costMP[SPELL_DUMMY], spell_tCast[SPELL_DUMMY], spell_tCooldown[SPELL_DUMMY], spell_optHP[SPELL_DUMMY], spell_optMP[SPELL_DUMMY]);
		break;

		case SPELL_TELEPORTBASE:
			h = new TeleportBase(spell_costMP[SPELL_TELEPORTBASE], spell_tCast[SPELL_TELEPORTBASE], spell_tCooldown[SPELL_TELEPORTBASE], spell_optHP[SPELL_TELEPORTBASE], spell_optMP[SPELL_TELEPORTBASE]);
		break;

	}

	return h;
}

//Update sound event spell position, otherwise will sound far as we move
void SpellManager::updateSoundEvents(Hechizo* h, Player* p) {
	if (h != nullptr){
		if (p != nullptr) {		
			if (h->getShotEvent() != nullptr) {
				h->getShotEvent()->setPosition(p->GetHeadPos()); //Update the event position
			}
			if (h->getVoiceEvent() != nullptr) {
				h->getVoiceEvent()->setPosition(p->GetHeadPos()); //Update the event position
			}
		}
	}
}

std::string SpellManager::GetPathFromEnum(SPELLCODE sKind){
	std::string toRet = "./../assets/textures/HUD/Spells/";
	
	switch(sKind){
        case SPELL_SPEED:
			toRet += "SPELL_SPEED.png";
        break;

        case SPELL_DEFENSE:
			toRet += "SPELL_DEFENSE.png";
        break;

        case SPELL_INVISIBILITY:
			toRet += "SPELL_INVISIBILITY.png";
        break;

        case SPELL_UNTARGET:
			toRet += "SPELL_UNTARGET.png";
        break;

        case SPELL_BASIC:
			toRet += "SPELL_BASIC.png";
        break;

		case SPELL_PROJECTILE:
			toRet += "SPELL_PROJECTILE.png";
        break;

		case SPELL_FIRE:
			toRet += "SPELL_FIRE.png";
        break;

		case SPELL_POISON:
			toRet += "SPELL_POISON.png";
        break;

		case SPELL_THUNDER:
			toRet += "SPELL_THUNDER.png";
        break;

		case SPELL_TELEPORT:
			toRet += "SPELL_TELEPORT.png";
        break;

		case SPELL_CLEANSE:
			toRet += "SPELL_CLEANSE.png";
        break;

		case SPELL_WALL:
			toRet += "SPELL_WALL.png";
        break;

		case SPELL_DUMMY:
			toRet += "SPELL_DUMMY.png";
        break;

		case SPELL_TELEPORTBASE:
			toRet += "SPELL_TELEPORTBASE.png";
        break;

		case SPELL_BLIZZARD:
			toRet += "SPELL_BLIZZARD.png";
        break;

        default:
			toRet = "";
        break;
    }

	return toRet;
}

int SpellManager::GetNumSpells(){ return numHechizos; }

std::vector<Hechizo*> SpellManager::GetSpells(Player* player){
	std::vector<Hechizo*> spells;

	for(int i = 0; i < numHechizos;i++){
		if(hechizos[i].find(player) != hechizos[i].end()){
			Hechizo* h = hechizos[i][player];
			if(h != nullptr) spells.push_back(h);
		}
	}

	return spells;
}

void SpellManager::ErasePlayer(Player* player){
	
	ResetDieHechizo(player);
	for(int i=0; i < numHechizos; i++){
		if(hechizos[i].find(player) != hechizos[i].end()){
			Hechizo* h = hechizos[i][player];
			if(h!=nullptr){
				delete h;
			}
		}
		hechizos[i].erase(player);
	}
	
}

// For refreshing newcomers on the server
void SpellManager::RefreshServerAll(){
	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	if(n_engine->IsServerInit()){
		Server* server = n_engine->GetServer();
		if(server != nullptr){
			std::vector<Player*> players = PlayerManager::GetInstance()->GetAllPlayers();
			for(int i = 0; i < players.size() ; i++){
				Player* currentPlayer = players.at(i);
				if(currentPlayer != nullptr){
					NetworkObject* nObject = currentPlayer->GetNetworkObject();
					if(nObject != nullptr){
						int netPlayerId = nObject->GetObjId();
						std::vector<Hechizo*> currentSpells = GetSpells(currentPlayer);
						for(int j = 0; j < currentSpells.size(); j++){
							Hechizo* currentSpell = currentSpells.at(j);
							if(currentSpell != nullptr){
								SPELLCODE type = currentSpell->GetType();
								int num = j;
								server->SetPlayerSpell(netPlayerId, num, type);
							}
						}
					}
				}
			}
		}
	}
}

std::vector<std::string> SpellManager::GetSpellInfo(SPELLCODE spell){
	std::vector<std::string> toRet;
	toRet.push_back(spell_names[spell]);
	toRet.push_back(spell_descriptions[spell]);
	toRet.push_back(spell_PROJECTILE[spell]);
	toRet.push_back(spell_EFFECT[spell]);
	return toRet;
}

std::vector<float> SpellManager::GetSpellProps(SPELLCODE spell){
	std::vector<float> toRet;
	toRet.push_back(spell_costMP[spell]);
	toRet.push_back(spell_tCast[spell]);
	toRet.push_back(spell_tCooldown[spell]);
	toRet.push_back(spell_optHP[spell]);
	toRet.push_back(spell_optMP[spell]);
	return toRet;
}

std::map<std::string, SPELLCODE> SpellManager::GetSPELLCODE_StrMap(){
	std::map<std::string, SPELLCODE> ret_map;
	ret_map.insert(std::pair<std::string, SPELLCODE>("NO_SPELL", NO_SPELL));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_SPEED", SPELL_SPEED));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_DEFENSE", SPELL_DEFENSE));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_INVISIBILITY", SPELL_INVISIBILITY));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_UNTARGET", SPELL_UNTARGET));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_BASIC", SPELL_BASIC));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_PROJECTILE", SPELL_PROJECTILE));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_FIRE", SPELL_FIRE));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_POISON", SPELL_POISON));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_THUNDER", SPELL_THUNDER));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_TELEPORT", SPELL_TELEPORT));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_CLEANSE", SPELL_CLEANSE));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_WALL", SPELL_WALL));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_DUMMY", SPELL_DUMMY));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_TELEPORTBASE", SPELL_TELEPORTBASE));
	ret_map.insert(std::pair<std::string, SPELLCODE>("SPELL_BLIZZARD", SPELL_BLIZZARD));

	return ret_map;
};