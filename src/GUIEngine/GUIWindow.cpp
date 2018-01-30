#include "GUIWindow.h"

GUIWindow::GUIWindow(CEGUI::Window* newWindow){
	myWindow = newWindow;
}

GUIWindow::~GUIWindow(){
	CEGUI::WindowManager::getSingleton().destroyWindow(myWindow);
}

void GUIWindow::SetText(std::string newText){
	myWindow->setText(newText);
}