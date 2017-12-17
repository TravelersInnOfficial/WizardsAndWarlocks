#include <iostream>
#include "./irrlicht/irrlicht.h"

enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED};

class EventReceiver : public irr::IEventReceiver {

protected:

    keyStatesENUM keyState[irr::KEY_KEY_CODES_COUNT];
    
    static const int numMouseButtons = 2;
    keyStatesENUM mouseButtonState[numMouseButtons];

    int selectedOption = -1;

    virtual bool OnEvent(const irr::SEvent& event);

public:

    bool keyPressed(irr::EKEY_CODE keycode);
    bool keyDown(irr::EKEY_CODE keycode);
    bool keyRelease(irr::EKEY_CODE keycode);
    bool keyUp(irr::EKEY_CODE keycode);
    
    void setKeyStatus(irr::EKEY_CODE keycode, keyStatesENUM state);
    keyStatesENUM GetKeyStatus(irr::EKEY_CODE keycode);

    int ReadMenu();

    void Update();

    EventReceiver();

};