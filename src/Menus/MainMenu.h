#ifndef MAINMENU_H
#define MAINMENU_H

#include <GUIEngine/Menu.h>

class MainMenu : public Menu{
public:
    MainMenu();
    ~MainMenu();
    void Update(bool open);
private:
    ImVec2 buttonSize;
    irr::video::ITexture* texture;
    IrrIMGUI::IGUITexture * imageid;
    const char * buttonKeys[4] = {"Single Player", "Multiplayer", "Options", "Exit"};
    const char * descriptions[4] ={"Play as a warlock against our AI","Play with your friends in LAN","Customize the game options","Exit the game"};
};
#endif