#ifndef GUIENGINE_H
#define GUIENGINE_H

#include <IrrIMGUI/IrrIMGUI.h>
#include <IMGUI/imgui.h>
#include <irrlicht/irrlicht.h>
#include <vector3d.h>
#include <vector2d.h>
#include <iostream>
#include <GraphicEngine/GraphicEngine.h>
#include <map>

class GUIEngine{
    friend class Menu;
    
public:
	static GUIEngine* GetInstance();
    ~GUIEngine();
    void Update();
    void Draw();
    void MakeTemporalNotification(std::string);
    void MakeCustomNotification(std::string, float);
    void ShowDeathMessage(std::string, float);

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
    void printNotifications();

    //notifications data
    float m_notifications_Ypos;
    float m_notifications_distance;
    float m_notifications_time;
    std::map<std::string,float> m_notifications_data;

};
#endif