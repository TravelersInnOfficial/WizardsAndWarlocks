#include "EndMatchMenu.h"
#include "./../../../Players/HumanPlayer.h"
#include "./../../../Managers/PlayerManager.h"
#include <TOcularEngine/TOcularEngine.h>
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GSprite.h>
#include <Assets.h>

EndMatchMenu::EndMatchMenu(MenuType type, int winners) : Menu(type){
    m_id = "EndMatchMenu";

    hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();    
    Alliance p_alliance = hp->GetAlliance();

    m_msg = "";
    m_victory = true;
    if(winners != (int)p_alliance) m_victory = false;

    HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
    hp->ToggleMenu(true);

    m_style.WindowBorderSize = 0.0f;
    
    TOEvector2di btn_dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_BUTTON]);
    buttonSize = ImVec2(btn_dims.X,btn_dims.Y);

    texture_init        = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON]);
    texture_button      = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON]);
    texture_hover       = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON_HOVER]);
    texture_pressed     = (void*)(size_t)toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON_PRESSED]);

    TOEvector2di bkg_dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_MATCH_VICTORY]);
    imgSize = ImVec2(bkg_dims.X,bkg_dims.Y);

    m_width = btn_dims.X + 5;
    m_height = btn_dims.Y + 20;

    m_posX = screenWidth/2-m_width/2;
    m_posY = screenHeight/2 + bkg_dims.Y/2 - m_height;
    
    bkg = nullptr;
}

EndMatchMenu::~EndMatchMenu(){
    delete bkg;
    delete m_cursor;    
    
}

void EndMatchMenu::Update(bool* open, float deltaTime){
    UpdateCursor();

    //NEXT WINDOW STYLE SETUPS
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(m_posX,m_posY));
    ImGui::SetNextWindowBgAlpha(0.0f);
    
    ImVec2 close_text_pos;
    if(!ImGui::Begin(m_id,open,w_flags |= ImGuiWindowFlags_NoScrollWithMouse )) ImGui::End(); //SI NO SE INICIA CERRAR INMEDIATAMENTE
    else{
        if(bkg == nullptr){
            TOEvector2di dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_MATCH_VICTORY]);
            float posX = screenWidth/2 - imgSize.x/2;
            float posY = screenHeight/2 - imgSize.y/2;

            if(m_victory){
                bkg = GraphicEngine::getInstance()->addSprite(TEXTUREMAP[TEXTURE_MATCH_VICTORY],vector2df(posX, posY),vector2df(dims.X, dims.Y));
            }
            else{
                bkg = GraphicEngine::getInstance()->addSprite(TEXTUREMAP[TEXTURE_MATCH_DEFEAT],vector2df(posX, posY),vector2df(dims.X, dims.Y));
            }
        }

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

        ImVec2 offset(buttonSize.x/2 - (15.5/2)*ImGui::GetFontSize(), buttonSize.y/2 - ImGui::GetFontSize());
        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*2.0f, ImVec2(close_text_pos.x+offset.x,close_text_pos.y+offset.y), IM_COL32(255,255,255,255), "Return to Lobby");

        ImGui::End();
    }

    ImGui::PopStyleVar();
}


void EndMatchMenu::closeMenu(bool* open){
    *open = false; 
    ImGui::GetIO().MouseDrawCursor = false;
    GraphicEngine::getInstance()->ToggleMenu(false);
    hp->ToggleMenu(false);
}