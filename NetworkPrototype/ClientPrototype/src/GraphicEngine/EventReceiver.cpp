#include "EventReceiver.h"

bool EventReceiver::OnEvent(const irr::SEvent& event) {

    if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
        if (event.KeyInput.PressedDown == true) {
            if (keyState[event.KeyInput.Key] == UP || keyState[event.KeyInput.Key] == RELEASED){
                keyState[event.KeyInput.Key] = PRESSED;
            }
            else keyState[event.KeyInput.Key] = DOWN;
        }
        else if (keyState[event.KeyInput.Key] != UP) keyState[event.KeyInput.Key] = RELEASED;
    }

    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT){
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN){
            if (mouseButtonState[0] == UP || mouseButtonState[0] == RELEASED) mouseButtonState[0] = PRESSED;
            else mouseButtonState[0] = DOWN;
        }
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP){
            if (mouseButtonState[0] != UP) mouseButtonState[0] = RELEASED;
        }
    }

    return false;
}

bool EventReceiver::leftMousePressed() { return (mouseButtonState[0] == PRESSED); }
bool EventReceiver::keyPressed(irr::EKEY_CODE keycode) { return (keyState[keycode] == PRESSED); }
bool EventReceiver::keyDown(irr::EKEY_CODE keycode) { return (keyState[keycode] == DOWN || keyState[keycode] == PRESSED); }

void EventReceiver::Update(){
    for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++){
        if(keyState[i] == PRESSED) keyState[i] = DOWN;
    }
    for (int i = 0; i < numMouseButtons; i++){
        if(mouseButtonState[i] == PRESSED) mouseButtonState[i] = DOWN;
    }
}

EventReceiver::EventReceiver() {
    for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++) keyState[i] = UP;
    for (int i = 0; i < numMouseButtons; i++) mouseButtonState[i] = UP;
}