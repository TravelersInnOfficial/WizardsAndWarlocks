#include "MainMenu.h"
#include "./../Managers/StateManager.h"
#include <vector2d.h>

bool f1 = true ,f2 = true,f3 = true,f4 = true,f5 = true;

bool MainMenu::m_options = false;
bool MainMenu::m_exit = false;
bool MainMenu::m_multiplayer = false;

MainMenu::MainMenu(){
    m_id = "MainMenu";

    m_start_host = false;
    m_direct_connection = false;
    m_none_selected = false;

    //WIDGET STYLE
    m_style.WindowBorderSize = 0.0f; //widget border size
    
    ImGui::GetIO().MouseDrawCursor = true; //cursor visible
    
    //BUTTONS DATA
    for(int i = 0; i<N_BUTTONS;i++){
        texture[i] = pDevice->getVideoDriver()->getTexture(buttonLayouts[i]);
        imageid[i] = GUI->createTexture(texture[i]);
    }
    buttonSize = ImVec2(texture[0]->getSize().Width,texture[0]->getSize().Height);

    //WIDGET SIZE
    m_width = texture[0]->getSize().Width + 30;
    m_height = texture[0]->getSize().Height * N_BUTTONS + 50;

    //netSeeker = new NetSeeker();
}
MainMenu::~MainMenu(){
    
    delete netSeeker;
}

void MainMenu::SinglePlayer(bool* open){
    closeMenu(open);
    StateManager::GetInstance()->PrepareStatus(STATE_GAME);
}

void MainMenu::MultiPlayer(bool* open){
    //std::cout<<"Start a new MULTIPLAYER game"<<std::endl;
    m_multiplayer = true;
}
void MainMenu::GameOptions(bool * open){
    m_options = true;
}
void MainMenu::ExitGame(bool * open){
    m_exit = true;
}

void MainMenu::Update(bool* open){
    //netSeeker->Update(GraphicEngine::getInstance()->getTime());
    //std::vector<ServerData> newServerList = netSeeker->GetList();

    //NEXT WINDOW STYLE SETUPS
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(screenWidth/2-m_width/2,screenHeight - m_height*2));
    ImGui::SetNextWindowBgAlpha(0.0f);

    //ImGui::NewFrame();
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
                actions[i](open);
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",descriptions[i]);
        }
        ImGui::PopStyleVar();

        //SERVER LIST MODAL
       if(m_multiplayer){
        /////////
       // std::cout<<"serverlist size: "<<serverList.size()<<std::endl;
        //std::cout<<"NEW serverlist size: "<<newServerList.size()<<"\n";
           /* if(serverList.size() != newServerList.size()){
                std::cout<<"----------------------"<<std::endl;
                std::cout<<"NEW LIST OF SERVERS: "<<std::endl;
                
                if(newServerList.size() == 0) std::cout<<"No server available"<<std::endl;
                else{
                    for(int i = 0; i < newServerList.size(); i++){
                        std::cout<<newServerList.at(i).name<<" - IP: "<<newServerList.at(i).ip;
                        std::cout<<":60000. Players: "<<newServerList.at(i).playerCount<<"/8";
                        std::cout<<". Game Started: "<<!newServerList.at(i).lobbyState<<"."<<std::endl;
                    }
                }
                
                std::cout<<"----------------------"<<std::endl;
                serverList = newServerList;
            }*/
           ////////////
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
                std::string server_name[3] = { "HEY HEY HEEEEEEEY", "WAZZU WAZZU WAZZU WAZZUUUUUUUUUUUUUUUP", "BITCONNEEEEEEEEEEECT" };
                const char* server_ip[3] = { "192.168.1.242", "192.168.23.114", "192.168.1.117" };
                const char* server_players[3] = { "4/8", "2/8", "8/8" };
                const char* server_status[3] = { "LOBBY", "LOBBY", "IN GAME" };
                static int selected = -1;
                int j = 0;
                for (int i = 0; i < 30; i++){
                    std::string label = server_name[j] + " ## " + std::to_string(i);
                    if (ImGui::Selectable(label.c_str(), selected == i, ImGuiSelectableFlags_SpanAllColumns)){
                        std::cout<<"I selected: "<<label<<std::endl;
                        selected = i;
                    }
                    
                    ImGui::NextColumn();
                    ImGui::Text(server_ip[j]); ImGui::NextColumn();
                    ImGui::Text(server_players[j]); ImGui::NextColumn();
                    ImGui::Text(server_status[j]); ImGui::NextColumn();

                    if(j==2) j = 0;
                    else j++;
                }
                ImGui::Columns(1);
                ImGui::EndChild();

                ImGui::Separator();

                if(ImGui::Button("CONNECT")){
                    if(selected == -1 ){
                        m_none_selected = true;
                        ImGui::CloseCurrentPopup(); 
                        m_multiplayer = false;
                    } 
                    else{
                        //CONNECT TO SELECTED
                    }
                }

                ImGui::SameLine();

                if(ImGui::Button("DIRECT CONNECTION")){
                    m_direct_connection = true;
                    ImGui::CloseCurrentPopup(); 
                    m_multiplayer = false;
                } 

                ImGui::SameLine();

                if(ImGui::Button("HOST A GAME")){ 
                    m_start_host = true;
                    ImGui::CloseCurrentPopup(); 
                    m_multiplayer = false;
                }

                ImGui::SameLine();

                if(ImGui::Button("CLOSE", ImVec2(120,0))){
                    ImGui::CloseCurrentPopup(); 
                    m_multiplayer = false;
                }

                ImGui::EndPopup();
            }
        }

        if(m_none_selected){
            ImGui::OpenPopup("Select a server");
            if(ImGui::BeginPopupModal("Select a server")){
                ImGui::Text("Select a server from the list");
                ImGui::Separator();
                if(ImGui::Button("OK")){
                    m_none_selected = false;
                    ImGui::CloseCurrentPopup(); 
                    m_multiplayer = true;
                }
                ImGui::EndPopup();
            }
        }

        if(m_direct_connection){
            ImGui::OpenPopup("Direct connection");
            if(ImGui::BeginPopupModal("Direct connection")){
                ImGui::Text("IP: ");
                ImGui::SameLine();
                static char str0[128] = "Introduce address...";
                ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
                ImGui::Separator();
                if(ImGui::Button("BACK")){
                    m_direct_connection = false;
                    ImGui::CloseCurrentPopup();
                    m_multiplayer = true;
                }
                if(ImGui::Button("CONNECT TO SERVER")){
                    //CONNECT TO THE SERVER DATA ABOVE
                }
                ImGui::EndPopup();
            }
        }

        if(m_start_host){
            ImGui::OpenPopup("Start a host");
            if(ImGui::BeginPopupModal("Start a host")){
                ImGui::Text("Your IP: 192.168.2.224");
                ImGui::Separator();
                if(ImGui::Button("BACK")){ 
                    m_start_host = false;
                    ImGui::CloseCurrentPopup();
                    m_multiplayer = true;
                }
                if(ImGui::Button("START HOSTING")){
                    //START A SERVER
                }
                ImGui::EndPopup();
            }
        }

        //OPTIONS MODAL
        if(m_options){
            ImGui::OpenPopup("Options");
            if(!ImGui::BeginPopupModal("Options")) ImGui::EndPopup();
            else{
                ImGui::Text("SOON\n\n");
                ImGui::Separator();
                if(ImGui::Button("OK", ImVec2(120,0))){
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
                if(ImGui::Button("YES", ImVec2(120,0))){
                    GraphicEngine::getInstance()->drop();
                }
                ImGui::SameLine();
                if(ImGui::Button("NO", ImVec2(120,0))){ 
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
        if(ImGui::GetTime() > 5 && f1) {gui_engine->MakeTemporalNotification("dentro del pecho"); f1 = false;}
        if(ImGui::GetTime() > 6 && f2) {gui_engine->MakeTemporalNotification("hay algo que hace pom pom");f2 = false;}
        if(ImGui::GetTime() > 7 && f3) {gui_engine->MakeTemporalNotification("pom\n pom\n pom\n pom\n");f3 = false;}
        if(ImGui::GetTime() > 8 && f4) {gui_engine->MakeTemporalNotification("si se te para");f4 = false;}
        if(ImGui::GetTime() > 9 && f5) {gui_engine->MakeTemporalNotification("ya puedes decir adios");f5 = false;}
        
    }
}

void MainMenu::closeMenu(bool* open){
    *open = false;
    GraphicEngine::getInstance()->ToggleMenu(false);
    ImGui::GetIO().MouseDrawCursor = false;
}
