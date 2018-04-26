#include "ResourceManager.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./Managers/StateManager.h"
#include <NetworkEngine/NetworkEngine.h>

ResourceManager::ResourceManager(){
	
}

void ResourceManager::LoadResources(){
	LoadingScreen* s = nullptr;
	if(!NetworkEngine::GetInstance()->IsServerInit()) s = new LoadingScreen();

	float totalSize = MESHMAP.size() + TEXTUREMAP.size();
	float cont = 0;
	float bar_width = 0;

	std::map<GAMEMESHES,std::string>::iterator meshIterator;
	std::map<GAMETEXTURES, std::string>::iterator textureIterator;

	// Load Textures
	for (textureIterator = TEXTUREMAP.begin(); textureIterator != TEXTUREMAP.end(); textureIterator++){
		toe::LoadTexture(textureIterator->second);
		cont++;
		bar_width = (cont*100)/totalSize;
		if(s!=nullptr) s->SetLoadingStatus(textureIterator->second, bar_width);
	}
	
	// Load Meshes
	for (meshIterator = MESHMAP.begin(); meshIterator != MESHMAP.end(); meshIterator++){
		toe::LoadMesh(meshIterator->second);
		cont++;
		bar_width = (cont*100)/totalSize;
		if(s!=nullptr) s->SetLoadingStatus(meshIterator->second, bar_width);
	}
}