#include "IrrEventReceiver.h"
#include <Keycodes.h>

IrrEventReceiver::IrrEventReceiver() {
	InitReceiver();
}

void IrrEventReceiver::InitReceiver(){
	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++) keyState[i] = UP;
	for (int i = 0; i < numMouseButtons; i++) mouseButtonState[i] = UP;
}

bool IrrEventReceiver::OnEvent(const irr::SEvent& event) {

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
		int keycode = -1;
		int DownUp = 0;	// 0 -> Down , 1 -> Up
		//LeftButton
		if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN){
			keycode = KEY_LBUTTON; DownUp = 0;
		}
		else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP){
			keycode = KEY_LBUTTON; DownUp = 1;
		}
		//RightButton
		if (event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN){
		 	keycode = KEY_RBUTTON; DownUp = 0;
		}
		else if (event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP){
			keycode = KEY_RBUTTON; DownUp = 1;
		}
		// WheelUp
		if (event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL){
			DownUp = 1;	// Pongo Released para que luego pase a UP al no existir un levantar rueda
			if(event.MouseInput.Wheel > 0){
				keycode = KEY_WHEEL_UP;
			}
			else{
				keycode = KEY_WHEEL_DOWN;
			}			 	
		}
		// Asignation
		if(keycode!=-1){
			if(DownUp == 0){
				if (mouseButtonState[keycode] == UP || mouseButtonState[keycode] == RELEASED) mouseButtonState[keycode] = PRESSED;
				else mouseButtonState[keycode] = DOWN;
			}
			else if(DownUp == 1){
				mouseButtonState[keycode] = RELEASED;
			}
		}
	}

	return false;
}

bool IrrEventReceiver::keyPressed(irr::EKEY_CODE keycode) { return (keyState[keycode] == PRESSED); }
bool IrrEventReceiver::keyDown(irr::EKEY_CODE keycode) { return (keyState[keycode] == DOWN || keyState[keycode] == PRESSED); }
bool IrrEventReceiver::keyRelease(irr::EKEY_CODE keycode) { return (keyState[keycode] == RELEASED); }
bool IrrEventReceiver::keyUp(irr::EKEY_CODE keycode) { return (keyState[keycode] == UP); }

keyStatesENUM IrrEventReceiver::GetKeyStatus(irr::EKEY_CODE keycode){
	if(keycode > numMouseButtons) return keyState[keycode];
	else return mouseButtonState[keycode];
}

void IrrEventReceiver::Update(){

	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++){
		if(keyState[i] == PRESSED) keyState[i] = DOWN;
		else if(keyState[i] == RELEASED) keyState[i] = UP;
	}
	for (int i = 0; i < numMouseButtons; i++){
		if(mouseButtonState[i] == PRESSED) mouseButtonState[i] = DOWN;
		else if(mouseButtonState[i] == RELEASED) mouseButtonState[i] = UP;
	}

}

void IrrEventReceiver::setKeyStatus(irr::EKEY_CODE keycode, keyStatesENUM state){
	if(keycode > numMouseButtons) keyState[keycode] = state;
	else mouseButtonState[keycode] = state;
}