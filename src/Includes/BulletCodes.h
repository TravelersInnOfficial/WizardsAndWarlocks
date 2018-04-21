#ifndef BULLETCODES_H
#define BULLETCODES_H

enum BULLETCODE
{
	BULLET_NONE = -1,
	BULLET_BASIC = 0,			// Basic shot for SpellProjectile
	BULLET_FIRE = 1,			// Fire ball thrown by DragonBreath
	BULLET_THUNDER = 2,			// Thunder ball thrown by OdinFury
	POISON_BOMB = 3				// Poison Explosion thrown by OgreBelch
};

#endif