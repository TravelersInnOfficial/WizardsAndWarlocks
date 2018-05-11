#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <BulletCodes.h>
#include <EffectCodes.h>
#include <vector>
#include <map>

class Projectile;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;


class BulletManager{
public:
	static BulletManager* GetInstance();
	~BulletManager();
	void InitObject();
	void EmptyObject();

	void AddProyectil(vector3df pos, vector3df rot, vector3df dir, int emi, float dmgMult, BULLETCODE b, EFFECTCODE bulletEffect = WEAK_BASIC);
	void AddToDeleteProyecil(Projectile* pro);
	void Update();
	void DeleteProyectiles();
	void DeleteAllProyectiles();

	std::map<std::string, BULLETCODE> GetBULLETCODE_StrMap();
	float GetBulletDamage(BULLETCODE bullet);
private:

	std::vector<Projectile*> proyectiles;
	std::vector<Projectile*> proyectilesToDelete;
	std::map<BULLETCODE, float> bullet_damage;
	std::map<BULLETCODE, float> bullet_radius;
	std::map<BULLETCODE, float> bullet_speed;
	std::map<BULLETCODE, float> bullet_max_distance;

	BulletManager();
	BulletManager(BulletManager&);
	BulletManager operator =(BulletManager&);

	void loadProjectileDamage();
};


#endif