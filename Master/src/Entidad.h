#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <string>
#include <iostream>

using namespace std;

class Entidad{
public:
	Entidad();
	virtual void Update();
	virtual void Contact(void* punt, string tipo);
	virtual void Interact();
	string GetClase();
	~Entidad();
protected:
	string clase;
private:

};

#endif