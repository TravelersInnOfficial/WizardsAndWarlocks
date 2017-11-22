#include "GBody.h"
#include "GraphicEngine.h"

GBody::GBody(irr::scene::ISceneNode* node){
    privateNode = node;
}

void GBody::setPosition(vector3df pos){
    privateNode->setPosition(irr::core::vector3df(pos.X, pos.Y, pos.Z));
}

void GBody::setRotation(vector3df rot){
    privateNode->setRotation(irr::core::vector3df(rot.X, rot.Y, rot.Z));
}

void GBody::setScale(vector3df scale){
    privateNode->setScale(irr::core::vector3df(scale.X, scale.Y, scale.Z));
}

vector3df GBody::getPosition(){
    irr::core::vector3df v = privateNode->getPosition();
    return vector3df(v.X, v.Y, v.Z);
}

vector3df GBody::getRotation(){
    irr::core::vector3df v = privateNode->getRotation();
    return vector3df(v.X, v.Y, v.Z);
}

vector3df GBody::getScale(){
    irr::core::vector3df v = privateNode->getScale();
    return vector3df(v.X, v.Y, v.Z);
}

void GBody::setMaterialTexture(std::string path){
    GraphicEngine::getInstance()->setTextureToBody(this, path);
}

void GBody::setMaterialFlag(std::string flag, bool value){
    GraphicEngine::getInstance()->setTextureFlag(this, flag, value);
}
