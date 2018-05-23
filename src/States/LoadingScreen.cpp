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

    int index = rand() % 10;
    alliance = index % 2;
    if(alliance == 0) m_bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_LOADING_SCREEN_WIZARD],vector2df(0,0),vector2df(W,H));
    else m_bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_LOADING_SCREEN_WARLOCK],vector2df(0,0),vector2df(W,H));

    m_bar_bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_LOADING_BAR_BKG],vector2df(bar_bkgX, barY),vector2df(bar_bkgW,barH));

    loading_bar = g_engine->add2DRect(vector2df(barX,barY),vector2df(bar_width,barH));
    loading_bar->SetMask(TEXTUREMAP[TEXTURE_LOADING_BAR_MASK]);
    if(alliance == 0) loading_bar->SetColor(0,0,1);
    else loading_bar->SetColor(0.8,0,0.8);
    
    loading_text = g_engine->add2DText("",vector2df(W/2,H/2));
    loading_text->SetTextureFont(TEXTUREMAP[TEXTURE_BLACK_BKG_FONT]);
    loading_perc = g_engine->add2DText("",vector2df(W/2,H/2 -  loading_text->GetSize().Y));
    loading_perc->SetTextureFont(TEXTUREMAP[TEXTURE_BLACK_BKG_FONT]);

    currentProgress = 0;
}

LoadingScreen::~LoadingScreen(){
    g_engine = nullptr;

    delete m_bkg;
    delete m_bar_bkg;
    delete loading_bar;
    delete loading_text;
    delete loading_perc;

    if(alliance == 0) g_engine->UnloadTexture(TEXTUREMAP[TEXTURE_LOADING_SCREEN_WIZARD]);
    else g_engine->UnloadTexture(TEXTUREMAP[TEXTURE_LOADING_SCREEN_WARLOCK]);
}

bool LoadingScreen::Input(){
    return false;
}

void LoadingScreen::Update(float deltaTime){
}

void  LoadingScreen::SetLoadingStatus(std::string status, float progress){
    
    std::ostringstream prog;
    prog << std::setprecision(3) << (int) progress << '%';
    
    loading_perc->SetText(prog.str());
    float loading_perc_posX = g_engine->GetScreenWidth()/2 - loading_perc->GetSize().X/2;
    float loading_perc_posY = g_engine->GetScreenHeight()/2 - loading_text->GetSize().Y;
    loading_perc->SetPosition(loading_perc_posX, loading_perc_posY);

    loading_bar->SetWidth(bar_width*(progress/100));

    int progressInt = (int)progress;

    if(progressInt!=currentProgress){
        if(currentProgress % 15 == 0){
            loading_text->SetText(p_getLoadingStatement());
            float loading_text_posX = g_engine->GetScreenWidth()/2 - loading_text->GetSize().X/2;
            float loading_text_posY = g_engine->GetScreenHeight()/3;
            loading_text->SetPosition(loading_text_posX, loading_text_posY);
        }
        currentProgress = progressInt;
        g_engine->drawAll();
    }
}

void LoadingScreen::Draw(){

}

std::string LoadingScreen::p_getLoadingStatement(){
    std::string loading_statement = "";
    int arraySize = sizeof(LoadingStatements)/sizeof(LoadingStatements[0]);
    int index = rand() % arraySize;
    loading_statement = LoadingStatements[index];
    return loading_statement;
}