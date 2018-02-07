#include "ShopMenu.h"

ShopMenu::ShopMenu(){
    m_id = "ShopMenu";

    m_width = screenWidth/1.3;
    m_height = screenHeight/1.3;

    buttonSize = ImVec2(120,60);
    m_style.WindowBorderSize = 1.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5));
    ImGui::GetIO().MouseDrawCursor = true;
}

ShopMenu::~ShopMenu(){}

void ShopMenu::Update(bool* open){
    ImGui::ShowTestWindow();
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::Begin(m_id,open,w_flags);
    for(int i = 0; i<N_OSPELLS; i++){
        ImGui::PushID(i);
        if(ImGui::Button(o_spellKeys[i], buttonSize)){
            std::cout<<"button "<<i<<" clicked"<<std::endl;
            //actions[i](open);
        }
        ImGui::PopID();
        
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",o_spell_descriptions[i]);
    }

    for(int i = 0; i<N_DSPELLS; i++){
        ImGui::PushID(i);
        if(ImGui::Button(d_spellKeys[i], buttonSize)){
            std::cout<<"button "<<i<<" clicked"<<std::endl;
            //actions[i](open);
        }
        ImGui::PopID();
        
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",d_spell_descriptions[i]);
    }

    for(int i = 0; i<N_TSPELLS; i++){
        ImGui::PushID(i);
        if(ImGui::Button(t_spellKeys[i], buttonSize)){
            std::cout<<"button "<<i<<" clicked"<<std::endl;
            //actions[i](open);
        }
        ImGui::PopID();
        
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",t_spell_descriptions[i]);
    }

    for(int i = 0; i<N_TRAPS; i++){
        ImGui::PushID(i);
        if(ImGui::Button(trapKeys[i], buttonSize)){
            std::cout<<"button "<<i<<" clicked"<<std::endl;
            //actions[i](open);
        }
        ImGui::PopID();
        
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",trap_descriptions[i]);
    }

    
    //HELP WINDOWS
    //ImGui::ShowTestWindow();
    //ImGui::ShowMetricsWindow();

    ImGui::End();
}