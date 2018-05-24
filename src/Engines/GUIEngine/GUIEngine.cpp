#include "GUIEngine.h"

#include <DeathMessages.h>
#include <TOcularEngine/VideoDriver.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw_gl3.h>
#include <GLFW/glfw3.h>

GUIEngine::GUIEngine(){
    g_engine = GraphicEngine::getInstance();

    //BIND IMGUI WITH GLFW
    GLFWwindow* glf_win = VideoDriver::GetInstance()->GetWindow();
    ImGui_ImplGlfwGL3_Init(glf_win, true);

    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontFromFileTTF("./../assets/fonts/JuliusSansOne-Regular.ttf", 13);
      
    //INITIAL NOTIFICATIONS POSITION
    m_notifications_Ypos = 10;
    m_notifications_distance = 10;
    entity_info = "";
    m_ID = 0;
}

GUIEngine* GUIEngine::GetInstance(){
    static GUIEngine instance = GUIEngine();
	return &instance;
}

GUIEngine::~GUIEngine(){
    //END IMGUI
    ImGui_ImplGlfwGL3_Shutdown();
}

void GUIEngine::InitReceiver(){
    ImGui_ImplGlfwGL3_InitReceiver();
}

void GUIEngine::Update(){
    //IMGUI BEGUIN DRAW
    ImGui_ImplGlfwGL3_NewFrame();
    printNotifications();
    printEntityInfo();
}

void GUIEngine::Draw(){
   //IMGUI END DRAW
   ImGui::Render();
}

/*************************************************************************************/
/**********************************\ NOTIFICATIONS \**********************************/
/*************************************************************************************/

void GUIEngine::printNotifications(){
    bool open = true;
    bool *p_open = &open;
    float initial_Y = m_notifications_Ypos;
    ImVec2 window_size;
    ImVec2 window_pos_pivot = ImVec2(1.0f, 0.0f);
    std::map<int,std::string>::reverse_iterator it = m_notifications_data.rbegin();
    std::vector<int> keysToDelete;  // Vector de strings con las keys a eliminar del mapa
    int i = 0;
    for(; it != m_notifications_data.rend(); ++it){
        
        std::map<int,std::string>::iterator death_it = m_notifications_death.find(it->first);
        std::map<int,float>::iterator time_it = m_notifications_time.find(it->first);
        std::map<int,GUIMSG_>::iterator type_it = m_notifications_type.find(it->first);

        ImGui::PushID(i);
        //ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4) ImColor::HSV(1.0f, 0.0f, 0.3f));
        ImVec2 window_pos = ImVec2(0, m_notifications_Ypos);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        //ImGui::SetNextWindowBgAlpha(0.3f);
        std::string w_id =std::to_string(it->first);
        if(!ImGui::Begin(w_id.c_str(), p_open, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
             ImGui::End();
        else{
            if(type_it!=m_notifications_type.end()){
                switch((GUIMSG_) type_it->second){
                    case GUIMSG_NOTIFICATION:{
                        ImGui::Text("%s",it->second.c_str());
                    break;
                    }
                    case GUIMSG_DEATH_WAR:{
                        if(death_it!=m_notifications_death.end()){
                            std::string death_msg = death_it->second;

                            std::string delimiter = "_";

                            size_t pos = 0;
                            std::string token;
                            while ((pos = death_msg.find(delimiter)) != std::string::npos) {
                                token = death_msg.substr(0, pos);
                                ImGui::Text("%s",token.c_str());
                                ImGui::SameLine();
                                ImGui::TextColored(ImVec4(0.5f,0.0f,0.5f,1.0f),"%s",it->second.c_str());
                                ImGui::SameLine();
                                death_msg.erase(0, pos + delimiter.length());
                            }
                            ImGui::Text("%s",death_msg.c_str());
                        }
                    break;
                    }
                    case GUIMSG_DEATH_WIZ:{
                        if(death_it!=m_notifications_death.end()){
                            std::string death_msg = death_it->second;

                            std::string delimiter = "_";

                            size_t pos = 0;
                            std::string token;
                            while ((pos = death_msg.find(delimiter)) != std::string::npos) {
                                token = death_msg.substr(0, pos);
                                ImGui::Text("%s",token.c_str());
                                ImGui::SameLine();
                                ImGui::TextColored(ImVec4(0.0f,0.0f,1.0f,1.0f),"%s",it->second.c_str());
                                ImGui::SameLine();
                                death_msg.erase(0, pos + delimiter.length());
                            }
                            ImGui::Text("%s",death_msg.c_str());
                        }
                    }
                    default:
                    break;
                }
            }
                                                   // Second part of the sentence
            window_size = ImGui::GetWindowSize();
            ImGui::End();
        }
        //ImGui::PopStyleColor();
        ImGui::PopID();
        m_notifications_Ypos = m_notifications_Ypos + m_notifications_distance + window_size.y;
        i++;

        if(time_it!=m_notifications_time.end() && time_it->second <= ImGui::GetTime()) keysToDelete.push_back(it->first);
    }

    int size = keysToDelete.size();
    for(int i=0; i<size; i++){
        m_notifications_data.erase(keysToDelete[i]); // Eliminamos por key
        m_notifications_death.erase(keysToDelete[i]);
        m_notifications_time.erase(keysToDelete[i]);
        m_notifications_type.erase(keysToDelete[i]);
        m_ID--;
    }
    keysToDelete.clear();


    m_notifications_Ypos = initial_Y;
}

void GUIEngine::MakeCustomNotification(std::string data, float time){
    m_notifications_data.insert(std::pair<int,std::string>  (m_ID,  data));
    m_notifications_death.insert(std::pair<int,std::string>  (m_ID,  ""));
    m_notifications_time.insert(std::pair<int,float>        (m_ID,  time+ImGui::GetTime()));
    m_notifications_type.insert(std::pair<int,GUIMSG_>      (m_ID,  GUIMSG_NOTIFICATION));
    m_ID++;
}

void GUIEngine::ShowDeathMessage(std::string victim,Alliance team, float time){
    m_notifications_data.insert(std::pair<int,std::string>  (m_ID,  victim));
    m_notifications_death.insert(std::pair<int,std::string> (m_ID,  getDeathMsg()));
    m_notifications_time.insert(std::pair<int,float>        (m_ID,  time+ImGui::GetTime()));
    if(team == ALLIANCE_WARLOCK) m_notifications_type.insert(std::pair<int,GUIMSG_>      (m_ID,  GUIMSG_DEATH_WAR));
    else if(team == ALLIANCE_WIZARD) m_notifications_type.insert(std::pair<int,GUIMSG_>  (m_ID,  GUIMSG_DEATH_WIZ));
    else m_notifications_type.insert(std::pair<int,GUIMSG_> (m_ID,  GUIMSG_NONE));
    m_ID++;
}

std::string GUIEngine::getDeathMsg(){
    std::string death_phrase = "";
    int arraySize = sizeof(deathMessages)/sizeof(deathMessages[0]);
    int index = rand() % arraySize;
    death_phrase = deathMessages[index];

    return death_phrase;
}
/*************************************************************************************/
/**********************************\ ENTITIES INFO \**********************************/
/*************************************************************************************/

void GUIEngine::printEntityInfo(){
    if(entity_info != ""){
        bool open = true;
        bool *p_open = &open;
        float W = g_engine->GetScreenWidth();
        float H = g_engine->GetScreenHeight();
        ImVec2 window_pos = ImVec2(W/2, H/2);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.3f);
        if(!ImGui::Begin(entity_info.c_str(), p_open, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
            ImGui::End();
        else{
            ImGui::Text("%s",entity_info.c_str());
            ImGui::End();
        }
        entity_info = "";
    }
}

void GUIEngine::ShowEntityInfo(std::string info){
    if(info != "") entity_info = info;
}