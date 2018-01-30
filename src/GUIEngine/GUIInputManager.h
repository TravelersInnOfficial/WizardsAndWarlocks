#ifndef GUIINPUTMANAGER_H
#define GUIINPUTMANAGER_H

#include <iostream>
#include <CEGUI/CEGUI.h>
#include "GUIEnums.h"


class GUIInputManager{

public:
	static GUIInputManager* GetInstance();
    
	void PositionMouse(float x, float y);
	void MousePress(int mouseButton = 0);
	void MouseRelease(int mouseButton = 0);
    
	~GUIInputManager();

private:
    GUIInputManager();

};

#endif