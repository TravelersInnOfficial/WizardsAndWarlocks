#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <iostream>
#include <map>
#include <AICodes.h>

class Blackboard{
public:
    Blackboard();
    void*   GetPuntero(AI_code name);
 	int 	GetInt(AI_code name);  

    void    SetPuntero(AI_code name, void* value);
    void	SetInt(AI_code name, int value);

    void SaveParent(Blackboard* parent);
private:
    Blackboard* 			blackboardParent;
    
    std::map< AI_code, void* > 	dataPuntero;
    std::map< AI_code, int > 	dataInt;

};

#endif