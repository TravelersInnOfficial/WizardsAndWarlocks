#include "GBody.h"
#include "GraphicEngine.h"
#include <TravelersOcularEngine/src/TOcularEngine/TOcularEngine.h>
#include <NetworkEngine/NetworkEngine.h>

GBody::GBody(TFMesh* node){
    privateNode = (TFNode*)node;
    if(GraphicEngine::getInstance()->m_currentRoom != nullptr){
    	GraphicEngine::getInstance()->m_currentRoom->AddChild(this);
    }
}

GBody::~GBody(){
	GraphicEngine::getInstance()->privateSManager->DeleteMesh(privateNode);
	// El TFNode se elimina correctamente
}

void GBody::setMaterialTexture(int layer, std::string path){
	if(!NetworkEngine::GetInstance()->IsServerInit()){
		TFMesh* mesh = (TFMesh*)privateNode;
		mesh->SetTexture(path);
	}
    //IrrEngine::getInstance()->setTextureToBody(this, layer, path);
    //privateNode->setMaterialType(irr::video::EMT_SOLID);
	//privateNode->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
}

void GBody::setMaterialFlag(MATERIAL_FLAG flag, bool value){
    //privateNode->setMaterialFlag((irr::video::E_MATERIAL_FLAG)flag, value);
}

void GBody::setMaterialType(MATERIAL_TYPE type){
    //privateNode->setMaterialType((irr::video::E_MATERIAL_TYPE)type);
}

void GBody::AddText(std::string text, vector3df position, int id){
	toe::core::TOEvector3df motorPosition(position.X, position.Y, position.Z);
	// COmprobamos si ya existia la id almacenada
	if(m_billboards.find(id) != m_billboards.end()){
		// Comprobamos que haya algo creado ahi
		int motorId = m_billboards[id];
		if(motorId != -1){
			privateNode->SetBillboardText(text, motorId);
			privateNode->SetBillboardPosition(motorPosition, motorId);

			// Una vez cambiado el texto y su posicion volvemos
			return;
		}
	}
	// Si se llega aqui significa que no existe ningun billboard
	m_billboards[id] = privateNode->AddBillboard(motorPosition, text);
}

void GBody::SetInvisible(){
	((TFMesh*)privateNode)->SetInvisible();
}

void GBody::ChangeShader(SHADERTYPE shader){
	((TFMesh*)privateNode)->SetProgram(shader);
}

void GBody::SetTextureScale(vector2df value){
	((TFMesh*)privateNode)->SetTextureScale(value.X, value.Y);
}