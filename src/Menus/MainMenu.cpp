#include "MainMenu.h"
#include "./../Managers/StateManager.h"

MainMenu::MainMenu(){
    m_id = "MainMenu";
    m_width = screenWidth/5;
    m_height = screenHeight/2;

    buttonSize = ImVec2(120,60);
    m_style.WindowBorderSize = 1.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5));
    ImGui::GetIO().MouseDrawCursor = true;
}
MainMenu::~MainMenu(){
    
}

void SinglePlayer(){
    std::cout<<"Start a new SINGLE PLAYER game"<<std::endl;
    GraphicEngine::getInstance()->ToggleMenu(false);
    ImGui::GetIO().MouseDrawCursor = false;
    StateManager::GetInstance()->PrepareStatus(STATE_GAME);
}
void MultiPlayer(){
    std::cout<<"Start a new MULTIPLAYER game"<<std::endl;
}
void GameOptions(){
    std::cout<<"Start OPTIONS manager"<<std::endl;
}
void ExitGame(){
    std::cout<<"EXIT the game"<<std::endl;
    
    ImGui::OpenPopup("Exit the game");
    if (ImGui::BeginPopupModal("Exit the game", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure?\n\n");
        ImGui::Separator();

        if (ImGui::Button("YES", ImVec2(120,0))) {
                GraphicEngine::getInstance()->drop();
            }
        ImGui::SameLine();
        if (ImGui::Button("NO", ImVec2(120,0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    GraphicEngine::getInstance()->drop();
}
 void (*actions[4])() = {SinglePlayer,MultiPlayer,GameOptions,ExitGame};

void MainMenu::Update(bool open){
    ImGui::ShowTestWindow();
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(screenWidth/3,screenHeight/5));
    ImGui::Begin(m_id,&open,w_flags);
    for(int i = 0; i<4; i++){
        ImGui::PushID(i);
        if(ImGui::Button(buttonKeys[i], buttonSize)){
            std::cout<<"button "<<i<<" clicked"<<std::endl;
            actions[i]();
        }
        ImGui::PopID();
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("%s",descriptions[i]);
    }
    ImGui::End();
}


