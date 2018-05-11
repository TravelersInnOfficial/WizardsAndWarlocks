#ifndef BLOCK_H
#define BLOCK_H

#include "../Entidad.h"
#include <iostream>

class BT_Body;
class GBody;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class Block : public Entidad{
public:
    
    Block(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string texture);  
    ~Block();

    void Update();
    vector3df GetPosition();

private:
    void UpdatePosShape();

    GBody*      graphBody;
    BT_Body*    physBody;
};

#endif