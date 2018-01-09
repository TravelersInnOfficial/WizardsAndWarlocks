#include "AllTasks.h"
// Engines
#include "./../../GraphicEngine/GraphicEngine.h"
// Managers
#include "./../../Managers/SpellManager.h"
#include "./../../Managers/ObjectManager.h"
#include "./../../Managers/PlayerManager.h"
// Steerings
#include "./../SteeringBehaviour/Seek.h"
#include "./../SteeringBehaviour/Flee.h"
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
//	DEBUG
//
// ================================================================================================= //

Debug::Debug(){}

bool Debug::run(Blackboard* bb){
	std::cout<<"HA LLEGADO HASTA AQUI\n";
	return true;
}

// ================================================================================================= //
//
//	MASTER MOVEMENT
//
// ================================================================================================= //

MasterAction::MasterAction(){}

bool MasterAction::run(Blackboard* bb){
	if(DEBUG) std::cout<<"MasterAction\n";

	Task* t = (Task*)bb->GetPuntero((AI_code)(bb->masterAction + AI_TASK_DEFAULT));
	if(t!=NULL){
		t->run(bb);
	}
	return true;
}

// ================================================================================================= //
//
//	MASTER MOVEMENT
//
// ================================================================================================= //

MasterMovement::MasterMovement(){}

bool MasterMovement::run(Blackboard* bb){
	if(DEBUG) std::cout<<"MasterMovement\n";

	Task* t = (Task*)bb->GetPuntero((AI_code)(bb->masterMovement + AI_MOVE_DEFAULT));
	if(t!=NULL){
		t->run(bb);	
	}
	return true;
}

// ================================================================================================= //
//
//	PUT DEFAULT ACTION
//
// ================================================================================================= //

PutDefaultAction::PutDefaultAction(){}

bool PutDefaultAction::run(Blackboard* bb){
	if(DEBUG) std::cout<<"PutDefaultAction\n";

	bb->SetMasterAction(AI_TASK_DEFAULT);

	return true;
}


// ================================================================================================= //
//
//	CHECK USE POTION
//
// ================================================================================================= //

UsePotion::UsePotion(){}

bool UsePotion::run(Blackboard* bb){
	if(DEBUG) std::cout<<"UsePotion\n";

	AIPlayer* character = bb->GetPlayer();
	if(character->HasObject()){
		character->UseObject();
		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK USE POTION
//
// ================================================================================================= //

CheckUsePotion::CheckUsePotion(){}

bool CheckUsePotion::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckUsePotion\n";

	AIPlayer* character = bb->GetPlayer();
	if(character->HasObject()){
		Potion* playerPotion = character->GetPotion();	
		int value = playerPotion->GetValue();	// Cogemos el valor de sanacion de la pocion en propiedad
		float playerHP = character->GetHP(); 	// Cogemos el valor de la vida actual de la IA
		if(playerHP + value <=100){				// La querremos usar en el caso de que no se malgaste al usarla
			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	RELEASE SPELL IF FALSE
//
// ================================================================================================= //

ReleaseSpell::ReleaseSpell(){}

bool ReleaseSpell::run(Blackboard* bb){
	if(DEBUG) std::cout<<"ReleaseSpell\n";
    
    // Miramos si el personaje a conseguido tirar un hechizo
    // En el caso de que no lo haya conseguido para asegurarnos
    // reseteamos el hechizo. De esta forma si el hechizo se cancela
    // este Decorador se encarga de resetearlo.
    if(child != NULL){
        if(!child->run(bb)){
        	AIPlayer* character = bb->GetPlayer();
        	character->SetController(ACTION_SHOOT, RELEASED);
        }
    }
    return true;
}

// ================================================================================================= //
//
//	SHOOT PROYECTILE BASIC
//
// ================================================================================================= //

UseSpell::UseSpell(){}

bool UseSpell::run(Blackboard* bb){
	if(DEBUG) std::cout<<"shootBasic\n";

	AIPlayer* character = bb->GetPlayer();
	// En el caso de que no se este casteando el hechizo
	if(!character->GetCastingSpell()){
			// Intetamos lanzar el hechizo
			character->SetController(ACTION_SHOOT, PRESSED);
			return true;
	// En el caso de que ya se estuviera lanzando
	}else{
		character->SetController(ACTION_SHOOT, DOWN);
		// Miramos a ver si el casteo finaliza
		if(character->GetShootSpell()){
			// En el caso de que se llegue a lanzar los resetamos y volvemos
			// a marcar como UP
			character->SetController(ACTION_SHOOT, RELEASED);
		}
		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	SEND ALL SIGNALS
//
// ================================================================================================= //

SendAllSignals::SendAllSignals(){}

bool SendAllSignals::run(Blackboard* bb){
	if(DEBUG) std::cout<<"SendAllSignals\n";

	// Enviamos a los jugadores
	PlayerManager* masterPlayer = PlayerManager::GetInstance();
	masterPlayer->SendVisualSignal();

	// Enviamos a los objetos
	ObjectManager* masterObject = ObjectManager::GetInstance();
	masterObject->SendAllSignal();

	return true;
}




// ================================================================================================= //
//
//	SEND PLAYER SIGNALS
//
// ================================================================================================= //

SendPlayerSignals::SendPlayerSignals(){}

bool SendPlayerSignals::run(Blackboard* bb){
	if(DEBUG) std::cout<<"SendPlayerSignals\n";

	PlayerManager* masterPlayer = PlayerManager::GetInstance(); 
	masterPlayer->SendVisualSignal();	// Cambiar nombre del metodo o modo de uso
	return true;
}

// ================================================================================================= //
//
//	CHECK SEE POTION TO CATCH
//
// ================================================================================================= //


CheckSawPotion::CheckSawPotion(){}

bool CheckSawPotion::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckSawPotion\n";

	int number = bb->GetNumberSight(AI_POTION);
	if(number>0){
		bb->SetTargetSound(AI_POTION, AI_TARGET);
		bb->SetMasterAction(AI_TASK_CATCH_POT);
		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK DISTANCE
//
// ================================================================================================= //

CheckDistance::CheckDistance(float dist){
	distance = dist;	// El limite del proyectil son 10
}

bool CheckDistance::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckDistance\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(target!=NULL){
		Kinematic cKin;
        Kinematic tKin;

        cKin = character->GetKinematic();
        tKin = target->kinematic;

        tKin.position.Y = cKin.position.Y;

        vector3df dist = tKin.position - cKin.position;
        float distLength = dist.length();

        if(distLength<=distance){
        	return true;
        }

	}
	return false;
}

// ================================================================================================= //
//
//	CHECK PLAYER HEARING
//
// ================================================================================================= //

CheckPlayerHearing::CheckPlayerHearing(){}

bool CheckPlayerHearing::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckPlayerHearing\n";

	AIPlayer* character = bb->GetPlayer();
	AI_code enemy = (AI_code)(AI_PLAYER_WIZA - character->GetAlliance());
	int number = bb->GetNumberSound(enemy);

	if(number>0){
		bb->SetTargetSound(enemy, AI_TARGET);
		return true;
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
	if(DEBUG) std::cout<<"CheckPlayerSight\n";

	AIPlayer* character = bb->GetPlayer();
	AI_code enemy = (AI_code)(AI_PLAYER_WIZA - character->GetAlliance());
	int number = bb->GetNumberSight(enemy);
	if(number>0){
		bb->SetTargetSight(enemy, AI_TARGET);
		bb->SetMasterAction(AI_TASK_SHOOT_SPELL);
		return true;
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
	if(DEBUG) std::cout<<"HasArrived\n";

	AIPlayer* character = bb->GetPlayer();
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
	return true;
}

// ================================================================================================= //
//
//	FACE THE TARGET
//
// ================================================================================================= //

FaceTarget::FaceTarget(){}

bool FaceTarget::run(Blackboard* bb){
	if(DEBUG) std::cout<<"FaceTarget\n";

	AIPlayer* character = bb->GetPlayer();
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
	if(DEBUG) std::cout<<"GoToTarget\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(target!=NULL){

		Kinematic cKin;
    	Kinematic tKin;

    	cKin = character->GetKinematic();
    	tKin = target->kinematic;

		SteeringOutput steering = Seek::GetSteering(cKin, tKin);

		SteeringOutput steering2 = Face::GetSteering(cKin, tKin);
		steering.angular = steering2.angular;

		character->Steering2Controller(steering);

		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	FLEE FROM TARGET
//
// ================================================================================================= //

FleeFromTarget::FleeFromTarget(){
	maxAcceleration = 30.0f;
}

bool FleeFromTarget::run(Blackboard* bb){
	if(DEBUG) std::cout<<"FleeFromTarget\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(target!=NULL){

		Kinematic cKin;
    	Kinematic tKin;

    	cKin = character->GetKinematic();
    	tKin = target->kinematic;

		SteeringOutput steering = Flee::GetSteering(cKin, tKin);

		SteeringOutput steering2 = Face::GetSteering(cKin, tKin);
		steering.angular = steering2.angular;

		character->Steering2Controller(steering);

		return true;
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
	if(DEBUG) std::cout<<"T_Wander\n";

	AIPlayer* character = bb->GetPlayer();
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


// ================================================================================================= //
//
//	ORDER SPELLS BY UTILITY
//
// ================================================================================================= //

SpellSecuencia::SpellSecuencia(){
}

bool SpellSecuencia::run(Blackboard* bb){
	if(DEBUG) std::cout<<"SpellSecuencia\n";
	SortVector(bb);
	if(DEBUG) std::cout<<"SpellSecuencia2\n";

	AIPlayer* character = bb->GetPlayer();
	int tamanyo = spellsOrder.size();
	for(int i=0; i<tamanyo; i++){
		// Conseguimos la tarea desde el blackboard
		// Cada vez que se cambie de hechizo tocará actualizar el puntero correspondiente
		Task* child = (Task*)bb->GetPuntero((AI_code)(AI_TASK_SPELL00 + spellsOrder[i]));
		if(child!=NULL){
			if(child->run(bb)){
				// En el caso de que este en medio de un caseo y quiero poner otro hechizo debera hacer release
				if(character->GetCastingSpell() && spellsOrder[i] != character->GetCurrentSpell()){
					return false; // El release se hace en otra tarea
				}else{
					character->SetCurrentSpell(spellsOrder[i]);
				}
				// el spellsOrder[i] es igual al numero de hechizo que se ha conseguido lanzar
				return true;
			}
		}
	}
	return false;
}

void SpellSecuencia::SortVector(Blackboard* bb){
	if(DEBUG) std::cout<<"SortVector SpellSecuencia\n";
	// Ordenar las tareas por utilidad
	// Se supone que cada tarea es un hechizo
	AIPlayer* character =bb->GetPlayer();
	SpellManager* spellMaster = SpellManager::GetInstance();

	// Conseguimos la utilidad de cada uno de los hechizos del personaje
	// Cogemos el numero de hechizos que tiene el personaje
	int size = character->GetNumberSpells();
	size = size + 1;
	// Insertamos en los vectores el num del hechizo y su utilidad en el mismo orden
	std::vector<float> values;
	std::vector<int> numbers;
	for(int i=0; i<size; i++){
		numbers.push_back(i);
		values.push_back(spellMaster->GetUtility(i, character));
	}

	// ORDENAR AMBOS VECTORES EN FUNCION DE LA UTILIDAD
	// Versión un poco cutre N²

	// El orden de los hechizos lo almacenamos en un vector propio de la clase
	std::vector<float> tempValues;
	spellsOrder.clear();
	int tam = size;
	float compare = -1;
	int pos = 0;
	for(int i=0; i<size; i++){
		pos = 0;
		compare = values[pos];

		for(int j=0; j<tam; j++){
			if(compare < values[j]){
				pos = j;
				compare = values[pos];
			}
		}

		tempValues.push_back(compare);
		spellsOrder.push_back(numbers[pos]);

		values.erase(values.begin() + pos);
		numbers.erase(numbers.begin() + pos);

		tam--;
	}

	if(DEBUG){
		for(int i=0; i<size; i++){
			std::cout<<spellsOrder[i]<<" ";
		}
		std::cout<<std::endl;
	}
}
