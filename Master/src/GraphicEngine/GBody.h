#ifndef GBODY_H
#define GBODY_H

#include <iostream>
#include "GEntity.h"
#include <MaterialTypes.h>
#include <MaterialFlags.h>

class GBody: public GEntity{

public:

    GBody();
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

    void setTriangleSelector();

    friend class GraphicEngine;

private:

    GBody(irr::scene::ISceneNode* node);
    irr::scene::IAnimatedMesh* privateAnimatedMesh;    
  
};

#endif