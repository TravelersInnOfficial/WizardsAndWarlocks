#include "T_Seek.h"
#include "./../../Players/Player.h"
#include "./../../Players/AIPlayer.h"

#include <vector2d.h>
#include <kinematicTypes.h>

#include "./../SteeringBehaviour/Face.h"
#include "./../SteeringBehaviour/Wander.h"

T_Seek::T_Seek(){
	maxAcceleration = 30.0f;
}

bool T_Seek::run(Blackboard* bb){
	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Player* target = (Player*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){
			vector3df cPos = character->GetPos();
			vector3df tPos = target->GetPos();
			vector3df cVel = character->GetVelocity();
			vector3df tVel = target->GetVelocity();
			vector3df cOri = character->GetRot();
			vector3df tOri = target->GetRot(); 
			vector3df cRot = character->GetAngularVelocity();
			vector3df tRot = target->GetAngularVelocity();

			//std::cout<<tOri<<std::endl;
			//std::cout<<tOri<<std::endl;

			Kinematic cKin;
			cKin.position = cPos;
			cKin.orientation =  vector2df(cOri);
        	cKin.velocity = cVel;
        	cKin.rotation = vector2df(cRot);

        	Kinematic tKin;
        	tKin.position = tPos;
        	tKin.orientation = vector2df(tOri);
        	tKin.velocity = tVel;
        	tKin.rotation = vector2df(tRot);


				SteeringOutput steering = Wander::GetSteering(cKin);

				//steering.linear.Y = 0;		// Para que no salga volando
				character->SetForces(steering.linear);

				vector2df linear = steering.angular;
				character->SetAngularForce(vector3df( (cos(cOri.Y)*linear.X) ,linear.Y, -(sin(cOri.Y)*linear.X)));
			//std::cout<<linear<<std::endl;

			return true;
		}
	}
	return false;
}