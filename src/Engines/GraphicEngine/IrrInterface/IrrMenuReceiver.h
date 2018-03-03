#ifndef IRRMENURECEIVER_H
#define IRRMENURECEIVER_H

#include <iostream>
#include <SoundEngine/SoundSystem.h>
#include <Menus.h>
#include <Keycodes.h>
//#include <IrrIMGUI/IrrIMGUI.h>
#include <irrlicht/irrlicht.h>

class IrrMenuReceiver : public irr::IEventReceiver /*IrrIMGUI::CIMGUIEventReceiver*/{

protected:
	bool escape;
    SoundEvent* soundEvent;
    
    virtual bool OnEvent(const irr::SEvent& event);
    void createSoundEvent();
    
public:
    bool EscPressed();
    void Update();
    IrrMenuReceiver();

};

#endif