#ifndef GBODY_H
#define GBODY_H

#include <iostream>
#include "GEntity.h"
#include <MaterialTypes.h>
#include <MaterialFlags.h>

//fast forward declaration
class TFMesh;
class GBody: public GEntity{
	friend class GraphicEngine;
public:

	~GBody();

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

private:
	//GBody(irr::scene::ISceneNode* node);
	GBody(TFMesh* node);
	TFMesh* privateAnimatedMesh; 
	std::map<int, int> m_billboards;		// ID del billboard, ID en el motor
	//irr::scene::IBillboardTextSceneNode* board;
  
};

#endif