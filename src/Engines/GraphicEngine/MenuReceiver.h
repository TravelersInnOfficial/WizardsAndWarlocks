#include <iostream>
//#include "./irrlicht/irrlicht.h"
#include <SoundEngine/SoundSystem.h>
#include <Menus.h>
#include <Keycodes.h>
#include <IrrIMGUI/IrrIMGUI.h>
#include <irrlicht/irrlicht.h>

/*
#include <IrrIMGUI/IrrIMGUI.h>
#include <irrlicht/irrlicht.h>

class GUIReceiver : private IrrIMGUI::CIMGUIEventReceiver{
    bool OnEvent(const irr::SEvent & rEvent){
        // do your own stuff with rEvent...
        // when rEvent has not been consumed by your game logic pass it to the CIMGUIEventReceiver class
        return CIMGUIEventReceiver::OnEvent(rEvent);
    }
};
*/
//class MenuReceiver : public irr::IEventReceiver {
class MenuReceiver : public IrrIMGUI::CIMGUIEventReceiver{

protected:
	bool escape;
    MenuOption selectedOption = NO_OPT;
    virtual bool OnEvent(const irr::SEvent& event);
    SoundEvent* soundEvent;
    void createSoundEvent();
    

public:

    MenuOption ReadMenu();
    MenuOption ReadButtonPressed();
    bool EscPressed();
    void Update();
    MenuReceiver();

};
