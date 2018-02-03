#include "ResourceManager.h"
#include <GraphicEngine/GraphicEngine.h>

ResourceManager::ResourceManager(){
	
}

void ResourceManager::LoadResources(){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	
	GBody* auxBody = NULL;
	vector3df zeroVector = vector3df(0,0,0);

	std::map<GAMEMESHES,std::string>::iterator meshIterator;
	std::map<GAMETEXTURES, std::string>::iterator textureIterator;

	// Load Meshes
	for (meshIterator = MESHMAP.begin(); meshIterator != MESHMAP.end(); meshIterator++){
		auxBody = g_engine->addObjMeshSceneNode(meshIterator->second, zeroVector, zeroVector, zeroVector);
	}

	// Load Textures
	for (textureIterator = TEXTUREMAP.begin(); textureIterator != TEXTUREMAP.end(); textureIterator++){
		auxBody->setMaterialTexture(0, textureIterator->second);
	}

}