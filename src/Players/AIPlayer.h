#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"
#include <vector2d.h>

class LookWhereYoureGoing;
class ObstacleAvoidance;
class SteeringOutput;
class BehaviourTree;
class Pathfinding;
class FollowPath;
class Wander;
class Sensor;
class Align;
class Face;
class Seek;
class Flee;

class AIPlayer: public Player{
public:
	AIPlayer();
	~AIPlayer();

	void ResetValues();
	void Update(float deltaTime) override;
	void DeadUpdate(float deltaTime) override;
	void SetAngularForce(vector3df v);
	void Die() override;

	void InitGame() override;
	void RestartMatchStatus() override;

	// Draw
	void Debug() override;

	// Steerings
	void SetForceToMove(vector3df force);
	vector3df GetForceToMove();
	void SetForceToRotate(vector2df force);
	vector2df GetForceToRotate();
	void Steering2Controller(float deltaTime);
	void ShortestPath(vector3df); //Calculates the shortest path between to a positio

	void CheckInput() override;

	// Getters
	vector2di GetActionMoveIA();
	int GetCurrentSpell();
	bool GetShootSpell();
	bool GetCastingSpell();
	float GetMinCostPM();
	Kinematic GetKinematic() override;

	// Setters
	void SetCurrentSpell(int num);

	// Steerings
	SteeringOutput GetAlign(Kinematic cKin, Kinematic tKin);
	SteeringOutput GetFace(Kinematic cKin, Kinematic tKin);
	SteeringOutput GetSeek(Kinematic cKin, Kinematic tKin);
	SteeringOutput GetFlee(Kinematic cKin, Kinematic tKin);
	SteeringOutput GetLookWhereYoureGoing(Kinematic cKin);
	SteeringOutput GetObstacleAvoid(Kinematic cKin);
	SteeringOutput GetFollowPath(Kinematic cKin);
	SteeringOutput GetWander(Kinematic cKin);

private:
	void SetSteerings();

	float 				elevation;	// Rotacion en X de la Cabeza, en el player se pilla directamente de la camara
	vector3df			m_headPos;	// Posicion de la cabeza

	BehaviourTree* 		behaviour;		// Arbol de decisiones de la IA
	Pathfinding* 		path;			// Pathfinding assistant
	Sensor*				sensor;			// Sensor de sentidos de la IA

	// Steerings
	LookWhereYoureGoing*	lookWhereYoureGoing;
	ObstacleAvoidance*  	obstacleAvoidance;
	FollowPath*				followPath;
	Wander*					wander;
	Align*					align;
	Face*					face;
	Seek*					seek;
	Flee*					flee;

	// Movement
	vector3df			forceToMove;
	vector2df			forceToRotate;

	// ShootSpells
	bool				shootSpell; 	// Ha conseguido lanzar el hechizo en este frame? Si/No
	bool 				castingSpell;	// Esta lanzando un hechizo? Si/No
	float 				minCostPM;		// El coste minimo de los hechizos de la IA
};


#endif