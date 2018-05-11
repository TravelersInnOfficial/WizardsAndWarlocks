#include "FollowPath.h"

#include <kinematicTypes.h>
#include "./../Pathfinding/Pathfinding.h"

FollowPath::FollowPath(Pathfinding* p){
	path = p;
	pathOffSet = 0;
	currentParam = 0;
}

FollowPath::~FollowPath(){
	// El path lo eliminara el player
}

void FollowPath::ResetValues(){
	currentParam = 0;
}

SteeringOutput FollowPath::GetSteering(Kinematic cKin){

	// 1. Necesitamos saber en que Nodo nos encontramos
	currentParam = path->GetIndexNearestNode(cKin.position, currentParam);

	// 2. Ha ese nodo le sumamos el offset
	vector3df posTarget = path->GetPosNode(currentParam+pathOffSet);

	Kinematic tKin;
	tKin.position = posTarget;
	// 3. Hacemos un Seek con la posicion del nodo y la del character
	return Seek::GetSteering(cKin, tKin);
}