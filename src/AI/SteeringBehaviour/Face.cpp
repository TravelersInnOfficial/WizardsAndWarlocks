#include "Face.h"

Face::Face(){}

Face::~Face(){}

SteeringOutput Face::GetSteering(Kinematic cKin, Kinematic tKin){
	vector3df direction = tKin.position - cKin.position;
	if (direction.length() == 0) {
		SteeringOutput output;
		return output;
	}

	vector2df angleXY(direction.X, direction.Y);

	tKin.orientation = vector2df(
							atan2(-direction.Y, angleXY.length()),
							atan2(direction.X, direction.Z)
						);

	return Align::GetSteering(cKin, tKin);
}