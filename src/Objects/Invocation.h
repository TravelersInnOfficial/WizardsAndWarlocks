#ifndef INVOCATION_H
#define INVOCATION_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>
#include <ColliderMasks.h>

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../SoundEngine/SoundSystem.h"
#include "./../Entidad.h"


class Invocation: public Entidad{
public:
	Invocation(int HP, float time, vector3df TPosition, vector3df TScale, vector3df TRotation);
	~Invocation();
	bool Update(float deltaTime);
	void Update();
	void ChangeHP(int value);
	void Contact(void* punt, EntityEnum tipo);

	// Sense Functios
	void SendSignal();
	Kinematic GetKinematic();

	//Sound Functions
	void playSpawnEvent(vector3df pos);

private:
	void CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation);
	void createSoundEvent();

	int 		m_HP;						// Vida de la invocacion
	float 		currentTime;				// Tiempo 

	BT_Body* 	bt_body;					// Cuerpo fisico de la invocacion
    GBody*    	m_invocationNode;			// Cuerpo visual de la invocacion
	SoundEvent* spawnEvent;					//Invocation sound event
	
};


#endif