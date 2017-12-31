#include "AllEffects.h"

////////////////////////////////////////////
///	WEAKENINGS 	////////////////////////////
////////////////////////////////////////////

// ===============================================================================================//
//
//	CONTINOUS DAMAGE EFFECT
//
// ===============================================================================================//

//================================================================
// BURNED
//================================================================
Burned::Burned(float time, int d):Effect(time, WEAK_BURNED){
	damage = d;
}

void Burned::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
}

//================================================================
// FROZEN
//================================================================
Frozen::Frozen(float time, int d):Effect(time, WEAK_FROZEN){
	damage = d;
}

void Frozen::UpdateEffect(Player* p){
	//slow gradually and then freeze
	p->ChangeHP(-damage);
}

//================================================================
// POISONED
//================================================================
Poisoned::Poisoned(float time, int d):Effect(time, WEAK_POISONED){
	damage = d;
}

void Poisoned::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
}

// ===============================================================================================//
//
//	ACTIVE AND RELEASE EFFECTS
//
// ===============================================================================================//

//================================================================
// SLOWED DOWN
//================================================================
SlowedDown::SlowedDown(float time, float value):Effect(time, WEAK_SLOWEDDOWN){
	factor = value;
}

void SlowedDown::ApplyEffect(Player* p){
	float vel = p->GetMaxVelocity();
	vel = vel/factor;
	p->SetMaxVelocity(vel);
}

void SlowedDown::RemoveEffect(Player* p){
	float vel = p->GetMaxVelocity();
	vel = vel*factor;
	p->SetMaxVelocity(vel);
}

//================================================================
// PARALYZED
//================================================================
Paralyzed::Paralyzed(float time, float value):Effect(time, WEAK_PARALYZED){
	factor = value;
}

void Paralyzed::ApplyEffect(Player* p){
	float vel = p->GetMaxVelocity();
	vel = vel/factor;
	p->SetMaxVelocity(vel);
}

void Paralyzed::RemoveEffect(Player* p){
	float vel = p->GetMaxVelocity();
	vel = vel*factor;
	p->SetMaxVelocity(vel);
}

//================================================================
// SILENCED
//================================================================
Silenced::Silenced(float time):Effect(time, WEAK_SILENCED){
}

void Silenced::ApplyEffect(Player* p){
}

void Silenced::RemoveEffect(Player* p){
}

//================================================================
// MADNESS
//================================================================
Madness::Madness(float time):Effect(time, WEAK_MADNESS){
}

void Madness::ApplyEffect(Player* p){
}

void Madness::RemoveEffect(Player* p){
}

//================================================================
// DeathSnare
//================================================================
DeathSnare::DeathSnare(float time, int d):Effect(time, WEAK_DEATHSNARE){
	damage=d;
}

void DeathSnare::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
}

void DeathSnare::ApplyEffect(Player* p){
	float vel = p->GetMaxVelocity();
	vel = vel/1000.0f;
	p->SetMaxVelocity(vel);
}

void DeathSnare::RemoveEffect(Player* p){
	float vel = p->GetMaxVelocity();
	vel = vel*1000.0f;
	p->SetMaxVelocity(vel);
}


////////////////////////////////////////////
///	UPGRADES 	////////////////////////////
////////////////////////////////////////////

//================================================================
// DAMAGE UP
//================================================================
DamageUp::DamageUp(float time):Effect(time, POWERUP_DAMAGE){
}

void DamageUp::ApplyEffect(Player* p){
}

void DamageUp::RemoveEffect(Player* p){
}

//================================================================
// DEFENSE UP
//================================================================
DefenseUp::DefenseUp(float time):Effect(time, POWERUP_DEFENSE){
}

void DefenseUp::ApplyEffect(Player* p){
}

void DefenseUp::RemoveEffect(Player* p){
}

//================================================================
// ELEMENT UP
//================================================================
ElementUp::ElementUp(float time):Effect(time, POWERUP_ELEMDEFENSE){
}

void ElementUp::ApplyEffect(Player* p){
}

void ElementUp::RemoveEffect(Player* p){
}

//================================================================
// SPEED UP
//================================================================
SpeedUp::SpeedUp(float time):Effect(time, POWERUP_SPEED){
}

void SpeedUp::ApplyEffect(Player* p){
}

void SpeedUp::RemoveEffect(Player* p){
}

//================================================================
// UNTARGETABLE
//================================================================
Untargetable::Untargetable(float time):Effect(time, POWERUP_UNTARGET){
}

void Untargetable::ApplyEffect(Player* p){
}

void Untargetable::RemoveEffect(Player* p){
}

//================================================================
// FIRE SHOTS
//================================================================
FireShots::FireShots(float time):Effect(time, POWERUP_FIRE){
}

void FireShots::ApplyEffect(Player* p){
}

void FireShots::RemoveEffect(Player* p){
}

//================================================================
// POISON SHOTS
//================================================================
PoisonShots::PoisonShots(float time):Effect(time, POWERUP_POISON){
}

void PoisonShots::ApplyEffect(Player* p){
}

void PoisonShots::RemoveEffect(Player* p){
}

//================================================================
// INVISIBLE
//================================================================
Invisible::Invisible(float time):Effect(time, POWERUP_INVISIBLE){
}

void Invisible::ApplyEffect(Player* p){
}

void Invisible::RemoveEffect(Player* p){
}