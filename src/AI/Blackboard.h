#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <iostream>
#include <map>
#include <AICodes.h>
#include <vector3d.h>
#include <kinematicTypes.h>
#include <vector>

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
    //Information
    void*   GetPuntero(AI_code name);
 	int 	GetInt(AI_code name); 
    void    SetPuntero(AI_code name, void* value);
    void    SetInt(AI_code name, int value);
    void    CleanPuntero(AI_code name);
    //Senses
    void    CleanSense(int id);

    int     GetNumberSound(AI_code name);
    int     GetNumberSight(AI_code name);
    void    SetTargetSight(AI_code name, AI_code where);
    void    SetTargetSound(AI_code name, AI_code where);
 	void 	SetSound(int id, AI_code name, Kinematic kin, float len, float dur);
 	void	SetSight(int id, AI_code name, Kinematic kin, float len, float dur);

    void SaveParent(Blackboard* parent);
private:

    Blackboard* 			blackboardParent;
    
    std::map< AI_code, void* > 	dataPuntero;
    std::map< AI_code, int > 	dataInt;

    //Escucha
    std::vector<Sense_struct*> soundSense;
    //Vista
    std::vector<Sense_struct*> sightSense;


};

#endif