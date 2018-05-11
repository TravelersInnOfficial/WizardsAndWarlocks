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
	float bar_width = 0;

	std::map<GAMEMESHES,std::string>::iterator meshIterator;
	std::map<GAMETEXTURES, std::string>::iterator textureIterator;
	std::vector<std::string>::iterator animIt;


	// Load Textures
	for (textureIterator = TEXTUREMAP.begin(); textureIterator != TEXTUREMAP.end(); textureIterator++){
		g_engine->LoadTexture(textureIterator->second);
		cont++;
		bar_width = (cont*100)/totalSize;
		if(s!=nullptr) s->SetLoadingStatus(textureIterator->second, bar_width);
	}

	// Load Meshes
	for (meshIterator = MESHMAP.begin(); meshIterator != MESHMAP.end(); meshIterator++){
		g_engine->LoadMesh(meshIterator->second);
		cont++;
		bar_width = (cont*100)/totalSize;
		if(s!=nullptr) s->SetLoadingStatus(meshIterator->second, bar_width);
	}

	// Load Animations
	for (animIt = ANIMATIONMAP.begin(); animIt != ANIMATIONMAP.end(); animIt++){
		g_engine->LoadMesh(*animIt);
		cont++;
		bar_width = (cont*100)/totalSize;
		s->SetLoadingStatus(*animIt, bar_width);
	}
	
}