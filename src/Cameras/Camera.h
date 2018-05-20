#ifndef CAMERA_H
#define CAMERA_H


#include "./../Entidad.h"

class GCamera;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

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

    virtual void UpdateCamera(vector3df target, float deltaTime) = 0;

protected:
    GCamera*    p_Camera;
    bool        m_working;
};


#endif