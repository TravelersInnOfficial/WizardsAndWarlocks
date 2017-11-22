#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <string>
#include <iostream>

class Entidad{
public:
	Entidad();
	virtual void Update();
	virtual void Contact(void* punt, std::string tipo);
	std::string GetClase();
	~Entidad();
protected:
	std::string clase;
private:

};

#endif