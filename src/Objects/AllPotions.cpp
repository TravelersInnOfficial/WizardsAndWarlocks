#include "AllPotions.h"
#include "../Managers/EffectManager.h"

// ===============================================================================================//
//
//	LIFE POTION
//	Recovers 20 hp
//
// ===============================================================================================//

LifePotion::LifePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "./../assets/textures/potions/POT_HP.png"){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_HP.png";
	CreatePotion(TPosition, TRotation);
}

void LifePotion::Use(Player* p){
	p->ChangeHP(value);
}

bool LifePotion::CheckUse(Player* p){
	return CheckUseHP(p);
}

// ===============================================================================================//
//
//	MANA POTION
//	Recovers 20 MP
//
// ===============================================================================================//

ManaPotion::ManaPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "./../assets/textures/potions/POT_MP.png"){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_MP.png";
	CreatePotion(TPosition, TRotation);
}

void ManaPotion::Use(Player* p){
	p->ChangeMP(value);
}

bool ManaPotion::CheckUse(Player* p){
	return CheckUseMP(p);
}

// ===============================================================================================//
//
//	ICE POTION
//	Recovers 60 hp and freeze player
//
// ===============================================================================================//

IcePotion::IcePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 60, "./../assets/textures/potions/POT_THUNDER.png"){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_ICE.png";
	CreatePotion(TPosition, TRotation);
}

void IcePotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, WEAK_FROZEN);
}

bool IcePotion::CheckUse(Player* p){
	return CheckUseHP(p);
}

// ===============================================================================================//
//
//	ELECTRIC POTION
//	Recovers 60 hp and paralyze player
//
// ===============================================================================================//

ElectricPotion::ElectricPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 60, "./../assets/textures/potions/POT_THUNDER.png"){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_THUNDER.png";
	CreatePotion(TPosition, TRotation);
}

void ElectricPotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, WEAK_PARALYZED);
}

bool ElectricPotion::CheckUse(Player* p){
	return CheckUseHP(p);
}

// ===============================================================================================//
//
//	FIRE POTION
//	Recovers 20 HP and applies fire hits
//
// ===============================================================================================//

FirePotion::FirePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "./../assets/textures/potions/POT_THUNDER.png"){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_FIRE.png";
	CreatePotion(TPosition, TRotation);
}

void FirePotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, POWERUP_FIRE);
}

bool FirePotion::CheckUse(Player* p){
	return CheckUseHP(p);
}

// ===============================================================================================//
//
//  POISON POTION
//	Recovers 20 HP and applies poison hits
//
// ===============================================================================================//

PoisonPotion::PoisonPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "./../assets/textures/potions/POT_THUNDER.png"){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_POISON.png";
	CreatePotion(TPosition, TRotation);
}

void PoisonPotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, POWERUP_POISON);
}

bool PoisonPotion::CheckUse(Player* p){
	return CheckUseHP(p);
}

// ===============================================================================================//
//
//	DEFENSE POTION
//	Recovers 20 HP and applies defense resistance
//
// ===============================================================================================//

DefensePotion::DefensePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "./../assets/textures/potions/POT_THUNDER.png"){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_SHIELD.png";
	CreatePotion(TPosition, TRotation);
}

void DefensePotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, POWERUP_DEFENSE);
}

bool DefensePotion::CheckUse(Player* p){
	return CheckUseHP(p);
}
