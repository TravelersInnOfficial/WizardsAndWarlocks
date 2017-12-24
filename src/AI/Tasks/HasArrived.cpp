#include "HasArrived.h"
#include "./../../Players/AIPlayer.h"

#include <vector2d.h>
#include <kinematicTypes.h>


HasArrived::HasArrived(){
	arrivedTarget = 1.0f;
}

bool HasArrived::run(Blackboard* bb){
	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){

			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();
        	tKin = target->kinematic;

        	vector3df dir = tKin.position - cKin.position;
        	float length = dir.length();

        	std::cout<<length<<std::endl;

        	if(length<arrivedTarget){
        		bb->CleanPuntero(AI_TARGET);
				bb->CleanSense(target->id);
        	}

			return true;
		}
	}
	return true;;
}