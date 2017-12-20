#ifndef ALLEFFECTS_H
#define ALLEFFECTS_H

#include "Effect.h"

////////////////////////////////////////////
///	WEAKENINGS 	////////////////////////////
////////////////////////////////////////////

//------------------------------------------
//	CONTINOUS DAMAGE EFFECTS
//------------------------------------------
// WEAK_BURNED
class Burned: public Effect{
public:
	Burned(float time, int dmg);
	void UpdateEffect(Player* p);
private:
	int damage;
};

// WEAK_FROZEN
class Frozen: public Effect{
public:
	Frozen(float time, int dmg);
	void UpdateEffect(Player* p);
private:
	int damage;
};

// WEAK_POISONED
class Poisoned: public Effect{
public:
	Poisoned(float time, int dmg);
	void UpdateEffect(Player* p);
private:
	int damage;
};

//------------------------------------------
//	ACTIVE AND RELEASE EFFECTS
//------------------------------------------

// WEAK_SLOWEDDOWN
class SlowedDown: public Effect{
public:
	SlowedDown(float time, float value);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	float factor;
};

// WEAK_PARALYZED
class Paralyzed: public Effect{
public:
	Paralyzed(float time, float value);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	float factor;
};

// WEAK_SILENCED
class Silenced: public Effect{
public:
	Silenced(float time, float value);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	float factor;
};

// WEAK_GHOSTDISTORSION
class GhostDistorsion: public Effect{
public:
	GhostDistorsion(float time, float value);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	float factor;
};

// WEAK_MADNESS
class Madness: public Effect{
public:
	Madness(float time);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
};


//------------------------------------------
//	BOTH EFFECTS
//------------------------------------------
// WEAK_DEATHSNARE
class DeathSnare: public Effect{
public:
	DeathSnare(float time, int d);
	void UpdateEffect(Player* p);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	int damage;
};

////////////////////////////////////////////
///	UPGRADES 	////////////////////////////
////////////////////////////////////////////


#endif