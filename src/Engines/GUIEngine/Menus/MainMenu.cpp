#include "MainMenu.h"
#include "./../Managers/StateManager.h"
#include <vector2d.h>
#include <GraphicEngine/GraphicEngine.h>
#include <SoundEngine/SoundSystem.h>

bool MainMenu::m_options = false;
bool MainMenu::m_exit = false;
bool MainMenu::m_multiplayer = false;
int MainMenu::m_selected_server = -1;

char MainMenu::player_name[MAX_NAME_SIZE] = "Player Name";
char MainMenu::server_name[MAX_STRING_SIZE] = "Unknown Castle";
char MainMenu::ip_address[MAX_STRING_SIZE] = "127.0.0.1";

MainMenu::MainMenu(MenuType type) : Menu(type){
    m_id = "MainMenu";

    m_start_host = false;
    m_direct_connection = false;
    m_none_selected = false;
    m_some_selected = false;

    //WIDGET STYLE
    m_style.WindowBorderSize = 0.0f; //widget border size
    m_style.FrameBorderSize = 0.0f;
    
    //ImGui::GetIO().MouseDrawCursor = true; //cursor visible
    //BUTTONS DATA
    texture_init = (void*) toe::GetTextureID(button_layout);
    for(int i = 0; i<N_BUTTONS;i++){
        texture[i] = texture_init;
    }
    texture_hover = (void*) toe::GetTextureID(button_hover_layout);
    texture_pressed = (void*) toe::GetTextureID(button_pressed_layout);
    bkg = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_BOOK_BACKGROUND].c_str());
    title_texture = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_MENU_TITLE]);
    toe::core::TOEvector2di t_dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_MENU_TITLE]);
    titleSize = ImVec2(t_dims.X/1.6,t_dims.Y/1.5);
    
    toe::core::TOEvector2di dims = toe::GetTextureDims(button_layout);

    buttonSize = ImVec2(dims.X,dims.Y);
    pu_buttonSize = ImVec2(140,0);

    //WIDGET SIZE
    m_width = dims.X + 30;
    m_height = dims.Y * N_BUTTONS + 50 + titleSize.y;
    netSeeker = new NetSeeker();
    
    m_fontSize = 0;
}

MainMenu::~MainMenu(){
    delete netSeeker;
    m_cursor->Erase();
    m_cursor = nullptr;
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
    UpdateCursor();

    //NEXT WINDOW STYLE SETUPS
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(screenWidth/2-m_width/2,screenHeight/2 - m_height/2));
    ImGui::SetNextWindowBgAlpha(0.0f);

    if(!ImGui::Begin(m_id,open,w_flags)) ImGui::End(); //SI NO SE INICIA CERRAR INMEDIATAMENTE
    else{
        std::vector<ImVec2> text_pos;
        ImGui::Image(title_texture,titleSize);
        for(int i = 0; i<N_BUTTONS; i++){
            ImGui::PushID(i);
            
            //NEXT BUTTON STYLE
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));

            text_pos.push_back(ImGui::GetCursorScreenPos());
            
            ImGui::ImageButton(texture[i],buttonSize);
            if(ImGui::IsItemActive()){
                texture[i] = texture_pressed;
                PlaySound();
                actions[i](open);
            }
            else if(ImGui::IsItemHovered()){ 
                ImGui::SetTooltip("%s",descriptions[i]);
                texture[i] = texture_hover;
                //m_cursor->SetTexture(TEXTUREMAP[TEXTURE_GUI_CURSOR_GLOW]);
            }
            else{
                texture[i] = texture_init;
                /*
                if(m_cursor->GetTexture()!=TEXTUREMAP[TEXTURE_GUI_CURSOR]){
                        m_cursor->SetTexture(TEXTUREMAP[TEXTURE_GUI_CURSOR]);
                        std::cout<<"changing cursor texture\n";
                }
                */
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();

        }

        for(int i = 0; i < N_BUTTONS; i++){
            if(m_fontSize == 0) m_fontSize = ImGui::GetFontSize()*2.0f;
            ImVec2 offset((buttonSize.x - (text_buttons[i].size()/2*m_fontSize))/2, buttonSize.y/2 - m_fontSize/2);
            ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), m_fontSize, ImVec2(text_pos[i].x+offset.x,text_pos[i].y+offset.y), IM_COL32(255,255,255,255), text_buttons[i].c_str());
        }
        
        //SERVER LIST MODAL
       if(m_multiplayer){
            ImGui::OpenPopup("Server list");
            ImGui::SetNextWindowSize(ImVec2(screenWidth/1.2,screenHeight/1.2));
            if(!ImGui::BeginPopupModal("Server list",nullptr,popup_flags)) ImGui::EndPopup();
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
            if(ImGui::BeginPopupModal("Introduce your name",nullptr,popup_flags)){
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
            if(ImGui::BeginPopupModal("Select a server",nullptr,popup_flags)){
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
            if(ImGui::BeginPopupModal("Direct connection",nullptr,popup_flags)){
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
            if(ImGui::BeginPopupModal("Start a host",nullptr,popup_flags)){
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
            ImGui::SetNextWindowSize(ImVec2(screenWidth/1.2,screenHeight/1.2));
            if(!ImGui::BeginPopupModal("Options",nullptr,popup_flags)) ImGui::EndPopup();
            else{
                ImGui::Text("Sound Options");
                ImGui::Separator();
                ImGui::Text("\nVolume:\n");
                
                static float master = SoundSystem::getInstance()->getVolume();
                static float music = SoundSystem::getInstance()->getVolumeVCA("Music");
                static float sfx = SoundSystem::getInstance()->getVolumeVCA("SFX");
                static float voices = SoundSystem::getInstance()->getVolumeVCA("Voices");

                master =master*10;
                music =music*10;
                sfx =sfx*10;
                voices =voices*10;

                ImGui::SliderFloat("Master", &master, 0.0f, 100.0f, "%.2f");
                ImGui::SliderFloat("Music", &music, 0.0f, 100.0f, "%.2f");
                ImGui::SliderFloat("SFX", &sfx, 0.0f, 100.0f, "%.2f");
                ImGui::SliderFloat("Voices", &voices, 0.0f, 100.0f, "%.2f");

                master =master/10;
                music =music/10;
                sfx =sfx/10;
                voices =voices/10;

                SoundSystem::getInstance()->setVolume(master);
                SoundSystem::getInstance()->setVolumeVCA("Music",music);
                SoundSystem::getInstance()->setVolumeVCA("SFX",sfx);
                SoundSystem::getInstance()->setVolumeVCA("Voices",voices);
                ImGui::Separator();

                ImGui::Text("\nScreen Options");
                ImGui::Separator();
                static bool shadows = true, particles  = true;
                ImGui::Checkbox("Shadows", &shadows);
                ImGui::Checkbox("Particles", &particles);
                ImGui::Separator();

                ImGui::Text("\nAI Options");
                ImGui::Separator();
                static int wizard_ai=3, warlock_ai=3, total_ai=7;
                ImGui::InputInt("Wizard AI", &wizard_ai);
                ImGui::InputInt("Warlock AI", &warlock_ai);
                if(wizard_ai<0) wizard_ai = 0;
                else if(wizard_ai> total_ai-warlock_ai) wizard_ai = total_ai-warlock_ai;
                
                if(warlock_ai<0) warlock_ai = 0;
                else if(warlock_ai> total_ai-wizard_ai) warlock_ai = total_ai-wizard_ai;


                ImGui::Separator();

                if(ImGui::Button("Close", pu_buttonSize)){
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
            if(!ImGui::BeginPopupModal("Exit?",nullptr,popup_flags)) ImGui::EndPopup();
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
        ImGui::ShowTestWindow();
        //ImGui::ShowMetricsWindow();
        
        ImGui::End();
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