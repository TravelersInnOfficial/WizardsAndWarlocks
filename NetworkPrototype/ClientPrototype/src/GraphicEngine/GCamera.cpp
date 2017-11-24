#include "GCamera.h"
#include "GraphicEngine.h"

GCamera::GCamera(irr::scene::ISceneNode* node){
    privateNode = node;
}

void GCamera::setTarget(vector3df target){
    irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*)privateNode;
    cam->setTarget(irr::core::vector3df(target.X, target.Y, target.Z));
}

void GCamera::bindTargetAndRotation(bool bound){
    irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*)privateNode;
    cam->bindTargetAndRotation(bound);
}