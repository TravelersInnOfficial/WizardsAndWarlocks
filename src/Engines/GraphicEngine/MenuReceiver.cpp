#include "MenuReceiver.h"

MenuReceiver::MenuReceiver() {
	escape = false;
}

bool MenuReceiver::OnEvent(const irr::SEvent& event) {

	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.PressedDown == true) {
			if (event.KeyInput.Key == (irr::EKEY_CODE)KEY_ESCAPE ){
				escape = true;
			}
		}
	}

	if (event.EventType == irr::EET_GUI_EVENT){
		int id = event.GUIEvent.Caller->getID();
		
		switch(event.GUIEvent.EventType){
			case irr::gui::EGET_BUTTON_CLICKED:{
				SoundEvent* click = SoundSystem::getInstance()->createEvent("event:/HUD/Click");
				SoundSystem::getInstance()->playEvent(click);
				selectedOption = (MenuOption)id;
				break;
			}
			default:{
				break;
			}
		}
	}

	//return false;
	return CIMGUIEventReceiver::OnEvent(event);
}

bool MenuReceiver::EscPressed(){
	return escape;
}

void MenuReceiver::Update(){
	selectedOption = NO_OPT;
}

MenuOption MenuReceiver::ReadButtonPressed(){
	MenuOption toRet = selectedOption;
	selectedOption = NO_OPT;
	return (toRet);
}