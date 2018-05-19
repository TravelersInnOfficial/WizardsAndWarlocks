#include "EndMatchMenu.h"
#include "./../../../Players/HumanPlayer.h"
#include "./../../../Managers/PlayerManager.h"
#include <TravelersOcularEngine/src/TOcularEngine/TOcularEngine.h>
#include <Assets.h>

EndMatchMenu::EndMatchMenu(MenuType type, int winners) : Menu(type){
    m_id = "EndMatchMenu";

    m_width = screenWidth/1.2;
    m_height = screenHeight/1.2;

    hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();    
    Alliance p_alliance = hp->GetAlliance();

    m_msg = "";
    m_victory = true;
    if(winners != (int)p_alliance) m_victory = false;

    HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
    hp->ToggleMenu(true);

    m_style.WindowBorderSize = 1.0f;
    ImGui::GetIO().MouseDrawCursor = true;
    
    TOEvector2di dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_BUTTON]);
    buttonSize = ImVec2(dims.X,dims.Y);

    texture_init        = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON]);
    texture_button      = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON]);
    texture_hover       = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON_HOVER]);
    texture_pressed     = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON_PRESSED]);

    dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_MATCH_VICTORY]);
    imgSize = ImVec2(dims.X,dims.Y);

    texture_VICTORY     = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_MATCH_VICTORY]);
    texture_DEFEAT      = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_MATCH_DEFEAT]);
}

EndMatchMenu::~EndMatchMenu(){
}

void EndMatchMenu::Update(bool* open, float deltaTime){
    //NEXT WINDOW STYLE SETUPS
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(screenWidth/2-m_width/2,screenHeight/2 - m_height/2));
    
    ImVec2 close_text_pos;
    if(!ImGui::Begin(m_id,open,w_flags)) ImGui::End(); //SI NO SE INICIA CERRAR INMEDIATAMENTE
    else{
        //ImGui::Text("%s",m_msg.c_str());
        if(m_victory) ImGui::Image(texture_VICTORY,imgSize);
        else ImGui::Image(texture_DEFEAT,imgSize);

        close_text_pos = ImGui::GetCursorScreenPos();
        
        //NEXT BUTTON STYLE
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));

        ImGui::ImageButton(texture_button,buttonSize);
        if(ImGui::IsItemActive()){
            texture_button = texture_pressed;
            closeMenu(open);
        }
        else if(ImGui::IsItemHovered()){ 
            texture_button = texture_hover;
        }
        else{
            texture_button = texture_init;
        }
        
        ImGui::PopStyleColor(3);

        ImVec2 offset(buttonSize.x/2 - (5/2)*ImGui::GetFontSize(), buttonSize.y/2 - ImGui::GetFontSize());
        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*2.0f, ImVec2(close_text_pos.x+offset.x,close_text_pos.y+offset.y), IM_COL32(255,255,255,255), "Return to Lobby");

        ImGui::End();
    }
}


void EndMatchMenu::closeMenu(bool* open){
    *open = false; 
    ImGui::GetIO().MouseDrawCursor = false;
    GraphicEngine::getInstance()->ToggleMenu(false);
    hp->ToggleMenu(false);
}