#ifndef FPSCAMERA_H
#define FPSCAMERA_H

#include "Camera.h"
#include <vector2d.h>

class FPSCamera : public Camera{
public:
    FPSCamera(vector3df position, vector3df rotation);
    ~FPSCamera();

    void UpdateCamera(vector3df position);
private:
	vector3df GetNewRotation(vector3df rotation);
	void CatchMouseInput();

	vector2di lastPos;      // Ultima posicion en la que se ha encontrado el raton

	int changeX;
	int changeY;

	float sensibility;
};

#endif