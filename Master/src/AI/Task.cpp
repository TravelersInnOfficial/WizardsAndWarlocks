#include "Task.h"

bool Task::run(Blackboard* bb){
	std::cout<< "Run de la tarea"<<std::endl;
	return true;
}

bool Task::terminate(){
	std::cout<< "Terminate de la tarea"<<std::endl;
	return true;
}

Task::~Task(){
}
