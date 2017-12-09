#include "EventReceiver.h"
#include "Keycodes.h"

EventReceiver::EventReceiver() {
	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++) keyState[i] = UP;
	for (int i = 0; i < numMouseButtons; i++) mouseButtonState[i] = UP;
}

bool EventReceiver::OnEvent(const irr::SEvent& event) {

	if (event.EventType == irr::EET_GUI_EVENT){
		int id = event.GUIEvent.Caller->getID();

		switch(event.GUIEvent.EventType){
			case irr::gui::EGET_BUTTON_CLICKED:
			switch(id) {
				case 1:{
					selectedOption = 1;
					break;
				}
				case 2:{
					selectedOption = 2;
					break;
				}
				case 3:{
					selectedOption = 3;
					break;
				}
				default:{
					return (false);
					break;
				}
			}
		}

	}

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
		//LeftButton
		if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN){
			if (mouseButtonState[KEY_LBUTTON] == UP || mouseButtonState[KEY_LBUTTON] == RELEASED) mouseButtonState[0] = PRESSED;
			else mouseButtonState[KEY_LBUTTON] = DOWN;
		}
		if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP){
			if (mouseButtonState[KEY_LBUTTON] != UP) mouseButtonState[KEY_LBUTTON] = RELEASED;
		}
		//RightButton
		 if (event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN){
			if (mouseButtonState[KEY_RBUTTON] == UP || mouseButtonState[KEY_RBUTTON] == RELEASED) mouseButtonState[1] = PRESSED;
			else mouseButtonState[KEY_RBUTTON] = DOWN;
		}
		if (event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP){
			if (mouseButtonState[KEY_RBUTTON] != UP) mouseButtonState[KEY_RBUTTON] = RELEASED;
		} 
	}

	return false;
}

bool EventReceiver::keyPressed(irr::EKEY_CODE keycode) { return (keyState[keycode] == PRESSED); }
bool EventReceiver::keyDown(irr::EKEY_CODE keycode) { return (keyState[keycode] == DOWN || keyState[keycode] == PRESSED); }
bool EventReceiver::keyRelease(irr::EKEY_CODE keycode) { return (keyState[keycode] == RELEASED); }
bool EventReceiver::keyUp(irr::EKEY_CODE keycode) { return (keyState[keycode] == UP); }

keyStatesENUM EventReceiver::GetKeyStatus(irr::EKEY_CODE keycode){
	if(keycode > 2) return keyState[keycode];
	else return mouseButtonState[keycode];
}

void EventReceiver::Update(){

	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++){
		if(keyState[i] == PRESSED) keyState[i] = DOWN;
		else if(keyState[i] == RELEASED) keyState[i] = UP;
	}
	for (int i = 0; i < numMouseButtons; i++){
		if(mouseButtonState[i] == PRESSED) mouseButtonState[i] = DOWN;
		else if(mouseButtonState[i] == RELEASED) mouseButtonState[i] = UP;
	}
	
	selectedOption = -1;

}

void EventReceiver::setKeyStatus(irr::EKEY_CODE keycode, keyStatesENUM state){
	if(keycode > 2) keyState[keycode] = state;
	else mouseButtonState[keycode] = state;
}

int EventReceiver::ReadMenu(){
	int toRet = selectedOption;
	selectedOption = -1;
	return (toRet);
}