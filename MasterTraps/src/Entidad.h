#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <string>
#include <iostream>

using namespace std;

enum EntityEnum{
	EENUM_PLAYER = 0x01,
	EENUM_PROJECTILE = 0x02,
	EENUM_TRAP = 0x03,
	EENUM_SWITCH = 0x04,
	EENUM_DOOR = 0x05,
	EENUM_POTION = 0x06
};

class Player;

class Entidad{
public:
	Entidad();
	virtual void Update();
	virtual void Contact(void* punt, EntityEnum tipo);
	virtual void Interact(Player* p);
	EntityEnum GetClase();
	~Entidad();
protected:
	EntityEnum clase;
private:

};

#endif