#ifndef CAMERA_H
#define CAMERA_H

#include <GraphicEngine/GCamera.h>
#include "./../Entidad.h"

class Camera: public Entidad{
public:
	Camera();
    virtual ~Camera();
    virtual void Contact(void* punt, EntityEnum tipo);

    virtual void SetPosition(vector3df position);
    vector3df GetPosition();

    void SetTarget(vector3df lookat);
    vector3df GetTarget();
    
    void SetRotation(vector3df rotation);
    vector3df GetRotation();

    bool GetWorking();

    virtual void SetWorking(bool wor) = 0;

    virtual void UpdateCamera(vector3df target) = 0;

protected:
    GCamera*    p_Camera;
    bool        m_working;
};


#endif