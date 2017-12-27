#include "T_Seek.h"
#include "./../../Players/Player.h"
#include "./../../Players/AIPlayer.h"

#include <vector2d.h>
#include <kinematicTypes.h>

#include "./../SteeringBehaviour/Arrive.h"
#include "./../SteeringBehaviour/LookWhereYoureGoing.h"

T_Seek::T_Seek(){
	maxAcceleration = 30.0f;
}

bool T_Seek::run(Blackboard* bb){
	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){

			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();
        	tKin = target->kinematic;

			SteeringOutput steering = Arrive::GetSteering(cKin, tKin);

			/*
			steering.linear.Y = 0;		// Para que no salga volando
			character->SetForces(steering.linear);
			*/
			SteeringOutput steering2 = LookWhereYoureGoing::GetSteering(cKin);
			steering.angular = steering2.angular;
			/*
			vector2df linear = steering.angular;
			//character->SetAngularForce(vector3df( (cos(cOri.Y)*linear.X) ,linear.Y, -(sin(cOri.Y)*linear.X)));
			character->SetAngularForce(vector3df( 0 ,linear.Y, 0));
			*/
			character->Steering2Controller(steering);

			return true;
		}
	}
	return false;
}
