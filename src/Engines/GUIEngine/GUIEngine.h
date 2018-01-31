#ifndef GUIENGINE_H
#define GUIENGINE_H

#include <IrrIMGUI/IrrIMGUI.h>
#include <irrlicht/irrlicht.h>

#include "./../../GraphicEngine/GraphicEngine.h"

class GUIEngine{
public:
	static GUIEngine* GetInstance();
    ~GUIEngine();
    
    void update();
    void draw();
    void createMenu();
    void createNotification();
    void createButton();

private:
    static GUIEngine* instance; 

    GraphicEngine* g_engine;
    IrrIMGUI::IIMGUIHandle * pGUI;

    GUIEngine();
};
#endif