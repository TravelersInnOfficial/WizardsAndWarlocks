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
    void ResetMousePos();
    
    GCamera* p_Camera;
    BT_Body* p_BtBody;

    float YAngle;           // Angulo que forma el movimiento horizontal
    float XZAngle;          // Angulo que forma el movimiento vertical
    float mouseVelocity;         // Velocidad, valor en grades por cada valor que se mueva el raton

    float cam_distance;
    bool colliding;

    vector2di lastCursorPosition;

    int sign(float value);

    void UpdateAngles();
    void checkMaxVelocity();

};

#endif