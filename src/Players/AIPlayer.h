#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector2d.h>

#include "Player.h"
#include "./PlayerController.h"
#include "./../AI/BehaviourTree.h"
#include "./../AI/SenseManager/Sensor.h"
#include "./../AI/Pathfinding/Pathfinding.h"
#include "./../AI/SteeringBehaviour/AllSteerings.h"

class AIPlayer: public Player{
public:
	AIPlayer();
	~AIPlayer();

	void Update(float deltaTime);
	void DeadUpdate();
	void SetAngularForce(vector3df v);
	void Die();
	void Debug();

	void Steering2Controller(SteeringOutput steering);
	void ShortestPath(vector3df); //Calculates the shortest path between to a positio

	void CheckInput();

	// Getters
	vector2di GetActionMoveIA();
	int GetCurrentSpell();
	bool GetShootSpell();
	bool GetCastingSpell();

	// Setters
	void SetCurrentSpell(int num);

	// Steerings
	SteeringOutput GetFace(Kinematic cKin, Kinematic tKin);
	SteeringOutput GetSeek(Kinematic cKin, Kinematic tKin);
	SteeringOutput GetFlee(Kinematic cKin, Kinematic tKin);
	SteeringOutput GetLookWhereYoureGoing(Kinematic cKin);
	SteeringOutput GetObstacleAvoid(Kinematic cKin);
	SteeringOutput GetFollowPath(Kinematic cKin);
	SteeringOutput GetWander(Kinematic cKin);

private:
	void SetRandomName();
	void SetSteerings();

	BehaviourTree* 		behaviour;		// Arbol de decisiones de la IA
	Sensor*				sensor;			// Sensor de sentidos de la IA
	Pathfinding* 		path;			// Pathfinding assistant

	// Steerings
	LookWhereYoureGoing*	lookWhereYoureGoing;
	ObstacleAvoidance*  	obstacleAvoidance;
	FollowPath*				followPath;
	Wander*					wander;
	Face*					face;
	Seek*					seek;
	Flee*					flee;

	bool				shootSpell; 	// Ha conseguido lanzar el hechizo en este frame? Si/No
	bool 				castingSpell;	// Esta lanzando un hechizo? Si/No
};


#endif