#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht/irrlicht.h>

class EventReceiver : public irr::IEventReceiver{

public:
    // This is the one method that we have to implement
    virtual bool OnEvent(const irr::SEvent& event)
    {
        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }
    
    EventReceiver()
    {
        for (irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

private:
        bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
};

#endif