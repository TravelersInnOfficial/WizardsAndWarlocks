#ifndef MENU_H
#define MENU_H

#include "GUIEngine.h"
#include <iostream>
#include <Menus.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw_gl3.h>
class TFSprite;
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
    Menu(MenuType); 
    virtual ~Menu();
    std::string GetID();
    virtual void Update(bool*, float);
    virtual void Close(bool*);
    virtual void Drop();

    float GetWidth();
    float GetHeight();
    MenuType* GetType();

    void SetWidth(float);
    void SetHeight(float);
    void UpdateCursor();
    
protected:
    GUIEngine* gui_engine;
    GraphicEngine* g_engine;
    
    ImGuiWindowFlags w_flags;
    WindowOptions w_opt;

    ImGuiWindowFlags popup_flags;
    WindowOptions popup_opt;
    ImGuiStyle& m_style = ImGui::GetStyle();
    
    const char * m_id;
    MenuType m_type;
    float m_width;
    float m_height;
    float screenWidth;
    float screenHeight;
    float m_posX;
    float m_posY;

    TFSprite* m_cursor;
    float m_fontSize;

    void CalculateFlags();
    static void closeMenu(bool*);

};

#endif