#include "IrrIrrGEntity.h"
#include "IrrEngine.h"

void IrrGEntity::setPosition(vector3df pos){
    privateNode->setPosition(irr::core::vector3df(pos.X, pos.Y, pos.Z));
}

void IrrGEntity::setRotation(vector3df rot){
    irr::core::vector3df irrRotation = irr::core::vector3df(rot.X, rot.Y, rot.Z);
    privateNode->setRotation(irrRotation);
}

void IrrGEntity::Rotate(vector3df rot){
    irr::core::matrix4 m;
    irr::core::matrix4 n;
    irr::core::vector3df irrRotation;

    irrRotation = irr::core::vector3df(rot.X, rot.Y, rot.Z);
    m.setRotationDegrees(privateNode->getRotation());
    n.setRotationDegrees(irrRotation);
    m *= n;
    
    privateNode->setRotation(m.getRotationDegrees());
    privateNode->updateAbsolutePosition();
}

void IrrGEntity::setScale(vector3df scale){
    privateNode->setScale(irr::core::vector3df(scale.X, scale.Y, scale.Z));
}

vector3df IrrGEntity::getPosition(){
    irr::core::vector3df v = privateNode->getPosition();
    return vector3df(v.X, v.Y, v.Z);
}

vector3df IrrGEntity::getRotation(){
    irr::core::vector3df v = privateNode->getRotation();
    return vector3df(v.X, v.Y, v.Z);
}

vector3df IrrGEntity::getScale(){
    irr::core::vector3df v = privateNode->getScale();
    return vector3df(v.X, v.Y, v.Z);
}

void IrrGEntity::updateAbsolutePosition(){
    privateNode->updateAbsolutePosition();
}

void IrrGEntity::setAutomaticCulling(){
    privateNode->setAutomaticCulling(irr::scene::EAC_FRUSTUM_BOX);
}

void IrrGEntity::Erase(){
    IrrEngine::getInstance()->privateSManager->addToDeletionQueue(privateNode);
}