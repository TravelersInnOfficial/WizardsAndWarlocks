#ifndef SHOPMENU_H
#define SHOPMENU_H

#include <GUIEngine/Menu.h>
#include <ItemsData.h>

class ShopMenu : public Menu{
    public:
    ShopMenu();
    ~ShopMenu();
    void Update(bool* open);

    private:

    static const int N_OSPELLS = 4;
    static const int N_DSPELLS = 2;
    static const int N_TSPELLS = 7;
    static const int N_TRAPS = 6;

    ImVec2 buttonSize;
/*
    const char * o_spellLayouts[N_OSPELLS];
    const char * d_spellLayouts[N_DSPELLS];
    const char * f_spellLayouts[N_TSPELLS];
    const char * trapLayouts[N_TRAPS];
 */   
    const char * o_spellKeys[N_OSPELLS] = { SPELLNAMES[SPELL_BLIZZARD], 
                                            SPELLNAMES[SPELL_FIRE], 
                                            SPELLNAMES[SPELL_POISON],
                                            SPELLNAMES[SPELL_THUNDER]};

    const char * d_spellKeys[N_DSPELLS] = { SPELLNAMES[SPELL_DEFENSE],
                                            SPELLNAMES[SPELL_UNTARGET]};

    const char * t_spellKeys[N_TSPELLS] = { SPELLNAMES[SPELL_CLEANSE],
                                            SPELLNAMES[SPELL_DUMMY],
                                            SPELLNAMES[SPELL_INVISIBILITY],
                                            SPELLNAMES[SPELL_SPEED],
                                            SPELLNAMES[SPELL_TELEPORT],
                                            SPELLNAMES[SPELL_TELEPORTBASE],
                                            SPELLNAMES[SPELL_WALL]};

    const char * trapKeys[N_TRAPS]      = { TRAPNAMES[TENUM_DEATH_CLAWS],
                                            TRAPNAMES[TENUM_DISTURBANCE],
                                            TRAPNAMES[TENUM_EXPLOSIVE],
                                            TRAPNAMES[TENUM_SILENCE],
                                            TRAPNAMES[TENUM_SPIRITS],
                                            TRAPNAMES[TENUM_TAXES]};

    const char * o_spell_descriptions[N_OSPELLS] = { SPELLDESC[SPELL_BLIZZARD], 
                                                     SPELLDESC[SPELL_FIRE], 
                                                     SPELLDESC[SPELL_POISON],
                                                     SPELLDESC[SPELL_THUNDER]};

    const char * d_spell_descriptions[N_DSPELLS] = { SPELLDESC[SPELL_DEFENSE],
                                                     SPELLDESC[SPELL_UNTARGET]};

    const char * t_spell_descriptions[N_TSPELLS] = { SPELLDESC[SPELL_CLEANSE],
                                                     SPELLDESC[SPELL_DUMMY],
                                                     SPELLDESC[SPELL_INVISIBILITY],
                                                     SPELLDESC[SPELL_SPEED],
                                                     SPELLDESC[SPELL_TELEPORT],
                                                     SPELLDESC[SPELL_TELEPORTBASE],
                                                     SPELLDESC[SPELL_WALL]};

    const char * trap_descriptions[N_TRAPS]      = { TRAPDESC[TENUM_DEATH_CLAWS],
                                                     TRAPDESC[TENUM_DISTURBANCE],
                                                     TRAPDESC[TENUM_EXPLOSIVE],
                                                     TRAPDESC[TENUM_SILENCE],
                                                     TRAPDESC[TENUM_SPIRITS],
                                                     TRAPDESC[TENUM_TAXES]};

};

#endif