#ifndef GUIENGINE_H
#define GUIENGINE_H

#include <IrrIMGUI/IrrIMGUI.h>
#include <irrlicht/irrlicht.h>
#include <vector3d.h>
#include <iostream>
#include <GraphicEngine/GraphicEngine.h>

class GUIEngine{
public:
	static GUIEngine* GetInstance();
    ~GUIEngine();
    void update();
    void draw();

    void ToggleMenu();

private:
    static GUIEngine* instance; 

    GraphicEngine* g_engine;

    //Create standard event receiver for the IrrIMGUI
    MenuReceiver* m_EventReceiver;
    //MGUI handler
    IrrIMGUI::IIMGUIHandle * m_GUIHandler;

    GUIEngine();
};
#endif