#include "StatusMenu.h"
#include "./../Managers/PlayerManager.h"

StatusMenu::StatusMenu(MenuType type) : Menu(type){
    m_id = "StatusMenu";
}

StatusMenu::~StatusMenu(){}

void StatusMenu::Drop(){

}

void StatusMenu::Close(bool* open){
    *open = false; 
}

void StatusMenu::Update(bool* open, float deltaTime){
    ImGui::SetNextWindowSize(ImVec2(screenWidth/2,screenHeight));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(0,0));
    if(!ImGui::Begin(m_id,open,w_flags)) ImGui::End();
    else{
        ImGui::Text("Game Status");
        ImGui::Separator();
        std::vector<Player*> players = PlayerManager::GetInstance()->GetAllPlayers();
        std::vector<std::string> wizards;
        std::vector<std::string> warlocks;
        std::vector<std::string> dead_wizards;
        std::vector<std::string> dead_warlocks;

        for(int i = 0; i<players.size();i++){
            if(players[i]->GetAlliance() == ALLIANCE_WIZARD){
                if(players[i]->IsDead()) dead_wizards.push_back(players[i]->GetName());
                else wizards.push_back(players[i]->GetName());
            }
            else if(players[i]->GetAlliance() == ALLIANCE_WARLOCK){
                if(players[i]->IsDead()) dead_warlocks.push_back(players[i]->GetName());
                else warlocks.push_back(players[i]->GetName());
            }
        }

        ImGui::Text("WIZARDS");
        ImGui::Separator();
        for(int i = 0; i<wizards.size();i++) ImGui::Text("%s",wizards[i].c_str());
        for(int i = 0; i<dead_wizards.size();i++) ImGui::TextColored(ImVec4(0.7f,0.5f,0.5f,1.0f), "%s", dead_wizards[i].c_str());
    
        
        ImGui::Separator();
        ImGui::Text("WARLOCKS");
        ImGui::Separator();
        for(int i = 0; i<warlocks.size();i++) ImGui::Text("%s",warlocks[i].c_str());
        for(int i = 0; i<dead_warlocks.size();i++) ImGui::TextColored(ImVec4(0.7f,0.5f,0.5f,1.0f), "%s", dead_warlocks[i].c_str());

        ImGui::End();
    }
}