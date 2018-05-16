#include "ResourceManager.h"

#include <GraphicEngine/GraphicEngine.h>
#include <NetworkEngine/NetworkEngine.h>
#include "./States/LoadingScreen.h"
#include <Assets.h>

ResourceManager::ResourceManager(){
	
}

void ResourceManager::LoadResources(){

	if(NetworkEngine::GetInstance()->IsServerInit()) return;
	
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	LoadingScreen* s = new LoadingScreen();

	float totalSize = MESHMAP.size() + TEXTUREMAP.size() + ANIMATIONMAP.size();
	float cont = 0;
	float progress = 0;

	std::map<GAMEMESHES,std::string>::iterator meshIterator;
	std::map<GAMETEXTURES, std::string>::iterator textureIterator;
	std::vector<std::string>::iterator animIt;


	// Load Textures
	for (textureIterator = TEXTUREMAP.begin(); textureIterator != TEXTUREMAP.end(); textureIterator++){
		g_engine->LoadTexture(textureIterator->second);
		cont++;
		progress = (cont*100)/totalSize;
		if(s!=nullptr) s->SetLoadingStatus(textureIterator->second, progress);
	}

	// Load Meshes
	for (meshIterator = MESHMAP.begin(); meshIterator != MESHMAP.end(); meshIterator++){
		g_engine->LoadMesh(meshIterator->second);
		cont++;
		progress = (cont*100)/totalSize;
		if(s!=nullptr) s->SetLoadingStatus(meshIterator->second, progress);
	}

	// Load Animations
	for (animIt = ANIMATIONMAP.begin(); animIt != ANIMATIONMAP.end(); animIt++){
		g_engine->LoadMesh(*animIt);
		cont++;
		progress = (cont*100)/totalSize;
		if(s!=nullptr) s->SetLoadingStatus(*animIt, progress);
	}

	delete s;
	
	//delete s;
}