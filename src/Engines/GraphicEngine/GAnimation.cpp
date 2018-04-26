#include "GAnimation.h"
#include "GraphicEngine.h"
#include <TravelersOcularEngine/src/TOcularEngine/TOcularEngine.h>

GAnimation::GAnimation(TFAnimation* animation) : GBody(nullptr) {
    privateNode = animation;
}

GAnimation::~GAnimation(){  }

void GAnimation::SetPaths(std::string ID, std::vector<std::string> &paths, int fps){
    ((TFAnimation*)privateNode)->SetAnimationPaths(ID, paths, fps);
}

void GAnimation::Update(float deltatime){
    ((TFAnimation*)privateNode)->Update(deltatime);
}

void GAnimation::SetAnimationLoop(std::string ID, int fps){
    ((TFAnimation*)privateNode)->ChangeAnimation(ID, fps);
}

void GAnimation::PlayAnimation(std::string ID, int fps){
    ((TFAnimation*)privateNode)->PlayAnimation(ID, fps);
}

void GAnimation::BindSyncAnimation(GAnimation* master){
    if (master != nullptr){
        ((TFAnimation*)privateNode)->BindSyncAnimation((TFAnimation*)master->privateNode);
    }
}

int GAnimation::GetAnimationFrame(){
    return ((TFAnimation*)privateNode)->GetAnimationFrame();
}