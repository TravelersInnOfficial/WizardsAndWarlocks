#include <iostream>
#include "./irrlicht/irrlicht.h"
#include <KeyStates.h>

class EventReceiver : public irr::IEventReceiver {

protected:

    keyStatesENUM keyState[irr::KEY_KEY_CODES_COUNT];
    
    static const int numMouseButtons = 4;
    keyStatesENUM mouseButtonState[numMouseButtons];
    bool OnEvent(const irr::SEvent& event);

public:

    bool keyPressed(irr::EKEY_CODE keycode);
    bool keyDown(irr::EKEY_CODE keycode);
    bool keyRelease(irr::EKEY_CODE keycode);
    bool keyUp(irr::EKEY_CODE keycode);
    
    void setKeyStatus(irr::EKEY_CODE keycode, keyStatesENUM state);
    void InitReceiver();
    keyStatesENUM GetKeyStatus(irr::EKEY_CODE keycode);

    void Update();

    EventReceiver();

};
