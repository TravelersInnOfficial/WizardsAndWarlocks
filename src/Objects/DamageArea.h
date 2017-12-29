#ifndef DAMAGEAREA_H
#define DAMAGEAREA_H

#include <iostream>
#include <vector3d.h>
#include <ColliderMasks.h>

#include "./../PhysicsEngine/BT_GhostObject.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../Entidad.h"

class DamageArea: public Entidad{
public:
	DamageArea(int dam, vector3df TPosition, vector3df TScale, vector3df TRotation);
	~DamageArea();
	bool Update(float deltaTime);
	void Update();
	void Contact(void* punt, EntityEnum tipo);
	void Deactivate();

	void SetPosition(vector3df TPosition);
	void SetRotation(vector3df TRotation);

private:
	void CreateDamageArea(vector3df TPosition, vector3df TScale, vector3df TRotation);

	bool 		activated;			// Esta activa? Deberia seguir existiendo? Si/No

	int 		damage; 			// Danyo que hacer la area

	float		currentCast;		// Tiempo que lleva de Casteo
	float 		casting;			// Tiempo total de casteo (intervalo de tiempo entre daño y daño)

	BT_GhostObject* 	bt_body;			// Cuerpo fisico de la invocacion
};

#endif