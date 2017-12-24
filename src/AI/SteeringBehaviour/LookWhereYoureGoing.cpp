#include "LookWhereYoureGoing.h"

SteeringOutput LookWhereYoureGoing::GetSteering(Kinematic cKin){
	SteeringOutput output;
	if(cKin.velocity.length() == 0){
		return output;
	}

	vector2df angleXY(cKin.velocity.X, cKin.velocity.Y);

	Kinematic tKin;
	tKin.orientation = vector2df(
							atan2(-cKin.velocity.Y, angleXY.length()),
							atan2(cKin.velocity.X, cKin.velocity.Z)
						);

	return Align::GetSteering(cKin,tKin);
}