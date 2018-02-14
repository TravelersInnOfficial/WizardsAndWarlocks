#ifndef MAINMENU_H
#define MAINMENU_H

#include <GUIEngine/Menu.h>
#include <NetworkEngine/NetSeeker.h>

class MainMenu : public Menu{
public:
    MainMenu();
    ~MainMenu();
    void Update(bool* open);

private:

    static const int N_BUTTONS = 4;

    ImVec2 buttonSize;

    static bool m_options;
    static bool m_exit;
    static bool m_multiplayer;
    bool m_none_selected;
    bool m_direct_connection;
    bool m_start_host;

    NetSeeker*		netSeeker;
    std::vector<ServerData> serverList;
    
    const char * buttonLayouts[N_BUTTONS]   = { "./../assets/textures/GUI/Menus/MainMenu/single_player_button.png",
                                                "./../assets/textures/GUI/Menus/MainMenu/multiplayer_button.png",
                                                "./../assets/textures/GUI/Menus/MainMenu/options_button.png",
                                                "./../assets/textures/GUI/Menus/MainMenu/exit_button.png" };

    irr::video::ITexture* texture[N_BUTTONS];
    IrrIMGUI::IGUITexture * imageid[N_BUTTONS];
    const char * descriptions[N_BUTTONS] ={"Play as a warlock against our AI","Play with your friends in LAN","Customize the game options","Exit the game"};

    static void SinglePlayer(bool*);
    static void MultiPlayer(bool*);
    static void GameOptions(bool*);
    static void ExitGame(bool*);

    static void closeMenu(bool*);

    typedef void (*METHOD_PTR)(bool*);
    METHOD_PTR actions[N_BUTTONS] = {&MainMenu::SinglePlayer,&MainMenu::MultiPlayer,&MainMenu::GameOptions,&MainMenu::ExitGame};
};

#endif