#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"
#include "./AI/BehaviourTree.h"

class AIPlayer: public Player{
public:
	AIPlayer();
	~AIPlayer();

	void Update();
	void SetForces(vector3df v);

private:

	BehaviourTree* controller;

};


#endif