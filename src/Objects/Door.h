#ifndef DOOR_H
#define DOOR_H

#include <iostream>

#include "./../Entidad.h"
#include "./../Players/Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include <ColliderMasks.h>

class Door: public Entidad{
public:
	Door(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	~Door();
	void Interact(Player* p);
	void Interact();
	void Update();
private:
	void CreateDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	void WorkDoor();
	void UpdatePosShape();

	bool 		working;		// Se esta abriendo o cerrando? Si/No
	int 		min;			// Minimo angulo a rotar
	int 		max;			// Maximo angulo a rotar
	int 		increment;		// Incremento a cada frame

	vector3df 	rotation;		// Rotacion actual de la puerta

	BT_Body*   	bt_body;		// Cuerpo fisico de la puerta
    GBody*    	m_doorNode;		// Cuerpo visual de la puerta
};

#endif