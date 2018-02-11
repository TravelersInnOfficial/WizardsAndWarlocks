#include "MainMenu.h"
#include "./../Managers/StateManager.h"
#include <vector2d.h>

bool f1 = true ,f2 = true,f3 = true,f4 = true,f5 = true;

bool MainMenu::m_options = false;
bool MainMenu::m_exit = false;

MainMenu::MainMenu(){
    m_id = "MainMenu";
    //WIDGET STYLE
    m_style.WindowBorderSize = 0.0f; //widget border size
    
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5)); //widget items spacing
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
}
MainMenu::~MainMenu(){
    
}

void MainMenu::SinglePlayer(bool* open){
    //std::cout<<"Start a new SINGLE PLAYER game"<<std::endl;
     //std::cout<<"OPEN VALUE TRUE? "<<*open;
    *open = false;
    GraphicEngine::getInstance()->ToggleMenu(false);
    ImGui::GetIO().MouseDrawCursor = false;
    StateManager::GetInstance()->PrepareStatus(STATE_GAME);
}

void MainMenu::MultiPlayer(bool* open){
    std::cout<<"Start a new MULTIPLAYER game"<<std::endl;
}
void MainMenu::GameOptions(bool * open){
    m_options = true;
}
void MainMenu::ExitGame(bool * open){
    m_exit = true;
}

void MainMenu::Update(bool* open){
    //std::cout<<"OPEN VALUE TRUE? "<<*open<<std::endl;
    //NEXT WINDOW STYLE SETUPS
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(screenWidth/2-m_width/2,screenHeight - m_height*2));
    ImGui::SetNextWindowBgAlpha(0.0f);

    //ImGui::NewFrame();
    ImGui::Begin(m_id,open,w_flags);
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

     //EXIT MENU POPUP
    if(m_exit){
        ImGui::OpenPopup("Exit?");
        if (ImGui::BeginPopupModal("Exit?")){
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

    //OPTIONS POPUP
    if(m_options){
        ImGui::OpenPopup("Options");
        if (ImGui::BeginPopupModal("Options")){
            ImGui::Text("SOON\n\n");
            ImGui::Separator();
            if(ImGui::Button("OK", ImVec2(120,0))){
                ImGui::CloseCurrentPopup(); 
                m_options = false;
            }
            ImGui::EndPopup();
        }
    }

    //HELP WINDOWS
    ImGui::ShowTestWindow();
    //ImGui::ShowMetricsWindow();
    //ImGui::PopStyleVar();
    ImGui::End();
    //ImGui::EndFrame();
    if(ImGui::GetTime() > 5 && f1){ gui_engine->MakeTemporalNotification("dentro del pecho"); f1 = false;}
    if(ImGui::GetTime() > 7 && f2) {gui_engine->MakeTemporalNotification("hay algo que hace pom pom");f2 = false;}
    if(ImGui::GetTime() > 10 && f3) {gui_engine->MakeTemporalNotification("pom\n pom\n pom\n pom\n");f3 = false;}
    if(ImGui::GetTime() > 15 && f4) {gui_engine->MakeTemporalNotification("si se te para");f4 = false;}
    if(ImGui::GetTime() > 18 && f5) {gui_engine->MakeTemporalNotification("ya puedes decir adios");f5 = false;}
}


