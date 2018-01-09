#include "BehaviourTree.h"

BehaviourTree::BehaviourTree(){
    informacion = new Blackboard();

    PrepareSubTrees();
    CreateReceive();
    CreateAction();
    CreateMovement();
}

BehaviourTree::~BehaviourTree(){
	delete informacion;
}

void BehaviourTree::run(){
    rootRecive->run(informacion);
    rootAction->run(informacion);
    rootMove->run(informacion);
}

void BehaviourTree::SetRootReceive(Task* t){
    rootRecive = t;
}

void BehaviourTree::SetRootAction(Task* t){
    rootAction = t;
}

void BehaviourTree::SetRootMove(Task* t){
    rootMove = t;
}

void BehaviourTree::ResetInformacion(){
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

void BehaviourTree::SetPlayer(AIPlayer* p){
    informacion->SetPlayer(p);
}

// ====================================================================================== //
//
//  SUBARBOLES
//
// ====================================================================================== //

void BehaviourTree::CreateReceive(){

    Selector* sc_checkActions = new Selector();
    sc_checkActions->addChild(new CheckUsePotion());
    sc_checkActions->addChild(new CheckPlayerSight());
    sc_checkActions->addChild(new PutDefaultAction());

    Secuencia* sc_receive = new Secuencia();

    SetRootReceive(sc_receive);
    sc_receive->addChild(new SendAllSignals());
    sc_receive->addChild(sc_checkActions);
}

void BehaviourTree::CreateAction(){
    Task* t_masterAction = new MasterAction();
    SetRootAction(t_masterAction);
}

void BehaviourTree::CreateMovement(){
    Task* t_masterMovement = new MasterMovement();
    SetRootMove(t_masterMovement);
}

void BehaviourTree::PrepareSubTrees(){
    // Movimiento Default
    CreateMoveDefault();
    // Movimiento de los Hechizos
    CreateMoveSpell();
    // Lanzamiento de los hechizos
    CreateShootSpell();

    // DECLARANDO FUNCIONES DE ATAQUE
    Task* t_shootBasic = new UseSpell();
    informacion->SetPuntero(AI_TASK_SPELL00, t_shootBasic);

    informacion->SetPuntero(AI_TASK_SPELL01, t_shootBasic);
    informacion->SetPuntero(AI_TASK_SPELL02, t_shootBasic);

    Secuencia* sc_distance_spell = new Secuencia();
    sc_distance_spell->addChild(t_shootBasic);
    sc_distance_spell->addChild(new CheckDistance(1.0f));
    informacion->SetPuntero(AI_TASK_SPELL03, sc_distance_spell);


}

void BehaviourTree::CreateShootSpell(){
    Secuencia* sc_attack = new Secuencia();
    sc_attack->addChild(new SendPlayerSignals());
    sc_attack->addChild(new CheckPlayerSight());   
    sc_attack->addChild(new SpellSecuencia());  

    Decorador* d_attack = new ReleaseSpell();
    d_attack->setChild(sc_attack);

    informacion->SetPuntero(AI_TASK_SHOOT_SPELL, sc_attack);
}

void BehaviourTree::CreateMoveDefault(){
    Secuencia* sc_sight = new Secuencia();
    sc_sight->addChild(new CheckPlayerSight());
    sc_sight->addChild(new GoToTarget());
    sc_sight->addChild(new HasArrived());

    Secuencia* sc_hearing = new Secuencia();
    sc_hearing->addChild(new CheckPlayerHearing());
    sc_hearing->addChild(new FaceTarget());
    
    Selector* sl_movement = new Selector();
    sl_movement->addChild(sc_sight);
    sl_movement->addChild(sc_hearing);
    sl_movement->addChild(new T_Wander());

    informacion->SetPuntero(AI_MOVE_DEFAULT, sl_movement);
}

void BehaviourTree::CreateMoveSpell(){
    Secuencia* sc_moveToTarget = new Secuencia();
    sc_moveToTarget->addChild(new CheckDistance(8.0f));
    sc_moveToTarget->addChild(new FleeFromTarget());

    Selector* sl_moveShoot = new Selector();
    sl_moveShoot->addChild(sc_moveToTarget);
    sl_moveShoot->addChild(new GoToTarget());

    informacion->SetPuntero(AI_MOVE_SPELL00, sl_moveShoot);
    informacion->SetPuntero(AI_MOVE_SPELL01, sl_moveShoot);
    informacion->SetPuntero(AI_MOVE_SPELL02, sl_moveShoot);
    informacion->SetPuntero(AI_MOVE_SPELL03, sl_moveShoot);
}