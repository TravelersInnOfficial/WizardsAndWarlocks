#ifndef POTION_H
#define POTION_H

#include <iostream>
#include <ColliderMasks.h>
#include <kinematicTypes.h>

#include "./../Players/Player.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../PhysicsEngine/BT_Body.h"
class Player;

class Potion:public Entidad{
public:
	~Potion();
	
	// functions from father
	void Update();
	void Interact(Player* p);
	void NetInteract(Player* p);
	
	// functions
	void CreatePotion(vector3df TPosition, vector3df TRotation);
	void Drop(vector3df force);
	void DrawHUD();

	// child functions
	virtual void Use(Player* p)=0;
	virtual bool CheckUse(Player* p);		// Metodo para la IA que le indica si es "optimo" utilizarla, no se malgasta

	bool CheckUseHP(Player* p);
	bool CheckUseMP(Player* p);

	//Getters
	int GetValue();

	// Sensorial Functions
	void SendSignal();
	Kinematic GetKinematic();

protected:
	int 		value;			// Cantidad de vida a curar
	bool 		picked;			// Ha sido pillada? Si/No

	// Variable almacenada para poder volver a formar el cuerpo una vez eliminado
	vector3df 	potionScale;	// Escala de la pocion
	std::string potionTexture;	// Textura de la pocion

	BT_Body* 	bt_body; 		// Cuerpo fisico de la pocion
    GBody*      m_potionNode;	// Cuerpo visual de la pocion

	std::string HUDTexturePath;	// Texture for the HUD to paint

	Potion(vector3df TScale, int value, std::string tex);
	void DeletePotion();
	void UpdatePosShape();
};

#endif