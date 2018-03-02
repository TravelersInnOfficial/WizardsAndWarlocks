#ifndef GCamera_H
#define GCamera_H

#include "GEntity.h"

class GCamera: public GEntity{
    friend class GraphicEngine;

public:

    /**
     * @brief Initializes empty pointer
     * 
     */
    GCamera();

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

private:
    GCamera(void* node);    
  
};

#endif