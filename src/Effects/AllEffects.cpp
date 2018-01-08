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
	p->max_velocity /= factor;
}

void SlowedDown::RemoveEffect(Player* p){
	p->max_velocity *= factor;
}

//================================================================
// PARALYZED
//================================================================
Paralyzed::Paralyzed(float time, float value):Effect(time, WEAK_PARALYZED){
	factor = value;
}

void Paralyzed::ApplyEffect(Player* p){
	p->max_velocity /= factor;
}

void Paralyzed::RemoveEffect(Player* p){
	p->max_velocity *= factor;
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
Madness::~Madness(){
}

Madness::Madness(float time):Effect(time, WEAK_MADNESS){
	actions.push_back(ACTION_MOVE_UP);
	actions.push_back(ACTION_MOVE_DOWN);
	actions.push_back(ACTION_MOVE_LEFT);
	actions.push_back(ACTION_MOVE_RIGHT);
}

void Madness::ApplyEffect(Player* p){
	
	std::random_shuffle ( actions.begin(), actions.end() );
	
	// create random key assign
	p->GetController()->SwapActions(actions[0], actions[1]);
	p->GetController()->SwapActions(actions[2], actions[3]);

}

void Madness::RemoveEffect(Player* p){
	p->GetController()->SwapActions(actions[0], actions[1]);
	p->GetController()->SwapActions(actions[2], actions[3]);
	actions.clear();
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
	p->max_velocity /= 1000.0f;
}

void DeathSnare::RemoveEffect(Player* p){
	p->max_velocity *= 1000.0f;
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
	p->m_DamageMult *= 1.6;
}

void DamageUp::RemoveEffect(Player* p){
	p->m_DamageMult /= 1.6;				// 1.6 * 0.625 = 1
}

//================================================================
// DEFENSE UP
//================================================================
DefenseUp::DefenseUp(float time):Effect(time, POWERUP_DEFENSE){
}

void DefenseUp::ApplyEffect(Player* p){
	p->m_Defense *= 1.6;
}

void DefenseUp::RemoveEffect(Player* p){
	p->m_Defense /= 1.6;
}

/*
//================================================================
// ELEMENT UP
//================================================================
ElementUp::ElementUp(float time):Effect(time, POWERUP_ELEMDEFENSE){
}

void ElementUp::ApplyEffect(Player* p){
	p->m_ElementDefense *= 1.6;
}

void ElementUp::RemoveEffect(Player* p){
	p->m_ElementDefense /= 1.6;
}
*/

//================================================================
// SPEED UP
//================================================================
SpeedUp::SpeedUp(float time):Effect(time, POWERUP_SPEED){
}

void SpeedUp::ApplyEffect(Player* p){
	p->max_velocity *= 1.5f;
	// TODO: APPLY COOLDOWN REDUCTION
}

void SpeedUp::RemoveEffect(Player* p){
	p->max_velocity /= 1.5f;
}

//================================================================
// UNTARGETABLE
//================================================================
Untargetable::Untargetable(float time):Effect(time, POWERUP_UNTARGET){
}

void Untargetable::ApplyEffect(Player* p){
	// TODO: FINISH EFFECT
}

void Untargetable::RemoveEffect(Player* p){
}

//================================================================
// FIRE SHOTS
//================================================================
FireShots::FireShots(float time):Effect(time, POWERUP_FIRE){
}

void FireShots::ApplyEffect(Player* p){
	// TODO: FINISH EFFECT
}

void FireShots::RemoveEffect(Player* p){
}

//================================================================
// POISON SHOTS
//================================================================
PoisonShots::PoisonShots(float time):Effect(time, POWERUP_POISON){
}

void PoisonShots::ApplyEffect(Player* p){
	// TODO: FINISH EFFECT
}

void PoisonShots::RemoveEffect(Player* p){
}

//================================================================
// INVISIBLE
//================================================================
Invisible::Invisible(float time):Effect(time, POWERUP_INVISIBLE){
}

void Invisible::ApplyEffect(Player* p){
	// TODO: FINISH EFFECT
}

void Invisible::RemoveEffect(Player* p){
}