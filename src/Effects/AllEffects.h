#ifndef ALLEFFECTS_H
#define ALLEFFECTS_H

#include <ParticleData.h>
#include "Effect.h"

////////////////////////////////////////////
///	WEAKENINGS 	////////////////////////////
////////////////////////////////////////////

// ===============================================================================================//
//
//	CONTINOUS DAMAGE EFFECTS
//
// ===============================================================================================//

// WEAK_BURNED
class Burned: public Effect{
public:
	Burned(float time, float dmg);
	void UpdateEffect(Player* p);
private:
	void createSoundEvent();
	float damage;
};

// WEAK_FROZEN
class Frozen: public Effect{
public:
	Frozen(float time, float dmg);
	void UpdateEffect(Player* p);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
	float damage;
};

// WEAK_POISONED
class Poisoned: public Effect{
public:
	Poisoned(float time, float dmg);
	void UpdateEffect(Player* p);
private:
	void createSoundEvent();
	float damage;
};

// ===============================================================================================//
//
//	ACTIVE AND RELEASE EFFECTS
//
// ===============================================================================================//

// WEAK_SLOWEDDOWN
class SlowedDown: public Effect{
public:
	SlowedDown(float time, float value);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
	float factor;
};

// WEAK_PARALYZED
class Paralyzed: public Effect{
public:
	Paralyzed(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
};

// WEAK_SILENCED
class Silenced: public Effect{
public:
	Silenced(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
};

// WEAK_GHOSTDISTORSION
class GhostDistorsion: public Effect{
public:
	GhostDistorsion(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
};

// WEAK_MADNESS
class Madness: public Effect{
public:
	Madness(float time);
	void UpdateEffect(Player* p);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
	std::vector<ACTION_ENUM> actions;
};

// FUZZY_EFFECT
class Fuzzy: public Effect{
public:
	Fuzzy(float time);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:

};

// ===============================================================================================//
//
//	BOTH EFFECTS
//
// ===============================================================================================//

// WEAK_DEATHSNARE
class DeathSnare: public Effect{
public:
	DeathSnare(float time, float d);
	void UpdateEffect(Player* p);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
	float damage;
};

////////////////////////////////////////////
///	UPGRADES 	////////////////////////////
////////////////////////////////////////////

// POWERUP_DAMAGE
class DamageUp: public Effect{
public:
	DamageUp(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
};

// POWERUP_DEFENSE
class DefenseUp: public Effect{
public:
	DefenseUp(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
};

/*
// POWERUP_ELEMDEFENSE
class ElementUp: public Effect{
public:
	ElementUp(float time);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
};
*/

// POWERUP_SPEED
class SpeedUp: public Effect{
public:
	SpeedUp(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
};

// POWERUP_UNTARGET
class Untargetable: public Effect{
public:
	Untargetable(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
};

// POWERUP_FIRE
class FireShots: public Effect{
public:
	FireShots(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
};

// POWERUP_POISON
class PoisonShots: public Effect{
public:
	PoisonShots(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	void createSoundEvent();
};

// POWERUP_INVISIBLE
class Invisible: public Effect{
public:
	Invisible(float time);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void UpdateEffectParticles(Player* p);
	void RemoveEffect(Player* p);
private:
	GParticle* particle;
	void createSoundEvent();
};

#endif