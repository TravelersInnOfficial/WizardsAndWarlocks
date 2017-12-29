#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include <iostream>

#include "./../Players/Player.h"
#include "./../Entidad.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include <ColliderMasks.h>

class Fountain: public Entidad{
public:
	Fountain(vector3df TPosition, vector3df TScale, vector3df TRotation);
	~Fountain();
	
	void Update();
	void Update(float deltaTime);
	void Interact(Player* p);
	void SetFree();
private:
	void Recover();
	bool Use();
	void UpdatePosShape();
	void CreateFountain(vector3df TPosition, vector3df TScale, vector3df TRotation);

	bool 		inUse;				// Esta en uso? Si/No
	Player* 	user;				// Jugador que la esta utilizando

	int 		maxValue;			// Valor maximo fuente
	int 		value;				// Valor actual fuente
	int 		incrementUse;		// Incremento Use
	int 		incrementValue;		// Incremento Recover

	float 		currentTime;		// Tiempo de casteo
	float		maxTime;			// Cada cuanto se curan los jugadores

	BT_Body*        bt_body;		// Cuerpo fisico de la fuente
    GBody*          m_fountainNode;	// Cuerpo visual de la fuente
};

#endif