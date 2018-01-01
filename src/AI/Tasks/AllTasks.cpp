#include "AllTasks.h"
// Engines
#include "./../../GraphicEngine/GraphicEngine.h"
// Managers
#include "./../../Managers/SpellManager.h"
#include "./../../Managers/PlayerManager.h"
// Steerings
#include "./../SteeringBehaviour/Face.h"
#include "./../SteeringBehaviour/Wander.h"
#include "./../SteeringBehaviour/Arrive.h"
#include "./../SteeringBehaviour/ObstacleAvoidance.h"
#include "./../SteeringBehaviour/LookWhereYoureGoing.h"
// IAPlaye
#include "./../../Players/AIPlayer.h"
// New information classes
#include <vector2d.h>
#include <kinematicTypes.h>
// Enums
#include <KeyStates.h>

bool DEBUG = false;

// ================================================================================================= //
//
//	ORDER SPELLS BY UTILITY
//
// ================================================================================================= //

SpellSecuencia::SpellSecuencia(){}

bool SpellSecuencia::run(Blackboard* bb){
	if(DEBUG) std::cout<<"SpellSecuencia"<<std::endl;

	int tamanyo = children.size();
	SortVector(bb);
	for(int i=0; i<tamanyo; i++){
		Task* child = children.at(i);
		if(!child->run(bb)){
			return false;
		}
	}
	return true;
}

void SpellSecuencia::SortVector(Blackboard* bb){
	if(DEBUG) std::cout<<"SortVector SpellSecuencia"<<std::endl;
	// Ordenar las tareas por utilidad
	// Se supone que cada tarea es un hechizo
	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		SpellManager* spellMaster = SpellManager::GetInstance();

		int size = character->GetNumberSpells();
		std::vector<float> values;
		std::vector<int> numbers;
		for(int i=0; i<size; i++){
			values.push_back(i);
			values.push_back(spellMaster->GetUtility(i, character));
		}

		// ORDENAR AMBOS VECTORES EN FUNCION DE LA UTILIDAD

	}
}

// ================================================================================================= //
//
//	RELEASE SPELL IF FALSE
//
// ================================================================================================= //

ReleaseSpell::ReleaseSpell(){}

bool ReleaseSpell::run(Blackboard* bb){
	if(DEBUG) std::cout<<"ReleaseSpell"<<std::endl;
    
    // Miramos si el personaje a conseguido tirar un hechizo
    // En el caso de que no lo haya conseguido para asegurarnos
    // reseteamos el hechizo. De esta forma si el hechizo se cancela
    // este Decorador se encarga de resetearlo.
    if(child != NULL){
        if(!child->run(bb)){
        	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
        	if(character!=NULL){
        		int currentSpell = character->GetCurrentSpell();
        		SpellManager::GetInstance()->ResetHechizo(currentSpell, character);
        		bb->SetInt(AI_SPELL_STATUS, UP);
        	}
        }
    }
    return true;
}

// ================================================================================================= //
//
//	SHOOT PROYECTILE BASIC
//
// ================================================================================================= //

ShootBasic::ShootBasic(){}

bool ShootBasic::run(Blackboard* bb){
	if(DEBUG) std::cout<<"shootBasic"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		SpellManager* spellMaster = SpellManager::GetInstance();

		int currentSpell = character->GetCurrentSpell();	// El hechizo actual del personaje	
		int status = bb->GetInt(AI_SPELL_STATUS); 			// Conseguimos el estado anterior del hechizo

		// En el caso de que estuviera levantado
		if(status == UP){
			// Intentamos empezar a lanzar el hechizo
			if(spellMaster->StartHechizo(currentSpell, character)){
				// En el caso de conseguirlo empezamos con el lanzamiento
				spellMaster->LanzarHechizo(currentSpell, character);
				bb->SetInt(AI_SPELL_STATUS, DOWN);
				return true;
			}
		// En el caso de que ya se estuviera lanzando
		}else if(status == DOWN){
			// Miramos a ver si el casteo finaliza
			if(spellMaster->LanzarHechizo(currentSpell, character)){
				// En el caso de que se llegue a lanzar los resetamos y volvemos
				// a marcar como UP
				spellMaster->ResetHechizo(currentSpell, character);
				bb->SetInt(AI_SPELL_STATUS, UP);
			}
			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	SEND PLAYER SIGNALS
//
// ================================================================================================= //

SendPlayerSignals::SendPlayerSignals(){}

bool SendPlayerSignals::run(Blackboard* bb){
	if(DEBUG) std::cout<<"sendPlayers"<<std::endl;

	PlayerManager* masterPlayer = PlayerManager::GetInstance(); 
	masterPlayer->SendVisualSignal();	// Cambiar nombre del metodo o modo de uso
	return true;
}

// ================================================================================================= //
//
//	CHECK PLAYER HEARING
//
// ================================================================================================= //

CheckPlayerHearing::CheckPlayerHearing(){}

bool CheckPlayerHearing::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckPlayerHearing"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		int number = bb->GetNumberSound(AI_PLAYER);

		if(number>0){
			bb->SetTargetSound(AI_PLAYER, AI_TARGET);
			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK PLAYER SIGHT
//
// ================================================================================================= //

CheckPlayerSight::CheckPlayerSight(){}

bool CheckPlayerSight::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckPlayerSight"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		int number = bb->GetNumberSight(AI_PLAYER);

		if(number>0){
			bb->SetTargetSight(AI_PLAYER, AI_TARGET);
			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	HAS ARRIVED?
//
// ================================================================================================= //

HasArrived::HasArrived(){
	arrivedTarget = 1.0f;
}

bool HasArrived::run(Blackboard* bb){
	if(DEBUG) std::cout<<"HasArrived"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){


			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();
        	tKin = target->kinematic;

        	// La altura a la que se encuentren los personajes para nuestro juego
        	// en el que no podran haber dos personajes a diferentes alturas da igual
        	tKin.position.Y = cKin.position.Y;

        	vector3df dir = tKin.position - cKin.position;
        	float length = dir.length();

        	if(length<arrivedTarget){
        		bb->CleanPuntero(AI_TARGET);
				bb->CleanSense(target->id);
        	}

			return true;
		}
	}
	return true;
}

// ================================================================================================= //
//
//	FACE THE TARGET
//
// ================================================================================================= //

FaceTarget::FaceTarget(){}

bool FaceTarget::run(Blackboard* bb){
	if(DEBUG) std::cout<<"FaceTarget"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){

			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();
        	tKin = target->kinematic;

			SteeringOutput steering = ObstacleAvoidance::GetSteering(cKin);
        	if(steering.linear.length() == 0){
        		steering = Wander::GetSteering(cKin);

        		SteeringOutput steering2 = Face::GetSteering(cKin, tKin);
        		steering.angular = steering2.angular;
        	}else{
        		SteeringOutput steering2 = Face::GetSteering(cKin, tKin);
        		steering.angular = steering2.angular;
        	}

			character->Steering2Controller(steering);

			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	GO TO TARGET
//
// ================================================================================================= //

GoToTarget::GoToTarget(){
	maxAcceleration = 30.0f;
}

bool GoToTarget::run(Blackboard* bb){
	if(DEBUG) std::cout<<"GoToTarget"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){

			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();
        	tKin = target->kinematic;

			SteeringOutput steering = Arrive::GetSteering(cKin, tKin);

			SteeringOutput steering2 = Face::GetSteering(cKin, tKin);
			steering.angular = steering2.angular;

			character->Steering2Controller(steering);

			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	DEAMBULAR
//
// ================================================================================================= //

T_Wander::T_Wander(){
	maxAcceleration = 30.0f;
}

bool T_Wander::run(Blackboard* bb){
	if(DEBUG) std::cout<<"T_Wander"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();

        	SteeringOutput steering = ObstacleAvoidance::GetSteering(cKin);
        	if(steering.linear.length() == 0){
        		steering = Wander::GetSteering(cKin);
        	}else{
        		SteeringOutput steering2 = LookWhereYoureGoing::GetSteering(cKin);
        		steering.angular = steering2.angular;
        	}
        	
        	character->Steering2Controller(steering);
			return true;
	}
	return false;
}