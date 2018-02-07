#ifndef ALLIANCEMENU_H
#define ALLIANCEMENU_H

#include <GUIEngine/Menu.h>

class AllianceMenu : public Menu{
public:
    AllianceMenu();
    ~AllianceMenu();
    void Update(bool*);

private:
    static const int N_BUTTONS = 2;

    ImVec2 buttonSize;
    
    const char * buttonKeys[N_BUTTONS] = {"WIZARDS", "WARLOCKS"};
    const char * descriptions[N_BUTTONS] ={"Get the grial back from the warlocks nasty claws!","Deploy traps and protect the grial from the wizards.. or just KILL EM ALL"};

    static void setPlayerWarlock(bool*);
    static void setPlayerWizard(bool*);

    typedef void (*METHOD_PTR)(bool*);
    METHOD_PTR actions[N_BUTTONS] = {&AllianceMenu::setPlayerWarlock,&AllianceMenu::setPlayerWizard};
};

#endif