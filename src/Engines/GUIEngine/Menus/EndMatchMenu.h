#ifndef ENDMATCHMENU_H
#define ENDMATCHMENU_H

# include <GUIEngine/Menu.h>

class HumanPlayer;
class GSprite;

class EndMatchMenu : public Menu{
public:
    EndMatchMenu(MenuType, int);
    ~EndMatchMenu();
    void Update(bool*, float);

private:
    HumanPlayer* hp;
    bool m_victory;
    std::string m_msg;
    void closeMenu(bool*);

    ImVec2 buttonSize;                  //MENU BUTTON SIZE
    ImVec2 imgSize;

    ImTextureID texture_init;
    ImTextureID texture_button;
    ImTextureID texture_hover;
    ImTextureID texture_pressed;
    
    GSprite* bkg;

};

#endif