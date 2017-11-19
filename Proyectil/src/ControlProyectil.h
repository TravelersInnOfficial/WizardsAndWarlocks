#ifndef CONTROLPROYECTIL_H
#define CONTROLPROYECTIL_H

#include <vector>
#include <string>
#include <irrlicht/irrlicht.h>
#include <btBulletDynamicsCommon.h>

#include "Proyectil.h"

class ControlProyectil{
public:
	ControlProyectil(btDiscreteDynamicsWorld* d, irr::scene::ISceneManager* s, irr::IrrlichtDevice* de, irr::video::IVideoDriver* dr);
	void AddProyectil(float dirX, float dirY, float dirZ, float r);
	void AddToDeleteProyecil(Proyectil* pro);
	void Update();
	void DeleteProyectiles();

	Proyectil* GetProyectil();
private:
	btDiscreteDynamicsWorld* dynamicsWorld;
	irr::scene::ISceneManager* sceneManager;
	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;

	vector<Proyectil*> proyectiles;
	vector<Proyectil*> proyectilesToDelete;
};


#endif