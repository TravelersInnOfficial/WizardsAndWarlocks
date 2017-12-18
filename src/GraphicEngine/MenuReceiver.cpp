#include "MenuReceiver.h"
#include "Keycodes.h"

MenuReceiver::MenuReceiver() {
}

bool MenuReceiver::OnEvent(const irr::SEvent& event) {

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
					break;
				}
			}
			default:{
				break;
			}
		}

	}

	return false;
}

void MenuReceiver::Update(){
	selectedOption = -1;
}

int MenuReceiver::ReadMenu(){
	int toRet = selectedOption;
	selectedOption = -1;
	return (toRet);
}