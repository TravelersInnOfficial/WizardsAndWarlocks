#include "AllEffects.h"

////////////////////////////////////////////
///	WEAKENINGS 	////////////////////////////
////////////////////////////////////////////

//================================================================
// BURNED
//================================================================
Burned::Burned(float time, int d):Effect(time){
	damage = d;
}

void Burned::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
	std::cout<<"QUEMANDO ENEMIGO CON "<<-damage<<"\n";
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

//------------------------------------------
//	ACTIVE AND RELEASE EFFECTS
//------------------------------------------

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