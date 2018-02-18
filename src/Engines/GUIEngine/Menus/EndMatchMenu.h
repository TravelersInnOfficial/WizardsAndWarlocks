#ifndef ENDMATCHMENU_H
#define ENDMATCHMENU_H

# include <GUIEngine/Menu.h>
class EndMatchMenu : public Menu{
public:
    EndMatchMenu(MenuType, int);
    ~EndMatchMenu();
    void Update(bool*, float);
    void Drop();

private:
    std::string m_winners;
    bool m_returnLobby;
    void closeMenu(bool*);

};

#endif