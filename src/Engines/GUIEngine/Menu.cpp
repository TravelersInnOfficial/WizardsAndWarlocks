#include "Menu.h"

Menu::Menu(MenuType type){
    g_engine = GraphicEngine::getInstance();
    gui_engine = GUIEngine::GetInstance();

    m_id = "";
    m_type = type;
    m_width = 0;
    m_height = 0;
    screenWidth = g_engine->GetScreenWidth();
    screenHeight = g_engine->GetScreenHeight();

    CalculateFlags();
}

Menu::~Menu(){}

std::string Menu::GetID(){return m_id;}

void Menu::Update(bool* open, float deltaTime){std::cout<<"REACHED MENU INTERFACE UPDATE"<<std::endl;}
void Menu::Close(bool* open){std::cout<<"REACHED MENU INTERFACE CLOSE"<<std::endl;}
void Menu::Drop(){std::cout<<"REACHED MENU INTERFACE DROP"<<std::endl;}

void Menu::CalculateFlags(){
    w_flags = 0;
    popup_flags = 0;

    if (w_opt.no_titlebar)  w_flags |= ImGuiWindowFlags_NoTitleBar;
    if (w_opt.no_scrollbar) w_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!w_opt.no_menu)     w_flags |= ImGuiWindowFlags_MenuBar;
    if (w_opt.no_move)      w_flags |= ImGuiWindowFlags_NoMove;
    if (w_opt.no_resize)    w_flags |= ImGuiWindowFlags_NoResize;
    if (w_opt.no_collapse)  w_flags |= ImGuiWindowFlags_NoCollapse;
    //if (no_close)     p_open = nullptr; // Don't pass our bool* to Begin

    popup_opt.no_titlebar = false;
    
    if (popup_opt.no_titlebar)  popup_flags |= ImGuiWindowFlags_NoTitleBar;
    if (popup_opt.no_scrollbar) popup_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!popup_opt.no_menu)     popup_flags |= ImGuiWindowFlags_MenuBar;
    if (popup_opt.no_move)      popup_flags |= ImGuiWindowFlags_NoMove;
    if (popup_opt.no_resize)    popup_flags |= ImGuiWindowFlags_NoResize;
    if (popup_opt.no_collapse)  popup_flags |= ImGuiWindowFlags_NoCollapse;
}

float Menu::GetWidth(){return m_width;}
float Menu::GetHeight(){return m_height;}
MenuType* Menu::GetType(){return &m_type;}

void Menu::SetWidth(float w){m_width = w;}
void Menu::SetHeight(float h){m_height = h;}