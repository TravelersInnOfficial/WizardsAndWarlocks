#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include "GEntity.h"
#include <vector3d.h>

class ObjectManager;

class Light: public GEntity{
	friend class GraphicEngine;
	friend class ObjectManager;

public:
	~Light();
	void SetActiveState(bool active);
	void ChangeColor(vector3df color);
	void ChangeAttenuation(float attenuation);

private:
	Light(vector3df TPosition, vector3df Color, float range);
  
};

#endif