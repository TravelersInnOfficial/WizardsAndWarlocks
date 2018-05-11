#ifndef PROP_H
#define PROP_H

#include <iostream>

class GBody;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class Prop{
public:
    Prop(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string model, std::string tex);
    ~Prop();

private:
    GBody*      graphBody;
};

#endif