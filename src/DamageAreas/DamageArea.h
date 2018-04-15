#ifndef DAMAGEAREA_H
#define DAMAGEAREA_H

#include <iostream>
#include <vector3d.h>
#include <ColliderMasks.h>

#include <PhysicsEngine/BT_GhostObject.h>
#include <GraphicEngine/GraphicEngine.h>
#include "./../Entidad.h"

class DamageArea: public Entidad{
public:
	DamageArea(float dam, float cast, vector3df TPosition, vector3df TScale, vector3df TRotation);
	~DamageArea();

	virtual bool Update(float deltaTime);
	void Update();

	virtual void Contact(void* punt, EntityEnum tipo);

	virtual void SetPosition(vector3df TPosition);
	void SetRotation(vector3df TRotation);

protected:
	int 		damage; 			// Danyo que hacer la area

	float		currentCast;		// Tiempo que lleva de Casteo
	float 		casting;			// Tiempo total de casteo (intervalo de tiempo entre daño y daño)

	GBody*      		m_areaNode;			// Cuerpo visual del area de danyo
	BT_GhostObject* 	bt_body;			// Cuerpo fisico del area de danyo
};

#endif