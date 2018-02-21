#ifndef ALLTASKS_H
#define ALLTASKS_H

#include <iostream>

#include "./../Secuencia.h"
#include "./../Decorador.h"
#include "./../Selector.h"
#include "./../Task.h"



// ================================================================================================= //
//
//	MASTER MOVEMENT
//
// ================================================================================================= //

class MasterAction: public Decorador{
public:
	MasterAction();
	bool run(Blackboard* bb);
private:
	int lastTask;
};

// ================================================================================================= //
//
//	MASTER MOVEMENT
//
// ================================================================================================= //

class MasterMovement: public Decorador{
public:
	MasterMovement();
	bool run(Blackboard* bb);
private:
	int lastTask;
};

// ================================================================================================= //
//
//	EMPTY TASK
//
// ================================================================================================= //

class EmptyTask: public Task{
public:
	EmptyTask();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	PUT DEFAULT ACTION
//
// ================================================================================================= //

class PutDefaultAction: public Task{
public:
	PutDefaultAction();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	DEBUG
//
// ================================================================================================= //

class NoMove: public Task{
public:
	NoMove();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	USE POTION
//
// ================================================================================================= //

class UsePotion: public Task{
public:
	UsePotion();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	USE FOUNTAIN
//
// ================================================================================================= //

class UseFountain: public Task{
public:
	UseFountain();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CATCH POTION
//
// ================================================================================================= //

class CatchPotion: public Task{
public:
	CatchPotion();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK GRAIL SEEN
//
// ================================================================================================= //

class CheckGrailSeen: public Task{
public:
	CheckGrailSeen();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	MOVE ESCAPE
//
// ================================================================================================= //

class MoveEscape: public Task{
public:
	MoveEscape();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK DOOR IN FRONT
//
// ================================================================================================= //

class CheckDoorInFront: public Task{
public:
	CheckDoorInFront(float raycastDistance);
	bool run(Blackboard* bb);
private:
	float raycastDistance;
};

// ================================================================================================= //
//
//	ROOM TRAVEL
//
// ================================================================================================= //

class TravelRoom: public Task{
public:
	TravelRoom();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK TRAVEL
//
// ================================================================================================= //

class CheckTravel: public Task{
public:
	CheckTravel();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	WHERE EXPLORE
//
// ================================================================================================= //

class WhereExplore: public Task{
public:
	WhereExplore();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK EXPLORE
//
// ================================================================================================= //

class CheckExplore: public Task{
public:
	CheckExplore();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK USE POTION
//
// ================================================================================================= //

class CheckUsePotion: public Task{
public:
	CheckUsePotion();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CATCH GRAIL
//
// ================================================================================================= //

class CatchGrail: public Task{
public:
	CatchGrail();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	DEFUSE TRAP
//
// ================================================================================================= //

class DefuseTrap: public Task{
public:
	DefuseTrap();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK SEE TRAPS TO DEFUSE
//
// ================================================================================================= //

class CheckSawTrap: public Task{
public:
	CheckSawTrap();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK SEE FOUNTAIN TO RECOVER
//
// ================================================================================================= //

class CheckSawFountain: public Task{
public:
	CheckSawFountain();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK SEE POTION TO CATCH
//
// ================================================================================================= //

class CheckSawPotion: public Task{
public:
	CheckSawPotion();
	bool run(Blackboard* bb);
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
};

// ================================================================================================= //
//
//	SHOOT PROYECTILE BASIC
//
// ================================================================================================= //
 
 class UseGuivernum: public Task{
 public:
 	UseGuivernum();
 	bool run(Blackboard* bb);
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
//	FACE TARGET
//
// ================================================================================================= //

class FaceObject: public Task{
public:
	FaceObject();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK PLAYER ATTACK
//
// ================================================================================================= //

class CheckPlayerAttack: public Task{
public:
	CheckPlayerAttack();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK PLAYER ESCAPE
//
// ================================================================================================= //

class CheckPlayerEscape: public Task{
public:
	CheckPlayerEscape();
	bool run(Blackboard* bb);
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
};

// ================================================================================================= //
//
//	PLAYER HEARING
//
// ================================================================================================= //

class PlayerHearing: public Task{
public:
	PlayerHearing();
	bool run(Blackboard* bb);
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
};

// ================================================================================================= //
//
//	TARGET PATH
//
// ================================================================================================= //

class TargetPath: public Task{
public:
	TargetPath();
	bool run(Blackboard* bb);
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

#endif