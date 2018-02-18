#include "AllianceMenu.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"

AllianceMenu::AllianceMenu(MenuType type) : Menu(type){
    m_id = "AllianceMenu";

    m_width = screenWidth/5;
    m_height = screenHeight/2;

    buttonSize = ImVec2(120,60);
    m_style.WindowBorderSize = 1.0f;
    ImGui::GetIO().MouseDrawCursor = true;
}

AllianceMenu::~AllianceMenu(){
    
}

void AllianceMenu::Drop(){
    AllianceMenu::~AllianceMenu();
}

void AllianceMenu::Close(bool* open){
    closeMenu(open);
}

void AllianceMenu::setPlayerWarlock(bool* open){
    HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
    hp->SetAlliance(ALLIANCE_WARLOCK);

    closeMenu(open);
}

void AllianceMenu::setPlayerWizard(bool* open){
    HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
    hp->SetAlliance(ALLIANCE_WIZARD);

    closeMenu(open);
}

void AllianceMenu::Update(bool* open, float deltaTime){
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(0,0));

    if(!ImGui::Begin(m_id,open,w_flags)) ImGui::End();
    else{
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5));
        for(int i = 0; i<N_BUTTONS; i++){
            ImGui::PushID(i);
            if(ImGui::Button(buttonKeys[i], buttonSize)){
                actions[i](open);
            }
            ImGui::PopID();
            
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",descriptions[i]);
        }

        //HELP WINDOWS
        //ImGui::ShowTestWindow();
        //ImGui::ShowMetricsWindow();
        
        ImGui::PopStyleVar();
        ImGui::End();
    }
}

void AllianceMenu::closeMenu(bool* open){
    *open = false;
    GraphicEngine::getInstance()->ToggleMenu(false);

    HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
    hp->ToggleMenu(false);

    ImGui::GetIO().MouseDrawCursor = false;
}