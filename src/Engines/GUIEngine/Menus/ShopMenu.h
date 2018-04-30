#ifndef SHOPMENU_H
#define SHOPMENU_H

#include <GUIEngine/Menu.h>
#include <Assets.h>
#include <map>
#include "TrapCodes.h"
#include "SpellCodes.h"


class GSprite;
class Player;
struct DDFlags{
    bool parentNullID = true;

};

class ShopMenu : public Menu{
public:
    ShopMenu(MenuType);
    ~ShopMenu();
    void Update(bool* open, float deltaTime);
    void Close(bool*);
    void Drop();

private:
    void load_imagesid(int number, const char * layouts[], ImTextureID* texture[], std::vector<SPELLCODE>, std::map<ImTextureID*, SPELLCODE>*);
    void load_imagesid(int number, const char * layouts[], ImTextureID* texture[], std::vector<TrapEnum>,  std::map<ImTextureID*, TrapEnum>*);
    void load_sockets(const char* id,const char* type, int total, int cols, std::vector<ImTextureID*> &items_selected);
    void load_items(const char* id,const char* type, int total, int cols, ImTextureID* texture[], std::vector<std::string> item_keys, ImTextureID category_banner, std::string banner_text);
   
    void ChangeSpell(int pos, SPELLCODE sEnum);
    void ChangeTrap(TrapEnum tEnum);

    bool already_selected(std::vector<ImTextureID*> &items_selected);
    void set_focused_button(int);
    void next_focused_button();
    static void closeMenu(bool*);

    static const int N_OSPELLS = 4;     //NUMBER OF OFENSIVE SPELLS
    static const int N_DSPELLS = 2;     //NUMBER OF DEFENSIVE SPELLS
    static const int N_TSPELLS = 7;     //NUMBER OF TACTIC SPELLS
    static const int N_TRAPS = 6;       //NUMBER OF TRAPS
    int N_SPELL_SOCKETS;                //NUMBER OF SPELL SOCKETS
    int N_TRAP_SOCKETS;                 //NUMBER OF TRAP SOCKETS
    int focused_button;                 //ACTUAL FOCUSED BUTTON

    const char* TYPE_SPELL = "image_spell";
    const char* TYPE_TRAP = "image_trap";
    
    char KEY_1 = '1';
    char KEY_2 = '2';
    char KEY_3 = '3';

    ImVec2 itemSize;                    //MENU ITEMS SIZE
    ImVec2 buttonSize;                  //MENU BUTTON SIZE
    ImVec2 bannerSize;                  //MENU BANNER SIZE
    ImGuiDragDropFlags imgui_ddflags;   //DRAG N DROP IMGUI FLAGS
    DDFlags ddflags;                    //DRAG N DROP PRIVATE FLAGS

    ImTextureID texture_init;
    ImTextureID texture_button;
    ImTextureID texture_hover;
    ImTextureID texture_pressed;

    ImTextureID ospells_banner;
    ImTextureID dspells_banner;
    ImTextureID tspells_banner;
    ImTextureID traps_banner;

    ImTextureID* o_spelltexture[N_OSPELLS];
    ImTextureID* d_spelltexture[N_DSPELLS];
    ImTextureID* t_spelltexture[N_TSPELLS];
    ImTextureID* trap_texture[N_TRAPS];

    GSprite* bkg;
    std::vector<ImVec2> slot_pos;
    std::vector<GSprite*> slots;

    std::vector<ImTextureID*> selected_spells;    //ACTUAL SELECTED SPELLS
    std::vector<ImTextureID*> selected_trap;      //ACTUAL SELECTED TRAPS

    ImTextureID* selected;                        //ITEM SELECTED FOR DRAG N DROP

    Player* hp;


    std::vector<SPELLCODE> o_spells_codes = {SPELL_BLIZZARD, SPELL_FIRE, SPELL_POISON, SPELL_THUNDER};
    std::vector<SPELLCODE> d_spells_codes = {SPELL_DEFENSE, SPELL_UNTARGET};
    std::vector<SPELLCODE> t_spells_codes = {SPELL_CLEANSE, SPELL_DUMMY, SPELL_INVISIBILITY, SPELL_SPEED,SPELL_TELEPORT, SPELL_TELEPORTBASE, SPELL_WALL};
    std::vector<TrapEnum> traps_codes = {TENUM_DEATH_CLAWS, TENUM_DISTURBANCE, TENUM_EXPLOSIVE, TENUM_SILENCE, TENUM_SPIRITS, TENUM_TAXES};

    std::map<ImTextureID*,SPELLCODE> spells_map;
    std::map<ImTextureID*,TrapEnum> traps_map;

    std::vector<std::string> banner_text;

//TEXTURES
    const char * o_spellLayouts[N_OSPELLS] = {  TEXTUREMAP[TEXTURE_SPELL_BLIZZARD_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_FIRE_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_POISON_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_THUNDER_HUD].c_str()};

    const char * d_spellLayouts[N_DSPELLS] = {  TEXTUREMAP[TEXTURE_SPELL_DEFENSE_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_UNTARGET_HUD].c_str()};

    const char * t_spellLayouts[N_TSPELLS] = {  TEXTUREMAP[TEXTURE_SPELL_CLEANSE_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_DUMMY_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_INVISIBILITY_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_SPEED_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_TELEPORT_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_TELEPORTBASE_HUD].c_str(),
                                                TEXTUREMAP[TEXTURE_SPELL_WALL_HUD].c_str()};
    
    const char * trapLayouts[N_TRAPS]     = {   TEXTUREMAP[TEXTURE_TENUM_DEATH_CLAWS].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_DISTURBANCE].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_EXPLOSIVE].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_SILENCE].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_SPIRITS].c_str(),
                                                TEXTUREMAP[TEXTURE_TENUM_TAXES].c_str()};

//descriptions 
    void loadShopItemsInfo();
    void loadOfensiveItem(SPELLCODE spell,std::vector<std::string>* data_stack);
    void loadDefensiveItem(SPELLCODE spell,std::vector<std::string>* data_stack);
    void loadTacticItem(SPELLCODE spell,std::vector<std::string>* data_stack);
    void loadTrapItem(TrapEnum trap,std::vector<std::string>* data_stack);
    std::vector<std::string> o_spellKeys;
    std::vector<std::string> d_spellKeys;
    std::vector<std::string> t_spellKeys;
    std::vector<std::string> trapKeys;

};

#endif