#include "AllianceMenu.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include <Assets.h>

AllianceMenu::AllianceMenu(MenuType type) : Menu(type){
    m_id = "AllianceMenu";

    m_width = screenWidth/6.5;
    m_height = screenHeight/4;

    m_style.WindowBorderSize = 0.0f;
    m_style.FrameBorderSize = 0.0f;
    
    ImGui::GetIO().MouseDrawCursor = true;
    bkg = (ImTextureID*) toe::GetTextureID(TEXTUREMAP[TEXTURE_PAPER_BACKGROUND]);

    tex_alliances_init[0] = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_WARLOCK_ALLIANCE]);
    tex_alliances_init[1] = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_WIZARD_ALLIANCE]);

    tex_alliances[0] = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_WARLOCK_ALLIANCE]);
    tex_alliances[1] = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_WIZARD_ALLIANCE]);

    tex_alliances_hover[0] = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_WARLOCK_ALLIANCE_HOVER]);
    tex_alliances_hover[1] = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_WIZARD_ALLIANCE_HOVER]);

    toe::core::TOEvector2di dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_WARLOCK_ALLIANCE]);
    buttonSize = ImVec2(dims.X,dims.Y);
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
    ImGui::SetNextWindowPos(ImVec2(screenWidth/2-m_width/2,screenHeight/2 - m_height/2));
    ImGui::SetNextWindowBgAlpha(0.0f);

    if(!ImGui::Begin(m_id,open,w_flags)) ImGui::End();
    else{
        ImVec2 text_pos;
        ImGui::GetWindowDrawList()->AddImage(bkg,ImVec2(0,0),ImVec2(screenWidth,screenHeight));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5));
        for(int i = 0; i<N_BUTTONS; i++){
            ImGui::PushID(i);

            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));

            text_pos = ImGui::GetCursorScreenPos();
            if(ImGui::ImageButton(tex_alliances[i], buttonSize)){
                actions[i](open);
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();
            
            if (ImGui::IsItemHovered()){
                ImGui::SetTooltip("%s",descriptions[i]);
                tex_alliances[i] = tex_alliances_hover[i];
            }
            else{
                tex_alliances[i] = tex_alliances_init[i];
            }

            
            ImVec2 offset(buttonSize.x/2 - (buttonKeys[i].size()/2)*ImGui::GetFontSize(), buttonSize.y/2 - ImGui::GetFontSize());
            ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*2.0f, ImVec2(text_pos.x+offset.x,text_pos.y+offset.y), IM_COL32(255,255,255,255), buttonKeys[i].c_str());
            
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