#include "ShopMenu.h"

ShopMenu::ShopMenu(){
    m_id = "ShopMenu";

    m_width = screenWidth/1.3;
    m_height = screenHeight/1.3;

    buttonSize = ImVec2(120,60);
    m_style.WindowBorderSize = 1.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5));
    ImGui::GetIO().MouseDrawCursor = true;

    load_imagesid(N_OSPELLS, o_spellLayouts, o_spelltexture, o_spellimageid);
    load_imagesid(N_DSPELLS, d_spellLayouts, d_spelltexture, d_spellimageid);
    load_imagesid(N_TSPELLS, t_spellLayouts, t_spelltexture, t_spellimageid);
    load_imagesid(N_TRAPS, trapLayouts, trap_texture, trap_imageid);

    empty_texture = pDevice->getVideoDriver()->getTexture(emptyLayout);
    for(int i = 0; i<N_SOCKETS; i++){
        empty_imageid[i] = GUI->createTexture(empty_texture);
    }

    const_empty_image = GUI->createTexture(empty_texture);
    buttonSize = ImVec2(trap_texture[0]->getSize().Width/10,trap_texture[0]->getSize().Height/10);
}

ShopMenu::~ShopMenu(){}

void ShopMenu::load_imagesid(int total, const char *layouts[], irr::video::ITexture* texture[], IrrIMGUI::IGUITexture* imageid[]){
    for(int i = 0; i<total;i++){
        texture[i] = pDevice->getVideoDriver()->getTexture(layouts[i]);
        imageid[i] = GUI->createTexture(texture[i]);
    }
}

void ShopMenu::load_items(const char* id, int total, int cols, IrrIMGUI::IGUITexture* imageids[], const char * descriptions[]){
    ImGui::Columns(cols, id, false);
    for(int i = 0; i<total; i++){
        ImGui::PushID(i);
        //if(ImGui::Button(o_spellKeys[i], buttonSize)){
        if(ImGui::ImageButton(imageids[i],buttonSize)){
            std::cout<<"button "<<i<<" clicked"<<std::endl;
            //actions[i](open);
        }
        if(ImGui::BeginDragDropSource()){
            ImGui::SetDragDropPayload("image_spell", &imageids[i], sizeof(IrrIMGUI::IGUITexture), ImGuiCond_Once);
            ImGui::EndDragDropSource();
        }
        ImGui::PopID();
        
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",descriptions[i]);
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
    ImGui::Separator();
}

void ShopMenu::Update(bool* open){
    ImGui::ShowTestWindow();
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::Begin(m_id,open,w_flags);

    ImGui::Columns(3, "selected_spells_columns", false);
    for(int i = 0; i<N_SOCKETS ;i++){
        ImGui::PushID(i);
        ImGui::ImageButton(empty_imageid[i],buttonSize);
        ImGui::PopID();

        if (ImGui::BeginDragDropTarget()){
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("image_spell")){
                memcpy((float*)&empty_imageid[i], payload->Data, sizeof(IrrIMGUI::IGUITexture));
                //*selected = const_empty_image;
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Separator();

    load_items("ofensive_spells_columns", N_OSPELLS, 4, o_spellimageid, o_spell_descriptions);
    load_items("defensive_spells_columns", N_DSPELLS, 4, d_spellimageid, d_spell_descriptions);
    load_items("tactic_spells_columns", N_TSPELLS, 4, t_spellimageid, t_spell_descriptions);
    load_items("traps_columns", N_TRAPS, 3, trap_imageid, trap_descriptions); 

    //HELP WINDOWS
    //ImGui::ShowTestWindow();
    //ImGui::ShowMetricsWindow();
    ImGui::End();
}