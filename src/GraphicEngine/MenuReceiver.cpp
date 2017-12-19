#include "MenuReceiver.h"

MenuReceiver::MenuReceiver() {
}

bool MenuReceiver::OnEvent(const irr::SEvent& event) {

	if (event.EventType == irr::EET_GUI_EVENT){
		int id = event.GUIEvent.Caller->getID();

		switch(event.GUIEvent.EventType){
			case irr::gui::EGET_BUTTON_CLICKED:{
				selectedOption = (MenuOption)id;
				break;
			}
			default:{
				break;
			}
		}

	}

	return false;
}

void MenuReceiver::Update(){
	selectedOption = NO_OPT;
}

MenuOption MenuReceiver::ReadButtonPressed(){
	MenuOption toRet = selectedOption;
	selectedOption = NO_OPT;
	return (toRet);
}