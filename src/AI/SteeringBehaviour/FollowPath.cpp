#include "FollowPath.h"

FollowPath::FollowPath(Pathfinding* p){
	path = p;
	pathOffSet = 5;
	currentParam = 0;
}

FollowPath::~FollowPath(){
	// El path lo eliminara el player
}

SteeringOutput FollowPath::GetSteering(Kinematic cKin){
	std::cout<<"-asdf"<<std::endl;
	// 1. Necesitamos saber en que Nodo nos encontramos
	currentParam = path->GetIndexNearestNode(cKin.position, currentParam);
	// 2. Ha ese nodo le sumamos el offset
	vector3df posTarget = path->GetPosNode(currentParam+pathOffSet);

	Kinematic tKin;
	tKin.position = posTarget;
	// 3. Hacemos un Seek con la posicion del nodo y la del character
	return Seek::GetSteering(cKin, tKin);
}