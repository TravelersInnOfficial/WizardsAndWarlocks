#ifndef MANAGERBULLET_H
#define MANAGERBULLET_H

#include <vector>
#include <string>
#include "./../Bullets/Proyectil.h"
#include "./../Bullets/BulletCodes.h"
#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class ManagerBullet{
public:
	static ManagerBullet* GetInstance();
	~ManagerBullet();

	void AddProyectil(vector3df pos, vector3df dir, float r, float v, BULLETCODE b);
	void AddToDeleteProyecil(Proyectil* pro);
	void Update();
	void DeleteProyectiles();
	void DeleteAllProyectiles();
private:

	vector<Proyectil*> proyectiles;
	vector<Proyectil*> proyectilesToDelete;

	ManagerBullet();
	static ManagerBullet* instance;
};


#endif