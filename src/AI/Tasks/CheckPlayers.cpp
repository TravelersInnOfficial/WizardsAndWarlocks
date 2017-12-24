#include "CheckPlayers.h"
#include "./../../Managers/PlayerManager.h"

CheckPlayers::CheckPlayers(){
}

bool CheckPlayers::run(Blackboard* bb){
	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		PlayerManager* masterPlayer = PlayerManager::GetInstance();
		masterPlayer->SendVisualSignal();

		int number = bb->GetNumberSight(AI_PLAYER);
		if(number>0){
			bb->SetTargetSight(AI_PLAYER, AI_TARGET);
			return true;
		}else{
			character->SetAngularForce(vector3df(0,0,0));
		}
	}
	return false;
}