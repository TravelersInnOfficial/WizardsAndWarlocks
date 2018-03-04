#include "GUIEngine.h"
#include <DeathMessages.h>
#include <TravelersOcularEngine/src/TOcularEngine/VideoDriver.h>
#include <SFML/Graphics.hpp>

GUIEngine::GUIEngine(){

    g_engine = GraphicEngine::getInstance();
    //sf::Texture* texture = MyEngine::CreateTextureFromMemoryPixels(pixels, width, height, TEXTURE_TYPE_RGBA);
     // TODO: Store your texture pointer/identifier (whatever your engine uses) in 'io.Fonts->TexID'. This will be passed back to your via the renderer.
    //ImGui::GetIO().Fonts->TexID = (void*)texture
    ImGui::SFML::Init(*VideoDriver::GetInstance()->GetWindow());
    //pDevice = g_engine->GetIrrlichtDevice();
    //pDevice = nullptr;
    //m_EventReceiver = g_engine->GetMenuReceiver();
    //m_EventReceiver = nullptr;
    unsigned char* pixels;
    int width, height;
    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    sf::Texture* cursor = new sf::Texture();
    cursor->loadFromFile("./../assets/textures/HUD/cursor.png");
    ImGui::GetIO().Fonts->TexID = (void*)cursor;

    // Create GUI object
    //m_GUIHandler = IrrIMGUI::createIMGUI(pDevice, m_EventReceiver);
    m_notifications_Ypos = 10;
    m_notifications_distance = 10;
}

GUIEngine* GUIEngine::GetInstance(){
    static GUIEngine instance = GUIEngine();
	return &instance;
}

GUIEngine::~GUIEngine(){
    //m_GUIHandler->drop();
    ImGui::SFML::Shutdown();
}

void GUIEngine::Update(){
    // create the GUI elements
    //m_GUIHandler->startGUI(); //HERE YOU CALL THE NEW FRAME METHOD
    //ImGui::NewFrame();
    std::vector<sf::Event*> m_events = VideoDriver::GetInstance()->GetSFMLEvents();
    for(int i = 0; i<m_events.size(); i++)ImGui::SFML::ProcessEvent(*m_events[i]);
    ImGui::SFML::Update(*VideoDriver::GetInstance()->GetWindow(),VideoDriver::GetInstance()->GetElapsedTime());
    printNotifications();
}

void GUIEngine::Draw(){
   //m_GUIHandler->drawAll(); //HERE YOU CALL THE RENDER METHOD
   //ImGui::EndFrame();
   ImGui::SFML::Render(*VideoDriver::GetInstance()->GetWindow());
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

//IrrIMGUI::IIMGUIHandle * GUIEngine::GetGuiHandler(){return m_GUIHandler;}
//irr::IrrlichtDevice* GUIEngine::GetPDevice(){return pDevice;}