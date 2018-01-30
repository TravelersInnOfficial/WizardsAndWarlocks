#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#include <iostream>
#include <CEGUI/CEGUI.h>
#include "GUIEnums.h"

class GUIEngine;

class GUIWindow{
    friend class GUIEngine;

public:
    
    template<typename T> void Subscribe(GUIEventType eventType, void(T::* function)(void), T* object){
	    switch(eventType){
            case(GUI_MOUSECLICK):{
                myWindow->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(function, object));
                break;
            }
            default:{
                break;
            }
	    }
    }

    void SetText(std::string newText);
    

private:
    CEGUI::Window* myWindow = NULL;
    GUIWindow(CEGUI::Window* newWindow);
    ~GUIWindow();
};

#endif