#include "AllPotions.h"
#include "../Managers/EffectManager.h"

// LIFE POTION
LifePotion::LifePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "./../assets/textures/potions/lifePotion.png")
{
	CreatePotion(TPosition, TRotation);
}

void LifePotion::Use(Player* p)
{
	p->ChangeHP(value);
}

// MANA POTION
ManaPotion::ManaPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "./../assets/textures/potions/manaPotion.png")
{
	CreatePotion(TPosition, TRotation);
}

void ManaPotion::Use(Player* p)
{
	p->ChangeMP(value);
}

// ICE POTION
IcePotion::IcePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 60, "./../assets/textures/potions/manaPotion.png")
{
	CreatePotion(TPosition, TRotation);
}

void IcePotion::Use(Player* p)
{
	// add 60 hp and freeze player
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, WEAK_FROZEN);
}

// ELECTRIC POTION
ElectricPotion::ElectricPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 60, "./../assets/textures/potions/electricPotion.png")
{
	CreatePotion(TPosition, TRotation);
}

void ElectricPotion::Use(Player* p)
{
	// add 60 hp and paralyze player
	p->ChangeHP(value);
	EffectManager::GetInstance()->AddEffect(p, WEAK_PARALYZED);

}

// FIRE POTION
FirePotion::FirePotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "")
{
	CreatePotion(TPosition, TRotation);
}

void FirePotion::Use(Player* p)
{
	p->ChangeHP(value);
}

// POISON POTION
PoisonPotion::PoisonPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "")
{
	CreatePotion(TPosition, TRotation);
}

void PoisonPotion::Use(Player* p)
{
	p->ChangeHP(value);
}

// ELEMENTAL POTION
ElementalPotion::ElementalPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 20, "")
{
	CreatePotion(TPosition, TRotation);
}

void ElementalPotion::Use(Player* p)
{
	p->ChangeHP(value);
}


	/*
			p->ChangeMP(value);
			goto Switch_End;

		break;
		case LifePotion:
			//does nothing special
		break;
		case IcePotion:
			//freeze player
		break;
		case ElectricPotion:
			//paralyze player:
		break;
		case FirePotion:
			//apply fire hits
		break;
		case PoisonPotion:
			//apply poison hits
		break;
		case ElementalPotion:
			//apply elemental spell damage reduction
		break;
	}*/