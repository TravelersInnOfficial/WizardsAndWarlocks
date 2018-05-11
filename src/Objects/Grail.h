#ifndef GRAIL_H
#define GRAIL_H

#include <vector3d.h>
#include "./../Entidad.h"

class SoundEvent;
class Kinematic;
class BT_Body;
class GBody;
class GRect;

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

    struct HUD_bar{
        HUD_bar();
        ~HUD_bar();
        void Update(float time, float total);
        float bar_width;
        GRect* bkg;
        GRect* progress_bar;
    };

	HUD_bar* deactivation_bar;
	bool playerOneInteraction;	// La esta pillando el jugador 1? Si/No
	bool recovered;				// El grial ha sido pillado? Si/No
	bool casting;				// Se esta pillando? Si/No

	float timeCasting;			// Tiempo que lleva casteando
	float maxCasting;			// Tiempo maximo de casteo

	void CreateGrail(vector3df TPosition, vector3df TScale, vector3df TRotation);
	void UpdatePosShape();
	void createSoundEvent();
	void drawProgressBar();

	BT_Body*        bt_body;		// Cuerpo fisico del grial
    GBody*          m_grailNode;	// Cuerpo visual del grial
	SoundEvent* 	soundEvent;		// Sonidos del grial
};

#endif