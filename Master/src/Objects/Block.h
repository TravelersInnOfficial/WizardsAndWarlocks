#ifndef BLOCK_H
#define BLOCK_H

#include "../Entidad.h"
#include "../GraphicEngine/GBody.h"
#include "../PhysicsEngine/BT_Body.h"
#include <vector3d.h>

class Block : public Entidad{

public:

    Block();
    ~Block();
    
    Block(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string texture);  

    void Update();

private:
    void UpdatePosShape();

    GBody*      graphBody;
    BT_Body*    physBody;
};

#endif