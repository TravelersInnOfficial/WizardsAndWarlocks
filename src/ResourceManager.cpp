#include "ResourceManager.h"
#include <GraphicEngine/GraphicEngine.h>

ResourceManager::ResourceManager(){
	
}

void ResourceManager::LoadResources(){
	std::map<GAMEMESHES,std::string>::iterator meshIterator;
	std::map<GAMETEXTURES, std::string>::iterator textureIterator;

	// Load Meshes
	for (meshIterator = MESHMAP.begin(); meshIterator != MESHMAP.end(); meshIterator++){
		toe::LoadMesh(meshIterator->second);
	}

	// Load Textures
	for (textureIterator = TEXTUREMAP.begin(); textureIterator != TEXTUREMAP.end(); textureIterator++){
		toe::LoadTexture(textureIterator->second);
	}

}