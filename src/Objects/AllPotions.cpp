#include "AllPotions.h"
#include "./../Managers/EffectManager.h"
#include "./../Players/Player.h"

// ===============================================================================================//
//
//	LIFE POTION
//	Recovers 50 hp
//
// ===============================================================================================//

LifePotion::LifePotion(vector3df TPosition, vector3df TScale, vector3df TRotation, float value, std::string info)
:Potion(TScale, value, info){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_HP.png";
	m_potionpath = "./../assets/modelos/hp.obj";
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
//	Recovers 50 MP
//
// ===============================================================================================//

ManaPotion::ManaPotion(vector3df TPosition, vector3df TScale, vector3df TRotation, float value, std::string info)
:Potion(TScale, value, info){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_MP.png";
	m_potionpath = "./../assets/modelos/mp.obj";
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
//	Recovers 60 hp, 60/2 mp and freezes player
//
// ===============================================================================================//

IcePotion::IcePotion(vector3df TPosition, vector3df TScale, vector3df TRotation, float value, std::string info)
:Potion(TScale, value, info){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_ICE.png";
	m_potionpath = "./../assets/modelos/ice.obj";
	CreatePotion(TPosition, TRotation);
}

void IcePotion::Use(Player* p){
	p->ChangeHP(value);
	p->ChangeMP(value/2.0f);
	EffectManager::GetInstance()->AddEffect(p, WEAK_FROZEN);
}

bool IcePotion::CheckUse(Player* p){
	return CheckUseHP(p);
}

// ===============================================================================================//
//
//	ELECTRIC POTION
//	Recovers 60 hp, 60/2 mp and paralyzes player
//
// ===============================================================================================//

ElectricPotion::ElectricPotion(vector3df TPosition, vector3df TScale, vector3df TRotation, float value, std::string info)
:Potion(TScale, value, info){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_THUNDER.png";
	m_potionpath = "./../assets/modelos/thunder.obj";
	CreatePotion(TPosition, TRotation);
}

void ElectricPotion::Use(Player* p){
	p->ChangeHP(value/2.0f);
	p->ChangeMP(value);
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

FirePotion::FirePotion(vector3df TPosition, vector3df TScale, vector3df TRotation, float value, std::string info)
:Potion(TScale, value, info){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_FIRE.png";
	m_potionpath = "./../assets/modelos/fire.obj";
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
//	Recovers 20 MP and applies poison hits
//
// ===============================================================================================//

PoisonPotion::PoisonPotion(vector3df TPosition, vector3df TScale, vector3df TRotation, float value, std::string info)
:Potion(TScale, value, info){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_POISON.png";
	m_potionpath = "./../assets/modelos/poison.obj";
	CreatePotion(TPosition, TRotation);
}

void PoisonPotion::Use(Player* p){
	p->ChangeMP(value);
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

DefensePotion::DefensePotion(vector3df TPosition, vector3df TScale, vector3df TRotation, float value, std::string info)
:Potion(TScale, value, info){
	HUDTexturePath = "./../assets/textures/HUD/Potions/POT_SHIELD.png";
	m_potionpath = "./../assets/modelos/shield.obj";
	CreatePotion(TPosition, TRotation);
}

void DefensePotion::Use(Player* p){
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, POWERUP_DEFENSE);
}

bool DefensePotion::CheckUse(Player* p){
	return CheckUseHP(p);
}