#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <vector>
#include <string>
#include "./../Projectiles/Projectile.h"
#include <BulletCodes.h>
#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class BulletManager{
public:
	static BulletManager* GetInstance();
	~BulletManager();

	void AddProyectil(vector3df pos, vector3df dir, int emi, float dmgMult, BULLETCODE b);
	void AddToDeleteProyecil(Projectile* pro);
	void Update();
	void DeleteProyectiles();
	void DeleteAllProyectiles();
private:

	std::vector<Projectile*> proyectiles;
	std::vector<Projectile*> proyectilesToDelete;

	BulletManager();
	static BulletManager* instance;
};


#endif