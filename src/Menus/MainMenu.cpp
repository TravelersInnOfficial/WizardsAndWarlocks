#include "MainMenu.h"
#include "./../Managers/StateManager.h"

bool exit_menu = false;
bool options_menu = false;

MainMenu::MainMenu(){
    m_id = "MainMenu";

    //WIDGET STYLE
    m_style.WindowBorderSize = 0.0f; //widget border size
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5)); //widget items spacing
    ImGui::GetIO().MouseDrawCursor = true; //cursor visible
    
    //BUTTONS DATA
    for(int i = 0; i<N_BUTTONS_MAIN;i++){
        texture[i] = pDevice->getVideoDriver()->getTexture(buttonLayouts[i]);
        imageid[i] = GUI->createTexture(texture[i]);
    }
    buttonSize = ImVec2(texture[0]->getSize().Width,texture[0]->getSize().Height);

    //WIDGET SIZE
    m_width = texture[0]->getSize().Width + 30;
    m_height = texture[0]->getSize().Height * N_BUTTONS_MAIN + 50;

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
    options_menu = true;
}
void ExitGame(){
    exit_menu = true;
}
void (*actions[N_BUTTONS_MAIN])() = {SinglePlayer,MultiPlayer,GameOptions,ExitGame};

void MainMenu::Update(bool open){

    //NEXT WINDOW STYLE SETUPS
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(screenWidth/2-m_width/2,screenHeight - m_height*2));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin(m_id,&open,w_flags);
    for(int i = 0; i<N_BUTTONS_MAIN; i++){
        ImGui::PushID(i);
        //NEXT BUTTON STYLE
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));

        if(ImGui::ImageButton(imageid[i],buttonSize)){
            actions[i]();
        }

        ImGui::PopStyleColor(3);
        ImGui::PopID();
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",descriptions[i]);
    }

     //EXIT MENU POPUP
        if(exit_menu){
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
                    exit_menu = false;
                }
                ImGui::EndPopup();
            }
        }

        //OPTIONS POPUP
        if(options_menu){
            ImGui::OpenPopup("Options");
            if (ImGui::BeginPopupModal("Options")){
                ImGui::Text("SOON\n\n");
                ImGui::Separator();
                if(ImGui::Button("OK", ImVec2(120,0))){
                    ImGui::CloseCurrentPopup(); 
                    options_menu = false;
                }
                ImGui::EndPopup();
            }
        }

    //HELP WINDOWS
    //ImGui::ShowTestWindow();
    //ImGui::ShowMetricsWindow();
        
    ImGui::End();
}


