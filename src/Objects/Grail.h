#ifndef GRAIL_H
#define GRAIL_H

#include <iostream>

#include "./../Entidad.h"
#include "./../Players/Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include <ColliderMasks.h>
#include <Alliance.h>

class Grail: public Entidad{
public:
	Grail(vector3df TPosition, vector3df TScale, vector3df TRotation);
	~Grail();
	void Update();
	void Update(float deltaTime);
	void Interact(Player* p);
private:
	bool casting;
	float deltaTime;
	float timeCasting;
	float maxCasting;

	void CreateGrail(vector3df TPosition, vector3df TScale, vector3df TRotation);
	void UpdatePosShape();

	BT_Body*        bt_body;
    GBody*          m_grailNode;
};

#endif