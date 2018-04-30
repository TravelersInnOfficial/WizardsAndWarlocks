#include "AllianceMenu.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include <Assets.h>

AllianceMenu::AllianceMenu(MenuType type) : Menu(type){
    m_id = "AllianceMenu";
    
     TOEvector2di s = toe::GetTextureDims(TEXTUREMAP[TEXTURE_ALLIANCE_BACKGROUND]); 
    vector2df siz(s.X,s.Y);
    m_width = siz.X;
    m_height = siz.Y;
    m_posX = screenWidth/2 - m_width/2;
    m_posY = screenHeight/2 - m_height/2;

    bkg = GraphicEngine::getInstance()->addSprite(TEXTUREMAP[TEXTURE_ALLIANCE_BACKGROUND], vector2df(m_posX, m_posY), siz);

    m_style.WindowBorderSize = 0.0f;
    m_style.FrameBorderSize = 0.0f;
    
    //ImGui::GetIO().MouseDrawCursor = true;

    tex_alliances_init[0] = (void*)(size_t) toe::GetTextureID(TEXTUREMAP[TEXTURE_WARLOCK_ALLIANCE]);
    tex_alliances_init[1] = (void*)(size_t) toe::GetTextureID(TEXTUREMAP[TEXTURE_WIZARD_ALLIANCE]);

    tex_alliances[0] = (void*)(size_t) toe::GetTextureID(TEXTUREMAP[TEXTURE_WARLOCK_ALLIANCE]);
    tex_alliances[1] = (void*)(size_t) toe::GetTextureID(TEXTUREMAP[TEXTURE_WIZARD_ALLIANCE]);

    tex_alliances_hover[0] = (void*)(size_t) toe::GetTextureID(TEXTUREMAP[TEXTURE_WARLOCK_ALLIANCE_HOVER]);
    tex_alliances_hover[1] = (void*)(size_t) toe::GetTextureID(TEXTUREMAP[TEXTURE_WIZARD_ALLIANCE_HOVER]);

     TOEvector2di dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_WARLOCK_ALLIANCE]);
    buttonSize = ImVec2(dims.X,dims.Y);
    m_fontSize = ImGui::GetFontSize()*1.3f;
}

AllianceMenu::~AllianceMenu(){
    delete bkg;
    bkg = nullptr;

    delete m_cursor;
    m_cursor = nullptr;
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
    UpdateCursor();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(m_width-buttonSize.x*2,0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,m_height/2-buttonSize.y/2));
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(m_posX,m_posY));
    ImGui::SetNextWindowBgAlpha(0.0f);

    if(!ImGui::Begin(m_id,open,w_flags)) ImGui::End();
    else{
        ImVec2 text_pos;
        /*
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
        */
        ImGui::Columns(2, "alliance_columns", false);

        for(int i = 0; i<N_BUTTONS; i++){
            ImGui::PushID(i);

            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5,5));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
            
            text_pos = ImGui::GetCursorScreenPos();
            if(ImGui::ImageButton(tex_alliances[i], buttonSize)){
                actions[i](open);
            }
            if (ImGui::IsItemHovered()){
                ImGui::SetTooltip("%s",descriptions[i]);
                tex_alliances[i] = tex_alliances_hover[i];
            }
            else{
                tex_alliances[i] = tex_alliances_init[i];
            }

            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(3);
            ImGui::PopID();
            
            
            ImVec2 offset((buttonSize.x - (buttonKeys[i].size()/2*m_fontSize))/2, buttonSize.y/2 - m_fontSize/2);
            ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), m_fontSize, ImVec2(text_pos.x+offset.x,text_pos.y+offset.y), IM_COL32(255,255,255,255), buttonKeys[i].c_str());
            ImGui::NextColumn();
        }

        ImGui::Columns(1);
        //HELP WINDOWS
        //ImGui::ShowTestWindow();
        //ImGui::ShowMetricsWindow();
        
        ImGui::End();
    }
    ImGui::PopStyleVar(2);
}

void AllianceMenu::closeMenu(bool* open){
    *open = false;
    GraphicEngine::getInstance()->ToggleMenu(false);

    HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
    hp->ToggleMenu(false);

    ImGui::GetIO().MouseDrawCursor = false;
}