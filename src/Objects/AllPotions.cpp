#include "AllPotions.h"
#include "../Managers/EffectManager.h"

// ===============================================================================================//
//
//	LIFE POTION
//	Recovers 20 hp
//
// ===============================================================================================//

LifePotion::LifePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "./../assets/textures/potions/lifePotion.png"){
	CreatePotion(TPosition, TRotation);
}

void LifePotion::Use(Player* p){
	p->ChangeHP(value);
}

// ===============================================================================================//
//
//	MANA POTION
//	Recovers 20 MP
//
// ===============================================================================================//

ManaPotion::ManaPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "./../assets/textures/potions/manaPotion.png"){
	CreatePotion(TPosition, TRotation);
}

void ManaPotion::Use(Player* p){
	p->ChangeMP(value);
}

// ===============================================================================================//
//
//	ICE POTION
//	Recovers 60 hp and freeze player
//
// ===============================================================================================//

IcePotion::IcePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 60, "./../assets/textures/potions/manaPotion.png"){
	CreatePotion(TPosition, TRotation);
}

void IcePotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, WEAK_FROZEN);
}

// ===============================================================================================//
//
//	ELECTRIC POTION
//	Recovers 60 hp and paralyze player
//
// ===============================================================================================//

ElectricPotion::ElectricPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 60, "./../assets/textures/potions/electricPotion.png"){
	CreatePotion(TPosition, TRotation);
}

void ElectricPotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, WEAK_PARALYZED);
}

// ===============================================================================================//
//
//	FIRE POTION
//	Recovers 20 HP and applies fire hits
//
// ===============================================================================================//

FirePotion::FirePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, ""){
	CreatePotion(TPosition, TRotation);
}

void FirePotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, POWERUP_FIRE);
}

// ===============================================================================================//
//
//  POISON POTION
//	Recovers 20 HP and applies poison hits
//
// ===============================================================================================//

PoisonPotion::PoisonPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, ""){
	CreatePotion(TPosition, TRotation);
}

void PoisonPotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, POWERUP_POISON);
}

// ===============================================================================================//
//
//	DEFENSE POTION
//	Recovers 20 HP and applies defense resistance
//
// ===============================================================================================//

DefensePotion::DefensePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, ""){
	CreatePotion(TPosition, TRotation);
}

void DefensePotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, POWERUP_DEFENSE);
}
