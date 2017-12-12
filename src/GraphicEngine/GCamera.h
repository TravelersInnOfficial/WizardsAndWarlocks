#ifndef GCamera_H
#define GCamera_H

#include "GEntity.h"

class GCamera: public GEntity{

public:

    /**
     * Initializes empty pointer
    */
    GCamera();

    /**
     *  Sets the look at target of the camera 
    */
    void setTarget(vector3df target);

    /**
     * Binds the camera scene node's rotation to its target position and vice vera, or unbinds them
    */
    void bindTargetAndRotation(bool bound);

    friend class GraphicEngine;

private:

    GCamera(irr::scene::ISceneNode* node);    
  
};

#endif