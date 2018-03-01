#ifndef IRRGBODY_H
#define IRRGBODY_H

#include <iostream>
#include "IrrGEntity.h"
#include <MaterialTypes.h>
#include <MaterialFlags.h>

class IrrGBody: public IrrGEntity{

public:

	IrrGBody();
	~IrrGBody();

	/**
	 * Set body texture
	 * layer: layer of texture
	 * path: relative path of texture
	*/
	void setMaterialTexture(int layer, std::string path);

	/**
	 * Sets the texture of the specified layer in all materials of this scene node to the new texture
	 * flag: Which flag of all materials to be set
	 * value: value of the flag (true/false)
	*/
	void setMaterialFlag(MATERIAL_FLAG flag, bool value);

	/**
	 * Sets the material type of all materials in this scene node to a new material type
	 * type: New type of material to be set.
	*/
	void setMaterialType(MATERIAL_TYPE type);
	
	void Remove();

	void AddText(std::string text, vector3df position, int id = 0);

	friend class IrrEngine;

private:

	IrrGBody(irr::scene::ISceneNode* node);
	irr::scene::IAnimatedMesh* privateAnimatedMesh; 
	irr::scene::IBillboardTextSceneNode* board;
  
};

#endif