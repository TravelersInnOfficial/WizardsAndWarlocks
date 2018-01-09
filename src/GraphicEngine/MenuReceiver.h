#include <iostream>
#include "./irrlicht/irrlicht.h"
#include "./../SoundEngine/SoundSystem.h"
#include <Menus.h>

class MenuReceiver : public irr::IEventReceiver {

protected:

    MenuOption selectedOption = NO_OPT;
    virtual bool OnEvent(const irr::SEvent& event);
    

public:

    MenuOption ReadMenu();
    MenuOption ReadButtonPressed();
    void Update();
    MenuReceiver();

};
