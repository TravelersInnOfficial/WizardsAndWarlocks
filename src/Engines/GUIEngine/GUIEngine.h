#ifndef GUIENGINE_H
#define GUIENGINE_H

#include <vector3d.h>
#include <vector2d.h>
#include <map>
#include <GraphicEngine/GraphicEngine.h>
#include <Alliance.h>

enum GUIMSG_{
    GUIMSG_NONE         = 0,
    GUIMSG_NOTIFICATION = 1,
    GUIMSG_DEATH_WAR    = 2,
    GUIMSG_DEATH_WIZ    = 3
};

// Fast-forward declaration
class GUIEngine{
    friend class Menu;
    
public:
	static GUIEngine* GetInstance();
    ~GUIEngine();
    void Update();
    void Draw();
    void MakeCustomNotification(std::string, float time = 3.5f);
    void ShowDeathMessage(std::string, Alliance team = NO_ALLIANCE, float time = 3.5f);
    void ShowEntityInfo(std::string);
    void InitReceiver();

private:
    GraphicEngine* g_engine;
    GUIEngine();
    void printNotifications();
    void printEntityInfo();
    std::string getDeathMsg();

    //NOTIFICATIONS DATA
    float m_notifications_Ypos;
    float m_notifications_distance;
    int m_ID;
    std::map<int,std::string> m_notifications_data;
    std::map<int,std::string> m_notifications_death;
    std::map<int,float> m_notifications_time;
    std::map<int,GUIMSG_> m_notifications_type;
    std::string entity_info;

};
#endif