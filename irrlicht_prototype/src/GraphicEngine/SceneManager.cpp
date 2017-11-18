#include "SceneManager.h"

SceneManager::SceneManager(){

}

SceneManager::SceneManager(irr::scene::ISceneManager* smgr){
    if (smgr){
        privateSceneManager = smgr;
    }
}

SceneNode* SceneManager::addSphereSceneNode(float radius, float polyCount,
    SceneNode* parent, signed int id,
    const irr::core::vector3df& position,
    const irr::core::vector3df& rotation,
    const irr::core::vector3df& scale){

    SceneNode* nod = new SceneNode();

}