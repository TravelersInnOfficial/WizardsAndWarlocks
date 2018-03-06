#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <iostream>
#include <KeyStates.h>
#include <TravelersOcularEngine/src/TOcularEngine/IODriver.h>

class EventReceiver : public IODriver {

protected:
    keyStatesENUM keyState[Key_KeyCount];

    bool OnEvent(const TEvent& event);

public:

    bool keyPressed(KeyboardKey keycode);
    bool keyDown(KeyboardKey keycode);
    bool keyRelease(KeyboardKey keycode);
    bool keyUp(KeyboardKey keycode);
    
    void setKeyStatus(KeyboardKey keycode, keyStatesENUM state);
    void InitReceiver();
    keyStatesENUM GetKeyStatus(KeyboardKey keycode);

    void Update();

    EventReceiver();

};

#endif