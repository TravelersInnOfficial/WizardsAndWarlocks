#include "GCamera.h"

GCamera::GCamera(void* node){
    //privateNode = node;
}

void GCamera::setTarget(vector3df target){
    //irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*)privateNode;
    //cam->setTarget(irr::core::vector3df(target.X, target.Y, target.Z));
}

void GCamera::bindTargetAndRotation(bool bound){
    //irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*)privateNode;
    //cam->bindTargetAndRotation(bound);
}

vector3df GCamera::getTarget(){
    //irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*)privateNode;
    //irr::core::vector3df target= cam->getTarget();
    //return vector3df(target.X, target.Y, target.Z);
    return vector3df(0, 0, 0);
}