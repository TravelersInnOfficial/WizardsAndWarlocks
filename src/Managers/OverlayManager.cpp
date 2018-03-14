#include "OverlayManager.h"

OverlayManager::OverlayManager(){

}

OverlayManager::~OverlayManager(){
	CleanOverlays();
}

void OverlayManager::SetTime(OverlayCodes overlay, float overlayTime){
	std::map<OverlayCodes, OverlayInfo*>::iterator it = overlays.find(overlay);
	
	// NO EXISTE --> CREAMOS UNO NUEVO
	if(it == overlays.end()){
		if(overlayTime > 0){
			OverlayInfo* oi = new OverlayInfo(overlayTime);
			overlays[overlay] = oi;
		}
	}

	// EXISTE --> ACTUALIZAMOS SU TIEMPO
	else if(it->second != nullptr) it->second->time = overlayTime;
}

void OverlayManager::CleanOverlays(){
	std::map<OverlayCodes, OverlayInfo*>::iterator it = overlays.begin();
	
	for(; it != overlays.end(); it++){
		if (it->second != nullptr) delete it->second;
	}

	overlays.clear();
}

void OverlayManager::Update(float deltaTime){
	std::map<OverlayCodes, OverlayInfo*>::iterator it = overlays.begin();
	
	for(; it != overlays.end(); /* No ponemos el incremento aqui*/){
		bool iterate  = true;
		if(it->second != nullptr){
			it->second->time -= deltaTime;
			if(it->second->time <= 0){ 
				delete it->second;
				overlays.erase(it++);
				iterate = false;
				GraphicEngine::getInstance()->ClearOverlay();
			}
		}
		if(iterate){
			++it;
		}
	}
}

void OverlayManager::Draw(){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	
	if(g_engine != nullptr){
		std::map<OverlayCodes, OverlayInfo*>::iterator it = overlays.begin();

		for(; it != overlays.end(); it++){
			if(it->second != nullptr && it->second->time > 0) g_engine->drawOverlays(it->first);
		}
	}
}