#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include <vector3d.h>
#include "./../Entidad.h"

class GBody;
class Player;
class BT_Body;
class Kinematic;
class SoundEvent;

class Fountain: public Entidad{
public:
	Fountain(vector3df TPosition, vector3df TScale, vector3df TRotation);
	~Fountain();
	
	void Update();
	void Update(float deltaTime);
	void Interact(Player* p);
	void ShowInteractInfo();
	void SetFree();
	bool GetInUse();
	float GetPercentualValue();

	// Sensorial Functions
	void SendSignal();
	Kinematic GetKinematic();

	//Sound functions
	void playSoundEvent(SoundEvent* event);

private:
	void Recover();
	bool Use();
	void UpdatePosShape();
	void CreateFountain(vector3df TPosition, vector3df TScale, vector3df TRotation);
	void createSoundEvent();

	vector3df	rotation;			// Rotacion de la fuente

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
	SoundEvent*		useEvent;		// Sonido de fuente en uso
	SoundEvent*		cantUseEvent;	// Sonido de no poder utilizar la fuente
};

#endif