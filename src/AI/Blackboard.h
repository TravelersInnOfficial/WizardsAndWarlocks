#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <iostream>
#include <map>
#include <AICodes.h>
#include <vector3d.h>
#include <kinematicTypes.h>
#include <vector>

class AIPlayer;     // No se van a hacer uso de sus funcionalidad, por eso se puede anyadir de esta forma

struct Sense_struct{
    int id;
	AI_code code;
	Kinematic kinematic;
    float length;
	float duration;

    ~Sense_struct();

};

class Blackboard{
public:
    Blackboard();
    ~Blackboard();

    void SetPlayer(AIPlayer* p);
    AIPlayer* GetPlayer();
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
 	void 	SetSound(int id, AI_code name, Kinematic kin, float len, float dur);
 	void	SetSight(int id, AI_code name, Kinematic kin, float len, float dur);

    void SaveParent(Blackboard* parent);
private:

    AIPlayer*                   currentPlayer;          // Jugador al que le pertenece el blackboard

    Blackboard* 			     blackboardParent;      // El blackboard padre del que pillar la informacion que pueda faltar
    
    std::map< AI_code, void* > 	dataPuntero;            // Vector de punteros del blackboard
    std::map< AI_code, int > 	dataInt;                // Vector de ints del blackboad

    std::vector<Sense_struct*>  soundSense;             // Vector con todas las cosas que haya OIDO
    std::vector<Sense_struct*>  sightSense;             // Vector con todas las cosas que haya VISTO


};

#endif