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
     
    toe::core::TOEvector2di s = toe::GetTextureDims(TEXTUREMAP[TEXTURE_SHOP_BACKGROUND]); 
    toe::core::TOEvector2df siz = toe::core::TOEvector2df(s.X,s.Y);
    
    m_width = siz.X;
    m_height = siz.Y;
    m_posX = screenWidth/2 - m_width/2;
    m_posY = screenHeight/2 - m_height/2;

    bkg = toe::AddSprite(TEXTUREMAP[TEXTURE_SHOP_BACKGROUND],toe::core::TOEvector2df(m_posX, m_posY),siz);
    
    itemSize = ImVec2(50,50);

    m_style.WindowBorderSize = 0.0f;
    m_style.FrameBorderSize = 0.0f;
    //ImGui::GetIO().MouseDrawCursor = false;

    hp = PlayerManager::GetInstance()->GetPlayerOne();

    load_imagesid(N_OSPELLS, o_spellLayouts, o_spelltexture, o_spells_codes, &spells_map);
    load_imagesid(N_DSPELLS, d_spellLayouts, d_spelltexture, d_spells_codes, &spells_map);
    load_imagesid(N_TSPELLS, t_spellLayouts, t_spelltexture, t_spells_codes, &spells_map);
    load_imagesid(N_TRAPS, trapLayouts, trap_texture, traps_codes, &traps_map);


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

    ospells_banner = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_OSPELLS_BANNER]);
    dspells_banner = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_DSPELLS_BANNER]);
    tspells_banner = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_TSPELLS_BANNER]);
    traps_banner = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_TRAPS_BANNER]);
    toe::core::TOEvector2di dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_OSPELLS_BANNER]);
    bannerSize = ImVec2(dims.X,dims.Y);

    texture_init = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON]);
    texture_button = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON]);
    texture_hover = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON_HOVER]);
    texture_pressed = (void*) toe::GetTextureID(TEXTUREMAP[TEXTURE_BUTTON_PRESSED]);

    dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_BUTTON]);
    buttonSize = ImVec2(dims.X,dims.Y);

    selected = nullptr;
}

ShopMenu::~ShopMenu(){
    m_cursor->Erase();
    m_cursor = nullptr;

    bkg->Erase();
    bkg = nullptr;

    for(int i = 0; i<slots.size(); i++){
        slots[i]->Erase();
        slots[i] = nullptr;
    }
    slots.clear();
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

        ImGui::ImageButton(items_selected[i],itemSize);
        if(ImGui::IsItemActive()){
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
    //ImGui::Separator();
}

void ShopMenu::load_items(const char* id,const char* type, int total, int cols, ImTextureID* texture[], const char * names[], const char * descriptions[], ImTextureID category_banner, std::string banner_text){
    //ImGui::Text("Spell Type Here");
    //ImVec2 banner_text_pos = ImGui::GetCursorScreenPos();
    ImGui::Image(category_banner,bannerSize);
    ImGui::Columns(cols, id, false);

    for(int i = 0; i<total; i++){

        ImGui::PushID(i);
        //NEXT BUTTON STYLE
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));

        //ImGui::Image(texture_slot,itemSize);
        //std::cout<<"slot_position:\t("<<pos.x<<","<<pos.y<<") \n size:\t("<<itemSize.x<<","<<itemSize.y<<"\n";
        
        slot_pos.push_back(ImGui::GetCursorScreenPos());
        
        if(ImGui::ImageButton(texture[i], ImVec2(itemSize.x/1.2,itemSize.y/1.2))){
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

        ImGui::PopStyleColor(3);
        ImGui::PopID();
        
        if(ImGui::BeginDragDropSource()){
            ImGui::SetDragDropPayload(type, &texture[i], sizeof(ImTextureID), ImGuiCond_Once);
            ImGui::Image(texture[i],itemSize);
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
    //ImGui::Separator();
    //ImVec2 offset(bannerSize.x/2 - (banner_text.size()/2)*ImGui::GetFontSize(), bannerSize.y/2 - ImGui::GetFontSize());
    //ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*1.5f, ImVec2(banner_text_pos.x+offset.x,banner_text_pos.y+offset.y), IM_COL32(255,255,255,255), banner_text.c_str());
    
}

void ShopMenu::Update(bool* open, float deltaTime){
    UpdateCursor();
    bkg->ToFront();
    bkg->ToBkg();
    //NEXT WINDOW STYLE SETUPS
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(70,90));
    ImGui::SetNextWindowSize(ImVec2(m_width,m_height));//sets the size of the next window
    ImGui::SetNextWindowPos(ImVec2(m_posX,m_posY));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImVec2 close_text_pos;
    if(!ImGui::Begin(m_id,open,w_flags |= ImGuiWindowFlags_NoScrollWithMouse)) ImGui::End();
    else{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5,5));
        load_sockets("selected_items_columns",TYPE_SPELL,(N_SPELL_SOCKETS-1),3, selected_spells);
        load_sockets("selected_items_columns",TYPE_TRAP,N_TRAP_SOCKETS,1, selected_trap);

        //ImGui::BeginChild("##list-of-items",ImVec2(0,m_height/2));
        
        load_items("ofensive_spells_columns",TYPE_SPELL, N_OSPELLS, N_OSPELLS, o_spelltexture, o_spellKeys, o_spell_descriptions, ospells_banner, "Ofensive Spells");
        load_items("defensive_spells_columns", TYPE_SPELL, N_DSPELLS, N_DSPELLS, d_spelltexture, d_spellKeys, d_spell_descriptions, dspells_banner, "Defensive Spells");
        load_items("tactic_spells_columns", TYPE_SPELL, N_TSPELLS, N_TSPELLS, t_spelltexture, t_spellKeys, t_spell_descriptions, tspells_banner, "Tactic Spells");
        load_items("traps_columns", TYPE_TRAP, N_TRAPS, N_TRAPS, trap_texture, trapKeys, trap_descriptions, traps_banner, "Traps"); 
        
        if(slots.empty()){
            for(int i = 0;i<slot_pos.size();i++) 
                slots.push_back(toe::AddSprite(TEXTUREMAP[TEXTURE_SHOP_SLOT],toe::core::TOEvector2df(slot_pos[i].x-5,screenHeight-slot_pos[i].y-itemSize.y-5),toe::core::TOEvector2df(itemSize.x+10,itemSize.y+10)));
        }
        //ImGui::EndChild();
        ImGui::PopStyleVar();
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

        if(ImGui::IsKeyPressed(KEY_3)) set_focused_button(2);
        if(ImGui::IsKeyPressed(KEY_2)) set_focused_button(1);
        if(ImGui::IsKeyPressed(KEY_1)) set_focused_button(0);
        
        //HELP WINDOWS
        //ImGui::ShowTestWindow();
        //ImGui::ShowMetricsWindow();
        
        ImVec2 offset(buttonSize.x/2 - (5/2)*ImGui::GetFontSize(), buttonSize.y/2 - ImGui::GetFontSize());
        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*2.0f, ImVec2(close_text_pos.x+offset.x,close_text_pos.y+offset.y), IM_COL32(255,255,255,255), "Close");


        ImGui::End();
    }
    ImGui::PopStyleVar(2);
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
