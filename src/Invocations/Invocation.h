#ifndef INVOCATION_H
#define INVOCATION_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>
#include <ColliderMasks.h>

#include <PhysicsEngine/BT_Body.h>
#include <GraphicEngine/GraphicEngine.h>
#include <SoundEngine/SoundSystem.h>
#include "./../Entidad.h"


class Invocation: public Entidad{
public:
	Invocation(int HP);
	~Invocation();
	virtual bool Update(float deltaTime);
	void Update();
	void ChangeHP(int value);
	virtual void Contact(void* punt, EntityEnum tipo);

	// Sense Functios
	void SendSignal();
	Kinematic GetKinematic();

protected:


	int 		m_HP;						// Vida de la invocacion

	BT_Body* 	bt_body;					// Cuerpo fisico de la invocacion
    GBody*    	m_invocationNode;			// Cuerpo visual de la invocacion
};


#endif