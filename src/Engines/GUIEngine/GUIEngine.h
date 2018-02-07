#ifndef GUIENGINE_H
#define GUIENGINE_H

#include <IrrIMGUI/IrrIMGUI.h>
#include <irrlicht/irrlicht.h>
#include <vector3d.h>
#include <iostream>
#include <GraphicEngine/GraphicEngine.h>

class GUIEngine{
    friend class Menu;
    
public:
	static GUIEngine* GetInstance();
    ~GUIEngine();
    void Update();
    void Draw();

private:
    static GUIEngine* instance; 

    GraphicEngine* g_engine;
    irr::IrrlichtDevice* pDevice;
    IrrIMGUI::IIMGUIHandle * m_GUIHandler; //MGUI handler

    //Create standard event receiver for the IrrIMGUI
    MenuReceiver* m_EventReceiver;

    GUIEngine();

    IrrIMGUI::IIMGUIHandle * GetGuiHandler();
    irr::IrrlichtDevice* GetPDevice();
};
#endif