#include "IrrGCamera.h"
#include "IrrEngine.h"

IrrGCamera::IrrGCamera(irr::scene::ISceneNode* node){
    privateNode = node;
}

void IrrGCamera::setTarget(vector3df target){
    irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*)privateNode;
    cam->setTarget(irr::core::vector3df(target.X, target.Y, target.Z));
}

void IrrGCamera::bindTargetAndRotation(bool bound){
    irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*)privateNode;
    cam->bindTargetAndRotation(bound);
}

vector3df IrrGCamera::getTarget(){
    irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*)privateNode;
    irr::core::vector3df target= cam->getTarget();
    return vector3df(target.X, target.Y, target.Z);
}