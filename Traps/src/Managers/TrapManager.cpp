#include "TrapManager.h"

TrapManager* TrapManager::instance = 0;

TrapManager* TrapManager::GetInstance(){
	if(instance==0){
		instance = new TrapManager();
	}
	return instance;
}

void TrapManager::Update(float deltaTime){
	UpdateTrap(deltaTime);
}

TrapManager::TrapManager(){}

TrapManager::~TrapManager(){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		delete t;
	}
	traps.clear();
}

Trap* TrapManager::AddTrap(vector3df pos, vector3df normal, TrapEnum type){
	Trap* t = new Trap(pos, normal, type);
	traps.push_back(t);
	return t;
}

void TrapManager::DeployTrap(TrapEnum type){
	std::map<int,std::vector<vector3df>> NodePointData = GraphicEngine::getInstance()->Raycast();
	int ID = 0;
	vector3df point(0,0,0);
	vector3df normal(0,0,0);

	std::map<int,std::vector<vector3df>>::iterator it = NodePointData.begin();
	for(; it != NodePointData.end(); ++it){
		ID = it->first;
		std::vector<vector3df> PointData = it->second;
		std::vector<vector3df>::iterator pointIt = PointData.begin();
		for (; pointIt != PointData.end(); ++pointIt){
			normal = PointData.at(0);
			point = PointData.at(1);
		}
	}
	std::cout<<"point X: "<<point.X<<" Y: "<<point.Y<<" Z: "<<point.Z<<std::endl;
	std::cout<<"normal X: "<<normal.X<<" Y: "<<normal.Y<<" Z: "<<normal.Z<<std::endl;

	if(!(normal.X == 0 && normal.Y != 0 && normal.Z == 0)
	&& !(normal.X == 0 && normal.Y == 0 && normal.Z == 0)
	&& !(normal.X == 90 && normal.Y == 0 && normal.Z == 0)
	){
		AddTrap(point,vector3df(-normal.X,-normal.Y,-normal.Z),type);
	}
}

void TrapManager::DeleteTrap(Trap* trap){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		if(t == trap){
			traps.erase(traps.begin() + i);
			delete t;
		}
	}
}

void TrapManager::UpdateTrap(float deltaTime){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		t->Update(deltaTime);
	}
}