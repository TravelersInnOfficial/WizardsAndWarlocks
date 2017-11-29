#include <iostream>
#include "./irrlicht/irrlicht.h"

class EventReceiver : public irr::IEventReceiver {

protected:

    enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED};
    keyStatesENUM keyState[irr::KEY_KEY_CODES_COUNT];
    
    static const int numMouseButtons = 1;
    keyStatesENUM mouseButtonState[numMouseButtons];

    virtual bool OnEvent(const irr::SEvent& event);

public:

    bool leftMousePressed();
    bool leftMouseDown();
    bool keyPressed(irr::EKEY_CODE keycode);
    bool keyDown(irr::EKEY_CODE keycode);
    bool keyRelease(irr::EKEY_CODE keycode);
    bool keyUp(irr::EKEY_CODE keycode);

    void Update();

    EventReceiver();

};
