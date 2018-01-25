#ifndef CAMERA_H
#define CAMERA_H

#include "../GraphicEngine/GCamera.h"

class Camera{
public:
    virtual ~Camera();

    void SetPosition(vector3df position);
    vector3df GetPosition();

    void SetTarget(vector3df lookat);
    vector3df GetTarget();
    
    void SetRotation(vector3df rotation);
    vector3df GetRotation();

    virtual void UpdateCamera(vector3df target) = 0;

protected:
    GCamera*    p_Camera;
};


#endif