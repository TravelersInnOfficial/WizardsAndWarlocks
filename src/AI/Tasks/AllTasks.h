#ifndef ALLTASKS_H
#define ALLTASKS_H

#include <iostream>

#include "./../Task.h"

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