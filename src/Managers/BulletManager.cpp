#include "BulletManager.h"

#include "./../Projectiles/Projectile.h"
#include "./../Projectiles/BasicProjectile.h"
#include "./../Projectiles/FireProjectile.h"
#include "./../Projectiles/ThunderProjectile.h"
#include "./../Projectiles/PoisonBomb.h"
#include <fstream>
#include <json.hpp>

static BulletManager* instance = nullptr;

BulletManager* BulletManager::GetInstance(){
	static BulletManager localInstance;
	if(instance == nullptr){
		localInstance.InitObject();
		instance = &localInstance;
	}
	return instance;
}

BulletManager::BulletManager(){}

BulletManager::~BulletManager(){
	EmptyObject();
}

void BulletManager::InitObject(){
	loadProjectileDamage();
}

void BulletManager::loadProjectileDamage(){
 	//Takes path from binary location (/bin)
	std::string jsonPath = "./../assets/json/game_data.json";
	std::ifstream i(jsonPath);
	nlohmann::json j;
	i >> j;

	std::map<std::string,BULLETCODE> bulletMap = GetBULLETCODE_StrMap();
	BULLETCODE ID;

	for(int i = 0; !j["projectiles_data"][i].is_null(); i++){
		ID =  bulletMap [j ["projectiles_data"][i]["ID"] ];
		bullet_damage.insert(std::pair<BULLETCODE,float>(ID,j["projectiles_data"][i]["damage"]));
		bullet_radius.insert(std::pair<BULLETCODE,float>(ID,j["projectiles_data"][i]["radius"]));
		bullet_speed.insert(std::pair<BULLETCODE,float>(ID,j["projectiles_data"][i]["speed"]));
		bullet_max_distance.insert(std::pair<BULLETCODE,float>(ID,j["projectiles_data"][i]["max_distance"]));
	}
}
void BulletManager::EmptyObject(){
	DeleteAllProyectiles();
	instance = nullptr;
}

void BulletManager::AddProyectil(vector3df pos, vector3df rot, vector3df dir, int emi, float dmgMult, BULLETCODE b, EFFECTCODE bulletEffect){
	float dmg;
	std::string separator(20, '.');
	switch(b){
		case BULLET_BASIC:
			dmg = bullet_damage[BULLET_BASIC] * dmgMult;
			proyectiles.push_back(new BasicProjectile(pos, rot, dir, emi, bulletEffect, bullet_radius[BULLET_BASIC], bullet_speed[BULLET_BASIC], dmg, bullet_max_distance[BULLET_BASIC]));
			break;
		
		case BULLET_FIRE:
			dmg = bullet_damage[BULLET_FIRE] * dmgMult;
			proyectiles.push_back(new FireProjectile(pos, rot, dir, emi,  bullet_radius[BULLET_FIRE], bullet_speed[BULLET_FIRE], dmg, bullet_max_distance[BULLET_FIRE]));
			break;
		
		case BULLET_THUNDER:
			dmg = bullet_damage[BULLET_THUNDER] * dmgMult;
			proyectiles.push_back(new ThunderProjectile(pos, rot, dir, emi,  bullet_radius[BULLET_THUNDER], bullet_speed[BULLET_THUNDER], dmg, bullet_max_distance[BULLET_THUNDER]));
			break;
		
		case POISON_BOMB:
			dmg = bullet_damage[POISON_BOMB] * dmgMult;
			proyectiles.push_back(new PoisonBomb(pos, rot, dir, emi,  bullet_radius[POISON_BOMB], bullet_speed[POISON_BOMB], dmg, bullet_max_distance[POISON_BOMB]));
			break;
		
		default:
			break;
	}
}

void BulletManager::Update(){
	DeleteProyectiles();
	int size = proyectiles.size();
	for(int i=size-1; i>=0; i--){
		proyectiles[i]->Update();
	}
}

/**
 * @brief [Mueve los objetos proyectil del vector de Update al de delete]
 * @details [long description]
 */
void BulletManager::AddToDeleteProyecil(Projectile* pro){
	// Comprueba que el proyectil no sea nullptr
	if(pro!=nullptr){
		// Nos guardamos el tamanyo el vector
		int size = proyectiles.size();
		// Recorremos el vector de proyectil en update
		for(int i=size-1; i>=0; i--){
			Projectile* p = proyectiles[i];
			// Comprobamos si el proyectil esta en el vector
			if(p == pro){
				// En el caso de que este lo borramos del primer vector y lo anyadimos en el de eliminar
				proyectiles.erase(proyectiles.begin()+i);
				proyectilesToDelete.push_back(pro);
				// Salimos del bucle
				break;
			}
		}
	}
}

/**
 * @brief [Eliminamos todos los proyectiles que esten en el vector]
 * @details [long description]
 */
void BulletManager::DeleteProyectiles(){
	// Nos guardamos el tamanyo del vector
	int size = proyectilesToDelete.size();
	// Recorremos el vector de proyectiles por eliminar
	for(int i=0; i<size; i++){
		// Cargamos el proyectil a eliminar
		Projectile* p = proyectilesToDelete[i];
		// Lo eliminamos del mundo de Irrlicht y Bullet3D
		// Delete del proyectil
		delete p;
	}
	// Limpiamos el vector de proyectiles por eliminar
	proyectilesToDelete.clear();
}

void BulletManager::DeleteAllProyectiles(){
	// =====================================
	//  PROYECTILES
	// =====================================
	int size = proyectiles.size();							// Nos guardamos el tamanyo del vector
	for(int i=0; i<size; i++){								// Recorremos el vector de proyectiles por eliminar
		Projectile* p = proyectiles[i];						// Cargamos el proyectil a eliminar
		delete p;											// Lo eliminamos del mundo de Irrlicht y Bullet3D
	}
	proyectiles.clear();									// Limpiamos el vector
	// =====================================
	// PROYECTILESTODELETE
	// =====================================
	size = proyectilesToDelete.size();					// Nos guardamos el tamanyo del vector
	for(int i=0; i<size; i++){								// Recorremos el vector de proyectiles por eliminar
		Projectile* p = proyectilesToDelete[i];				// Cargamos el proyectil a eliminar
		delete p;											// Delete del proyectil
	}
	proyectilesToDelete.clear();							// Limpiamos el vector
}

std::map<std::string, BULLETCODE> BulletManager::GetBULLETCODE_StrMap(){
	std::map<std::string, BULLETCODE> ret_map;
	ret_map.insert(std::pair<std::string, BULLETCODE>("BULLET_NONE", BULLET_NONE));
	ret_map.insert(std::pair<std::string, BULLETCODE>("BULLET_BASIC", BULLET_BASIC));
	ret_map.insert(std::pair<std::string, BULLETCODE>("BULLET_FIRE", BULLET_FIRE));
	ret_map.insert(std::pair<std::string, BULLETCODE>("BULLET_THUNDER", BULLET_THUNDER));
	ret_map.insert(std::pair<std::string, BULLETCODE>("POISON_BOMB", POISON_BOMB));

	return ret_map;
};

float BulletManager::GetBulletDamage(BULLETCODE bullet){
	return bullet_damage[bullet];
}