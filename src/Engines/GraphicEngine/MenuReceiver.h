#include <iostream>
#include "./irrlicht/irrlicht.h"
#include <SoundEngine/SoundSystem.h>
#include <Menus.h>
#include <Keycodes.h>

class MenuReceiver : public irr::IEventReceiver {

protected:
	bool escape;
    MenuOption selectedOption = NO_OPT;
    virtual bool OnEvent(const irr::SEvent& event);
    

public:

    MenuOption ReadMenu();
    MenuOption ReadButtonPressed();
    bool EscPressed();
    void Update();
    MenuReceiver();

};
