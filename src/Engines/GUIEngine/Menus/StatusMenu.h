#ifndef STATUSMENU_H
#define STATUSMENU_H

#include <GUIEngine/Menu.h>

class StatusMenu : public Menu{
public:
    StatusMenu(MenuType);
    ~StatusMenu();
    void Update(bool* open, float deltaTime);
    void Close(bool*);
private:

    ImTextureID warlock_icon;
    ImTextureID wizard_icon;

    ImVec2 icon_size;
};

#endif