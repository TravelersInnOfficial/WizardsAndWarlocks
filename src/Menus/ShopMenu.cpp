#include "ShopMenu.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./../Managers/ObjectManager.h"
#include "./../Npcs/NpcSeller.h"

ShopMenu::ShopMenu(){
    m_id = "ShopMenu";

    m_width = screenWidth/1.3;
    m_height = screenHeight/1.3;

    buttonSize = ImVec2(120,60);
    m_style.WindowBorderSize = 1.0f;
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5));
    ImGui::GetIO().MouseDrawCursor = true;

    load_imagesid(N_OSPELLS, o_spellLayouts, o_spelltexture, o_spellimageid, o_spells_codes, &spells_map);
    load_imagesid(N_DSPELLS, d_spellLayouts, d_spelltexture, d_spellimageid, t_spells_codes, &spells_map);
    load_imagesid(N_TSPELLS, t_spellLayouts, t_spelltexture, t_spellimageid, d_spells_codes, &spells_map);
    load_imagesid(N_TRAPS, trapLayouts, trap_texture, trap_imageid, traps_codes, &traps_map);

    empty_texture = pDevice->getVideoDriver()->getTexture(emptyLayout);
    for(int i = 0; i<N_SOCKETS; i++){
        empty_spell_imageid[i] = GUI->createTexture(empty_texture);
    }
    empty_trap_imageid[0] = GUI->createTexture(empty_texture); 

    const_empty_image = GUI->createTexture(empty_texture);
    buttonSize = ImVec2(trap_texture[0]->getSize().Width/10,trap_texture[0]->getSize().Height/10);

    imgui_ddflags = 0;
    if (ddflags.parentNullID)  imgui_ddflags |= ImGuiDragDropFlags_SourceAllowNullID;
    
    for(int i = 0; i<N_SOCKETS;i++){
        selected_spells.push_back(const_empty_image);
    }
    selected_trap.push_back(const_empty_image);

    selected = NULL;
}

ShopMenu::~ShopMenu(){}

void ShopMenu::load_imagesid(int total, const char *layouts[], irr::video::ITexture* texture[], IrrIMGUI::IGUITexture* imageid[], std::vector<SPELLCODE> codes, std::map<IrrIMGUI::IGUITexture*,SPELLCODE>* map){
    for(int i = 0; i<total;i++){
        texture[i] = pDevice->getVideoDriver()->getTexture(layouts[i]);
        imageid[i] = GUI->createTexture(texture[i]);
        map->insert(std::pair<IrrIMGUI::IGUITexture*,SPELLCODE>(imageid[i],codes[i]));
    }
}

void ShopMenu::load_imagesid(int total, const char *layouts[], irr::video::ITexture* texture[], IrrIMGUI::IGUITexture* imageid[], std::vector<TrapEnum> codes, std::map<IrrIMGUI::IGUITexture*,TrapEnum>* map){
    for(int i = 0; i<total;i++){
        texture[i] = pDevice->getVideoDriver()->getTexture(layouts[i]);
        imageid[i] = GUI->createTexture(texture[i]);
        map->insert(std::pair<IrrIMGUI::IGUITexture*,TrapEnum>(imageid[i],codes[i]));
    }
}

void ShopMenu::load_sockets(const char* id,const char* type, int total, int cols, IrrIMGUI::IGUITexture* imageids[],std::vector<IrrIMGUI::IGUITexture*> &items_selected){
    ImGui::Columns(cols, id, false);
    for(int i = 0; i<total ;i++){
        ImGui::PushID(i);
        if(ImGui::ImageButton(imageids[i],buttonSize)){
            //focus button?
        }
        ImGui::PopID();

        if (ImGui::BeginDragDropTarget()){ 
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type,imgui_ddflags)){
                bool already_in = false;
                for(int j = 0; j<items_selected.size();j++){
                    if(items_selected[j] == selected) already_in = true;
                }
                if(!already_in){
                    memcpy((float*)&imageids[i], payload->Data, sizeof(IrrIMGUI::IGUITexture));
                    items_selected[i] = selected;
                    
                    //GET THE ELEMENT ENUM CODE
                    if(type == "image_spell") std::cout<<"spell code selected: "<<spells_map[selected]<<std::endl;
                    else std::cout<<"trap code selected: "<< traps_map[selected]<<std::endl;
                }
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Separator();
}

void ShopMenu::load_items(const char* id, const char* type, int total, int cols, IrrIMGUI::IGUITexture* imageids[],const char * names[], const char * descriptions[]){
    ImGui::Columns(cols, id, false);
    for(int i = 0; i<total; i++){
        ImGui::PushID(i);
        if(ImGui::ImageButton(imageids[i],buttonSize)){
            //std::cout<<"button "<<i<<" clicked"<<std::endl;
        }
        ImGui::PopID();
        
        if(ImGui::BeginDragDropSource()){
            ImGui::SetDragDropPayload(type, &imageids[i], sizeof(IrrIMGUI::IGUITexture), ImGuiCond_Once);
            ImGui::Image(imageids[i],buttonSize);
            selected = imageids[i];
            ImGui::EndDragDropSource();
        }
        if (ImGui::IsItemHovered()){ 
            ImGui::BeginTooltip();

            ImGui::Text("%s\n",names[i]);
            ImGui::Text("%s\n",descriptions[i]);

            ImGui::EndTooltip();
        }
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Separator();
}

void ShopMenu::Update(bool* open){
    //ImGui::ShowTestWindow();
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(0,0));

    //ImGui::NewFrame();
    ImGui::Begin(m_id,open,w_flags);

    load_sockets("selected_items_columns","image_spell",N_SOCKETS,3,empty_spell_imageid, selected_spells);
    load_sockets("selected_items_columns","image_trap",1,1,empty_trap_imageid, selected_trap);

    load_items("ofensive_spells_columns","image_spell", N_OSPELLS, 4, o_spellimageid, o_spellKeys, o_spell_descriptions);
    load_items("defensive_spells_columns", "image_spell", N_DSPELLS, 4, d_spellimageid, d_spellKeys, d_spell_descriptions);
    load_items("tactic_spells_columns", "image_spell", N_TSPELLS, 4, t_spellimageid, t_spellKeys, t_spell_descriptions);
    load_items("traps_columns", "image_trap", N_TRAPS, 3, trap_imageid, trapKeys, trap_descriptions); 
    
    if(ImGui::Button("Close",ImVec2(100,50))){
        //*open = false;
        //GraphicEngine::getInstance()->ToggleMenu(false);
        //ObjectManager::GetInstance()->StopInteractionsNPC();
    }
    //HELP WINDOWS
    //ImGui::ShowTestWindow();
    //ImGui::ShowMetricsWindow();
    //ImGui::PopStyleVar();
    ImGui::End();
    //ImGui::Render();
}