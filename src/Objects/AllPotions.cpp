#include "AllPotions.h"

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
:Potion(TScale, 60, "")
{
	CreatePotion(TPosition, TRotation);
}

void IcePotion::Use(Player* p)
{
	p->ChangeHP(value);
}

// ELECTRIC POTION
ElectricPotion::ElectricPotion(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Potion(TScale, 60, "")
{
	CreatePotion(TPosition, TRotation);
}

void ElectricPotion::Use(Player* p)
{
	p->ChangeHP(value);
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