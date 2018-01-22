#ifndef FOLLOWPATH_H
#define FOLLOWPATH_H

#include "./Seek.h"
#include "./../Pathfinding/Pathfinding.h"

class FollowPath: public Seek{
public:
	FollowPath(Pathfinding* path);
	~FollowPath();
	SteeringOutput GetSteering(Kinematic cKin);
private:
	Pathfinding* 	path;			// Puntero al pathfinding de la IA
	int 			pathOffSet;		// PathOffSet
	int 			currentParam;	// CurrentParam
};


#endif