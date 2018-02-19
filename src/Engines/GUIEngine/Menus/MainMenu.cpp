#include "MainMenu.h"
#include "./../Managers/StateManager.h"
#include <vector2d.h>

bool MainMenu::m_options = false;
bool MainMenu::m_exit = false;
bool MainMenu::m_multiplayer = false;
int MainMenu::m_selected_server = -1;

char MainMenu::player_name[MAX_NAME_SIZE] = "Player Name";
char MainMenu::server_name[MAX_STRING_SIZE] = "Unknown Castle";
char MainMenu::ip_address[MAX_STRING_SIZE] = "127.0.0.1";

//FOR TESTING NOTIFICATIONS 
//bool f1 = true ,f2 = true,f3 = true,f4 = true,f5 = true, f6 = true, f7 = true;

MainMenu::MainMenu(MenuType type) : Menu(type){
    m_id = "MainMenu";

    m_start_host = false;
    m_direct_connection = false;
    m_none_selected = false;
    m_some_selected = false;

    //WIDGET STYLE
    m_style.WindowBorderSize = 0.0f; //widget border size
    
    ImGui::GetIO().MouseDrawCursor = true; //cursor visible
    
    //BUTTONS DATA
    for(int i = 0; i<N_BUTTONS;i++){
        texture[i] = pDevice->getVideoDriver()->getTexture(buttonLayouts[i]);
        imageid[i] = GUI->createTexture(texture[i]);
    }
    buttonSize = ImVec2(texture[0]->getSize().Width,texture[0]->getSize().Height);
    pu_buttonSize = ImVec2(140,0);

    //WIDGET SIZE
    m_width = texture[0]->getSize().Width + 30;
    m_height = texture[0]->getSize().Height * N_BUTTONS + 50;

    netSeeker = new NetSeeker();
}
MainMenu::~MainMenu(){
    for(int i = 0; i<N_BUTTONS; i++){
        GUI->deleteTexture(imageid[i]);
    }
    delete netSeeker;
}

void MainMenu::Close(bool* open){
    closeMenu(open);
}

void MainMenu::Drop(){
    MainMenu::~MainMenu();
}

void MainMenu::SinglePlayer(bool* open){
    closeMenu(open);
    StateManager::GetInstance()->PrepareStatus(STATE_GAME);
}

void MainMenu::MultiPlayer(bool* open){
    m_multiplayer = true;
}
void MainMenu::GameOptions(bool * open){
    m_options = true;
}
void MainMenu::ExitGame(bool * open){
    m_exit = true;
}

void MainMenu::Update(bool* open, float deltaTime){
    netSeeker->Update(deltaTime);

    //NEXT WINDOW STYLE SETUPS
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(screenWidth/2-m_width/2,screenHeight - m_height*2));
    ImGui::SetNextWindowBgAlpha(0.0f);

    if(!ImGui::Begin(m_id,open,w_flags)) ImGui::End(); //SI NO SE INICIA CERRAR INMEDIATAMENTE
    else{
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5)); //widget items spacing
        for(int i = 0; i<N_BUTTONS; i++){
            ImGui::PushID(i);
            //NEXT BUTTON STYLE
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));

            if(ImGui::ImageButton(imageid[i],buttonSize)){
                PlaySound();
                actions[i](open);
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",descriptions[i]);
        }
        ImGui::PopStyleVar();

        //SERVER LIST MODAL
       if(m_multiplayer){
            ImGui::OpenPopup("Server list");
            ImGui::SetNextWindowSize(ImVec2(screenWidth/1.2,screenHeight/1.2));
            if(!ImGui::BeginPopupModal("Server list")) ImGui::EndPopup();
            else{
                ImGui::Columns(4, "server-labels"); // 4-ways, with border
                ImGui::Separator();
                ImGui::Text("NAME"); ImGui::NextColumn();
                ImGui::Text("IP"); ImGui::NextColumn();
                ImGui::Text("PLAYERS"); ImGui::NextColumn();
                ImGui::Text("GAME STATUS"); ImGui::NextColumn();
                ImGui::Separator();
                ImGui::Columns(1);

                ImGui::SetNextWindowContentSize(ImVec2(screenWidth/1.21, screenHeight/1.2));
                ImGui::BeginChild("##list-of-servers",ImVec2(0,screenHeight/1.5));
                ImGui::Columns(4, "server-columns"); // 4-ways, with border

                std::vector<ServerData> newServerList = netSeeker->GetList();

                serverList = newServerList;

                for(int i = 0; i < newServerList.size(); i++){
                    //PRINT SERVERS DATA
                    std::string label = newServerList.at(i).name + "##" + std::to_string(i);
                    if (ImGui::Selectable(label.c_str(), m_selected_server == i, ImGuiSelectableFlags_SpanAllColumns)){
                        m_selected_server = i;
                        strcpy(ip_address, (serverList[m_selected_server].ip).c_str());
                        m_some_selected = true;
                        ImGui::CloseCurrentPopup();
                        m_multiplayer = false;
                    }
                    ImGui::NextColumn();
                    //server ip
                    std::string server_ip = newServerList.at(i).ip;
                    ImGui::Text("%s", server_ip.c_str()); ImGui::NextColumn();
                    //server player count
                    std::string server_player_count = std::to_string(newServerList.at(i).playerCount);
                    ImGui::Text("%s", server_player_count.c_str()); ImGui::NextColumn();
                    //server lobby StateManager
                    std::string server_lobby_state =std::to_string(!newServerList.at(i).lobbyState);
                    ImGui::Text("%s", server_lobby_state.c_str()); ImGui::NextColumn();
                }
           
                ImGui::Columns(1);
                ImGui::EndChild();

                ImGui::Separator();

                if(ImGui::Button("CONNECT", pu_buttonSize)){
                    PlaySound();
                    if(m_selected_server == -1 ){
                        m_none_selected = true;
                        ImGui::CloseCurrentPopup(); 
                        m_multiplayer = false;
                    }
                    else{
                        strcpy(ip_address, (serverList[m_selected_server].ip).c_str());
                        m_some_selected = true;
                        ImGui::CloseCurrentPopup(); 
                        m_multiplayer = false;
                    }
                }

                ImGui::SameLine();

                if(ImGui::Button("DIRECT CONNECTION", pu_buttonSize)){
                    PlaySound();
                    m_direct_connection = true;
                    ImGui::CloseCurrentPopup(); 
                    m_multiplayer = false;
                } 

                ImGui::SameLine();

                if(ImGui::Button("HOST A GAME", pu_buttonSize)){
                    PlaySound();
                    m_start_host = true;
                    ImGui::CloseCurrentPopup(); 
                    m_multiplayer = false;
                }

                ImGui::SameLine();

                if(ImGui::Button("CLOSE", pu_buttonSize)){
                    PlaySound();
                    ImGui::CloseCurrentPopup(); 
                    m_multiplayer = false;
                }

                ImGui::EndPopup();
            }
        }

////CHECK MULTIPLAYER STATES////
        //SERVER SELECTED
        if(m_some_selected){
            ImGui::OpenPopup("Introduce your name");
            if(ImGui::BeginPopupModal("Introduce your name")){
                ImGui::Text("Your Name: ");
                ImGui::SameLine();
                ImGui::InputText("##player_name", player_name, IM_ARRAYSIZE(player_name));
                ImGui::Separator();
                if(ImGui::Button("CONNECT", pu_buttonSize)){
                    PlaySound();
                    m_some_selected = false;
                    ImGui::CloseCurrentPopup();
                    PrepareClient(true);
                    closeMenu(open);
                }
                if(ImGui::Button("BACK", pu_buttonSize)){
                    PlaySound();
                    m_some_selected = false;
                    ImGui::CloseCurrentPopup();
                    m_multiplayer = true;
                }
                ImGui::EndPopup();
            }
        }

        //NO SERVER SELECTED
        if(m_none_selected){
            ImGui::OpenPopup("Select a server");
            if(ImGui::BeginPopupModal("Select a server")){
                ImGui::Text("Select a server from the list");
                ImGui::Separator();
                if(ImGui::Button("OK",pu_buttonSize)){
                    PlaySound();
                    m_none_selected = false;
                    ImGui::CloseCurrentPopup(); 
                    m_multiplayer = true;
                }
                ImGui::EndPopup();
            }
        }

        //DIRECT CONNECTION
        if(m_direct_connection){
            ImGui::OpenPopup("Direct connection");
            ImGui::SetNextWindowSize(ImVec2(screenWidth/1.2,screenHeight/1.2));
            if(ImGui::BeginPopupModal("Direct connection")){
                ImGui::Text("Your Name: ");
                ImGui::SameLine();
                ImGui::InputText("##player_name", player_name, IM_ARRAYSIZE(player_name));
               
                ImGui::Text("IP: ");
                ImGui::SameLine();
                ImGui::InputText("##ip_address", ip_address, IM_ARRAYSIZE(ip_address));
                ImGui::Separator();

                if(ImGui::Button("CONNECT TO SERVER",pu_buttonSize)){
                    PlaySound();
                    //CONNECT TO THE SERVER DATA ABOVE
                    m_direct_connection = false;
                    ImGui::CloseCurrentPopup();
                    PrepareClient(true);
                    closeMenu(open);
                }
                if(ImGui::Button("BACK",pu_buttonSize)){
                    PlaySound();
                    m_direct_connection = false;
                    ImGui::CloseCurrentPopup();
                    m_multiplayer = true;
                }

                ImGui::EndPopup();
            }
        }

        //START A HOST
        if(m_start_host){
            ImGui::OpenPopup("Start a host");
            ImGui::SetNextWindowSize(ImVec2(screenWidth/1.2,screenHeight/1.2));
            if(ImGui::BeginPopupModal("Start a host")){
                ImGui::Text("Server Name: ");
                ImGui::SameLine();
                ImGui::InputText("##server_name", server_name, IM_ARRAYSIZE(player_name));

                ImGui::Text("Your Name: ");
                ImGui::SameLine();
                ImGui::InputText("##player_name", player_name, IM_ARRAYSIZE(player_name));
                ImGui::Text("Your IP: 192.168.2.224");
                ImGui::Separator();

                if(ImGui::Button("START HOSTING", pu_buttonSize)){
                    PlaySound();
                    //START A SERVER
                    strcpy(ip_address,"127.0.0.1");
                    std::string path = "./WizardsAndWarlocks -i ";
                    path += "\'"; path += server_name; path += "\'"; path += " &";
                    #ifdef _WIN64
                        path = "START /B WizardsAndWarlocks.exe -i ";
                        path += "\'"; path += server_name; path += "\'";
                    #endif
                    std::system(path.c_str());
                    PrepareClient(true);
                    closeMenu(open);
                }
                if(ImGui::Button("BACK", pu_buttonSize)){ 
                    PlaySound();
                    m_start_host = false;
                    ImGui::CloseCurrentPopup();
                    m_multiplayer = true;
                }

                ImGui::EndPopup();
            }
        }
/////////////////////////////////////

        //OPTIONS MODAL
        if(m_options){
            ImGui::OpenPopup("Options");
            if(!ImGui::BeginPopupModal("Options")) ImGui::EndPopup();
            else{
                ImGui::Text("SOON\n\n");
                ImGui::Separator();
                if(ImGui::Button("OK", pu_buttonSize)){
                    PlaySound();
                    ImGui::CloseCurrentPopup(); 
                    m_options = false;
                }
                ImGui::EndPopup();
            }
        }

        //EXIT MENU MODAL
        if(m_exit){
            ImGui::OpenPopup("Exit?");
            if(!ImGui::BeginPopupModal("Exit?")) ImGui::EndPopup();
            else{
                ImGui::Text("Are you sure?\n\n");
                ImGui::Separator();
                if(ImGui::Button("YES", pu_buttonSize)){
                    //PlaySound();
                    StateManager::GetInstance()->CloseGame();
                }
                ImGui::SameLine();
                if(ImGui::Button("NO", pu_buttonSize)){
                    PlaySound();
                    ImGui::CloseCurrentPopup(); 
                    m_exit = false;
                }
                ImGui::EndPopup();
            }
        }

        //HELP WINDOWS
        //ImGui::ShowTestWindow();
        //ImGui::ShowMetricsWindow();
        
        ImGui::End();

        //NOTIFICATIONS TEST
        /*
        if(ImGui::GetTime() > 5 && f1) {gui_engine->MakeTemporalNotification("dentro del pecho"); f1 = false;}
        if(ImGui::GetTime() > 6 && f2) {gui_engine->MakeTemporalNotification("hay algo que hace pom pom");f2 = false;}
        if(ImGui::GetTime() > 6 && f6) {gui_engine->MakeCustomNotification("custom popup (10s)", 10);f6 = false;}
        if(ImGui::GetTime() > 6 && f7) {gui_engine->ShowDeathMessage("Florbo", 10); f7 = false;}
        if(ImGui::GetTime() > 7 && f3) {gui_engine->MakeTemporalNotification("pom\n pom\n pom\n pom\n");f3 = false;}
        if(ImGui::GetTime() > 8 && f4) {gui_engine->MakeTemporalNotification("si se te para");f4 = false;}
        if(ImGui::GetTime() > 9 && f5) {gui_engine->MakeTemporalNotification("ya puedes decir adios");f5 = false;}      
        */
    }
}

void MainMenu::closeMenu(bool* open){
    *open = false;
    GraphicEngine::getInstance()->ToggleMenu(false);
    ImGui::GetIO().MouseDrawCursor = false;
}

void MainMenu::PrepareClient(bool proprietary){
	NetworkEngine* n_engine;
	n_engine = NetworkEngine::GetInstance();
	n_engine->SetIp(ip_address);
	n_engine->StartClient(proprietary);
	n_engine->GetClient()->SetClientName(player_name);
	StateManager::GetInstance()->PrepareStatus(STATE_NETGAME_CLIENT);
}

void MainMenu::PlaySound(){
    soundEvent = SoundSystem::getInstance()->createEvent("event:/HUD/Click");	//Create the click event
	SoundSystem::getInstance()->playEvent(soundEvent);	                        //Play the event
	soundEvent->release();								                        //Release the event
}