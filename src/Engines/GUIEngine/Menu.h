#ifndef MENU_H
#define MENU_H

#include "GUIEngine.h"
#include <iostream>

struct WindowOptions{
    bool no_titlebar = true;
    bool no_scrollbar = true;
    bool no_menu = true;
    bool no_move = true;
    bool no_resize = true;
    bool no_collapse = true;
    bool no_close = true;
};

class Menu{
public:
    Menu(); 
    virtual ~Menu();
    std::string GetID();
    virtual void Update(bool*);

    float GetWidth();
    float GetHeight();

    void SetWidth(float);
    void SetHeight(float);
    
protected:
    GUIEngine* gui_engine;
    GraphicEngine* g_engine;
    irr::IrrlichtDevice* pDevice;
    IrrIMGUI::IIMGUIHandle * GUI;
    
    ImGuiWindowFlags w_flags;
    WindowOptions w_opt;
    ImGuiStyle& m_style = ImGui::GetStyle();
    
    const char * m_id;
    float m_width;
    float m_height;
    float screenWidth;
    float screenHeight;

    void CalculateFlags();
    static void closeMenu(bool*);

};

#endif