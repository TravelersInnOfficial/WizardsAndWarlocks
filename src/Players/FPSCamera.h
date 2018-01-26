#ifndef FPSCAMERA_H
#define FPSCAMERA_H

#include "Camera.h"

class FPSCamera : public Camera{
public:
    FPSCamera(float rotateSpeed, float moveSpeed);
    ~FPSCamera();

    void UpdateCamera(vector3df position);

};

#endif