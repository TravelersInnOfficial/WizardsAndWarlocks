#include <irrlicht/irrlicht.h>
#include "SceneNode.h"
#include "vector3d.h"

class SceneManager{

public:

    SceneManager();
    SceneManager(irr::scene::ISceneManager* smgr);
    SceneNode* addSphereSceneNode(
        float radius= 5.0f, float polyCount=16,
        SceneNode* parent=0, signed int id=-1,
        const irr::core::vector3df& position = irr::core::vector3df(0,0,0),
        const irr::core::vector3df& rotation = irr::core::vector3df(0,0,0),
        const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f)
        );

private:

    /**Irrlicht provisional scene manager*/
    irr::scene::ISceneManager* privateSceneManager;
};