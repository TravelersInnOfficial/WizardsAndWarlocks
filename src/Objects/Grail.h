#ifndef GRAIL_H
#define GRAIL_H

#include <iostream>

#include "./../Entidad.h"
#include "./../Players/Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../SoundEngine/SoundSystem.h"
#include <ColliderMasks.h>
#include <Alliance.h>

class Grail: public Entidad{
public:
	Grail(vector3df TPosition, vector3df TScale, vector3df TRotation);
	~Grail();
	void Update();
	void Update(float deltaTime);
	void Interact(Player* p);
	bool CheckIfWon();

	void drawGUI();

	void SendSignal();
	Kinematic GetKinematic();

	//Sound Functions
	void playEvent(vector3df pos);
	
private:
	bool playerOneInteraction;	// La esta pillando el jugador 1? Si/No
	bool recovered;				// El grial ha sido pillado? Si/No
	bool casting;				// Se esta pillando? Si/No

	float timeCasting;			// Tiempo que lleva casteando
	float maxCasting;			// Tiempo maximo de casteo

	void CreateGrail(vector3df TPosition, vector3df TScale, vector3df TRotation);
	void UpdatePosShape();
	void createSoundEvent();

	BT_Body*        bt_body;		// Cuerpo fisico del grial
    GBody*          m_grailNode;	// Cuerpo visual del grial
	SoundEvent* 	soundEvent;		// Sonidos del grial
};

#endif