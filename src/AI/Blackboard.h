#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "./RoomGraph/RoomGraph.h"

#include <iostream>
#include <map>
#include <AICodes.h>
#include <vector3d.h>
#include <kinematicTypes.h>
#include <vector>
#include <limits>

class AIPlayer;    

struct Sense_struct{
    int id;
    void* pointer;
	AI_code code;
	Kinematic kinematic;
	float duration;

    ~Sense_struct();

};

class Blackboard{
public:
    Blackboard();
    ~Blackboard();

    void LoadRoomGraph();
    void UpdateRoomGraph();
    void SetPlayer(AIPlayer* p);

    AIPlayer* GetPlayer();
    RoomGraph* GetRoomGraph();
    AI_code GetEnemyAlliance();
    //Information
    void*   GetPuntero(AI_code name);
 	int 	GetInt(AI_code name); 
    void    SetPuntero(AI_code name, void* value);
    void    SetInt(AI_code name, int value);
    void    CleanPuntero(AI_code name);
    //Senses
    void    CleanSense(int id);
    void    CleanSense();

    int     GetNumberSound(AI_code name);
    int     GetNumberSight(AI_code name);
    void    SetTargetSight(AI_code name, AI_code where);
    void    SetTargetSound(AI_code name, AI_code where);
 	void 	SetSound(int id, void* punt, AI_code name, Kinematic kin, float dur);
 	void	SetSight(int id, void* punt, AI_code name, Kinematic kin, float dur);

    void    SaveParent(Blackboard* parent);

    void    SetMasterAction(AI_code name);
    void    SetMasterMovement(AI_code name);

    int                         masterAction;           // Int que marca que subarbol de accion ha de utilizar
    int                         masterMovement;         // Int que marca que subarbol de movimiento ha de utilizar

private:

    AIPlayer*                   currentPlayer;          // Jugador al que le pertenece el blackboard
    RoomGraph*                  roomGraph;              // Grafo de las habitacion del mapa actual

    Blackboard* 			    blackboardParent;       // El blackboard padre del que pillar la informacion que pueda faltar

    std::map< AI_code, void* > 	dataPuntero;            // Vector de punteros del blackboard
    std::map< AI_code, int > 	dataInt;                // Vector de ints del blackboad

    std::vector<Sense_struct*>  soundSense;             // Vector con todas las cosas que haya OIDO
    std::vector<Sense_struct*>  sightSense;             // Vector con todas las cosas que haya VISTO


};

#endif