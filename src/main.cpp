#include <iostream>
#include "./Managers/StateManager.h"

int main() {
	StateManager* state = StateManager::GetInstance();
	while(state->Update()){}
	delete state;
	return 0;
}