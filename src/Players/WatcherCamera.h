#ifndef WATCHERCAMERA_H
#define WATCHERCAMERA_H

#include "../Entidad.h"
#include "../GraphicEngine/GraphicEngine.h"
#include "../PhysicsEngine/BT_Body.h"

class WatcherCamera : public Entidad{
public:
    WatcherCamera(vector3df position, vector3df lookat);
    ~WatcherCamera();

    void SetPosition(vector3df position);
    void SetTarget(vector3df lookat);
    void UpdateCamera(vector3df target);

private:
    void Contact(void* punt, EntityEnum tipo);
    
    GCamera* p_Camera;
    BT_Body* p_BtBody;
    int YAngle;
    int XZAngle;
    float cam_distance;
    bool colliding;

    vector2di lastCursorPosition;

    void UpdateAngles();
    void checkMaxVelocity();

};

#endif