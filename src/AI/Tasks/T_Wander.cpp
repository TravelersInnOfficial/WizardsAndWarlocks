#include "T_Wander.h"
#include "./../../Players/AIPlayer.h"

#include <vector2d.h>
#include <kinematicTypes.h>

#include "./../SteeringBehaviour/Wander.h"
#include "./../SteeringBehaviour/ObstacleAvoidance.h"
#include "./../SteeringBehaviour/LookWhereYoureGoing.h"

T_Wander::T_Wander(){
	maxAcceleration = 30.0f;
}

bool T_Wander::run(Blackboard* bb){
	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();

        	SteeringOutput steering = ObstacleAvoidance::GetSteering(cKin);
        	if(steering.linear.length() == 0){
        		steering = Wander::GetSteering(cKin);
        	}else{
        		SteeringOutput steering2 = LookWhereYoureGoing::GetSteering(cKin);
        		steering.angular = steering2.angular;
        	}
        	
        	character->Steering2Controller(steering);
        	/*
			steering.linear.Y = 0;		// Para que no salga volando
			character->SetForces(steering.linear);

			vector2df linear = steering.angular;
			//character->SetAngularForce(vector3df( (cos(cOri.Y)*linear.X) ,linear.Y, -(sin(cOri.Y)*linear.X)));
			character->SetAngularForce(vector3df( 0 ,linear.Y, 0));
			*/
			return true;
	}
	return false;
}