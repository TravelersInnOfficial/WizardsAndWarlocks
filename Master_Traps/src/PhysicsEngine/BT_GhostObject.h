#ifndef BT_GHOSTOBJECT_H
#define BT_GHOSTOBJECT_H

#include "../vector3d.h"
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

class BT_GhostObject{
public:
    BT_GhostObject();

    void CreateGhostBox(vector3df*, vector3df*);

    void SetObjectPtr(void*);

    void Erase();
    ~BT_GhostObject();

private:

    vector3df* m_position;
    vector3df* m_dimensions;
    btGhostObject* m_body;

};

#endif