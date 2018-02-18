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
				createSoundEvent();									//Create the click event
				SoundSystem::getInstance()->playEvent(soundEvent);	//Play the event
				soundEvent->release();								//Release the event
				selectedOption = (MenuOption)id;
				break;
			}
			//case ImGui::GetIO()->
			/*
			for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) 
			*/
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

void MenuReceiver::createSoundEvent() {
	soundEvent = SoundSystem::getInstance()->createEvent("event:/HUD/Click");
}