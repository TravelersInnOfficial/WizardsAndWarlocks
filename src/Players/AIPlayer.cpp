#include "AIPlayer.h"
#include "./../Managers/SpellManager.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../GraphicEngine/GraphicEngine.h"

AIPlayer::AIPlayer():Player(false){
	behaviour = new BehaviourTree();
	behaviour->AnyadirInformacion(AI_CHARACTER, this);
	behaviour->AnyadirInformacion(AI_SPELL_STATUS, UP);

	RegionalSenseManager* senseManager = RegionalSenseManager::GetInstance();
	sensor = senseManager->AddSensor(id, &m_position, &rotation, 0.0f, behaviour->GetBlackboard());
}

AIPlayer::~AIPlayer(){
	delete behaviour;
}

void AIPlayer::Update(){
	if(hasCharacter){
		SetAllInput(UP);
		behaviour->run();
		// En el caso de que se cumpla alguna de las condiciones de muerte lo matamos
		Player::Update();
	}
}

void AIPlayer::SetForces(vector3df v){
	if(hasCharacter){
		bt_body->ApplyCentralImpulse(v);
	}
}

void AIPlayer::SetAngularForce(vector3df v){
	if(hasCharacter){
		bt_body->SetAngularVelocity(v);
	}
}

void AIPlayer::Die(){
	behaviour->ResetInformacion();
	RegionalSenseManager::GetInstance()->ResetSensor(sensor);
	Player::Die();
}

void AIPlayer::Debug(){
	if(hasCharacter){
		vector3df p = m_position;
		vector3df l = rotation;
		vector3df c = vector3df(1,1,1);

		vector3df o = vector3df(p.X+sin(l.Y+0.5235)*3, p.Y + sin(-l.X+0.5235)*3, p.Z+cos(l.Y+0.5235)*3 );
		GraphicEngine::getInstance()->paintLineDebug(p, o, c);
		o = vector3df(p.X+sin(l.Y-0.5235)*3, p.Y + sin(-l.X+0.5235)*3, p.Z+cos(l.Y-0.5235)*3 );
		GraphicEngine::getInstance()->paintLineDebug(p, o, c);
		o = vector3df(p.X+sin(l.Y+0.5235)*3, p.Y + sin(-l.X-0.5235)*3, p.Z+cos(l.Y+0.5235)*3 );
		GraphicEngine::getInstance()->paintLineDebug(p, o, c);
		o = vector3df(p.X+sin(l.Y-0.5235)*3, p.Y + sin(-l.X-0.5235)*3, p.Z+cos(l.Y-0.5235)*3 );
		GraphicEngine::getInstance()->paintLineDebug(p, o, c);


		float lookAHead = 2.5f;
		float lookAHead2 = 1.0f;
		c = vector3df(0,1,0);
		o = vector3df(p.X+sin(l.Y)*lookAHead, p.Y, p.Z+cos(l.Y)*lookAHead); 
		GraphicEngine::getInstance()->paintLineDebug(p, o, c);
		o = vector3df(p.X+sin(l.Y + M_PI/4)*lookAHead2, p.Y, p.Z+cos(l.Y + M_PI/4)*lookAHead2);
		GraphicEngine::getInstance()->paintLineDebug(p, o, c);
		o = vector3df(p.X+sin(l.Y - M_PI/4)*lookAHead2, p.Y, p.Z+cos(l.Y - M_PI/4)*lookAHead2);
		GraphicEngine::getInstance()->paintLineDebug(p, o, c);
	}
}

// ========================================================================================= //
//
//	CONTROLLER FUNCTINS
//
// ========================================================================================= //


void AIPlayer::CheckInput(){
	// Movimiento
	if(controller->IsKeyDown(ACTION_MOVE_LEFT)){ this->MoveX(-1); }
	if(controller->IsKeyDown(ACTION_MOVE_DOWN)){ this->MoveZ(-1); }
	if(controller->IsKeyDown(ACTION_MOVE_RIGHT)){ this->MoveX(1); }
	if(controller->IsKeyDown(ACTION_MOVE_UP)){ this->MoveZ(1); }
	if(controller->IsKeyPressed(ACTION_JUMP)){ this->Jump(); }
	// Acciones
	if(controller->IsKeyDown(ACTION_RAYCAST)){ this->Raycast(); }
	if(controller->IsKeyPressed(ACTION_USE_OBJECT)){ this->UseObject();}
	if(controller->IsKeyPressed(ACTION_DROP_OBJECT)){ this->DropObject(); }
	// Hechizos
	//if(controller->IsKeyPressed(ACTION_SHOOT)){ SpellManager::GetInstance()->StartHechizo(currentSpell,this); }
	//if(controller->IsKeyReleased(ACTION_SHOOT)){ SpellManager::GetInstance()->ResetHechizo(currentSpell,this); }
	//if(controller->IsKeyDown(ACTION_SHOOT)){ SpellManager::GetInstance()->LanzarHechizo(currentSpell,this); }
	//if(controller->IsKeyReleased(ACTION_CHANGE_SPELL_UP)){ ChangeCurrentSpell(1); }
	//if(controller->IsKeyReleased(ACTION_CHANGE_SPELL_DOWN)){ ChangeCurrentSpell(-1); }
	// Trampas
	if(controller->IsKeyPressed(ACTION_DEPLOY_TRAP)){ this->DeployTrap(); }
}

void AIPlayer::SetController(ACTION_ENUM action, keyStatesENUM state){
	controller->SetStatus(action, state);
}

void AIPlayer::Steering2Controller(SteeringOutput steering){
	// Ahora mismo por muy pequenya que sea la fuerza la IA se mueve
	// Puede que en un futuro lo mejor sea comprobar el length del 
	// vector fuerza, para ver si es tan pequenyo que no deberia
	// moverse


	// Primero de todo sacamos el angulo que forman las fuerzas X e Z
	vector3df linear = steering.linear;
	float dir = atan2(linear.X, linear.Z);
	// Luego le restamos la rotacion propia del personaje 
	dir = rotation.Y - dir;
	dir = dir*180.0f/M_PI;						// Lo pasamos a Grados para que sea más intuitivo operar con ellos

	if(dir<-180) dir += 360;					// Comprobamos que ningun valor se salga de [-180, 180]
	if(dir> 180) dir -=360;

	float tempdir = abs(dir);					// Para controlar si se mueve arriba o abajo es más facil con el valor absoluto		
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

	vector2df angular = steering.angular;		// Como en el controlador aun no hay para la camara la fuerza angular se la ponemos a pelo
	SetAngularForce(vector3df( 0 ,angular.Y, 0));
}


// ========================================================================================= //
//
//	GETTERS
//
// ========================================================================================= //

int AIPlayer::GetCurrentSpell(){
	return currentSpell;
}