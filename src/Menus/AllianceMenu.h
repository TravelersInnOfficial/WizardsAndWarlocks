#ifndef ALLIANCEMENU_H
#define ALLIANCEMENU_H

#include <GUIEngine/Menu.h>

class AllianceMenu : public Menu{
public:
    AllianceMenu();
    ~AllianceMenu();
    void Update(bool);

private:
    ImVec2 buttonSize;
    const char * buttonKeys[2] = {"WIZARDS", "WARLOCKS"};
    const char * descriptions[2] ={"Get the grial back from the warlocks nasty claws!","Deploy traps and protect the grial from the wizards.. or just KILL EM ALL"};
};

#endif