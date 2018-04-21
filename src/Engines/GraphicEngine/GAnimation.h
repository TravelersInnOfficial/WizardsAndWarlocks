#ifndef GANIMATION_H
#define GANIMATION_H

#include "GBody.h"

class TFAnimation;

class GAnimation : public GBody{

public:
	friend class GraphicEngine;
    ~GAnimation();

	void SetPaths(std::string ID, std::vector<std::string> paths, int fps = 25);

    void Update(float deltatime);

    void SetAnimationLoop(std::string ID, int fps = 25);

	void PlayAnimation(std::string ID, int fps = 25);

	void BindSyncAnimation(GAnimation* master);

	int GetAnimationFrame();

protected:
    GAnimation(TFAnimation* animation);

};

#endif