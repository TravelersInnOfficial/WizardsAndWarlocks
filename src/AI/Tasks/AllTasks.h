#ifndef ALLTASKS_H
#define ALLTASKS_H

#include <iostream>

#include "./../Secuencia.h"
#include "./../Decorador.h"
#include "./../Selector.h"
#include "./../Task.h"

// ================================================================================================= //
//
//	ORDER SPELLS BY UTILITY
//
// ================================================================================================= //

class SpellSecuencia: public Selector{
public:
	SpellSecuencia();
	bool run(Blackboard* bb);
	void SortVector(Blackboard* bb);
private:
	std::vector<int> spellsOrder;
};

// ================================================================================================= //
//
//	RUN CORRECT TASK OF MOVEMENT 
//
// ================================================================================================= //

class RunMovementTask: public Decorador{
public:
	RunMovementTask();
	bool run(Blackboard* bb);
private:
	void CheckChangeTask(Blackboard* bb);
	void ChangeTask(Blackboard* bb);

	int lastValue;
};

// ================================================================================================= //
//
//	RELEASE SPELL IF FALSE
//
// ================================================================================================= //

class ReleaseSpell: public Decorador{
public:
	ReleaseSpell();
	bool run(Blackboard* bb);
private:
};

// ================================================================================================= //
//
//	SHOOT PROYECTILE BASIC
//
// ================================================================================================= //
 
 class UseSpell: public Task{
 public:
 	UseSpell();
 	bool run(Blackboard* bb);
 private:
 };

// ================================================================================================= //
//
//	SEND ALL SIGNALS
//
// ================================================================================================= //

class SendAllSignals: public Task{
public:
	SendAllSignals();
	bool run(Blackboard* bb);
private:
};

// ================================================================================================= //
//
//	SEND PLAYER SIGNALS
//
// ================================================================================================= //

class SendPlayerSignals: public Task{
public:
	SendPlayerSignals();
	bool run(Blackboard* bb);
private:
};

// ================================================================================================= //
//
//	CHECK DISTANCE
//
// ================================================================================================= //

class CheckDistance: public Task{
public:
	CheckDistance(float dist);
	bool run(Blackboard* bb);
private:
	float distance;
};

// ================================================================================================= //
//
//	CHECK PLAYER SIGHT
//
// ================================================================================================= //

class CheckPlayerSight: public Task{
public:
	CheckPlayerSight();
	bool run(Blackboard* bb);
private:
};

// ================================================================================================= //
//
//	CHECK PLAYER HEARING
//
// ================================================================================================= //

class CheckPlayerHearing: public Task{
public:
	CheckPlayerHearing();
	bool run(Blackboard* bb);
private:
};

// ================================================================================================= //
//
//	HAS ARRIVED?
//
// ================================================================================================= //

class HasArrived: public Task{
public:
	HasArrived();
	bool run(Blackboard* bb);
private:
	float arrivedTarget;
};

// ================================================================================================= //
//
//	FACE THE TARGET
//
// ================================================================================================= //

class FaceTarget: public Task{
public:
	FaceTarget();
	bool run(Blackboard* bb);
private:
};

// ================================================================================================= //
//
//	GO TO TARGET
//
// ================================================================================================= //

class GoToTarget: public Task{
public:
	GoToTarget();
	bool run(Blackboard* bb);
private:
	float maxAcceleration;
};

// ================================================================================================= //
//
//	FLEE FROM TARGET
//
// ================================================================================================= //

class FleeFromTarget: public Task{
public:
	FleeFromTarget();
	bool run(Blackboard* bb);
private:
	float maxAcceleration;
};

// ================================================================================================= //
//
//	DEAMBULAR
//
// ================================================================================================= //

class T_Wander: public Task{
public:
	T_Wander();
	bool run(Blackboard* bb);
private:
	float maxAcceleration;
};

#endif