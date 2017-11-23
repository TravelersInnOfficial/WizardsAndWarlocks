#ifndef CONTROLPROYECTIL_H
#define CONTROLPROYECTIL_H

#include <vector>
#include <string>
#include "Proyectil.h"

#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"

class ControlProyectil{
public:
	static ControlProyectil* GetInstance();

	void AddProyectil(float dirX, float dirY, float dirZ, float r, float v);
	void AddToDeleteProyecil(Proyectil* pro);
	void Update();
	void DeleteProyectiles();
	void DeleteAllProyectiles();
private:

	vector<Proyectil*> proyectiles;
	vector<Proyectil*> proyectilesToDelete;

	ControlProyectil();
	static ControlProyectil* instance;
};


#endif