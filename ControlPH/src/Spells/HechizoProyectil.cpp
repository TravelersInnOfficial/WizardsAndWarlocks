#include "HechizoProyectil.h"
#include "./../ControlProyectil.h"

HechizoProyectil::HechizoProyectil(int costPM, float tCast, float tCoolDown):Hechizo(costPM, tCast, tCoolDown){
	//Hechizo(costPM, tCast, tCoolDown);
}

void HechizoProyectil::Lanzar(Player* p){
	//GraphicEngine* g = GraphicEngine::getInstance();
	//cout<<g->getTime()<<endl;
	
	//std::cout<<"Has lanzado el hechizo"<<std::endl;
	vector3df rot = p->GetRot();

	rot.X = -rot.X;

	vector3df pos = p->GetPos();
	pos.X = pos.X + sin(rot.Y)*cos(rot.X)*1;
	pos.Y = pos.Y + sin(rot.X)*1;
	pos.Z = pos.Z + cos(rot.Y)*cos(rot.X)*1;

	ControlProyectil* c = ControlProyectil::GetInstance();
	c->AddProyectil(pos, vector3df( sin(rot.Y), sin(rot.X), cos(rot.Y)), 0.05f, 10.0f, BULLET_BASIC);
}