#include "AllianceMenu.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"

AllianceMenu::AllianceMenu(){
    m_id = "AllianceMenu";

    m_width = screenWidth/5;
    m_height = screenHeight/2;

    buttonSize = ImVec2(120,60);
    m_style.WindowBorderSize = 1.0f;
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5));
    ImGui::GetIO().MouseDrawCursor = true;
}

AllianceMenu::~AllianceMenu(){}

void AllianceMenu::setPlayerWarlock(bool* open){
    Player* hp = PlayerManager::GetInstance()->GetPlayerOne();
    hp->SetAlliance(ALLIANCE_WARLOCK);

    closeMenu(open);
}

void AllianceMenu::setPlayerWizard(bool* open){
    Player* hp = PlayerManager::GetInstance()->GetPlayerOne();
    hp->SetAlliance(ALLIANCE_WIZARD);

    closeMenu(open);
}

void AllianceMenu::Update(bool* open){
    //ImGui::ShowTestWindow();
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(0,0));

    //ImGui::NewFrame();
    ImGui::Begin(m_id,open,w_flags);
    for(int i = 0; i<N_BUTTONS; i++){
        ImGui::PushID(i);
        if(ImGui::Button(buttonKeys[i], buttonSize)){
            std::cout<<"button "<<i<<" clicked"<<std::endl;
            actions[i](open);
        }
        ImGui::PopID();
        
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",descriptions[i]);
    }
    //HELP WINDOWS
    //ImGui::ShowTestWindow();
    //ImGui::ShowMetricsWindow();
    //ImGui::PopStyleVar();
    ImGui::End();
    //ImGui::EndFrame();
}

void AllianceMenu::closeMenu(bool* open){

    //*open = false; //TODO:: FIX por alguna razon esto peta al salir y volver a entrar en el menu
    GraphicEngine::getInstance()->ToggleMenu(false);
    ObjectManager::GetInstance()->StopInteractionsNPC();
}