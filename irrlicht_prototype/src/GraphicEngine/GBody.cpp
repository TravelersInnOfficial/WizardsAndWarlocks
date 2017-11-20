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

//void GBody::setTexture(){
//    privateNode->setMaterialTexture(0, GraphicEngine::getInstance()->getTexture("../media/t351sml.jpg"));
//}