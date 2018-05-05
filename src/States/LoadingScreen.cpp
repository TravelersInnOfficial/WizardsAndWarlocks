#include "LoadingScreen.h"
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/MenuManager.h>
#include <sstream>
#include <iomanip>

LoadingScreen::LoadingScreen(){
    g_engine = GraphicEngine::getInstance();
    float W = g_engine->GetScreenWidth();
    //loading_bar = g_engine->add2DRect(vector2df(0,0),vector2df(W,100));
    //loading_bar->SetColor(1,0,0);
    bar_width = W;
}

LoadingScreen::~LoadingScreen(){
    g_engine = nullptr;

    delete loading_bar;
    loading_bar = nullptr;
}

bool LoadingScreen::Input(){
    return false;
}

void LoadingScreen::Update(float deltaTime){
}

void  LoadingScreen::SetLoadingStatus(std::string status, float progress){
    //std::cout<<"now loading: "<<status<<"\n";
    std::ostringstream bar_progress;
    bar_progress << std::setprecision(4) << "bar progress: " << progress <<"%\n";
    //std::cout<<bar_progress.str();
    //loading_bar->SetWidth(bar_width*progress);
}

void LoadingScreen::Draw(){

}