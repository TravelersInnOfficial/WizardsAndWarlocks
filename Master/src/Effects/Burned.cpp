#include "Burned.h"


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
