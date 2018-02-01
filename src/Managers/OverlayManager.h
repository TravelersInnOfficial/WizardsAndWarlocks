#ifndef OVERLAYMANAGER_H
#define OVERLAYMANAGER_H

#include <iostream>
#include <vector>
#include <OverlayCodes.h>
#include <GraphicEngine/GraphicEngine.h>

struct OverlayInfo {
	float time;
	OverlayInfo(float time = 0){ this->time = time; }
	~OverlayInfo(){}
	void SetTime(float newTime){ time = newTime; }
};

class OverlayManager{

public:
	OverlayManager();
	~OverlayManager();

	void SetTime(OverlayCodes overlay, float overlayTime);
	void CleanOverlays();
	void Update(float deltaTime);
	void Draw();

private:
	std::map<OverlayCodes, OverlayInfo*> overlays;

};

#endif