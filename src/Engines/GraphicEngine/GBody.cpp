#include "GBody.h"
#include "GraphicEngine.h"

GBody::GBody(irr::scene::ISceneNode* node){
    privateNode = node;
    board = NULL;
}

GBody::~GBody(){
}

void GBody::setMaterialTexture(int layer, std::string path){
    GraphicEngine::getInstance()->setTextureToBody(this, layer, path);
    privateNode->setMaterialType(irr::video::EMT_SOLID);
	privateNode->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
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

void GBody::Remove(){
    privateNode->remove();
}

void GBody::AddText(std::string text, vector3df position, int id){
	if(board!=NULL){  
		std::wstring textTmp(text.begin(), text.end());
		board->setText(textTmp.c_str());

		float dimX = text.length() * 0.1;
		float dimY = 0.25f;
		irr::core::dimension2d<irr::f32> dim = irr::core::dimension2d<irr::f32>(dimX, dimY);
		board->setSize(dim);

	}else{
 	   board = GraphicEngine::getInstance()->addBillboardText(text, privateNode, position, id);	
	}
}