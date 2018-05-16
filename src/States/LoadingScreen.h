#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <iostream>
#include "State.h"

class GSprite;
class GRect;
class GText2D;
class GraphicEngine;

class LoadingScreen : public State{
public:
    LoadingScreen();
    ~LoadingScreen();
    bool Input();
    void Update(float deltaTime = 0.0f);
    void Draw();

    void SetLoadingStatus(std::string status = "", float progress = 0.0f);
private:
    std::string p_getLoadingStatement();
    void p_updateDotsAnimation();

    GraphicEngine* g_engine;
    
    GSprite* m_bkg;
    GSprite* m_bar_bkg;
    
    GRect* loading_bar;
    
    GText2D* loading_text;
    GText2D* loading_perc;
    GText2D* dots_anim;
    float anim_time;
    float bar_width;
    std::string actual_folder;
};

#endif