#ifndef MENURECEIVER_H
#define MENURECEIVER_H

#include <iostream>
#include <SoundEngine/SoundSystem.h>
#include <Menus.h>
#include <Keycodes.h>
#include <IrrIMGUI/IrrIMGUI.h>
#include <irrlicht/irrlicht.h>

class MenuReceiver : public IrrIMGUI::CIMGUIEventReceiver{

protected:
	bool escape;
    SoundEvent* soundEvent;

    virtual bool OnEvent(const irr::SEvent& event);
    void createSoundEvent();
    
public:
    bool EscPressed();
    void Update();
    MenuReceiver();

};

#endif