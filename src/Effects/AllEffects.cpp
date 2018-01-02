#include "AllEffects.h"
#include <KeyStates.h>
#include "./../Players/HumanPlayer.h"
#include <algorithm>    // std::random_shuffle

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
	std::vector<ACTION_ENUM> actions;
	actions.push_back(ACTION_MOVE_UP);
	actions.push_back(ACTION_MOVE_DOWN);
	actions.push_back(ACTION_MOVE_LEFT);
	actions.push_back(ACTION_MOVE_RIGHT);
	
	std::random_shuffle ( actions.begin(), actions.end() );
	
	// create random key assign
	HumanPlayer* h = (HumanPlayer*)p;
	h->playerActions.up_key    = actions[0];
	h->playerActions.down_key  = actions[1];
	h->playerActions.left_key  = actions[2];
	h->playerActions.right_key = actions[3];
}

void Madness::RemoveEffect(Player* p){
	HumanPlayer* h = (HumanPlayer*)p;
	h->playerActions.up_key    = ACTION_MOVE_UP;
	h->playerActions.down_key  = ACTION_MOVE_DOWN;
	h->playerActions.left_key  = ACTION_MOVE_LEFT;
	h->playerActions.right_key = ACTION_MOVE_RIGHT;
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