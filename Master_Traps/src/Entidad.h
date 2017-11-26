#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <string>
#include <iostream>

enum EntityEnum: int{
	EENUM_PLAYER = 0,
	EENUM_PROJECTILE = 1,
	EENUM_TRAP = 2,
	EENUM_SWITCH = 3
};

class Entidad{
public:
	Entidad();
	virtual void Update();
	virtual void Contact(void* punt, EntityEnum tipo);
	EntityEnum GetClase();
	~Entidad();
protected:
	//std::string clase;
	EntityEnum clase;
private:

};

#endif