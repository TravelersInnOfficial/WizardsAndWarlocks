#include "HechizoProyectil.h"
#include "./../Managers/BulletManager.h"

HechizoProyectil::HechizoProyectil(int costPM, float tCast, float tCoolDown):Hechizo(costPM, tCast, tCoolDown){
	//Hechizo(costPM, tCast, tCoolDown);
}

void HechizoProyectil::Lanzar(Player* p){
	//GraphicEngine* g = GraphicEngine::getInstance();
	//cout<<g->getTime()<<endl;
	
	//std::cout<<"Has lanzado el hechizo"<<std::endl;
	vector3df rot = p->GetRot();

	rot.X = -rot.X;

	vector3df pos = p->GetHeadPos();
	float max = 1.2;
	pos.X = pos.X + sin(rot.Y)*cos(rot.X)*max;
	pos.Y = pos.Y + sin(rot.X)*max;
	pos.Z = pos.Z + cos(rot.Y)*cos(rot.X)*max;

	BulletManager* c = BulletManager::GetInstance();
	c->AddProyectil(pos, vector3df( sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X)), 0.05f, 30.0f, BULLET_BASIC);

	Hechizo::Lanzar(p);
}