#include "State.h"

State::State(){}

State::~State(){}

bool State::Input(){
	std::cout<<"INPUT DEL ESTADO"<<std::endl;
	return true;
}

void State::Update(){
	std::cout<<"UPDATE DEL ESTADO"<<std::endl;
}

void State::Draw(){
	std::cout<<"DRAW DEL ESTADO"<<std::endl;
}