
#include "BehaviourTree.h"

#include "Blackboard.h"
#include "./Tasks/AllTasks.h"

BehaviourTree::BehaviourTree(){
    informacion = new Blackboard();

    rootRecive = nullptr;
    rootAction = nullptr;
    rootMove = nullptr;

    PrepareSubTrees();
    CreateReceive();
    CreateAction();
    CreateMovement();
}

BehaviourTree::~BehaviourTree(){
	delete informacion;

    int size = tasks.size();
    for(int i=0; i<size; i++){
        Task* t = tasks[i];
        delete t;
    }
    tasks.clear();

    if(rootRecive != nullptr) delete rootRecive;
    if(rootAction != nullptr) delete rootAction;
    if(rootMove   != nullptr) delete rootMove;

    rootRecive = nullptr;
    rootAction = nullptr;
    rootMove   = nullptr;
}

void BehaviourTree::Update(float deltaTime){
    informacion->SetFloat(AI_DELTA, deltaTime);
    informacion->UpdateRoomGraph(deltaTime);
    Run();
}

void BehaviourTree::Run(){
    rootRecive->run(informacion);
    rootAction->run(informacion);
    rootMove->run(informacion);
}

void BehaviourTree::SetRootReceive(Task* t){
    if(rootRecive != nullptr) delete rootRecive;
    rootRecive = t;
}

void BehaviourTree::SetRootAction(Task* t){
    if(rootAction != nullptr) delete rootAction;
    rootAction = t;
}

void BehaviourTree::SetRootMove(Task* t){
    if(rootMove != nullptr) delete rootMove;
    rootMove = t;
}

void BehaviourTree::ResetInformacion(){
    informacion->CleanPuntero(AI_TARGET);
    informacion->CleanSense();
}

void BehaviourTree::LoadRoomGraph(){
    informacion->LoadRoomGraph();
}

void BehaviourTree::UnloadRoomGraph(){
    informacion->UnloadRoomGraph();
}

void BehaviourTree::SetInformation(Blackboard* bb){
    informacion->SaveParent(bb);
}

void BehaviourTree::AnyadirInformacion(AI_code name, void* value){
    informacion->SetPuntero(name, value);
}

void BehaviourTree::AnyadirInformacion(AI_code name, float value){
    informacion->SetFloat(name, value);
}

Blackboard* BehaviourTree::GetBlackboard(){
	return informacion;
}

RoomGraph* BehaviourTree::GetRoomGraph(){
    return informacion->GetRoomGraph();
}

void BehaviourTree::SetPlayer(AIPlayer* p){
    informacion->SetPlayer(p);
}

// ====================================================================================== //
//
//  SUBARBOLES
//
// ====================================================================================== //

// ObstacleAvoidanceTask

void BehaviourTree::CreateReceive(){

    Selector* sl_checkSightPlayer = new Selector();          // Selector para las comprobacion de vision de player
    sl_checkSightPlayer->addChild(new CheckPlayerEscape());
    sl_checkSightPlayer->addChild(new CheckDirectVision());
    sl_checkSightPlayer->addChild(new CheckPlayerAttack());

    Secuencia* sc_seePlayers = new Secuencia();         // Secuencia de comprobaciones por vision de player
    sc_seePlayers->addChild(new CheckPlayerSight());
    sc_seePlayers->addChild(sl_checkSightPlayer);

    Selector* sl_checkHearPlayer = new Selector();
    sl_checkHearPlayer->addChild(new CheckPlayerEscape());
    sl_checkHearPlayer->addChild(new PlayerHearing());

    Secuencia* sc_hearPlayers = new Secuencia();
    sc_hearPlayers->addChild(new CheckPlayerHearing());
    sc_hearPlayers->addChild(sl_checkHearPlayer);


    Selector* sc_checkActions = new Selector();
    sc_checkActions->addChild(new CheckUsePotion());    // Comprobamos si usar una pocion
    sc_checkActions->addChild(sc_seePlayers);           // Comprobamos si estamos viendo algun jugador
    sc_checkActions->addChild(sc_hearPlayers);          // Comprobamos si estamos oyendo algun jugador
    sc_checkActions->addChild(new CheckSawTrap());      // Comprobamos si hemos visto alguna trampa
    sc_checkActions->addChild(new CheckSawPotion());    // Comprobamos si hemos visto alguan fuente
    sc_checkActions->addChild(new CheckSawFountain());  // Comprobamos si hemos visto alguna fuente
    sc_checkActions->addChild(new CheckGrailSeen());    // Comprobamos si hemos visto el grial
    sc_checkActions->addChild(new CheckExplore());      // Comprobamos si hemos explorado la habitacion
    sc_checkActions->addChild(new CheckTravel());       // Viajamos a la siguiente sala
    sc_checkActions->addChild(new PutDefaultAction());  // Accion por defecto

    SetRootReceive(sc_checkActions);
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
    // No movimiento
    CreateNoMove();
    // Movimiento esquivando
    CreateMoveAvoid();
    // Tarea por defecto
    CreateTaskDefault();
    // Encarar target
    CreateMoveFace();
    // Movimiento Default
    CreateMoveDefault();
    // Movimiento a un target
    CreateMoveToTarget();
    // Movimiento a un target con path
    CreatePathToTarget();
    // Movimiento de los Hechizos
    CreateMoveSpell();
    // Lanzamiento de los hechizos
    CreateShootSpell();
    // Coger Pocion
    CreateCathPotion();
    // Beber Pocion
    CreateDrinkPotion();
    // Usar fuente
    CreateUseFountain();
    // Desactivar Trampa
    CreateDefuseTrap();
    // Movimiento de Escapar
    CreateEscapeMove();
    // Tarea de Escapar
    CreateEscapeTask();
    // Explorar
    CreateExploreMove();
    // Explorar Tarea
    CreateExploreTask();
    // Travel Task
    CreateTravelTask();
    // Travel Move 
    CreateTravelMove();
    // Move Open Door
    CreateOpenDoor();
    // Move use fountain
    CreateMoveFountain();
    // Catch grail
    CreateCatchGrail();
    // Move to Interact
    CreateMoveInteract();

    // DECLARANDO FUNCIONES DE ATAQUE
    Task* t_shootBasic = new UseSpell();
    informacion->SetPuntero(AI_TASK_SPELL00, t_shootBasic);
    informacion->SetPuntero(AI_TASK_SPELL01, t_shootBasic);
    informacion->SetPuntero(AI_TASK_SPELL02, t_shootBasic);

    Secuencia* sc_distance_spell = new Secuencia();
    sc_distance_spell->addChild(new CheckDistance(1.0f));
    sc_distance_spell->addChild(new UseGuivernum());
    informacion->SetPuntero(AI_TASK_SPELL03, sc_distance_spell);

    tasks.push_back(t_shootBasic);
    tasks.push_back(sc_distance_spell);
}

void BehaviourTree::CreateShootSpell(){
    Secuencia* sc_attack = new Secuencia();
    sc_attack->addChild(new SpellSecuencia());  

    Decorador* d_attack = new ReleaseSpell();
    d_attack->setChild(sc_attack);

    informacion->SetPuntero(AI_TASK_SHOOT_SPELL, d_attack);
    tasks.push_back(d_attack);
}

void BehaviourTree::CreateTaskDefault(){
    Task* t = new EmptyTask();
    informacion->SetPuntero(AI_TASK_DEFAULT, t);
    tasks.push_back(t);
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
    tasks.push_back(sl_movement);
}

void BehaviourTree::CreateMoveSpell(){
    Secuencia* sc_dontMove = new Secuencia();
    sc_dontMove->addChild(new CheckDistance(9.0f));
    sc_dontMove->addChild(new FaceObject());

    Secuencia* sc_moveToTarget = new Secuencia();
    sc_moveToTarget->addChild(new CheckDistance(8.0f));
    sc_moveToTarget->addChild(new FleeFromTarget());

    Selector* sl_moveShoot = new Selector();
    sl_moveShoot->addChild(sc_moveToTarget);    // [0-8] Apartarse
    sl_moveShoot->addChild(sc_dontMove);        // [8-9] Nada
    sl_moveShoot->addChild(new GoToTarget());   // [9- ] Acercarse

    informacion->SetPuntero(AI_MOVE_SPELL00, sl_moveShoot);
    informacion->SetPuntero(AI_MOVE_SPELL01, sl_moveShoot);
    informacion->SetPuntero(AI_MOVE_SPELL02, sl_moveShoot);
    informacion->SetPuntero(AI_MOVE_SPELL03, sl_moveShoot);

    tasks.push_back(sl_moveShoot);
}

void BehaviourTree::CreateCathPotion(){
    Secuencia* sc_catchPotion = new Secuencia();
    sc_catchPotion->addChild(new CheckDistance(2.5f));  // Distancia del raycast
    sc_catchPotion->addChild(new CatchPotion());

    informacion->SetPuntero(AI_TASK_CATCH_POT, sc_catchPotion);
    tasks.push_back(sc_catchPotion);
}

void BehaviourTree::CreateMoveFace(){
    Task* t = new FaceTarget();
    informacion->SetPuntero(AI_MOVE_FACE, t);
    tasks.push_back(t);
}

void BehaviourTree::CreateMoveToTarget(){
    Task* t = new GoToTarget();
    informacion->SetPuntero(AI_MOVE_GOTARGET, t);
    tasks.push_back(t);
}

void BehaviourTree::CreatePathToTarget(){
    Secuencia* sc_pathToTarget = new Secuencia();
    sc_pathToTarget->addChild(new TargetPath());
    sc_pathToTarget->addChild(new CheckJump());

    informacion->SetPuntero(AI_MOVE_TARGETPATH, sc_pathToTarget);
    tasks.push_back(sc_pathToTarget);
}

void BehaviourTree::CreateDrinkPotion(){
    Task* t = new UsePotion();
    informacion->SetPuntero(AI_TASK_DRINK_POT, t);
    tasks.push_back(t);
}

void BehaviourTree::CreateUseFountain(){
    Secuencia* sc_useFountain = new Secuencia();
    sc_useFountain->addChild(new CheckDistance(2.5f));  // Distancia del raycast
    sc_useFountain->addChild(new UseFountain());

    informacion->SetPuntero(AI_TASK_USE_FOUNT, sc_useFountain);
    tasks.push_back(sc_useFountain);
}

void BehaviourTree::CreateDefuseTrap(){
    Secuencia* sc_defuseTrap = new Secuencia();
    sc_defuseTrap->addChild(new CheckDistance(2.5f));   // Distancia del raycast
    sc_defuseTrap->addChild(new DefuseTrap());

    informacion->SetPuntero(AI_TASK_DEFUSE_TRAP, sc_defuseTrap);
    tasks.push_back(sc_defuseTrap);
}

void BehaviourTree::CreateNoMove(){
    Task* t = new NoMove();
    informacion->SetPuntero(AI_MOVE_NO, t);
    tasks.push_back(t);
}

void BehaviourTree::CreateEscapeTask(){
    Task* t = new EmptyTask();
    informacion->SetPuntero(AI_TASK_ESCAPE, t);
    tasks.push_back(t);
}

void BehaviourTree::CreateEscapeMove(){
    Secuencia* sc_escape = new Secuencia();
    sc_escape->addChild(new MoveEscape());
    sc_escape->addChild(new CheckJump());

    informacion->SetPuntero(AI_MOVE_ESCAPE, sc_escape);
    tasks.push_back(sc_escape);
}

void BehaviourTree::CreateExploreTask(){
    Task* t = new CheckDoorInFront(2.0f);
    informacion->SetPuntero(AI_TASK_EXPLORE, t);
    tasks.push_back(t);
}

void BehaviourTree::CreateExploreMove(){
    Secuencia* sc_whereExplore = new Secuencia();
    sc_whereExplore->addChild(new WhereExplore());
    sc_whereExplore->addChild(new CheckJump());

    informacion->SetPuntero(AI_MOVE_EXPLORE, sc_whereExplore);
    tasks.push_back(sc_whereExplore);
}

void BehaviourTree::CreateTravelTask(){
    Secuencia* sc_travelTask = new Secuencia();
    //sc_travelTask->addChild(new EmptyTask());
    sc_travelTask->addChild(new CheckDoorInFront(2.0f));

    informacion->SetPuntero(AI_TASK_TRAVEL, sc_travelTask);
    tasks.push_back(sc_travelTask);
}

void BehaviourTree::CreateTravelMove(){
    Secuencia* sc_travelMove = new Secuencia();
    sc_travelMove->addChild(new TravelRoom());
    sc_travelMove->addChild(new CheckDoorWay(1.0f));
    sc_travelMove->addChild(new CheckJump());

    informacion->SetPuntero(AI_MOVE_TRAVEL, sc_travelMove);
    tasks.push_back(sc_travelMove);
}

void BehaviourTree::CreateOpenDoor(){
    Task* t = new NoMove();
    informacion->SetPuntero(AI_MOVE_OPEN_DOOR, t);
    tasks.push_back(t);
}

void BehaviourTree::CreateMoveFountain(){
    Secuencia* sc_moveFountain = new Secuencia();
    sc_moveFountain->addChild(new CheckDistance(1.5f));
    sc_moveFountain->addChild(new NoMove());

    Selector* sc_selectOption = new Selector();
    sc_selectOption->addChild(sc_moveFountain);
    sc_selectOption->addChild(new TargetPath());

    informacion->SetPuntero(AI_MOVE_FOUNTAIN, sc_selectOption);
    tasks.push_back(sc_selectOption);
}

void BehaviourTree::CreateCatchGrail(){
    Secuencia* sc_catchGrail = new Secuencia();
    sc_catchGrail->addChild(new CheckDistance(2.0f));   // Distancia del raycast
    sc_catchGrail->addChild(new CatchGrail());

    informacion->SetPuntero(AI_TASK_CATCH_GRAIL, sc_catchGrail);
    tasks.push_back(sc_catchGrail);
}

void BehaviourTree::CreateMoveInteract(){
    Secuencia* sc_moveInteract = new Secuencia();
    sc_moveInteract->addChild(new CheckDistance(2.0f)); // Distancia del raycast
    sc_moveInteract->addChild(new FaceObject());

    Secuencia* sc_movePath = new Secuencia();
    sc_movePath->addChild(new TargetPath());
    sc_movePath->addChild(new CheckJump());

    Selector* sc_moveTarget = new Selector();
    sc_moveTarget->addChild(sc_moveInteract);
    sc_moveTarget->addChild(new CheckDoorInFront(2.0f));
    sc_moveTarget->addChild(sc_movePath);

    informacion->SetPuntero(AI_MOVE_INTERACT, sc_moveTarget);
    tasks.push_back(sc_moveTarget);
}

void BehaviourTree::CreateMoveAvoid(){
    Selector* sl_moveAvoid = new Selector();
    sl_moveAvoid->addChild(new ObstacleAvoidanceTask());
    sl_moveAvoid->addChild(new TargetPath());

    informacion->SetPuntero(AI_MOVE_AVOID, sl_moveAvoid);
    tasks.push_back(sl_moveAvoid);
}
