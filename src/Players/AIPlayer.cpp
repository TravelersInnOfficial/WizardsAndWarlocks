#include "AIPlayer.h"

#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../AI/SteeringBehaviour/AllSteerings.h"
#include "./../AI/Pathfinding/Pathfinding.h"
#include "./../AI/SenseManager/Sensor.h"
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GAnimation.h>
#include "./../AI/RoomGraph/RoomGraph.h"
#include "./../Managers/SpellManager.h"
#include "./../AI/BehaviourTree.h"
#include <PhysicsEngine/BT_Body.h>
#include "./PlayerController.h"
#include "./../AI/Blackboard.h"
#include <kinematicTypes.h>
#include <Constants.h>
#include <cmath>

AIPlayer::AIPlayer():Player(false){
	SpellManager* spellManager = SpellManager::GetInstance();
	spellManager->AddHechizo(0, this, SPELL_PROJECTILE);
	spellManager->AddHechizo(1, this, SPELL_FIRE);
	spellManager->AddHechizo(2, this, SPELL_WALL);
	spellManager->AddHechizo(3, this, SPELL_BLIZZARD);

	minCostPM = SpellManager::GetInstance()->GetMinCostPM(this);


	behaviour = new BehaviourTree();
	behaviour->SetPlayer(this);

	path = new Pathfinding();

	m_headPos = GetHeadPos();

	RegionalSenseManager* senseManager = RegionalSenseManager::GetInstance();
	sensor = senseManager->AddSensor(id, &m_headPos, &m_rotation, 0.0f, behaviour->GetBlackboard());

	shootSpell = false;
	castingSpell = false;
	elevation = 0;

	SetSteerings();
}

AIPlayer::~AIPlayer(){
	delete behaviour;
	delete path;
	RegionalSenseManager::GetInstance()->DeleteSensor(sensor);

	delete lookWhereYoureGoing;
	delete obstacleAvoidance;
	delete followPath;
	delete wander;
	delete align;
	delete face;
	delete seek;
	delete flee;
}

void AIPlayer::SetSteerings(){
	lookWhereYoureGoing = new LookWhereYoureGoing();
	obstacleAvoidance = new ObstacleAvoidance();
	followPath = new FollowPath(path);
	wander = new Wander();
	align = new Align();
	face = new Face();
	seek = new Seek();
	flee = new Flee();
}

void AIPlayer::InitGame(){
	// Inicializamos las variables de lanzar hechizos propias del IAPlayer
	shootSpell = false;
	castingSpell = false;
	// Reseteamos la informacion del BehaviourTree
	behaviour->ResetInformacion();
	// Reseteamos las notificaciones que queden en el sensor
	RegionalSenseManager::GetInstance()->ResetSensor(sensor);
	// Hacemos que la IA cargue la habitacion
	behaviour->LoadRoomGraph();
	// Hacemos init del player
	Player::InitGame();
}

void AIPlayer::RestartMatchStatus(){
	// Inicializamos las variables de lanzar hechizos propias del IAPlayer
	shootSpell = false;
	castingSpell = false;
	// Resetemos la informacion del BehaviourTree
	behaviour->ResetInformacion();
	// Reseteamos las notificaciones que queden en el sensor
	RegionalSenseManager::GetInstance()->ResetSensor(sensor);
	// Hacemos que la IA cargue la habitacion
	behaviour->UnloadRoomGraph();
	// Hacemos RestartMatchStatus
	Player::RestartMatchStatus();
}

// STEERINGS
static bool DEBUG_STR = false;
SteeringOutput AIPlayer::GetLookWhereYoureGoing(Kinematic cKin){
	if(DEBUG_STR) std::cout<<"LookWhereYoureGoing\n";
	return lookWhereYoureGoing->GetSteering(cKin);
}

SteeringOutput AIPlayer::GetObstacleAvoid(Kinematic cKin){
	if(DEBUG_STR) std::cout<<"ObstacleAvoid\n";
	return obstacleAvoidance->GetSteering(cKin);
}

SteeringOutput AIPlayer::GetWander(Kinematic cKin){
	if(DEBUG_STR) std::cout<<"Wander\n";
	return wander->GetSteering(cKin);
}

SteeringOutput AIPlayer::GetFace(Kinematic cKin, Kinematic tKin){
	if(DEBUG_STR) std::cout<<"Face\n";
	return face->GetSteering(cKin, tKin);
}

SteeringOutput AIPlayer::GetSeek(Kinematic cKin, Kinematic tKin){
	if(DEBUG_STR) std::cout<<"Seek\n";
	return seek->GetSteering(cKin, tKin);
}

SteeringOutput AIPlayer::GetFlee(Kinematic cKin, Kinematic tKin){
	if(DEBUG_STR) std::cout<<"Flee\n";
	return flee->GetSteering(cKin, tKin);
}

SteeringOutput AIPlayer::GetAlign(Kinematic cKin, Kinematic tKin){
	if(DEBUG_STR) std::cout<<"Align\n";
	return align->GetSteering(cKin, tKin);
}

SteeringOutput AIPlayer::GetFollowPath(Kinematic cKin){
	if(DEBUG_STR) std::cout<<"FollowPath\n";
	SteeringOutput output = followPath->GetSteering(cKin);
	output.angular = lookWhereYoureGoing->GetSteering(cKin).angular;
	return output;
}

void AIPlayer::ResetValues(){
	SetAllInput(UP);
	forceToMove = vector3df(0,0,0);
	forceToRotate = vector2df(0,0);
	m_rotation.X = -elevation;
}

Kinematic AIPlayer::GetKinematic(){
	Kinematic cKin;
	cKin.position = GetHeadPos();
	cKin.orientation =  vector2df(GetRot());
	cKin.orientation.X = elevation;
   	cKin.velocity = GetVelocity();
    cKin.rotation = vector2df(GetAngularVelocity());
    return cKin;
}

void AIPlayer::Update(float deltaTime){
	if(m_hasCharacter){
		ResetValues();
		behaviour->Update(deltaTime);
		Steering2Controller(deltaTime);
		shootSpell = false; 	// Reseteamos la variable
		Player::Update(deltaTime);		// Check Inputs
		m_headPos = GetHeadPos();		// Tiene que calcularse a posteriori para que coincida el valor con el del sensor
	}
}

void AIPlayer::DeadUpdate(){
	
}

void AIPlayer::SetAngularForce(vector3df v){
	if(m_hasCharacter){
		bt_body->SetAngularVelocity(v);
	}
}

void AIPlayer::Die(){
	behaviour->ResetInformacion();
	RegionalSenseManager::GetInstance()->ResetSensor(sensor);
	Player::Die();
	elevation = 0;
}

// ========================================================================================= //
//
//	CONTROLLER FUNCTINS
//
// ========================================================================================= //


void AIPlayer::CheckInput(){
	// Movimiento
	if(m_controller->IsKeyDown(ACTION_MOVE_LEFT)){ this->MoveX(-1); }
	if(m_controller->IsKeyDown(ACTION_MOVE_DOWN)){ this->MoveZ(-1); }
	if(m_controller->IsKeyDown(ACTION_MOVE_RIGHT)){ this->MoveX(1); }
	if(m_controller->IsKeyDown(ACTION_MOVE_UP)){ this->MoveZ(1); }
	if(m_controller->IsKeyPressed(ACTION_JUMP)){ this->Jump(); }
	// Acciones
	if(m_controller->IsKeyDown(ACTION_RAYCAST)){ this->Raycast(); }
	if(m_controller->IsKeyPressed(ACTION_USE_OBJECT)){ this->UseObject();}
	if(m_controller->IsKeyPressed(ACTION_DROP_OBJECT)){ this->DropObject(); }
	// Hechizos
	if(m_controller->IsKeyPressed(ACTION_SHOOT)){ 
		if(StartSpell()){
			castingSpell = true;
		} 
	}
	if(m_controller->IsKeyReleased(ACTION_SHOOT)){ 
		ResetSpell();
		castingSpell = false;
	}
	if(m_controller->IsKeyDown(ACTION_SHOOT)){ 
		if(ShootSpell()){
			shootSpell = true;
		} 
	}
	//if(m_controller->IsKeyReleased(ACTION_CHANGE_SPELL_UP)){ ChangeCurrentSpell(1); }
	//if(m_controller->IsKeyReleased(ACTION_CHANGE_SPELL_DOWN)){ ChangeCurrentSpell(-1); }
	// Trampas
	if(m_controller->IsKeyPressed(ACTION_DEPLOY_TRAP)){ this->DeployTrap(); }
}

void AIPlayer::SetForceToMove(vector3df force){
	forceToMove = force;
}

vector3df AIPlayer::GetForceToMove(){
	return forceToMove;
}

void AIPlayer::SetForceToRotate(vector2df force){
	forceToRotate = force;
}

vector2df AIPlayer::GetForceToRotate(){
	return forceToRotate;
}


void AIPlayer::Steering2Controller(float deltaTime){
	// CAMBIAMOS LA POSICION DEL PLAYER, EL MOVIMIENTO
	// Primero de todo sacamos el angulo que forman las fuerzas X e Z
	if(forceToMove.length()>5){
		float dir = atan2(forceToMove.X, forceToMove.Z);
		// Luego le restamos la rotacion propia del personaje 
		dir = m_rotation.Y - dir;
		dir = dir*180.0f/M_PI;						// Lo pasamos a Grados para que sea más intuitivo operar con ellos

		if(dir<-180) dir += 360;					// Comprobamos que ningun valor se salga de [-180, 180]
		if(dir> 180) dir -=360;

		float tempdir = std::abs(dir);					// Para controlar si se mueve arriba o abajo es más facil con el valor absoluto		
		if(tempdir<=60){
			SetController(ACTION_MOVE_UP, DOWN);
		}
		else if(tempdir>=120){
			SetController(ACTION_MOVE_DOWN, DOWN);
		}

		if(dir>=30 && dir<=150){					// En el caso de derecha, izquierda lo hacemos directamente con el valor
			SetController(ACTION_MOVE_LEFT, DOWN);
		}
		else if(dir>=-120 && dir<=-30){
			SetController(ACTION_MOVE_RIGHT, DOWN);
		}
	}
	// CAMBIAMOS LA ROTACION EN Y DE LA IA, EL GIRO BASICO
	SetAngularForce(vector3df( 0 ,forceToRotate.Y, 0));

	// CAMBIAMOS LA ROTACION EN X DE LA IA, MIRAR ARRIBA/ABAJO
	float changeElevation = (forceToRotate.X/2) * deltaTime;
	elevation += changeElevation;
}

// ========================================================================================= //
//
//	PATHFINDING
//
// ========================================================================================= //

void AIPlayer::ShortestPath(vector3df to){
	// Reset del comportamiento de movimiento al realizar un nuevo path
	RoomGraph* room = behaviour->GetRoomGraph();
	if(room!=nullptr){
		vector3df firstC = room->GetFirstCorner();
		vector3df secondC = room->GetSecondCorner();
		vector3df from = m_position;
		if(path->AStar(from,to, firstC, secondC)){
			followPath->ResetValues();
		}
	}
}

// ========================================================================================= //
//
//	GETTERS
//
// ========================================================================================= //

vector2di AIPlayer::GetActionMoveIA(){
	vector2di output(0,0);
	output.X = behaviour->GetBlackboard()->masterAction;
	output.Y = behaviour->GetBlackboard()->masterMovement;
	return output;
}

int AIPlayer::GetCurrentSpell(){
	return m_currentSpell;
}

bool AIPlayer::GetShootSpell(){
	return shootSpell;
}

bool AIPlayer::GetCastingSpell(){
	return castingSpell;
}

float AIPlayer::GetMinCostPM(){
	return minCostPM;
}

// ========================================================================================= //
//
//	SETTERS
//
// ========================================================================================= //

void AIPlayer::SetCurrentSpell(int num){
	m_currentSpell = num;
}

void AIPlayer::Debug(){
	if(m_hasCharacter){
		GraphicEngine* g_engine = GraphicEngine::getInstance();

		vector3df p = GetHeadPos();
		vector3df l = m_rotation;
		vector3df c = vector3df(1,1,1);
		vector3df o = vector3df(0,0,0);

		// Cono de vision
		if(false){
			vector3df o = vector3df(p.X+sin(l.Y+0.5235)*3, p.Y + sin(-l.X+0.5235)*3, p.Z+cos(l.Y+0.5235)*3 );
			g_engine->paintLineDebug(p, o, c);
			o = vector3df(p.X+sin(l.Y-0.5235)*3, p.Y + sin(-l.X+0.5235)*3, p.Z+cos(l.Y-0.5235)*3 );
			g_engine->paintLineDebug(p, o, c);
			o = vector3df(p.X+sin(l.Y+0.5235)*3, p.Y + sin(-l.X-0.5235)*3, p.Z+cos(l.Y+0.5235)*3 );
			g_engine->paintLineDebug(p, o, c);
			o = vector3df(p.X+sin(l.Y-0.5235)*3, p.Y + sin(-l.X-0.5235)*3, p.Z+cos(l.Y-0.5235)*3 );
			g_engine->paintLineDebug(p, o, c);
		}

		// Rayos verdes
		float lookAHead = 2.5f;
		float lookAHead2 = 1.0f;
		if(false){
			c = vector3df(0,1,0);
			o = vector3df(p.X+sin(l.Y)*lookAHead, p.Y, p.Z+cos(l.Y)*lookAHead); 
			g_engine->paintLineDebug(p, o, c);
			o = vector3df(p.X+sin(l.Y + M_PI/4)*lookAHead2, p.Y, p.Z+cos(l.Y + M_PI/4)*lookAHead2);
			g_engine->paintLineDebug(p, o, c);
			o = vector3df(p.X+sin(l.Y - M_PI/4)*lookAHead2, p.Y, p.Z+cos(l.Y - M_PI/4)*lookAHead2);
			g_engine->paintLineDebug(p, o, c);
		}

		// Altura de vision
		if(true){
			c = vector3df(0,0,1);
			o = vector3df(p.X+sin(l.Y)*lookAHead, p.Y+sin(elevation)*lookAHead, p.Z+cos(l.Y)*lookAHead);
			g_engine->paintLineDebug(p, o, c);
		}

		if(true){
			p = m_position;
			p.Y -= 0.7;
			vector3df toMove = GetForceToMove();
			toMove.Y = 0;
			toMove.normalize();
			toMove = toMove * 1.0f;

			o = p + toMove;
			g_engine->paintLineDebug(p, o, c);
		}

		//Varibles IA
		if(false){
			// Esta disparando hechizo? CastingSpell
			c = vector3df(255*castingSpell, 0, 0);
			// Ha conseguido lanzar el hechizo? ShootSpell
			c = vector3df(0, 255*shootSpell, 0);
			// CurrentSpell
			if(m_currentSpell == 0) 		c = vector3df(255,0,0);
			else if(m_currentSpell == 1)	c = vector3df(0,255,0);
			else if(m_currentSpell == 2)	c = vector3df(0,0,255);
			else if(m_currentSpell == 3)	c = vector3df(255,255,0);
		}
		//Comportamientos IA
		if(true){
			vector2di values = GetActionMoveIA();
			std::string text = AICode_str[values.X] + " / " + AICode_str[values.Y];
			m_playerNode->EditText(text);
		}
	}
}