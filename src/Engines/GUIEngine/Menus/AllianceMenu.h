#ifndef ALLIANCEMENU_H
#define ALLIANCEMENU_H

#include <GUIEngine/Menu.h>

class GSprite;

class AllianceMenu : public Menu{
public:
    AllianceMenu(MenuType);
    ~AllianceMenu();
    void Update(bool*, float deltaTime);
    void Close(bool*);
    void Drop();

private:
    static const int N_BUTTONS = 2;

    ImVec2 buttonSize;
    
    GSprite* bkg;

    std::vector<std::string> buttonKeys = { "WARLOCKS","WIZARDS"};
    const char * descriptions[N_BUTTONS] = {"Deploy traps and protect the grial from the wizards.. or just KILL EM ALL","Get the grial back from the warlocks nasty claws!"};
    
    ImTextureID tex_alliances_init[N_BUTTONS];
    ImTextureID tex_alliances[N_BUTTONS];
    ImTextureID tex_alliances_hover[N_BUTTONS];

    static void setPlayerWarlock(bool*);
    static void setPlayerWizard(bool*);

    typedef void (*METHOD_PTR)(bool*);
    METHOD_PTR actions[N_BUTTONS] = {&AllianceMenu::setPlayerWarlock,&AllianceMenu::setPlayerWizard};

    static void closeMenu(bool*);
};

#endif