#ifndef GRIAL_H
#define GRIAL_H

#include <iostream>

#include "./../Entidad.h"
#include "./../Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class Grial: public Entidad{
public:
	Grial();
	~Grial();
	void Update();
	void Interact(Player* p);
private:
	bool casting;

	float	startTime;
	float 	timeCasting;
	float 	currentCasting;

	void CreateGrial();
	void UpdatePosShape();

	BT_Body*        bt_body;
    GBody*          m_grialNode;
};

#endif