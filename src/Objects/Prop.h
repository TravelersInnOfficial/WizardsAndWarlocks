#ifndef PROP_H
#define PROP_H

#include "../Entidad.h"
#include "../GraphicEngine/GBody.h"
#include "../PhysicsEngine/BT_Body.h"
#include <ColliderMasks.h>
#include <vector3d.h>

class Prop : public Entidad{
public:
    Prop(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string model, std::string tex);
    ~Prop();
    
    void Update();
    vector3df GetPosition();

private:
    void UpdatePosShape();

    GBody*      graphBody;
    BT_Body*    physBody;
};

#endif