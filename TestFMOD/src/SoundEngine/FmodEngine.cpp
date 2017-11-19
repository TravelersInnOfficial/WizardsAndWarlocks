#include "FmodEngine.h"

static FmodEngine* instance;

FmodEngine* FmodEngine::GetInstance() {
    if (instance == NULL) instance = new FmodEngine();
    return (instance);
}

FmodEngine::FmodEngine(){

}

FmodEngine::~FmodEngine(){
    
}