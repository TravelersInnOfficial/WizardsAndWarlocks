#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <vector>
#include <string>
#include "./../Projectiles/Proyectil.h"
#include <BulletCodes.h>
#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class BulletManager{
public:
	static BulletManager* GetInstance();
	~BulletManager();

	void AddProyectil(vector3df pos, vector3df dir, float r, float v, int emi, BULLETCODE b);
	void AddToDeleteProyecil(Proyectil* pro);
	void Update();
	void DeleteProyectiles();
	void DeleteAllProyectiles();
private:

	vector<Proyectil*> proyectiles;
	vector<Proyectil*> proyectilesToDelete;

	BulletManager();
	static BulletManager* instance;
};


#endif