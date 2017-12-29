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
Burned::Burned(float time, int d):Effect(time){
	damage = d;
}

void Burned::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
}

//================================================================
// FROZEN
//================================================================
Frozen::Frozen(float time, int d):Effect(time){
	damage = d;
}

void Frozen::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
}

//================================================================
// POISONED
//================================================================
Poisoned::Poisoned(float time, int d):Effect(time){
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
SlowedDown::SlowedDown(float time, float value):Effect(time){
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
Paralyzed::Paralyzed(float time, float value):Effect(time){
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
Silenced::Silenced(float time):Effect(time){
}

void Silenced::ApplyEffect(Player* p){
}

void Silenced::RemoveEffect(Player* p){
}

//================================================================
// MADNESS
//================================================================
Madness::Madness(float time):Effect(time){
}

void Madness::ApplyEffect(Player* p){
}

void Madness::RemoveEffect(Player* p){
}


////////////////////////////////////////////
///	UPGRADES 	////////////////////////////
////////////////////////////////////////////

//================================================================
// DAMAGE UP
//================================================================
DamageUp::DamageUp(float time):Effect(time){
}

void DamageUp::ApplyEffect(Player* p){
}

void DamageUp::RemoveEffect(Player* p){
}

//================================================================
// DEFENSE UP
//================================================================
DefenseUp::DefenseUp(float time):Effect(time){
}

void DefenseUp::ApplyEffect(Player* p){
}

void DefenseUp::RemoveEffect(Player* p){
}

//================================================================
// ELEMENT UP
//================================================================
ElementUp::ElementUp(float time):Effect(time){
}

void ElementUp::ApplyEffect(Player* p){
}

void ElementUp::RemoveEffect(Player* p){
}

//================================================================
// SPEED UP
//================================================================
SpeedUp::SpeedUp(float time):Effect(time){
}

void SpeedUp::ApplyEffect(Player* p){
}

void SpeedUp::RemoveEffect(Player* p){
}

//================================================================
// UNTARGETABLE
//================================================================
Untargetable::Untargetable(float time):Effect(time){
}

void Untargetable::ApplyEffect(Player* p){
}

void Untargetable::RemoveEffect(Player* p){
}

//================================================================
// FIRE SHOTS
//================================================================
FireShots::FireShots(float time):Effect(time){
}

void FireShots::ApplyEffect(Player* p){
}

void FireShots::RemoveEffect(Player* p){
}

//================================================================
// POISON SHOTS
//================================================================
PoisonShots::PoisonShots(float time):Effect(time){
}

void PoisonShots::ApplyEffect(Player* p){
}

void PoisonShots::RemoveEffect(Player* p){
}

//================================================================
// INVISIBLE
//================================================================
Invisible::Invisible(float time):Effect(time){
}

void Invisible::ApplyEffect(Player* p){
}

void Invisible::RemoveEffect(Player* p){
}