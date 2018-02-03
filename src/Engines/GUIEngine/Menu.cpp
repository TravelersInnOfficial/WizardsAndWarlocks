#include "Menu.h"

Menu::Menu(){
    m_id = "";
    g_engine = GraphicEngine::getInstance();
    screenWidth = g_engine->GetScreenWidth();
    screenHeight = g_engine->GetScreenHeight();
    m_width = 0;
    m_height = 0;

    w_flags = 0;
    CalculateFlags();
}

Menu::~Menu(){}

std::string Menu::GetID(){return m_id;}

void Menu::Update(bool open){std::cout<<"REACHED MENU INTERFACE UPDATE"<<std::endl;}

void Menu::CalculateFlags(){
    w_flags = 0;

    if (w_opt.no_titlebar)  w_flags |= ImGuiWindowFlags_NoTitleBar;
    if (w_opt.no_scrollbar) w_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!w_opt.no_menu)     w_flags |= ImGuiWindowFlags_MenuBar;
    if (w_opt.no_move)      w_flags |= ImGuiWindowFlags_NoMove;
    if (w_opt.no_resize)    w_flags |= ImGuiWindowFlags_NoResize;
    if (w_opt.no_collapse)  w_flags |= ImGuiWindowFlags_NoCollapse;
    //if (no_close)     p_open = NULL; // Don't pass our bool* to Begin
}

float Menu::GetWidth(){return m_width;}
float Menu::GetHeight(){return m_height;}

void Menu::SetWidth(float w){m_width = w;}
void Menu::SetHeight(float h){m_height = h;}