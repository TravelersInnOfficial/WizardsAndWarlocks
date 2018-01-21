#include <iostream>
#include "./Managers/StateManager.h"


int main() {

	// MAIN MENU
	StateManager* state = StateManager::GetInstance();
	while(state->Update()){}

	delete state;
	
	return 0;
}