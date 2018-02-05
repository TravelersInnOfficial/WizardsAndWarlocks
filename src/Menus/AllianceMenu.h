#ifndef ALLIANCEMENU_H
#define ALLIANCEMENU_H

#include <GUIEngine/Menu.h>

const int N_BUTTONS_ALLIANCE = 2;

class AllianceMenu : public Menu{
public:
    AllianceMenu();
    ~AllianceMenu();
    void Update(bool);

private:
    ImVec2 buttonSize;
    
    const char * buttonKeys[N_BUTTONS_ALLIANCE] = {"WIZARDS", "WARLOCKS"};
    const char * descriptions[N_BUTTONS_ALLIANCE] ={"Get the grial back from the warlocks nasty claws!","Deploy traps and protect the grial from the wizards.. or just KILL EM ALL"};
};

#endif