#ifndef GRAIL_H
#define GRAIL_H

#include <iostream>

#include "./../Entidad.h"
#include "./../Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class Grail: public Entidad{
public:
	Grail();
	~Grail();
	void Update();
	void Interact(Player* p);
private:
	bool casting;

	float 	timeCasting;
	float 	maxCasting;

	void CreateGrail();
	void UpdatePosShape();

	BT_Body*        bt_body;
    GBody*          m_grailNode;
};

#endif