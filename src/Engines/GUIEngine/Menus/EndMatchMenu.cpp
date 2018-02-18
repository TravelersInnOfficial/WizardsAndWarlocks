#include "EndMatchMenu.h"
#include "./../Managers/StateManager.h"

EndMatchMenu::EndMatchMenu(MenuType type, int winners) : Menu(type){
    m_id = "EndMatchMenu";

    m_width = screenWidth/2;
    m_height = screenHeight/2;
    
    m_winners = "";
    if(winners == (int)ALLIANCE_WIZARD) m_winners = "THE WIZARDS WON!";
    else if(winners == (int) ALLIANCE_WARLOCK) m_winners = "THE WARLOCKS WON!";

    m_returnLobby = false;

    m_style.WindowBorderSize = 1.0f;
    ImGui::GetIO().MouseDrawCursor = true;
}

EndMatchMenu::~EndMatchMenu(){
}

void EndMatchMenu::Drop(){
    EndMatchMenu::~EndMatchMenu();
}

void EndMatchMenu::Update(bool* open, float deltaTime){
    //NEXT WINDOW STYLE SETUPS
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(screenWidth/2-m_width/2,screenHeight - m_height*2));

    if(!ImGui::Begin(m_id,open,w_flags)) ImGui::End(); //SI NO SE INICIA CERRAR INMEDIATAMENTE
    else{
        ImGui::Text("%s",m_winners.c_str());
        if(ImGui::Button("OK",ImVec2(120,0))){
            std::cout<<"TURN BACK TO MAIN MENU"<<std::endl;
            closeMenu(open);
            m_returnLobby = true;
        }
        ImGui::End();
    }
    if(m_returnLobby){
        //TODO:: RETURN TO THE ACTUAL MATCH LOBBY
    }
}


void EndMatchMenu::closeMenu(bool* open){
    std::cout<<"EEEEEEEEEEH\n";
    *open = false; 
    GraphicEngine::getInstance()->ToggleMenu(false);
    HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
    hp->ToggleMenu(false);
    ImGui::GetIO().MouseDrawCursor = false;
}