#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <string>
#include <iostream>

class Entidad{
public:
	Entidad();
	virtual void Update();
	virtual void Contact(void* punt, string tipo);
	string GetClase();
	~Entidad();
protected:
	string clase;
private:

};

#endif