#include "EventReceiver.h"
#include "Keycodes.h"

EventReceiver::EventReceiver() {
	InitReceiver();
}

void EventReceiver::InitReceiver(){
	for (int i = 0; i < Key_KeyCount; i++) keyState[i] = UP;
}

bool EventReceiver::OnEvent(const TEvent& event) {

	std::cout<<"ENTRO"<<std::endl;

	if (event.m_type == Type_KeyPressed || event.m_type == Type_KeyReleased) {
		KeyboardKey eventKey = event.m_key.code;
		if (event.m_type == Type_KeyPressed) {
			if (keyState[eventKey] == UP || keyState[eventKey] == RELEASED){
				keyState[eventKey] = PRESSED;
			}
			else keyState[eventKey] = DOWN;
		}
		else if (keyState[eventKey] != UP) keyState[eventKey] = RELEASED;
	}

	if (event.m_type == Type_MouseButtonPressed || event.m_type == Type_MouseButtonReleased){
		KeyboardKey eventKey = event.m_mouseButton.button;
		if (event.m_type == Type_MouseButtonPressed) {
			if (keyState[eventKey] == UP || keyState[eventKey] == RELEASED){
				keyState[eventKey] = PRESSED;
			}
			else keyState[eventKey] = DOWN;
		}
		else if (keyState[eventKey] != UP) keyState[eventKey] = RELEASED;
	}

	if (event.m_type == Type_MouseWheelScrolled){
		if(event.m_mouseWheel.delta > 0) keyState[Key_MouseMiddle_Up] = RELEASED;
		else if (event.m_mouseWheel.delta < 0) keyState[Key_MouseMiddle_Down] = RELEASED;
	}

	return false;
}

bool EventReceiver::keyPressed(KeyboardKey keycode) { return (keyState[keycode] == PRESSED); }
bool EventReceiver::keyDown(KeyboardKey keycode) { return (keyState[keycode] == DOWN || keyState[keycode] == PRESSED); }
bool EventReceiver::keyRelease(KeyboardKey keycode) { return (keyState[keycode] == RELEASED); }
bool EventReceiver::keyUp(KeyboardKey keycode) { return (keyState[keycode] == UP); }

keyStatesENUM EventReceiver::GetKeyStatus(KeyboardKey keycode){
	return keyState[keycode];
}

void EventReceiver::Update(){
	for (int i = 0; i < Key_KeyCount; i++){
		if(keyState[i] == PRESSED) keyState[i] = DOWN;
		else if(keyState[i] == RELEASED) keyState[i] = UP;
	}
}

void EventReceiver::setKeyStatus(KeyboardKey keycode, keyStatesENUM state){
	keyState[keycode] = state;
}