#ifndef PROP_H
#define PROP_H

#include "../GraphicEngine/GBody.h"
#include <vector3d.h>

class Prop{
public:
    Prop(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string model, std::string tex);
    ~Prop();

private:
    GBody*      graphBody;
};

#endif