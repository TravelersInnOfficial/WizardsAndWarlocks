#include "GUIInputManager.h"

static GUIInputManager* instance = NULL;

GUIInputManager::GUIInputManager(){

}

GUIInputManager* GUIInputManager::GetInstance(){
	if(instance == NULL) instance = new GUIInputManager();
	return instance;
}

void GUIInputManager::PositionMouse(float x, float y){
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(x, y);
}

void GUIInputManager::MousePress(int mouseButton){
	if(mouseButton == 0) CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
	else if (mouseButton == 1) CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::RightButton);
}

void GUIInputManager::MouseRelease(int mouseButton){
	if(mouseButton == 0) CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
	else if (mouseButton == 1) CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
}