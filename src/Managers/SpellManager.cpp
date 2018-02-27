#include "SpellManager.h"
#include <GraphicEngine/GraphicEngine.h>
#include "EffectManager.h"
#include "PlayerManager.h"
#include "./../Spells/SpellsInclude.h"
#include <limits>

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
			h = new SpellProjectile(0, 0.0f, 0.5f, 100, 0);
		break;
		
		case SPELL_FIRE:		// Hechizo bola fuego
			h = new DragonBreath(-20, .5f, 2.5f, 100, 100);
			//h = new DragonBreath(-0, 0.0f, 0.2f, 100, 100);
		break;
		
		case SPELL_THUNDER:		// Hechizo paralizador
			h = new OdinFury(-20, .5f, 2.5f, 100, 100);
		break;
		
		case SPELL_POISON:		// Bomba de veneno
			h = new OgreBelch(-20, 1.0f, 5.0f, 100, 100);
		break;
		
		case SPELL_WALL:		// Hechizo Invocacion Muro
			h = new DesperationWall(-15, 0.0f, 5.0f, 100, 100);
		break;
		
		case SPELL_BLIZZARD:	// Hechizo continuo hielo
			h = new GuivernoWind(-2, 0.0f, 0.0f, 100, 75);
		break;

		case SPELL_TELEPORT:	// Hechizo de teleport
			h = new Teleport(-15, 0.0f, 2.0f, 100, 100);
		break;

		case SPELL_INVISIBILITY:	// Hechizo de invisibilidad
			h = new InvisibilityCape(-20, 1.5f, 10.0f, 100, 100);
		break;

		case SPELL_SPEED:
			h = new Superspeed(-15, 0.0f, 5.0f, 100, 100);
		break;

		case SPELL_DEFENSE:
			h = new OhmnioProtection(-20, 0.0f, 5.0f, 100, 100);
		break;

		case SPELL_UNTARGET:
			h = new DivinePoncho(-30, 0.0f, 7.0f, 100, 100);
		break;

		case SPELL_CLEANSE:
			h = new GaiaCleanse(-20, 0.0f, 5.0f, 100, 100);
		break;

		case SPELL_DUMMY:
			h = new SpellDummy(-15, 0.0f, 5.0f, 100, 100);
		break;

		case SPELL_TELEPORTBASE:
			h = new TeleportBase(-20, 0.0f, 3.0f, 100, 100);
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

void SpellManager::DrawHUDSpells(Player* p, int current){
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	int W = g_engine->GetScreenWidth();
	int H = g_engine->GetScreenHeight();

	float sizeBox = W * 0.075;	// Tamanyo de los cuadrados del hechizo

	float xInit = W/20;			// X inicial del primer hechizo
	float yInit = H*0.85;		// Y inicial

	float space = W * 0.03;		// Espacio entre hechizos
	float outline = 5;			// Borde de los hechizo


	float xInitSpell = 0.0f;
	for(int i = 0; i<numHechizos;i++){
		if(hechizos[i].find(p) != hechizos[i].end()){
			Hechizo* h = hechizos[i][p];
			if(h!=nullptr){
				xInitSpell = xInit + (sizeBox + space)*i;	// Calcula la X inicial de cada hechizo
				h->DrawHUD(xInitSpell, yInit, sizeBox, outline, i==current);
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