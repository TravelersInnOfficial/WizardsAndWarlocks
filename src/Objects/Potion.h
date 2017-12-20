#ifndef POTION_H
#define POTION_H

#include <iostream>
#include <ColliderMasks.h>

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
	
	// functions
	void CreatePotion(vector3df TPosition, vector3df TRotation);
	void Drop(vector3df force);

	//child functions
	virtual void Use(Player* p)=0;

protected:
	int 		value;
	bool 		picked;
	// Variable almacenada para poder volver a formar el cuerpo una vez eliminado
	vector3df 	potionScale;			
	std::string potionTexture;

	BT_Body*    bt_body;
    GBody*      m_potionNode;

protected:
	Potion(vector3df TScale, int value, std::string tex);
	void DeletePotion();
	void UpdatePosShape();
};

#endif