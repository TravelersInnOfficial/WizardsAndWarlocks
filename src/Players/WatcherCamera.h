#ifndef WATCHERCAMERA_H
#define WATCHERCAMERA_H

#include "Camera.h"
#include "../Entidad.h"
#include "../GraphicEngine/GraphicEngine.h"
#include "../PhysicsEngine/BT_Body.h"

class WatcherCamera : public Camera{
public:
    WatcherCamera(vector3df lookat);
    ~WatcherCamera();

    void UpdateCamera(vector3df target);

private:
    void CheckDistance();
    void Contact(void* punt, EntityEnum tipo);
    vector3df GetTarget(float X, float Y);
    void CheckMaxVelocity();
    void CheckCollision();
    void ResetMousePos();
    void UpdateAngles();
    
    BT_Body* p_BtBody;

    float XAngle;           // Angulo que forma el movimiento horizontal
    float YAngle;           // Angulo que forma el movimiento vertical
    float mouseVelocity;    // Velocidad, valor en grades por cada valor que se mueva el raton

    float cam_distance;         // Radio del circulo a la que se mueve la camara
    float minCamDistance;       // Radio minimo que puede estar la camara
    float maxCamDistance;       // Radio maximo que puede estar la camara
    float updateCamDistancia;   // Incremento en el cam_distance

    float distanceTarget;   // Distance to the target
    bool maskActivated;     // Tiene la mascara activa? Si/No

    bool colliding;         // Esta colisionando? Si/No

    int sign(float value);
};

#endif