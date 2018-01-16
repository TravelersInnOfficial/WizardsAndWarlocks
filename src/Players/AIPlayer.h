#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector2d.h>

#include "Player.h"
#include "./PlayerController.h"
#include "./../AI/BehaviourTree.h"
#include "./../AI/SenseManager/Sensor.h"
#include "./../AI/Pathfinding/Pathfinding.h"

class AIPlayer: public Player{
public:
	AIPlayer();
	~AIPlayer();

	void Update();
	void SetForces(vector3df v);
	void SetAngularForce(vector3df v);
	void Die();
	void Debug();

	void Steering2Controller(SteeringOutput steering);
	void ShortestPath(vector3df); //Calculates the shortest path between to a position

	// Controller
	void SetController(ACTION_ENUM action, keyStatesENUM state);
	void CheckInput();

	// Getters
	vector2di GetActionMoveIA();
	int GetCurrentSpell();
	bool GetShootSpell();
	bool GetCastingSpell();

	// Setters
	void SetCurrentSpell(int num);

private:

	BehaviourTree* 		behaviour;		// Arbol de decisiones de la IA
	Sensor*				sensor;			// Sensor de sentidos de la IA
	Pathfinding* 		path;			// Pathfinding assistant

	bool				shootSpell; 	// Ha conseguido lanzar el hechizo en este frame? Si/No
	bool 				castingSpell;	// Esta lanzando un hechizo? Si/No
};


#endif