#include "BehaviourTree.h"

BehaviourTree::BehaviourTree(){
    informacion = new Blackboard();

    CreateAttack();
    CreateMovement();

}

void BehaviourTree::CreateAttack(){
    // DECLARANDO FUNCIONES DE ATAQUE
    Task* t_shootBasic = new ShootBasic();
    informacion->SetPuntero(AI_TASK_SPELL00, t_shootBasic);
    informacion->SetPuntero(AI_TASK_SPELL01, t_shootBasic);
    informacion->SetPuntero(AI_TASK_SPELL02, t_shootBasic);
    informacion->SetPuntero(AI_TASK_SPELL03, t_shootBasic);

    // ATAQUE
    Secuencia* sc_attack = new Secuencia();
    sc_attack->addChild(new SendPlayerSignals());       // Primero envio la senyales sonoras y visuales
    sc_attack->addChild(new CheckPlayerSight());        // Comprobamos que vea al jugador
    sc_attack->addChild(new SpellSecuencia());          // Dispara

    Decorador* d_attack = new ReleaseSpell();
    d_attack->setChild(sc_attack);

    SetRootAttack(d_attack);
}

void BehaviourTree::CreateMovement(){
    // MOVIMIENTO
    Task* t_checkSight  = new CheckPlayerSight();
    Task* t_goToTarget  = new GoToTarget();
    Task* t_hasArrived  = new HasArrived();

    Secuencia* sc_sight = new Secuencia();
    sc_sight->addChild(t_checkSight);
    sc_sight->addChild(t_goToTarget);
    sc_sight->addChild(t_hasArrived);

    Task* t_checkHearing = new CheckPlayerHearing();
    Task* t_faceTarget = new FaceTarget();

    Secuencia* sc_hearing = new Secuencia();
    sc_hearing->addChild(t_checkHearing);
    sc_hearing->addChild(t_faceTarget);

    Task* t_wander = new T_Wander();
    
    Selector* sl_movement   = new Selector();
    sl_movement->addChild(sc_sight);
    sl_movement->addChild(sc_hearing);
    sl_movement->addChild(t_wander);

    SetRootMove(sl_movement);
}

BehaviourTree::~BehaviourTree(){
	delete informacion;
}

void BehaviourTree::run(){
    rootAttack->run(informacion);
    rootMove->run(informacion);
}

void BehaviourTree::SetRootAttack(Task* t){
    rootAttack = t;
}

void BehaviourTree::SetRootMove(Task* t){
    rootMove = t;
}

void BehaviourTree::ResetInformacion(){
    informacion->SetInt(AI_SPELL_STATUS, UP);
    informacion->CleanPuntero(AI_TARGET);
    informacion->CleanSense();
}

void BehaviourTree::SetInformation(Blackboard* bb){
    informacion->SaveParent(bb);
}

void BehaviourTree::AnyadirInformacion(AI_code name, void* value){
    informacion->SetPuntero(name, value);
}

void BehaviourTree::AnyadirInformacion(AI_code name, int value){
    informacion->SetInt(name, value);
}

Blackboard* BehaviourTree::GetBlackboard(){
	return informacion;
}
