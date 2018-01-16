#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector2d.h>

#include "Player.h"
#include "./PlayerController.h"
#include "./../AI/BehaviourTree.h"
#include "./../AI/SenseManager/Sensor.h"

class AIPlayer: public Player{
public:
	AIPlayer();
	~AIPlayer();

	void Update();
	void SetAngularForce(vector3df v);
	void Die();
	void Debug();

	void Steering2Controller(SteeringOutput steering);

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

	bool				shootSpell; 	// Ha conseguido lanzar el hechizo en este frame? Si/No
	bool 				castingSpell;	// Esta lanzando un hechizo? Si/No
};


#endif