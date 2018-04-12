#include "AllTasks.h"
// Engines
#include <GraphicEngine/GraphicEngine.h>
#include <PhysicsEngine/BulletEngine.h>
// Managers
#include "./../../Managers/SpellManager.h"
#include "./../../Managers/ObjectManager.h"
#include "./../../Managers/PlayerManager.h"
// IAPlayer
#include "./../../Players/AIPlayer.h"
#include "./../RoomGraph/RoomGraph.h"
// New information classes
#include <vector2d.h>
#include <kinematicTypes.h>
// Enums
#include <KeyStates.h>

bool DEBUG = false;

// ================================================================================================= //
//
//	MASTER ACTION
//
// ================================================================================================= //

MasterAction::MasterAction(){
	lastTask = -1;
}

MasterAction::~MasterAction(){
	// En este caso el hijo no esta ligado a la accion
	// Es eliminada en otro sitio
	child = nullptr;
}

bool MasterAction::run(Blackboard* bb){
	if(DEBUG) std::cout<<"MasterAction\n";

	int number = (int)(bb->masterAction);
	if(number != lastTask){
		lastTask = number;
		Task* t  = (Task*)bb->GetPuntero((AI_code)lastTask);
		setChild(t);
	}

	if(child != nullptr){
		child->run(bb);
	}
	return true;
}

// ================================================================================================= //
//
//	MASTER MOVEMENT
//
// ================================================================================================= //

MasterMovement::MasterMovement(){
	lastTask = -1;
}

MasterMovement::~MasterMovement(){
	// En este caso el hijo no esta ligado a la accion
	// Es eliminada en otro sitio
	child = nullptr;
}

bool MasterMovement::run(Blackboard* bb){
	if(DEBUG) std::cout<<"MasterMovement\n";

	int number = (int)(bb->masterMovement);
	if(number != lastTask){
		lastTask = number;
		Task* t  = (Task*)bb->GetPuntero((AI_code)lastTask);
		setChild(t);
	}

	if(child != nullptr){
		child->run(bb);
	}
	return true;
}

// ================================================================================================= //
//
//	EMPTY TASK
//
// ================================================================================================= //

EmptyTask::EmptyTask(){}

bool EmptyTask::run(Blackboard* bb){
	if(DEBUG) std::cout<<"EmptyTask"<<std::endl;

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
	bb->SetMasterMovement(AI_MOVE_DEFAULT);

	return true;
}

// ================================================================================================= //
//
//	NO MOVE
//
// ================================================================================================= //

NoMove::NoMove(){}

bool NoMove::run(Blackboard* bb){
	if(DEBUG) std::cout<<"NoMove\n";

	AIPlayer* character = bb->GetPlayer();
	if(character != nullptr){
		SteeringOutput steering;
		character->SetForceToMove(steering.linear);
		character->SetForceToRotate(steering.angular);
	}
	return true;
}

// ================================================================================================= //
//
//	CATCH POTION
//
// ================================================================================================= //

CatchPotion::CatchPotion(){}

bool CatchPotion::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CatchPotion\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(character != nullptr && target!=nullptr){
		character->SetController(ACTION_RAYCAST, PRESSED);
		bb->CleanSense(target->id);
		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	ROOM TRAVEL
//
// ================================================================================================= //

TravelRoom::TravelRoom(){}

bool TravelRoom::run(Blackboard* bb){
	if(DEBUG) std::cout<<"TravelRoom\n";

	AIPlayer* character = bb->GetPlayer();
	RoomGraph* room = bb->GetRoomGraph();
	if(character!=nullptr && room!=nullptr){
		vector3df pos = room->NextRoomPos();
		character->ShortestPath(pos);

		Kinematic cKin = character->GetKinematic();

		SteeringOutput steering = character->GetFollowPath(cKin);

		character->SetForceToMove(steering.linear);
		character->SetForceToRotate(steering.angular);

		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK GRAIL SEEN
//
// ================================================================================================= //

CheckGrailSeen::CheckGrailSeen(){}

bool CheckGrailSeen::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckGrailSeen\n";

	AIPlayer* character = bb->GetPlayer();
	int number = bb->GetNumberSight(AI_GRAIL);
	if(number>0 && character->GetAlliance()==ALLIANCE_WIZARD){
		bb->SetTargetSight(AI_GRAIL, AI_TARGET);
		bb->SetMasterAction(AI_TASK_DEFUSE_TRAP);
		bb->SetMasterMovement(AI_MOVE_INTERACT);
		return true;
	
	}
	return false;
}

// ================================================================================================= //
//
//	MOVE ESCAPE
//
// ================================================================================================= //

MoveEscape::MoveEscape(){}

bool MoveEscape::run(Blackboard* bb){
	if(DEBUG) std::cout<<"MoveEscape\n";

	AIPlayer* character = bb->GetPlayer();
	RoomGraph* room = bb->GetRoomGraph();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(room!=nullptr && character!=nullptr && target!=nullptr){
		vector3df pos = room->GetEscapeRoom(character->GetPos(), target->kinematic.position);


		Kinematic cKin = character->GetKinematic();
		Kinematic tKin = target->kinematic;
		SteeringOutput steering;

		if(pos.X != std::numeric_limits<float>::max()){
			character->ShortestPath(pos);
	    	steering = character->GetFollowPath(cKin);
		}else{
			steering = character->GetFlee(cKin, tKin);
			SteeringOutput steering2 = character->GetLookWhereYoureGoing(cKin);
			steering.angular = steering2.angular;
		}

		character->SetForceToMove(steering.linear);
		character->SetForceToRotate(steering.angular);
		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK DIRECT VISION
//
// ================================================================================================= //

CheckDirectVision::CheckDirectVision(){}

bool CheckDirectVision::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckDirectVision\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(character != nullptr && target != nullptr){
		vector3df cPos = character->GetPos();
		vector3df tPos = target->kinematic.position;

		int collisionFilter = C_WALL | C_FOUNTAIN | C_DOOR | C_GRAIL;
		BulletEngine* f_engine = BulletEngine::GetInstance();
		void* object = f_engine->Raycast(cPos, tPos, collisionFilter);

		if(object != nullptr){
			bb->SetMasterAction(AI_TASK_DEFAULT);
			bb->SetMasterMovement(AI_MOVE_AVOID);
			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK DOOR IN FRONT
//
// ================================================================================================= //

CheckDoorInFront::CheckDoorInFront(float dist){
	raycastDistance = dist;
}

bool CheckDoorInFront::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckDoorInFront\n";

	AIPlayer* character = bb->GetPlayer();
	if(character!=nullptr){
		// Obtenemos la posicion inicial y final de raycast
		vector3df charPos = character->GetPos();
		vector3df endPos;
		// Calculamos la posicion final del raycast
		vector3df rot = character->GetRot();
		rot.X = -rot.X;
		endPos.X = charPos.X + sin(rot.Y)*cos(rot.X)*raycastDistance;
		endPos.Y = charPos.Y + sin(rot.X)*raycastDistance;
		endPos.Z = charPos.Z + cos(rot.Y)*cos(rot.X)*raycastDistance;

		void* Object = BulletEngine::GetInstance()->Raycast(
		charPos, 
		endPos);

		// Miramos si encontramos una puerta en el camino
		if(Object!=nullptr){
			Entidad* h = (Entidad*)Object;
			if(h->GetClase()==EENUM_DOOR){
				Door* door = (Door*)Object;
				if(!door->GetOpenState()){
					character->SetController(ACTION_RAYCAST, PRESSED);
					return true;
				}
			}
		}
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK TRAVEL
//
// ================================================================================================= //

CheckTravel::CheckTravel(){}

bool CheckTravel::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckTravel\n";

	AIPlayer* character = bb->GetPlayer();
	RoomGraph* room = bb->GetRoomGraph();
	if(character!=nullptr && room!=nullptr){
		if(room->NextRoom()){
			bb->SetMasterAction(AI_TASK_TRAVEL);
			bb->SetMasterMovement(AI_MOVE_TRAVEL);
			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	WHERE EXPLORE
//
// ================================================================================================= //

WhereExplore::WhereExplore(){}

bool WhereExplore::run(Blackboard* bb){
	if(DEBUG) std::cout<<"Where Explore\n";

	AIPlayer* character = bb->GetPlayer();
	RoomGraph* room = bb->GetRoomGraph();
	if(room!=nullptr && character!=nullptr){
		vector3df pos = room->WhereExplore(character->GetPos());
		character->ShortestPath(pos);

		Kinematic cKin;
		Kinematic tKin;

		cKin = character->GetKinematic();

    	SteeringOutput steering = character->GetFollowPath(cKin);

		character->SetForceToMove(steering.linear);
		character->SetForceToRotate(steering.angular);
		
		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK EXPLORE
//
// ================================================================================================= //

CheckExplore::CheckExplore(){}
	
bool CheckExplore::run(Blackboard* bb){
	if(DEBUG) std::cout<<"Check Explore\n";

	RoomGraph* room = bb->GetRoomGraph();
	if(room!=nullptr){
		if(!room->RoomExplored()){
			bb->SetMasterAction(AI_TASK_EXPLORE);
			bb->SetMasterMovement(AI_MOVE_EXPLORE);
			return true;
		}
	}
	return false;
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
//	USE FOUNTAIN
//
// ================================================================================================= //

UseFountain::UseFountain(){}

bool UseFountain::run(Blackboard* bb){
	if(DEBUG) std::cout<<"UseFountain\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(target != nullptr){
		character->SetController(ACTION_RAYCAST, PRESSED);

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
		if(playerPotion->CheckUse(character)){
			bb->SetMasterAction(AI_TASK_DRINK_POT);
			return true;				// La querremos usar en el caso de que no se malgaste la pocion
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
    if(child != nullptr){
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

UseGuivernum::UseGuivernum(){}

bool UseGuivernum::run(Blackboard* bb){
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
		return true;
	}
	return false;
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
//	CATCH GRAIL
//
// ================================================================================================= //

CatchGrail::CatchGrail(){}

bool CatchGrail::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CatchGrail\n";

	AIPlayer* character = bb->GetPlayer();
	if(character!=nullptr){
		character->SetController(ACTION_RAYCAST, PRESSED);

		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	DEFUSE TRAP
//
// ================================================================================================= //

DefuseTrap::DefuseTrap(){}

bool DefuseTrap::run(Blackboard* bb){
	if(DEBUG) std::cout<<"DefuseTrap\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(character != nullptr && target!=nullptr){
		character->SetController(ACTION_RAYCAST, PRESSED);
		bb->CleanSense(target->id);
		return true;
	}
	return false;
}


// ================================================================================================= //
//
//	CHECK SEE TRAPS TO DEFUSE
//
// ================================================================================================= //

CheckSawTrap::CheckSawTrap(){}

bool CheckSawTrap::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckSawTrap\n";

	int number = bb->GetNumberSight(AI_TRAP);
	if(number>0){
		bb->SetTargetSight(AI_TRAP, AI_TARGET);
		bb->SetMasterAction(AI_TASK_DEFUSE_TRAP);
		bb->SetMasterMovement(AI_MOVE_INTERACT);
		return true;
	}
	return false;
}


// ================================================================================================= //
//
//	CHECK SEE FOUNTAIN TO RECOVER
//
// ================================================================================================= //

CheckSawFountain::CheckSawFountain(){}

bool CheckSawFountain::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckSawFountain\n";

	AIPlayer* character = bb->GetPlayer();
	if(character->GetHP()<80){	// En el caso de que la vida del personaje sea inferior al 90%
		int number = bb->GetNumberSight(AI_FOUNTAIN);
		if(number>0){			// Recuerde más de una fuente
			bb->SetTargetSight(AI_FOUNTAIN, AI_TARGET);
			bb->SetMasterAction(AI_TASK_USE_FOUNT);
			bb->SetMasterMovement(AI_MOVE_INTERACT);
			return true;
		}
	}
	return false;
}


// ================================================================================================= //
//
//	CHECK SEE POTION TO CATCH
//
// ================================================================================================= //


CheckSawPotion::CheckSawPotion(){}

bool CheckSawPotion::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckSawPotion\n";

	AIPlayer* character = bb->GetPlayer();
	if(!character->HasObject()){	// Comprobamos si el jugador no tiene pocion y le hace falta	
									// ir a por una
		int number = bb->GetNumberSight(AI_POTION);
		if(number>0){				// Miramos si la IA tiene alguna pocion en memoria para ir a por ella
			bb->SetTargetSight(AI_POTION, AI_TARGET);	// Ponemos el target
			bb->SetMasterAction(AI_TASK_CATCH_POT);		// Cambiamos la tarea
			bb->SetMasterMovement(AI_MOVE_INTERACT);	// Cambiamos el movimiento
			return true;
		}
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
	if(target!=nullptr){
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
//	OBSTACLE AVOIDANCE TASK
//
// ================================================================================================= //

ObstacleAvoidanceTask::ObstacleAvoidanceTask(){}

bool ObstacleAvoidanceTask::run(Blackboard* bb){
	if(DEBUG) std::cout<<"ObstacleAvoidanceTask\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(character != nullptr && target != nullptr){
		Kinematic cKin = character->GetKinematic();
		Kinematic tKin = target->kinematic;

		SteeringOutput steering = character->GetObstacleAvoid(cKin);
		if(steering.linear.length()!=0){
			character->SetForceToMove(steering.linear);
			steering = character->GetFace(cKin, tKin);
			character->SetForceToRotate(steering.angular);
			return true;
		}
	}

	return false;
}

// ================================================================================================= //
//
//	FACE TARGET
//
// ================================================================================================= //

FaceObject::FaceObject(){}

bool FaceObject::run(Blackboard* bb){
	if(DEBUG) std::cout<<"FaceObject\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(character!=nullptr && target!=nullptr){
		Kinematic cKin = character->GetKinematic();
		Kinematic tKin = target->kinematic;

		SteeringOutput steering = character->GetFace(cKin, tKin);
		
		character->SetForceToMove(steering.linear);
		character->SetForceToRotate(steering.angular);
		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK PLAYER ATTACK
//
// ================================================================================================= //

CheckPlayerAttack::CheckPlayerAttack(){}

bool CheckPlayerAttack::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckPlayerAttack\n";

	AIPlayer* character = bb->GetPlayer();
	if(character!=nullptr){
		float min = character->GetMinCostPM();
		float MP = character->GetMP();
		if(MP>min){
			// if(Meter condicion para atacar)
			bb->SetMasterAction(AI_TASK_SHOOT_SPELL);
		}
	}
	return true;
}

// ================================================================================================= //
//
//	CHECK PLAYER ESCAPE
//
// ================================================================================================= //

CheckPlayerEscape::CheckPlayerEscape(){}

bool CheckPlayerEscape::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckPlayerEscape\n";

	AIPlayer* character = bb->GetPlayer();
	float character_HP = character->GetHP();

	if(character_HP<25.0f){	// Si la vida del personaje es inferior al 25% escapa
		RoomGraph* room = bb->GetRoomGraph();
		if(room!=nullptr){
			// Ponemos la tarea y movimiento de escape
			bb->SetMasterAction(AI_TASK_ESCAPE);
			bb->SetMasterMovement(AI_MOVE_ESCAPE);
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
//	PLAYER HEARING
//
// ================================================================================================= //

PlayerHearing::PlayerHearing(){}

bool PlayerHearing::run(Blackboard* bb){
	if(DEBUG) std::cout<<"PlayerHearing\n";

	AIPlayer* character = bb->GetPlayer();
	if(character != nullptr){
		RoomGraph* room = bb->GetRoomGraph();
		if(room!=nullptr){
			bb->SetMasterAction(AI_TASK_TRAVEL);
			bb->SetMasterMovement(AI_MOVE_TARGETPATH);
		}else{
			bb->SetMasterAction(AI_TASK_DEFAULT);
			bb->SetMasterMovement(AI_MOVE_FACE);
		}

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
		// En el caso de que se haya visto a alguno de los enemigos decrementamos
		// el nivel de seguridad de la habitacion
		RoomGraph* room = bb->GetRoomGraph();
		if(room != nullptr){
			// Cambiamos el nivel de seguridad en funcion de los enemigos que haya visto
			// Quitamos 10 de seguridad por enemigo visto
			float delta = bb->GetFloat(AI_DELTA);
			float value = -10.0f*number*delta;
			room->ChangeSecurityLevel(value);
		}
		// Preparamos el target para las siguientes operaciones
		// Conseguimos el codigo de la IA del equipo enemigo
		AI_code enemy = (AI_code)(AI_PLAYER_WIZA - character->GetAlliance());
		bb->SetTargetSight(enemy, AI_TARGET);
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
	if(target!=nullptr){


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
	if(target!=nullptr){

		Kinematic cKin;
    	Kinematic tKin;

    	cKin = character->GetKinematic();
    	tKin = target->kinematic;

		SteeringOutput steering = character->GetObstacleAvoid(cKin);
		SteeringOutput steering2;

    	if(steering.linear.length() == 0){
    		steering = character->GetWander(cKin);

    		steering2 = character->GetFace(cKin, tKin);
    	}else{
    		steering2 = character->GetFace(cKin, tKin);
    	}

		character->SetForceToMove(steering.linear);
		character->SetForceToRotate(steering2.angular);

		return true;
	}
	return false;
}

// ================================================================================================= //
//
//	TARGET PATH
//
// ================================================================================================= //

TargetPath::TargetPath(){}

bool TargetPath::run(Blackboard* bb){
	if(DEBUG) std::cout<<"TargetPath\n";

	AIPlayer* character = bb->GetPlayer();
	Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
	if(character!=nullptr && target!=nullptr){
		Kinematic cKin = character->GetKinematic();

		character->ShortestPath(target->kinematic.position);

		SteeringOutput steering = character->GetFollowPath(cKin);

		character->SetForceToMove(steering.linear);
		character->SetForceToRotate(steering.angular);
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
	if(target!=nullptr){

		Kinematic cKin;
		Kinematic tKin;

    	cKin = character->GetKinematic();
    	tKin = target->kinematic;

		SteeringOutput steering = character->GetSeek(cKin, tKin);
		SteeringOutput steering2 = character->GetFace(cKin, tKin);

		character->SetForceToMove(steering.linear);
		character->SetForceToRotate(steering2.angular);

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
	if(target!=nullptr){

		Kinematic cKin;
    	Kinematic tKin;

    	cKin = character->GetKinematic();
    	tKin = target->kinematic;

		SteeringOutput steering = character->GetFlee(cKin, tKin);
		SteeringOutput steering2 = character->GetFace(cKin, tKin);

		character->SetForceToMove(steering.linear);
		character->SetForceToRotate(steering2.angular);

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

	SteeringOutput steering = character->GetObstacleAvoid(cKin);
	if(steering.linear.length() == 0){
		steering = character->GetWander(cKin);
	}else{
		SteeringOutput steering2 = character->GetLookWhereYoureGoing(cKin);
		steering.angular = steering2.angular;
	}
	
	character->SetForceToMove(steering.linear);
	character->SetForceToRotate(steering.angular);
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
		if(child!=nullptr){
			if(child->run(bb)){
				// En el caso de que este en medio de un casteo y quiero poner otro hechizo debera hacer release
				if(character->GetCastingSpell() && spellsOrder[i] != character->GetCurrentSpell()){
					return false; // El release se hace en otra tarea
				}else{
					character->SetCurrentSpell(spellsOrder[i]);
					bb->SetMasterMovement((AI_code)(AI_MOVE_SPELL00+spellsOrder[i]));
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
