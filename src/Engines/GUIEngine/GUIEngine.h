#ifndef GUIENGINE_H
#define GUIENGINE_H

#include <vector3d.h>
#include <vector2d.h>
#include <map>
#include <GraphicEngine/GraphicEngine.h>

// Fast-forward declaration
class GUIEngine{
    friend class Menu;
    
public:
	static GUIEngine* GetInstance();
    ~GUIEngine();
    void Update();
    void Draw();
    void MakeCustomNotification(std::string, float time = 3.5f);
    void ShowDeathMessage(std::string, float time = 3.5f);
    void InitReceiver();

private:
    GraphicEngine* g_engine;
    GUIEngine();
    void printNotifications();

    //NOTIFICATIONS DATA
    float m_notifications_Ypos;
    float m_notifications_distance;
    std::map<std::string,float> m_notifications_data;

};
#endif