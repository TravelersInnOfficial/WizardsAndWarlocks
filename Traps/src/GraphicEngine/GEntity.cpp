#include "GEntity.h"
#include "GraphicEngine.h"

void GEntity::setPosition(vector3df pos){
    privateNode->setPosition(irr::core::vector3df(pos.X, pos.Y, pos.Z));
}

void GEntity::setRotation(vector3df rot){
    privateNode->setRotation(irr::core::vector3df(rot.X, rot.Y, rot.Z));
}

void GEntity::setScale(vector3df scale){
    privateNode->setScale(irr::core::vector3df(scale.X, scale.Y, scale.Z));
}

vector3df GEntity::getPosition(){
    irr::core::vector3df v = privateNode->getPosition();
    return vector3df(v.X, v.Y, v.Z);
}

vector3df GEntity::getRotation(){
    irr::core::vector3df v = privateNode->getRotation();
    return vector3df(v.X, v.Y, v.Z);
}

vector3df GEntity::getScale(){
    irr::core::vector3df v = privateNode->getScale();
    return vector3df(v.X, v.Y, v.Z);
}

void GEntity::updateAbsolutePosition(){
    privateNode->updateAbsolutePosition();
}

void GEntity::setAutomaticCulling(){
    privateNode->setAutomaticCulling(irr::scene::EAC_FRUSTUM_BOX);
}

void GEntity::Erase(){
    GraphicEngine::getInstance()->addToDeletionQueue(privateNode);
}