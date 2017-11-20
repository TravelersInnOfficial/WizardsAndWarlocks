#include "ControlProyectil.h"

ControlProyectil::ControlProyectil(btDiscreteDynamicsWorld* d, irr::scene::ISceneManager* s, irr::IrrlichtDevice* de, irr::video::IVideoDriver* dr){
	dynamicsWorld = d;
	sceneManager = s;
	device = de;
	driver = dr;
}

void ControlProyectil::AddProyectil(float dirX, float dirY, float dirZ, float r){
	proyectiles.push_back(new Proyectil(dirX, dirY, dirZ, r));
	proyectiles[proyectiles.size()-1]->CreateProyectil(dynamicsWorld, sceneManager, device, driver);
}

void ControlProyectil::Update(){
	int size = proyectiles.size();
	for(int i=0; i<size; i++){
		proyectiles[i]->Update();
	}
}

void ControlProyectil::AddToDeleteProyecil(Proyectil* pro){
	if(pro!=NULL){
	int size = proyectiles.size();
	for(int i=0; i<size; i++){
		Proyectil* p = proyectiles[i];
		if(p == pro){
			proyectiles.erase(proyectiles.begin()+i);
			break;
		}
	}
	proyectilesToDelete.push_back(pro);
	}
}

void ControlProyectil::DeleteProyectiles(){
	int size = proyectilesToDelete.size();
	for(int i=0; i<size; i++){
		Proyectil* p = proyectilesToDelete[i];
		//Quitar del mundo de Irrlicht y Bullet3D
		dynamicsWorld->removeCollisionObject(p->GetBody());
		sceneManager->addToDeletionQueue(p->GetShape());
		delete p;
	}
	proyectilesToDelete.clear();
}

Proyectil* ControlProyectil::GetProyectil(){
	if(proyectiles.size()!=0)
	return proyectiles[0];
	return NULL;
}
