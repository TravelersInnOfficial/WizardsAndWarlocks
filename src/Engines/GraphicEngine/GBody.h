#ifndef GBODY_H
#define GBODY_H

#include <iostream>
#include "GEntity.h"
#include <MaterialTypes.h>
#include <MaterialFlags.h>
#include <vector2d.h>

//fast forward declaration
class TFMesh;
class GBody: public GEntity{
	friend class GraphicEngine;
public:

	virtual ~GBody();

	/**
	 * Set body texture
	 * layer: layer of texture
	 * path: relative path of texture
	*/
	void setMaterialTexture(int layer, std::string path);
	
	void AddText(std::string text, vector3df position, int id = 0);

	void SetInvisible();

	void ChangeShader(SHADERTYPE shader);

	void SetTextureScale(vector2df value);

protected:
	//GBody(irr::scene::ISceneNode* node);
	GBody(TFMesh* node);
	
	std::map<int, int> m_billboards;		// ID del billboard, ID en el motor
  
};

#endif