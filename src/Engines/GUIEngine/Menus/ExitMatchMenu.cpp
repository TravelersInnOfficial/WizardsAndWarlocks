#include "ExitMatchMenu.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Players/HumanPlayer.h"
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GSprite.h>

ExitMatchMenu::ExitMatchMenu(MenuType type) : Menu(type){
    m_id = "ExitMatchMenu";
}

ExitMatchMenu::~ExitMatchMenu(){
    delete m_cursor;    
    m_cursor = nullptr;
}

void ExitMatchMenu::Update(bool* open, float deltaTime){
    UpdateCursor();

    std::string popuptxt = "Return to main menu?";
    ImGui::OpenPopup(popuptxt.c_str());
    if(ImGui::BeginPopupModal(popuptxt.c_str(),nullptr,popup_flags)){
        if(ImGui::Button("YES", ImVec2(100,100))){
            StateManager::GetInstance()->PrepareStatus(STATE_MENU);
        }
        ImGui::SameLine();
        if(ImGui::Button("NO", ImVec2(100,100))){
            Close(open);
        }
        ImGui::EndPopup();
    }
}

void ExitMatchMenu::Close(bool* open){
    *open = false;
    HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
    hp->ToggleMenu(false);
}