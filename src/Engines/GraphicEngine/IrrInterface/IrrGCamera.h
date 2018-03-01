#ifndef IRRGCAMERA_H
#define IRRGCAMERA_H

#include "IrrGEntity.h"

class IrrGCamera: public IrrGEntity{

public:

    /**
     * @brief Initializes empty pointer
     * 
     */
    IrrGCamera();

    /**
     * @brief Sets the look at target of the camera 
     * 
     * @param target: position where to look at
     */
    void setTarget(vector3df target);

    /**
     * @brief gets the look at target of the camera 
     * 
     * @return vector3df: target of the camera
     */
    vector3df getTarget();

    /**
     * @brief Binds the camera scene node's rotation to its target position and vice vera, or unbinds them
     * 
     * @param bound: bind or free camera rotation
     */
    void bindTargetAndRotation(bool bound);

    // allows graphic engine to access private variables and methods
    friend class IrrEngine;

private:

    IrrGCamera(irr::scene::ISceneNode* node);    
  
};

#endif