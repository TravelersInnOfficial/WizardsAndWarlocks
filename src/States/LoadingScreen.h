#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include "State.h"

class GraphicEngine;
class GRect;
class LoadingScreen : public State{
public:
    LoadingScreen();
    ~LoadingScreen();
    bool Input();
    void Update(float deltaTime = 0.0f);
    void Draw();

    void SetLoadingStatus(std::string status = "", float progress = 0.0f);
private:
    GraphicEngine* g_engine;
    GRect* loading_bar;
    float bar_width;
};

#endif