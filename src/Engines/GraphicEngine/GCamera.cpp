#include "GCamera.h"

GCamera::GCamera(TFCamera* node){
    privateNode = (TFNode*) node;
}

void GCamera::setTarget(vector3df target){
	 TOEvector3df targetPos =  TOEvector3df(target.X, target.Y, target.Z);
	((TFCamera*)privateNode)->LookAt(targetPos);
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