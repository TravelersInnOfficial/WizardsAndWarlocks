#ifndef POTION_H
#define POTION_H

#include "./../Entidad.h"
#include <vector3d.h>

class GRect;
class GBody;
class Player;
class GSprite;
class BT_Body;
class Kinematic;

class Potion:public Entidad{
public:
	~Potion();
	
	// functions from father
	void Update();
	void ShowInteractInfo();
	void ShowDropPotion();
	void Interact(Player* p);
	void NetInteract(Player* p);
	
	// functions
	void CreatePotion(vector3df TPosition, vector3df TRotation);
	void Drop(vector3df force);

	// child functions
	virtual void Use(Player* p)=0;
	virtual bool CheckUse(Player* p);		// Metodo para la IA que le indica si es "optimo" utilizarla, no se malgasta

	bool CheckUseHP(Player* p);
	bool CheckUseMP(Player* p);

	//Getters
	int GetValue();
	bool GetPickedState();
	Player* GetUser();
	vector3df GetPosition();
	std::string GetHUDTexture();

	// Setters
	void SetPosition(vector3df pos);

	// Sensorial Functions
	void SendSignal();
	Kinematic GetKinematic();

protected:
	int 		value;			// Cantidad de vida a curar
	bool 		picked;			// Ha sido pillada? Si/No

	Player* 	player;				// Player that posseses the potion

	// Variable almacenada para poder volver a formar el cuerpo una vez eliminado
	vector3df 	potionScale;	// Escala de la pocion

	BT_Body* 	bt_body; 		// Cuerpo fisico de la pocion
    GBody*      m_potionNode;	// Cuerpo visual de la pocion

	std::string m_info;			// Potion info
	std::string HUDTexturePath;	// Texture for the HUD to paint

	Potion(vector3df TScale, int value, std::string info);
	void DeletePotion();
	void UpdatePosShape();

	GRect* m_rect;
	GSprite* m_sprite;

	std::string m_potionpath;
};

#endif