#include "GAnimation.h"
#include "GraphicEngine.h"
#include <TravelersOcularEngine/src/TOcularEngine/TOcularEngine.h>
#include <NetworkEngine/NetworkEngine.h>

GAnimation::GAnimation(TFAnimation* animation) : GBody(nullptr) {
	privateNode = (TFNode*)animation;
	isNet = NetworkEngine::GetInstance()->IsServerInit();
}

GAnimation::~GAnimation(){ 
	if(!NetworkEngine::GetInstance()->IsServerInit()) GraphicEngine::getInstance()->privateSManager->DeleteAnimation(privateNode);
}

void GAnimation::SetPaths(std::string ID, std::vector<std::string> &paths, int fps){
	if(!isNet) ((TFAnimation*)privateNode)->SetAnimationPaths(ID, paths, fps);
}

void GAnimation::Update(float deltatime){
	if(!isNet) ((TFAnimation*)privateNode)->Update(deltatime);
}

void GAnimation::SetAnimationLoop(std::string ID, int fps){
	if(!isNet) ((TFAnimation*)privateNode)->ChangeAnimation(ID, fps);
}

void GAnimation::PlayAnimation(std::string ID, int fps){
	if(!isNet) ((TFAnimation*)privateNode)->PlayAnimation(ID, fps);
}

void GAnimation::BindSyncAnimation(GAnimation* master){
	if (master != nullptr){
		if(!isNet) ((TFAnimation*)privateNode)->BindSyncAnimation((TFAnimation*)master->privateNode);
	}
}

int GAnimation::GetAnimationFrame(){
	if(!isNet) return 0;
	return ((TFAnimation*)privateNode)->GetAnimationFrame();
}