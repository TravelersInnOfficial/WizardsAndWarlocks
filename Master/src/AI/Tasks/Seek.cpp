#include "Seek.h"
#include "./../../Player.h"
#include "./../../AIPlayer.h"

Seek::Seek(){
	maxAcceleration = 30.0f;
}

bool Seek::run(Blackboard* bb){
	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Player* target = (Player*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){
			vector3df cPos = character->GetPos();
			vector3df tPos = target->GetPos();
			vector3df linear = tPos - cPos;

			linear.normalize();
			linear = linear * maxAcceleration;

			linear.Y = 0;		// Para que no salga volando
			character->SetForces(linear);

			return true;
		}
	}
	return false;
}