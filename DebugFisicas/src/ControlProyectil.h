#ifndef CONTROLPROYECTIL_H
#define CONTROLPROYECTIL_H

#include <vector>
#include <string>
#include <irrlicht/irrlicht.h>
#include <btBulletDynamicsCommon.h>

#include "Proyectil.h"

class ControlProyectil{
public:
	static ControlProyectil* GetInstance();

	void AddThings(btDiscreteDynamicsWorld* d, irr::scene::ISceneManager* s, irr::IrrlichtDevice* de, irr::video::IVideoDriver* dr);
	void AddProyectil(float dirX, float dirY, float dirZ, float r, float v);
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

	ControlProyectil();
	static ControlProyectil* instance;
};


#endif