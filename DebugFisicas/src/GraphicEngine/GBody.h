#ifndef GBODY_H
#define GBODY_H

#include <irrlicht/irrlicht.h>
#include <iostream>
#include "vector3d.h"

class GBody{

public:

    GBody();
    
    /**
     * Set body position
     * pos: vector x,y,z of body position
    */
    void setPosition(vector3df pos);

    /**
     * Set body rotation
     * rot: vector x,y,z of body rotation
    */
    void setRotation(vector3df rot);
    
    /**
     * Set body scale
    */
    vector3df getScale();

    /**
     * Get body position
    */
    vector3df getPosition();

    /**
     * Get body rotation
    */
    vector3df getRotation();
    
    /**
     * Get body scale
    */
    void setScale(vector3df scale);
    /**
     * Set body texture
     * path: relative path of texture
    */
    void setMaterialTexture(std::string path);

    /**
     * Set body flag texture
     * flag: name of the flag
     * value: value of the flag (true/false)
    */
    void setMaterialFlag(std::string flag, bool value);

    friend class GraphicEngine;

private:

    GBody(irr::scene::ISceneNode* node);    
    irr::scene::ISceneNode* privateNode;
  
};

#endif