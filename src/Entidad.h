#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <string>
#include <iostream>

enum EntityEnum{
	EENUM_PLAYER 		= 0x01,
	EENUM_PROJECTILE 	= 0x02,
	EENUM_TRAP 			= 0x03,
	EENUM_SWITCH 		= 0x04,
	EENUM_DOOR 			= 0x05,
	EENUM_POTION 		= 0x06,
	EENUM_FOUNTAIN 		= 0x07,
	EENUM_FLOOR 		= 0x08,
	EENUM_NPC 			= 0x09,
	EENUM_INVOCATION 	= 0x0A,
	EENUM_PROP			= 0x0B,
	EENUM_CAMERA		= 0x0C,
	EENUM_DAMAGEAREA 	= 0x0D,
	EENUM_GRAIL			= 0x0E
};

class Player;

class Entidad{
public:
	Entidad();
	virtual void Contact(void* punt, EntityEnum tipo);
	virtual void Interact(Player* p);
	EntityEnum GetClase();
	int GetId();
	virtual ~Entidad();
protected:
	static int ITERADORENTIDAD;
	EntityEnum clase;
	int id;
private:

};

#endif