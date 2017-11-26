#include "GBody.h"
#include "GraphicEngine.h"

GBody::GBody(irr::scene::ISceneNode* node){
    privateNode = node;
}

void GBody::setMaterialTexture(std::string path,int layer){
    GraphicEngine::getInstance()->setTextureToBody(this, layer, path);
}

void GBody::setMaterialFlag(std::string flag, bool value){
    GraphicEngine::getInstance()->setTextureFlag(this, flag, value);
}
