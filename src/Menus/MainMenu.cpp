#include "MainMenu.h"
#include "./../Managers/StateManager.h"

MainMenu::MainMenu(){
    m_id = "MainMenu";

    //WIDGET STYLE
    m_style.WindowBorderSize = 1.0f; //widget border size
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5)); //widget items spacing
    ImGui::GetIO().MouseDrawCursor = true; //cursor visible
    
    //BUTTONS DATA
    texture = pDevice->getVideoDriver()->getTexture("./../assets/textures/GUI/Menus/menu_button.png");
    imageid = GUI->createTexture(texture);
    buttonSize = ImVec2(texture->getSize().Width,texture->getSize().Height);

    //WIDGET SIZE
    m_width = texture->getSize().Width + 30;
    m_height = screenHeight/2;
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
    ImGui::ShowMetricsWindow();
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(screenWidth/3,screenHeight/5));
    ImGui::Begin(m_id,&open,w_flags);
    for(int i = 0; i<4; i++){
        ImGui::PushID(i);
        if(ImGui::ImageButton(imageid,buttonSize)){
            std::cout<<"button "<<i<<" clicked"<<std::endl;
            actions[i]();
        }
        ImGui::PopID();
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",descriptions[i]);
    }
    ImGui::End();
}


