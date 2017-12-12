#ifndef KINEMATICTYPES_H
#define KINEMATICTYPES_H

#include <iostream>
#include <math.h>
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

    friend inline std::ostream& operator << (std::ostream &o,const Kinematic &v){
        o << "(POSITION: "<<"x->"<<v.position.X<<" y->"<<v.position.Y<<" z->"<<v.position.Z<<" )"<<std::endl;
        o << "(ORIENTATION: "<<"x->"<<v.orientation.X<<" y->"<<v.orientation.Y<<" )"<<std::endl;
        o << "(VELOCITY: "<<"x->"<<v.velocity.X<<" y->"<<v.velocity.Y<<" z->"<<v.velocity.Z<<" )"<<std::endl;
        o << "(ROTATION: "<<"x->"<<v.rotation.X<<" y->"<<v.rotation.Y<<" )"<<std::endl;
        return o;
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

    friend inline std::ostream& operator << (std::ostream &o,const SteeringOutput &v){
        o << "(LINEAR: "<<"x->"<<v.linear.X<<" y->"<<v.linear.Y<<" z->"<<v.linear.Z<<" )"<<std::endl;
        o << "(ANGULAR: "<<"x->"<<v.angular.X<<" y->"<<v.angular.Y<<" )";
        return o;
    }

};

#endif