#ifndef ITEMSDATA_H
#define ITEMSDATA_H

#include <iostream>
#include <map>

#include "TrapCodes.h"
#include "SpellCodes.h"

static std::map<SPELLCODE, const char *> SPELL_NAMES(){
    std::map<SPELLCODE, const char *> spell_name;

    //OFENSIVE SPELLS
    spell_name[SPELL_BLIZZARD]       = "GUIVERN WIND";
    spell_name[SPELL_FIRE]           = "DRAGON BREATH";
    spell_name[SPELL_POISON]         = "OGRE BURP";
    spell_name[SPELL_THUNDER]        = "ODIN FURY";

    //DEFENSIVE SPELLS
    spell_name[SPELL_DEFENSE]        = "DIVINE PONCHO";
    spell_name[SPELL_UNTARGET]       = "OHMIO PROTECTION";

    //TACTIC SPELLS
    spell_name[SPELL_CLEANSE]        = "GAIA WATERS";
    spell_name[SPELL_DUMMY]          = "DUMMY CLONE";
    spell_name[SPELL_INVISIBILITY]   = "INVISIBILITY CAPE";
    spell_name[SPELL_SPEED]          = "SUPREME SPEED";
    spell_name[SPELL_TELEPORT]       = "TELEPORT";
    spell_name[SPELL_TELEPORTBASE]   = "TELEPORT BASE";
    spell_name[SPELL_WALL]           = "DESPERATION WALL";

    return spell_name;
};

static std::map<SPELLCODE, const char *> SPELL_DESCRIPTIONS(){
    std::map<SPELLCODE, const char *> spell_desc;

    //OFENSIVE SPELLS
    spell_desc[SPELL_BLIZZARD]       = "Blizzard area that damages your enemy and slows it down gradually until freeze.";
    spell_desc[SPELL_FIRE]           = "Fireball that causes burn effect for several seconds.";
    spell_desc[SPELL_POISON]         = "Poison bomb that creates a big poisonus damage area.";
    spell_desc[SPELL_THUNDER]        = "Thunderball that causes paralysis.";

    //DEFENSIVE SPELLS
    spell_desc[SPELL_DEFENSE]        = "Protects against all spells for several seconds.";
    spell_desc[SPELL_UNTARGET]       = "Small protection.";

    //TACTIC SPELLS
    spell_desc[SPELL_CLEANSE]        = "Clears all active effects upon you.";
    spell_desc[SPELL_DUMMY]          = "Creates a clone of you.";
    spell_desc[SPELL_INVISIBILITY]   = "Makes you invisible for a short time.";
    spell_desc[SPELL_SPEED]          = "Makes you incredibly fast.";
    spell_desc[SPELL_TELEPORT]       = "Teleports you a small distance the way you are looking.";
    spell_desc[SPELL_TELEPORTBASE]   = "Sets a teleport point. Click again on the spell to teleport back to that point.";
    spell_desc[SPELL_WALL]           = "Summons a great protection wall.";

    return spell_desc;
};

static std::map<TrapEnum, const char *> TRAP_NAMES(){
    std::map<TrapEnum, const char *> trap_name;

    trap_name[TENUM_DEATH_CLAWS]    = "DEATH CLAWS";
    trap_name[TENUM_SPIRITS]        = "UNDERWORLD SPIRITS";
    trap_name[TENUM_SILENCE]        = "SPECTRAL SILENCE";
    trap_name[TENUM_TAXES]          = "CARONTE TAXES";
    trap_name[TENUM_DISTURBANCE]    = "BEING DISTURBANCE";
    trap_name[TENUM_EXPLOSIVE]      = "EXPLOSIVE";

    return trap_name;
};

static std::map<TrapEnum, const char *> TRAP_DESCRIPTIONS(){
    std::map<TrapEnum, const char *> trap_desc;

    trap_desc[TENUM_DEATH_CLAWS]    = "Freezes the target and makes damage.";
    trap_desc[TENUM_SPIRITS]        = "Creates an illusion that makes sight difficult.";
    trap_desc[TENUM_SILENCE]        = "Bans the target for casting any spell.";
    trap_desc[TENUM_TAXES]          = "Destroys the targets actual potion.";
    trap_desc[TENUM_DISTURBANCE]    = "Reverses the target controls.";
    trap_desc[TENUM_EXPLOSIVE]      = "Makes damage to the target with an explosion.";

    return trap_desc;
};

static std::map<SPELLCODE, const char *> SPELLNAMES = SPELL_NAMES();
static std::map<SPELLCODE, const char *> SPELLDESC = SPELL_DESCRIPTIONS();

static std::map<TrapEnum, const char *> TRAPNAMES = TRAP_NAMES();
static std::map<TrapEnum, const char *> TRAPDESC = TRAP_DESCRIPTIONS();


#endif