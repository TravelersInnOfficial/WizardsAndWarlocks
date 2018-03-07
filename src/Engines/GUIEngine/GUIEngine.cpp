#include "GUIEngine.h"

#include <DeathMessages.h>
#include <TravelersOcularEngine/src/TOcularEngine/VideoDriver.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw_gl3.h>
#include <GLFW/glfw3.h>

GUIEngine::GUIEngine(){
    g_engine = GraphicEngine::getInstance();

    //BIND IMGUI WITH GLFW
    GLFWwindow* glf_win = VideoDriver::GetInstance()->GetWindow();
    ImGui_ImplGlfwGL3_Init(glf_win, true);

    //INITIAL NOTIFICATIONS POSITION
    m_notifications_Ypos = 10;
    m_notifications_distance = 10;
}

GUIEngine* GUIEngine::GetInstance(){
    static GUIEngine instance = GUIEngine();
	return &instance;
}

GUIEngine::~GUIEngine(){
    //END IMGUI
    ImGui_ImplGlfwGL3_Shutdown();
}

void GUIEngine::Update(){
    //IMGUI BEGUIN DRAW
    ImGui_ImplGlfwGL3_NewFrame();
    printNotifications();
}

void GUIEngine::Draw(){
   //IMGUI END DRAW
   ImGui::Render();
}

void GUIEngine::printNotifications(){
    bool open = true;
    bool *p_open = &open;
    float initial_Y = m_notifications_Ypos;
    ImVec2 window_size;
    ImVec2 window_pos_pivot = ImVec2(1.0f, 0.0f);
    std::map<std::string,float>::reverse_iterator it = m_notifications_data.rbegin();
    std::vector<std::string> keysToDelete;  // Vector de strings con las keys a eliminar del mapa
    int i = 0;
    for(; it != m_notifications_data.rend(); ++it){

        ImGui::PushID(i);
        ImVec2 window_pos = ImVec2(ImGui::GetIO().DisplaySize.x - initial_Y, m_notifications_Ypos);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowBgAlpha(0.3f);
        std::string w_id = it->first;
        if(!ImGui::Begin(w_id.c_str(), p_open, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
             ImGui::End();
        else{
            ImGui::Text("%s",it->first.c_str());
            window_size = ImGui::GetWindowSize();
            ImGui::End();
        }
        ImGui::PopID();
        m_notifications_Ypos = m_notifications_Ypos + m_notifications_distance + window_size.y;
        i++;

        if(it->second <= ImGui::GetTime()) //m_notifications_data.erase((++it).base());
        keysToDelete.push_back(it->first);
    }

    int size = keysToDelete.size();
    for(int i=0; i<size; i++){
        m_notifications_data.erase(keysToDelete[i]); // Eliminamos por key
    }
    keysToDelete.clear();


    m_notifications_Ypos = initial_Y;
}

void GUIEngine::MakeCustomNotification(std::string data, float time){
    m_notifications_data.insert(std::pair<std::string,float>(data, time+ImGui::GetTime()));
}

void GUIEngine::ShowDeathMessage(std::string victim, float time){
    std::string death_phrase = "";
    int arraySize = sizeof(deathMessages)/sizeof(deathMessages[0]);
    int index = rand() % arraySize;
    death_phrase = deathMessages[index];

    int name_pos = death_phrase.find("_");
    if(name_pos != -1) death_phrase.replace(name_pos,1, victim);

    m_notifications_data.insert(std::pair<std::string,float>(death_phrase, time+ImGui::GetTime()));
}

void GUIEngine::InitReceiver(){
    ImGui_ImplGlfwGL3_InitReceiver();
}