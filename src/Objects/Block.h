#ifndef BLOCK_H
#define BLOCK_H

#include "../Entidad.h"
#include "../GraphicEngine/GBody.h"
#include "../PhysicsEngine/BT_Body.h"
#include <ColliderMasks.h>
#include <vector3d.h>

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