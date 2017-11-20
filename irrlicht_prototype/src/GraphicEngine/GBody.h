#ifndef GBODY_H
#define GBODY_H

#include <irrlicht/irrlicht.h>
#include "vector3d.h"

class GBody{

public:

    GBody();
    GBody(irr::scene::ISceneNode* node);
    
    /**
     * Set body position
    */
    void setPosition(vector3df pos);

    /**
     * Set body rotation
    */
    void setRotation(vector3df rot);
    
    /**
     * Set body scale
    */
    void setScale(vector3df scale);

    /**
     * Set body texture
    */
    //void setTexture();


private:

    irr::scene::ISceneNode* privateNode;
  
};

#endif