#include "Decorador.h"

Decorador::Decorador(){

}

Decorador::~Decorador(){

}

bool Decorador::run(Blackboard* bb){
    cout<<"Run del Decorador básico"<<endl;
    if(child != NULL){
        return child->run(bb);
    }
    return false;
}

bool Decorador::terminate(){
    cout<<"Terminate del decorador básico"<<endl;
    return true;
}

void Decorador::setChild(Task* newChild){
    child = newChild;
}

//---------------------------------------------------------------------------------------

CountTime::CountTime(int n){
	contador = n;
	maxContador = n;
}

bool CountTime::run(Blackboard* bb){
	contador--;
	if(contador>=0){
		if(child!=NULL){
			return child->run(bb);
		}
		return false;
	}
	contador = maxContador;
	return false;
}
