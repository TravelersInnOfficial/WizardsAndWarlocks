#include "Selector.h"

Selector::Selector(){
}

void Selector::addChild(Task* task){
	children.push_back(task);
}

void Selector::addChildren(vector<Task*> tasks){
	int tamanyo = tasks.size();
	for(int i=0; i<tamanyo; i++){
		addChild(tasks.at(i));
	}
}

bool Selector::run(Blackboard* bb){
	int tamanyo = children.size();
	for(int i=0; i<tamanyo; i++){
		Task* child = children.at(i);
		if(child->run(bb)){
			return true;
		}		
	}
	return false;
}

bool Selector::terminate(){
	std::cout<< "Terminate de la tarea"<<std::endl;
	return true;
}

RandomSelector::RandomSelector(){}

bool RandomSelector::run(Blackboard* bb){
	int tamanyo = children.size();
	shuffleVector();
	for(int i=0; i<tamanyo; i++){
		Task* child = children.at(i);
		if(child->run(bb)){
			return true;
		}		
	}
	return false;
}

void RandomSelector::shuffleVector(){
	int n = children.size();
	while(n>1){
		int k = rand() % n;
		n--;
		Task* temp = children.at(k);
		children.at(k) = children.at(n);
		children.at(n) = temp;
	}
}