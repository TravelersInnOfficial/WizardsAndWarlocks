#include "ShopMenu.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./../Managers/ObjectManager.h"
#include "./../Managers/TrapManager.h"
#include <string.h>

ShopMenu::ShopMenu(){
    N_SPELL_SOCKETS = SpellManager::GetInstance()->GetNumSpells();
    N_TRAP_SOCKETS = 1;

    m_id = "ShopMenu";
    
    m_width = screenWidth/1.3;
    m_height = screenHeight/1.05;

    m_style.WindowBorderSize = 1.0f;
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,5));
    ImGui::GetIO().MouseDrawCursor = true;

    hp = PlayerManager::GetInstance()->GetPlayerOne();

    load_imagesid(N_OSPELLS, o_spellLayouts, o_spelltexture, o_spellimageid, o_spells_codes, &spells_map);
    load_imagesid(N_DSPELLS, d_spellLayouts, d_spelltexture, d_spellimageid, d_spells_codes, &spells_map);
    load_imagesid(N_TSPELLS, t_spellLayouts, t_spelltexture, t_spellimageid, t_spells_codes, &spells_map);
    load_imagesid(N_TRAPS, trapLayouts, trap_texture, trap_imageid, traps_codes, &traps_map);

    buttonSize = ImVec2(trap_texture[0]->getSize().Width/10,trap_texture[0]->getSize().Height/10);

    imgui_ddflags = 0;
    if (ddflags.parentNullID)  imgui_ddflags |= ImGuiDragDropFlags_SourceAllowNullID;

    std::map<IrrIMGUI::IGUITexture*,SPELLCODE>::iterator it;
    for(int i = 1; i<N_SPELL_SOCKETS; i++){
        SPELLCODE spell = SpellManager::GetInstance()->GetSpellCode(i,hp);
        for(it = spells_map.begin(); it!=spells_map.end(); ++it){
            if(it->second ==  spell ){
                selected_spells.push_back(it->first);
                break;
            }
        }
    }

    std::map<IrrIMGUI::IGUITexture*,TrapEnum>::iterator itt;
    TrapEnum trap = TrapManager::GetInstance()->getPlayerTrap(hp);
    for(itt = traps_map.begin(); itt!=traps_map.end(); ++itt){
        if(itt->second ==  trap ){
            selected_trap.push_back(itt->first);
            break;
        }
    }

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

void ShopMenu::load_sockets(const char* id,const char* type, int total, int cols,std::vector<IrrIMGUI::IGUITexture*> &items_selected){
    ImGui::Columns(cols, id, false);
    for(int i = 0; i<total ;i++){
        ImGui::PushID(i);
        if(ImGui::ImageButton(items_selected[i],buttonSize)){
            //focus button?
        }
        ImGui::PopID();

        if (ImGui::BeginDragDropTarget()){ 
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type,imgui_ddflags)){
                bool already_in = false;
                for(int j = 0; j<items_selected.size();j++){
                    if(items_selected[j] == selected){ 
                        already_in = true;
                        break;
                    }
                }
                if(!already_in){
                    memcpy((float*)&items_selected[i], payload->Data, sizeof(IrrIMGUI::IGUITexture));
                    
                    //GET THE ELEMENT ENUM CODE
                    const char* t = "image_spell";
                    if(type == t){
                        ChangeSpell(i+1, spells_map[selected]);
                    }
                    else{
                        ChangeTrap(traps_map[selected]);
                    }
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

    load_sockets("selected_items_columns","image_spell",(N_SPELL_SOCKETS-1),3, selected_spells);
    load_sockets("selected_items_columns","image_trap",N_TRAP_SOCKETS,1, selected_trap);

    load_items("ofensive_spells_columns","image_spell", N_OSPELLS, 4, o_spellimageid, o_spellKeys, o_spell_descriptions);
    load_items("defensive_spells_columns", "image_spell", N_DSPELLS, 4, d_spellimageid, d_spellKeys, d_spell_descriptions);
    load_items("tactic_spells_columns", "image_spell", N_TSPELLS, 4, t_spellimageid, t_spellKeys, t_spell_descriptions);
    load_items("traps_columns", "image_trap", N_TRAPS, 3, trap_imageid, trapKeys, trap_descriptions); 
    
    if(ImGui::Button("Close",ImVec2(100,50))){
        closeMenu(open);
    }
    //HELP WINDOWS
    //ImGui::ShowTestWindow();
    //ImGui::ShowMetricsWindow();
    //ImGui::PopStyleVar();
    ImGui::End();
    //ImGui::Render();
}

void ShopMenu::ChangeSpell(int pos, SPELLCODE sEnum){
    if(hp != NULL){
		SpellManager::GetInstance()->AddHechizo(pos, hp, sEnum);
	}
}

void ShopMenu::ChangeTrap(TrapEnum tEnum){
	if(hp != NULL){
		TrapManager::GetInstance()->setPlayerTrap(hp, tEnum);
		TrapManager::GetInstance()->setPlayerUsings(hp, 10);
	}
}

void ShopMenu::closeMenu(bool* open){
    
    //*open = false; //TODO:: FIX por alguna razon esto peta al salir y volver a entrar en el menu
    GraphicEngine::getInstance()->ToggleMenu(false);
    ObjectManager::GetInstance()->StopInteractionsNPC();
}