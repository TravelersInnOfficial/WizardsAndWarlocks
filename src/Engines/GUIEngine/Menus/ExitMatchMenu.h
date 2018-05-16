#ifndef EXITMATCHMENU_H
#define EXITMATCHMENU_H

#include <GUIEngine/Menu.h>

class ExitMatchMenu : public Menu{
public:
    ExitMatchMenu(MenuType);
    ~ExitMatchMenu();

    void Update(bool* open, float deltaTime);
    void Close(bool*);

private:
};

#endif