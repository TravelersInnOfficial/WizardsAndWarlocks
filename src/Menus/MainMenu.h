#ifndef MAINMENU_H
#define MAINMENU_H

#include <GUIEngine/Menu.h>

const int N_BUTTONS = 4;

class MainMenu : public Menu{
public:
    MainMenu();
    ~MainMenu();
    void Update(bool open);
private:
    
    ImVec2 buttonSize;

    const char * buttonLayouts[N_BUTTONS]   = { "./../assets/textures/GUI/Menus/MainMenu/single_player_button.png",
                                                "./../assets/textures/GUI/Menus/MainMenu/multiplayer_button.png",
                                                "./../assets/textures/GUI/Menus/MainMenu/options_button.png",
                                                "./../assets/textures/GUI/Menus/MainMenu/exit_button.png" };

    irr::video::ITexture* texture[N_BUTTONS];
    IrrIMGUI::IGUITexture * imageid[N_BUTTONS];
    const char * descriptions[N_BUTTONS] ={"Play as a warlock against our AI","Play with your friends in LAN","Customize the game options","Exit the game"};

};
#endif