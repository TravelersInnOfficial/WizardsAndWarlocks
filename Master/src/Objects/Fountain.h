#ifndef FOUNTAIN_H
#define FOUNTAIN_H


#include <iostream>

#include "./../Player.h"
#include "./../Entidad.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class Fountain: public Entidad{
public:
	Fountain();
	void Update(float deltaTime);
	void Interact(Player* p);
	void SetFree();
private:
	void Recover();
	bool Use();
	void UpdatePosShape();
	void CreateFountain();

	bool 		inUse;
	Player* 	user;

	int 		maxValue;			//Valor maximo fuente
	int 		value;				//Valor actual fuente
	int 		incrementUse;		//Incremento Use
	int 		incrementValue;		//Incremento Recover

	float 		currentTime;
	float		maxTime;

	BT_Body*        bt_body;
    GBody*          m_fountainNode;
};

#endif