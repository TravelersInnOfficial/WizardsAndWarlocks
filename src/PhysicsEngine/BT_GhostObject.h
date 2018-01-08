#ifndef BT_GHOSTOBJECT_H
#define BT_GHOSTOBJECT_H

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <vector3d.h>

class BT_GhostObject{
public:
    BT_GhostObject();

    void CreateGhostBox(vector3df, vector3df, vector3df);
    void Rotate(vector3df rot);

    void AssignPointer(void*);

    void SetPosition(vector3df pos);
    void SetScale(float ammount);

    void Erase();
    ~BT_GhostObject();

private:

    vector3df* m_position;
    vector3df* m_dimensions;
    vector3df* m_rotation;
    vector3df* m_center;
    btGhostObject* m_body;

};

#endif