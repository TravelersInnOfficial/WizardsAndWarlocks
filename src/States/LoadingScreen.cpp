#include "LoadingScreen.h"

#include <sstream>
#include <iomanip>
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GRect.h>
#include <GraphicEngine/GSprite.h>
#include <GraphicEngine/GText2D.h>
#include <LoadingStatements.h>
#include <Assets.h>

LoadingScreen::LoadingScreen(){
    g_engine = GraphicEngine::getInstance();
    float W = g_engine->GetScreenWidth();
    float H = g_engine->GetScreenHeight();
    anim_time = g_engine->getTime()/1000; //secondss

    bar_width = W/1.2;
    float barH = H/12;
    float barX = W/2 - bar_width/2;
    float barY = H/2;

    float bar_bkgX = barX - 30;
    float bar_bkgW = bar_width + 60;

    m_bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_LOADING_SCREEN_BKG],vector2df(0,0),vector2df(W,H));
    m_bar_bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_LOADING_BAR_BKG],vector2df(bar_bkgX, barY),vector2df(bar_bkgW,barH));

    loading_bar = g_engine->add2DRect(vector2df(barX,barY),vector2df(bar_width,barH));
    loading_bar->SetMask(TEXTUREMAP[TEXTURE_LOADING_BAR_MASK]);
    loading_bar->SetColor(0,0,1);
    
    loading_text = g_engine->add2DText("",vector2df(W/2,H/3));
    dots_anim = g_engine->add2DText("",vector2df(W/2,H/2.5));
    loading_perc = g_engine->add2DText("",vector2df(W/2,H/2));
    actual_folder = "";

    currentProgress = 0;
}

LoadingScreen::~LoadingScreen(){
    g_engine = nullptr;

    delete m_bkg;
    delete m_bar_bkg;
    delete loading_bar;
    delete loading_text;
    delete loading_perc;
    delete dots_anim;
}

bool LoadingScreen::Input(){
    return false;
}

void LoadingScreen::Update(float deltaTime){
}

void  LoadingScreen::SetLoadingStatus(std::string status, float progress){
    std::string txt = status;
    std::string toErase = "./../assets/";
    std::string folder;

    std::ostringstream prog;
    prog << std::setprecision(3) << (int) progress << '%';

    int pos = 0;
    pos = txt.find(toErase, pos);
    
    if(pos != std::string::npos) txt.erase(pos,toErase.size());

    std::size_t found = txt.find_last_of("/\\");
    folder = txt.substr(0,found);
    if(folder != actual_folder){
        actual_folder = folder;
        loading_text->SetText(p_getLoadingStatement());
        float loading_text_posX = g_engine->GetScreenWidth()/2 - loading_text->GetSize().X/2;
        float loading_text_posY = g_engine->GetScreenHeight()/3;
        loading_text->SetPosition(loading_text_posX, loading_text_posY);
    }
    
    loading_perc->SetText(prog.str());
    float loading_perc_posX = g_engine->GetScreenWidth()/2 - loading_perc->GetSize().X/2;
    float loading_perc_posY = g_engine->GetScreenHeight()/2;
    loading_perc->SetPosition(loading_perc_posX, loading_perc_posY);

    loading_bar->SetWidth(bar_width*(progress/100));
    
    p_updateDotsAnimation();

    int progressInt = (int)progress;

    if(progressInt!=currentProgress){
        currentProgress = progressInt;
        g_engine->drawAll();
    }
}

void LoadingScreen::Draw(){

}

void LoadingScreen::p_updateDotsAnimation(){
    float deltatime =  g_engine->getTime()/1000.0f; //seconds
    if(deltatime - anim_time >= 0.5f){ //si pasan tres segundos
        std::string txt = dots_anim->GetText();
        float siz = txt.size();
        if(siz >= 0 && siz<3){
            txt += '.';
            dots_anim->SetText(txt);
        }else dots_anim->SetText("");

        float dots_anim_posX = g_engine->GetScreenWidth()/2 - dots_anim->GetSize().X/2;
        float dots_anim_posY = g_engine->GetScreenHeight()/2.5;
        dots_anim->SetPosition(dots_anim_posX, dots_anim_posY);
        anim_time = deltatime;
    }
}

std::string LoadingScreen::p_getLoadingStatement(){
    std::string loading_statement = "";
    int arraySize = sizeof(LoadingStatements)/sizeof(LoadingStatements[0]);
    int index = rand() % arraySize;
    loading_statement = LoadingStatements[index];
    return loading_statement;
}