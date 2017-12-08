#include "GBody.h"
#include "GraphicEngine.h"

GBody::GBody(irr::scene::ISceneNode* node){
    privateNode = node;
}

GBody::~GBody(){
}

void GBody::setMaterialTexture(int layer, std::string path){
    GraphicEngine::getInstance()->setTextureToBody(this, layer, path);
    privateNode->setMaterialType(irr::video::EMT_SOLID);
}

void GBody::setMaterialFlag(MATERIAL_FLAG flag, bool value){
    privateNode->setMaterialFlag((irr::video::E_MATERIAL_FLAG)flag, value);
}

void GBody::setMaterialType(MATERIAL_TYPE type){
    privateNode->setMaterialType((irr::video::E_MATERIAL_TYPE)type);
}

void GBody::setTriangleSelector(){
    privateNode->setTriangleSelector(GraphicEngine::getInstance()->AddTriangleSelector(privateNode));
}