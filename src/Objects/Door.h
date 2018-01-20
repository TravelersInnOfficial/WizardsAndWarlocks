#ifndef DOOR_H
#define DOOR_H

#include <iostream>

#include "./../Entidad.h"
#include "./../Players/Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../SoundEngine/SoundSystem.h"
#include <ColliderMasks.h>

class Door: public Entidad{
public:
	Door(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	~Door();
	void Interact(Player* p);
	void Interact();
	void Update();
	void SetBlock(bool bl);

	// Sense Functions
	void SendSignal();
	Kinematic GetKinematic();

	// Sound Functions
	void playOpen();
	void playClose();

	// Sync Functions
	void NetInteract();
	void ForceOpen();

	bool GetOpenState();

private:
	void CreateDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	void WorkDoor();
	void UpdatePosShape();
	void createSoundEvents();	//Create the sound events needed for the player

	bool 		working;		// Se esta abriendo o cerrando? Si/No
	bool		isOpen;			// Estado de la puerta, abierta o cerrada? true/false
	bool 		block; 			// La puerta tiene el interact de player bloqueado? Si/No
	int 		min;			// Minimo angulo a rotar
	int 		max;			// Maximo angulo a rotar
	int 		increment;		// Incremento a cada frame

	vector3df 	rotation;		// Rotacion actual de la puerta

	BT_Body*   	bt_body;		// Cuerpo fisico de la puerta
    GBody*    	m_doorNode;		// Cuerpo visual de la puerta

	std::map<std::string, SoundEvent*> soundEvents;		//Sound events
};

#endif