#ifndef KINEMATICTYPES_H
#define KINEMATICTYPES_H

#include <iostream>
#include <vector3d.h>
#include <vector2d.h>

class Kinematic{
private:
public:
    vector3df position;
    vector2df orientation;
    vector3df velocity;
    vector2df rotation;

    Kinematic(){
        position = vector3df();
        orientation = vector2df();
        velocity = vector3df();
        rotation = vector2df();
    }

};

class SteeringOutput{
private:
public:
    vector3df linear;
    vector2df angular;

    SteeringOutput(){
        linear = vector3df();
        angular = vector2df();
    }

};

#endif