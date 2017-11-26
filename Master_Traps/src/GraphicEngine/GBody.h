#ifndef GBODY_H
#define GBODY_H

#include <irrlicht/irrlicht.h>
#include <iostream>
#include "GEntity.h"

class GBody: public GEntity{

public:

    GBody();
    
    /**
     * Set body texture
     * layer: layer of texture
     * path: relative path of texture
    */
    void setMaterialTexture(int layer, std::string path);

    /**
     * Set body flag texture
     * flag: name of the flag
     * value: value of the flag (true/false)
    */
    void setMaterialFlag(std::string flag, bool value);

    friend class GraphicEngine;

private:

    GBody(irr::scene::ISceneNode* node);    
  
};

#endif