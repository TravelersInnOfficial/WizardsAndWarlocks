#ifndef GUIENGINE_H
#define GUIENGINE_H

#include <IMGUI/imgui.h>
#include <IMGUI/imgui-SFML.h>
#include <vector3d.h>
#include <vector2d.h>
#include <GraphicEngine/GraphicEngine.h>
#include <map>
#include <SFML/Graphics.hpp>

class GUIEngine{
    friend class Menu;
    
public:
	static GUIEngine* GetInstance();
    ~GUIEngine();
    void Update();
    void Draw();
    void MakeCustomNotification(std::string, float time = 3.5f);
    void ShowDeathMessage(std::string, float time = 3.5f);

private:
    GraphicEngine* g_engine;
    //irr::IrrlichtDevice* pDevice;
    //IrrIMGUI::IIMGUIHandle * m_GUIHandler; //MGUI handler

    //Create standard event receiver for the IrrIMGUI
    //MenuReceiver* m_EventReceiver;

    GUIEngine();

    //IrrIMGUI::IIMGUIHandle * GetGuiHandler();
    //irr::IrrlichtDevice* GetPDevice();
    void printNotifications();

    //notifications data
    float m_notifications_Ypos;
    float m_notifications_distance;
    std::map<std::string,float> m_notifications_data;
    sf::Texture* cursor;

};
#endif