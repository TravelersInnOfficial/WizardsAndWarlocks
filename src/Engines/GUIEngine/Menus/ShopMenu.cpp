#include "ShopMenu.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./../Managers/ObjectManager.h"
#include "./../Managers/TrapManager.h"
#include <string.h>
#include "./../Managers/PlayerManager.h"
#include "./../Managers/SpellManager.h"

ShopMenu::ShopMenu(MenuType type) : Menu(type){
    N_SPELL_SOCKETS = SpellManager::GetInstance()->GetNumSpells();
    N_TRAP_SOCKETS = 1;

    m_id = "ShopMenu";
    focused_button = 0;
    
    m_width = screenWidth/1.3;
    m_height = screenHeight/1.05;

    m_style.WindowBorderSize = 1.0f;
    ImGui::GetIO().MouseDrawCursor = true;

    hp = PlayerManager::GetInstance()->GetPlayerOne();

    load_imagesid(N_OSPELLS, o_spellLayouts, o_spelltexture, o_spells_codes, &spells_map);
    load_imagesid(N_DSPELLS, d_spellLayouts, d_spelltexture, d_spells_codes, &spells_map);
    load_imagesid(N_TSPELLS, t_spellLayouts, t_spelltexture, t_spells_codes, &spells_map);
    load_imagesid(N_TRAPS, trapLayouts, trap_texture, traps_codes, &traps_map);

    buttonSize = ImVec2(screenWidth/16,screenWidth/16);

    imgui_ddflags = 0;
    if (ddflags.parentNullID)  imgui_ddflags |= ImGuiDragDropFlags_SourceAllowNullID;

    std::map<ImTextureID*,SPELLCODE>::iterator it;
    for(int i = 1; i<N_SPELL_SOCKETS; i++){
        SPELLCODE spell = SpellManager::GetInstance()->GetSpellCode(i,hp);
        for(it = spells_map.begin(); it!=spells_map.end(); ++it){
            if(it->second ==  spell ){
                selected_spells.push_back(it->first);
                break;
            }
        }
    }

    std::map<ImTextureID*,TrapEnum>::iterator itt;
    TrapEnum trap = TrapManager::GetInstance()->getPlayerTrap(hp);
    for(itt = traps_map.begin(); itt!=traps_map.end(); ++itt){
        if(itt->second ==  trap ){
            selected_trap.push_back(itt->first);
            break;
        }
    }

    selected = nullptr;
}

ShopMenu::~ShopMenu(){
}

void ShopMenu::Drop(){
    ShopMenu::~ShopMenu();
}

void ShopMenu::Close(bool* open){
    closeMenu(open);
}

void ShopMenu::load_imagesid(int total, const char* layouts[], ImTextureID* texture[], std::vector<SPELLCODE> codes, std::map<ImTextureID*, SPELLCODE>* map){
    for(int i = 0; i<total;i++){
        texture[i] = (ImTextureID*) toe::GetTextureID(layouts[i]);
        map->insert(std::pair<ImTextureID*,SPELLCODE>(texture[i],codes[i]));
    }
}

void ShopMenu::load_imagesid(int total, const char * layouts[], ImTextureID* texture[], std::vector<TrapEnum> codes,  std::map<ImTextureID*, TrapEnum>* map){
    for(int i = 0; i<total;i++){
        texture[i] = (ImTextureID*) toe::GetTextureID(layouts[i]);
        map->insert(std::pair<ImTextureID*,TrapEnum>(texture[i],codes[i]));
    }
}

void ShopMenu::load_sockets(const char* id,const char* type, int total, int cols, std::vector<ImTextureID*> &items_selected){
    ImGui::Columns(cols, id, false);
    int next_focused = -1; 
    for(int i = 0; i<total ;i++){
        ImGui::PushID(i);
        if(i == focused_button && type != TYPE_TRAP) ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
        if(ImGui::ImageButton( items_selected[i],buttonSize)){
            if(type == TYPE_SPELL) next_focused = i;
        }
        if(i == focused_button && type != TYPE_TRAP) ImGui::PopStyleColor();
        ImGui::PopID();
        
        if(next_focused!=-1) set_focused_button(next_focused);

        if (ImGui::BeginDragDropTarget()){ 
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type,imgui_ddflags)){
                if(!already_selected(items_selected)){
                    memcpy((float*)&items_selected[i], payload->Data, sizeof(ImTextureID));

                    items_selected[i] = selected;
                    //GET THE ELEMENT ENUM CODE
                    if(type == TYPE_SPELL){
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

void ShopMenu::load_items(const char* id,const char* type, int total, int cols, ImTextureID* texture[], const char * names[], const char * descriptions[]){
    ImGui::Columns(cols, id, false);
    for(int i = 0; i<total; i++){
        ImGui::PushID(i);
        if(ImGui::ImageButton( texture[i],buttonSize)){
            selected = texture[i];

            if(!already_selected(selected_spells)){
                if(type == TYPE_SPELL){
                    memcpy((float*)&selected_spells[focused_button], &selected, sizeof(ImTextureID));
                    ChangeSpell(focused_button+1, spells_map[selected]);
                    next_focused_button();
                }
                else{
                    memcpy((float*)&selected_trap[0], &selected, sizeof(ImTextureID));
                    ChangeTrap(traps_map[selected]);
                }
            }
            else if(type == TYPE_SPELL && selected_spells[focused_button] == selected) next_focused_button();
        }
        ImGui::PopID();
        
        if(ImGui::BeginDragDropSource()){
            ImGui::SetDragDropPayload(type, &texture[i], sizeof(ImTextureID), ImGuiCond_Once);
            ImGui::Image(texture[i],buttonSize);
            selected = texture[i];
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

void ShopMenu::Update(bool* open, float deltaTime){
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(0,0));

    if(!ImGui::Begin(m_id,open,w_flags)) ImGui::End();
    else{
        load_sockets("selected_items_columns",TYPE_SPELL,(N_SPELL_SOCKETS-1),3, selected_spells);
        load_sockets("selected_items_columns",TYPE_TRAP,N_TRAP_SOCKETS,1, selected_trap);

        load_items("ofensive_spells_columns",TYPE_SPELL, N_OSPELLS, 4, o_spelltexture, o_spellKeys, o_spell_descriptions);
        load_items("defensive_spells_columns", TYPE_SPELL, N_DSPELLS, 4, d_spelltexture, d_spellKeys, d_spell_descriptions);
        load_items("tactic_spells_columns", TYPE_SPELL, N_TSPELLS, 4, t_spelltexture, t_spellKeys, t_spell_descriptions);
        load_items("traps_columns", TYPE_TRAP, N_TRAPS, 3, trap_texture, trapKeys, trap_descriptions); 
        
        if(ImGui::Button("Close",ImVec2(100,50))){
            closeMenu(open);
        }
        if(ImGui::IsKeyPressed(KEY_3)) set_focused_button(2);
        if(ImGui::IsKeyPressed(KEY_2)) set_focused_button(1);
        if(ImGui::IsKeyPressed(KEY_1)) set_focused_button(0);
        
        //HELP WINDOWS
        //ImGui::ShowTestWindow();
        //ImGui::ShowMetricsWindow();

        ImGui::End();
    }
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

bool ShopMenu::already_selected(std::vector<ImTextureID*> &items_selected){
    bool already_in = false;
    for(int j = 0; j<items_selected.size();j++){
        if(items_selected[j] == selected){ 
            already_in = true;
            break;
        }
    }
    return already_in;
}

void ShopMenu::set_focused_button(int val){
    int aux = 0;
    aux = (val>2) ? 2 : val;
    aux = (val<0) ? 0 : aux;

    focused_button = aux;
}

void ShopMenu::next_focused_button(){
    if(focused_button < 2) focused_button++;
    else focused_button = 0;
}

void ShopMenu::closeMenu(bool* open){
    *open = false; 
    GraphicEngine::getInstance()->ToggleMenu(false);
    HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
    hp->ToggleMenu(false);
    ImGui::GetIO().MouseDrawCursor = false;
}
