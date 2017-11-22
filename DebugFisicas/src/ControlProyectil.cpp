#include "ControlProyectil.h"

ControlProyectil* ControlProyectil::instance = 0;

ControlProyectil::ControlProyectil(){}

ControlProyectil* ControlProyectil::GetInstance(){
	if(instance==0){
		instance = new ControlProyectil();
	}
	return instance;
}

void ControlProyectil::AddThings(btDiscreteDynamicsWorld* d, irr::scene::ISceneManager* s, irr::IrrlichtDevice* de, irr::video::IVideoDriver* dr){
	dynamicsWorld = d;
	sceneManager = s;
	device = de;
	driver = dr;
}

void ControlProyectil::AddProyectil(float dirX, float dirY, float dirZ, float r, float v){
	proyectiles.push_back(new Proyectil(dirX, dirY, dirZ, r, v));
	proyectiles[proyectiles.size()-1]->CreateProyectil(dynamicsWorld, sceneManager, device, driver);
}

void ControlProyectil::Update(){
	int size = proyectiles.size();
	for(int i=0; i<size; i++){
		proyectiles[i]->Update();
	}
}

/**
 * @brief [Mueve los objetos proyectil del vector de Update al de delete]
 * @details [long description]
 */
void ControlProyectil::AddToDeleteProyecil(Proyectil* pro){
	// Comprueba que el proyectil no sea NULL
	if(pro!=NULL){
		// Nos guardamos el tamanyo el vector
		int size = proyectiles.size();
		// Recorremos el vector de proyectil en update
		for(int i=0; i<size; i++){
			Proyectil* p = proyectiles[i];
			// Comprobamos si el proyectil esta en el vector
			if(p == pro){
				// En el caso de que este lo borramos del primer vector y lo anyadimos en el de eliminar
				proyectiles.erase(proyectiles.begin()+i);
				proyectilesToDelete.push_back(pro);
				// Salimos del bucle
				break;
			}
		}
	}
}

/**
 * @brief [Eliminamos todos los proyectiles que esten en el vector]
 * @details [long description]
 */
void ControlProyectil::DeleteProyectiles(){
	// Nos guardamos el tamanyo del vector
	int size = proyectilesToDelete.size();
	// Recorremos el vector de proyectiles por eliminar
	for(int i=0; i<size; i++){
		// Cargamos el proyectil a eliminar
		Proyectil* p = proyectilesToDelete[i];
		// Lo eliminamos del mundo de Irrlicht y Bullet3D
		dynamicsWorld->removeCollisionObject(p->GetBody());
		sceneManager->addToDeletionQueue(p->GetShape());
		// Delete del proyectil
		delete p;
	}
	// Limpiamos el vector de proyectiles por eliminar
	proyectilesToDelete.clear();
}

void ControlProyectil::DeleteAllProyectiles(){
	// =====================================
	//  PROYECTILES
	// =====================================
	int size = proyectiles.size();							// Nos guardamos el tamanyo del vector
	for(int i=0; i<size; i++){								// Recorremos el vector de proyectiles por eliminar
		Proyectil* p = proyectiles[i];						// Cargamos el proyectil a eliminar
		dynamicsWorld->removeCollisionObject(p->GetBody());	// Lo eliminamos del mundo de Irrlicht y Bullet3D
		sceneManager->addToDeletionQueue(p->GetShape());
		delete p;											// Lo eliminamos del mundo de Irrlicht y Bullet3D
	}
	proyectiles.clear();									// Limpiamos el vector
	// =====================================
	// PROYECTILESTODELETE
	// =====================================
	size = proyectilesToDelete.size();					// Nos guardamos el tamanyo del vector
	for(int i=0; i<size; i++){								// Recorremos el vector de proyectiles por eliminar
		Proyectil* p = proyectilesToDelete[i];				// Cargamos el proyectil a eliminar
		dynamicsWorld->removeCollisionObject(p->GetBody());	// Lo eliminamos del mundo de Irrlicht y Bullet3D
		sceneManager->addToDeletionQueue(p->GetShape());
		delete p;											// Delete del proyectil
	}
	proyectilesToDelete.clear();							// Limpiamos el vector
}
