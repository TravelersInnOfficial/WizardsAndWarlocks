#ifndef ASSETS_H
#define ASSETS_H

#include <iostream>
#include <vector>
#include <map>
#include "Animation_Strings.h"

enum GAMETEXTURES {
	TEXTURE_tilefloor,
	TEXTURE_stones,
	TEXTURE_npc,
	TEXTURE_button,
	TEXTURE_door,
	TEXTURE_Warlock,
	TEXTURE_game_icon,
	TEXTURE_uno,
	TEXTURE_cuatro,
	TEXTURE_dos,
	TEXTURE_tres,
	TEXTURE_cero,
	TEXTURE_TENUM_TAXES,
	TEXTURE_TENUM_DISTURBANCE,
	TEXTURE_TENUM_SILENCE,
	TEXTURE_TENUM_DEATH_CLAWS,
	TEXTURE_TENUM_EXPLOSIVE,
	TEXTURE_TENUM_SPIRITS,
	TEXTURE_POT_FIRE_HUD,
	TEXTURE_POT_SHIELD_HUD,
	TEXTURE_POT_MP_HUD,
	TEXTURE_POT_ICE_HUD,
	TEXTURE_POT_POISON_HUD,
	TEXTURE_POT_THUNDER_HUD,
	TEXTURE_POT_HP_HUD,
	TEXTURE_SPELL_SPEED_HUD,
	TEXTURE_SPELL_DUMMY_HUD,
	TEXTURE_SPELL_CLEANSE_HUD,
	TEXTURE_SPELL_PROJECTILE_WIZARD_HUD,
	TEXTURE_SPELL_BLIZZARD_HUD,
	TEXTURE_SPELL_WALL_HUD,
	TEXTURE_SPELL_PROJECTILE_WARLOCK_HUD,
	TEXTURE_SPELL_TELEPORTBASE_HUD,
	TEXTURE_SPELL_THUNDER_HUD,
	TEXTURE_SPELL_FIRE_HUD,
	TEXTURE_SPELL_UNTARGET_HUD,
	TEXTURE_SPELL_POISON_HUD,
	TEXTURE_SPELL_DEFENSE_HUD,
	TEXTURE_SPELL_TELEPORT_HUD,
	TEXTURE_SPELL_INVISIBILITY_HUD,
	TEXTURE_BLOOD,
	TEXTURE_FUZZY,
	TEXTURE_HITLANDED,
	TEXTURE_decal,
	TEXTURE_skydome,
	TEXTURE_SPELL_WALL,
	TEXTURE_SPELL_THUNDER,
	TEXTURE_SPELL_FIRE,
	TEXTURE_SPELL_POISON,
	TEXTURE_SPELL_PROJECTILE,
	TEXTURE_gold,
	TEXTURE_grass,
	TEXTURE_bookcase,
	TEXTURE_white_floor,
	TEXTURE_gardenwall,
	TEXTURE_rock,
	TEXTURE_none,
	TEXTURE_Switch,
	TEXTURE_2897,
	TEXTURE_doorblocked,
	TEXTURE_Wizard,
	TEXTURE_marbre5,
	TEXTURE_negro,
	TEXTURE_yellow,
	TEXTURE_madera,
	TEXTURE_BOOK_BACKGROUND,
	TEXTURE_SHOP_BACKGROUND,
	TEXTURE_ALLIANCE_BACKGROUND,
	TEXTURE_BUTTON,
	TEXTURE_BUTTON_HOVER,
	TEXTURE_BUTTON_PRESSED,
	TEXTURE_GUI_CURSOR,
	TEXTURE_GUI_CURSOR_GLOW,
	TEXTURE_SHOP_SLOT,
	TEXTURE_WARLOCK_ALLIANCE,
	TEXTURE_WIZARD_ALLIANCE,
	TEXTURE_WARLOCK_ALLIANCE_HOVER,
	TEXTURE_WIZARD_ALLIANCE_HOVER,
	TEXTURE_OSPELLS_BANNER,
	TEXTURE_DSPELLS_BANNER,
	TEXTURE_TSPELLS_BANNER,
	TEXTURE_TRAPS_BANNER,
	TEXTURE_MENU_TITLE,
	TEXTURE_ORB_BACK,
	TEXTURE_ORB_FRONT,
	TEXTURE_ORB_FILL,
	TEXTURE_ORB_SCROLL_FILL,
	TEXTURE_ORB_SCROLL_LIP,
	TEXTURE_ORB_SCROLL_FILL_MASK,
	TEXTURE_SPELL_SLOT,
	TEXTURE_ITEM_SLOT,
	TEXTURE_ITEM_SLOT_MASK
};

enum GAMEMESHES {
	MESH_grail,
	MESH_WarlockArm,
	MESH_Wizard,
	MESH_door,
	MESH_fountain,
	MESH_button,
	MESH_WizardArm,
	MESH_potion,
	MESH_bomb,
	MESH_Warlock,
	MESH_npc,
	MESH_bookcase
};

enum GAMEANIMATIONS {
	ANIM_WIZARD_TSHOOT1,
	ANIM_WIZARD_TSHOOT2,
	ANIM_WIZARD_TSHOOT3,
	ANIM_WIZARD_TDRINK,
	ANIM_WIZARD_TINTERACT,
	ANIM_WIZARD_TIDLE,
	ANIM_WIZARD_BIDLE,
	ANIM_WIZARD_TWALK,
	ANIM_WIZARD_BWALK,
	ANIM_WIZARD_TRUN,
	ANIM_WIZARD_BRUN,
	ANIM_WIZARD_ARMIDLE,
	ANIM_WIZARD_ARMSHOOT1,
	ANIM_WIZARD_ARMWALK,
};


static std::map<GAMETEXTURES, std::string> CREATE_TEXTUREMAP(){
	std::map<GAMETEXTURES, std::string> m;

	m[TEXTURE_tilefloor]					= "./../assets/textures/tilefloor.png";
	m[TEXTURE_stones]						= "./../assets/textures/stones.jpg";
	m[TEXTURE_npc]							= "./../assets/textures/npc.png";
	m[TEXTURE_button]						= "./../assets/textures/button.png";
	m[TEXTURE_door]							= "./../assets/textures/door.jpg";
	m[TEXTURE_Warlock]						= "./../assets/textures/Warlock.png";
	m[TEXTURE_game_icon]					= "./../assets/textures/game_icon.png";
	m[TEXTURE_uno]							= "./../assets/textures/HUD/Counter/1.png";
	m[TEXTURE_cuatro]						= "./../assets/textures/HUD/Counter/4.png";
	m[TEXTURE_dos]							= "./../assets/textures/HUD/Counter/2.png";
	m[TEXTURE_tres]							= "./../assets/textures/HUD/Counter/3.png";
	m[TEXTURE_cero]							= "./../assets/textures/HUD/Counter/0.png";
	m[TEXTURE_TENUM_TAXES]					= "./../assets/textures/HUD/Traps/TENUM_TAXES.png";
	m[TEXTURE_TENUM_DISTURBANCE]			= "./../assets/textures/HUD/Traps/TENUM_DISTURBANCE.png";
	m[TEXTURE_TENUM_SILENCE]				= "./../assets/textures/HUD/Traps/TENUM_SILENCE.png";
	m[TEXTURE_TENUM_DEATH_CLAWS]			= "./../assets/textures/HUD/Traps/TENUM_DEATH_CLAWS.png";
	m[TEXTURE_TENUM_EXPLOSIVE]				= "./../assets/textures/HUD/Traps/TENUM_EXPLOSIVE.png";
	m[TEXTURE_TENUM_SPIRITS]				= "./../assets/textures/HUD/Traps/TENUM_SPIRITS.png";
	m[TEXTURE_POT_FIRE_HUD]					= "./../assets/textures/HUD/Potions/POT_FIRE.png";
	m[TEXTURE_POT_SHIELD_HUD]				= "./../assets/textures/HUD/Potions/POT_SHIELD.png";
	m[TEXTURE_POT_MP_HUD]					= "./../assets/textures/HUD/Potions/POT_MP.png";
	m[TEXTURE_POT_ICE_HUD]					= "./../assets/textures/HUD/Potions/POT_ICE.png";
	m[TEXTURE_POT_POISON_HUD]				= "./../assets/textures/HUD/Potions/POT_POISON.png";
	m[TEXTURE_POT_THUNDER_HUD]				= "./../assets/textures/HUD/Potions/POT_THUNDER.png";
	m[TEXTURE_POT_HP_HUD]					= "./../assets/textures/HUD/Potions/POT_HP.png";
	m[TEXTURE_SPELL_SPEED_HUD]				= "./../assets/textures/HUD/Spells/SPELL_SPEED.png";
	m[TEXTURE_SPELL_DUMMY_HUD]				= "./../assets/textures/HUD/Spells/SPELL_DUMMY.png";
	m[TEXTURE_SPELL_CLEANSE_HUD]			= "./../assets/textures/HUD/Spells/SPELL_CLEANSE.png";
	m[TEXTURE_SPELL_PROJECTILE_WIZARD_HUD]	= "./../assets/textures/HUD/Spells/SPELL_PROJECTILE_WIZARD.png";
	m[TEXTURE_SPELL_BLIZZARD_HUD]			= "./../assets/textures/HUD/Spells/SPELL_BLIZZARD.png";
	m[TEXTURE_SPELL_WALL_HUD]				= "./../assets/textures/HUD/Spells/SPELL_WALL.png";
	m[TEXTURE_SPELL_PROJECTILE_WARLOCK_HUD]	= "./../assets/textures/HUD/Spells/SPELL_PROJECTILE_WARLOCK.png";
	m[TEXTURE_SPELL_TELEPORTBASE_HUD]		= "./../assets/textures/HUD/Spells/SPELL_TELEPORTBASE.png";
	m[TEXTURE_SPELL_THUNDER_HUD]			= "./../assets/textures/HUD/Spells/SPELL_THUNDER.png";
	m[TEXTURE_SPELL_FIRE_HUD]				= "./../assets/textures/HUD/Spells/SPELL_FIRE.png";
	m[TEXTURE_SPELL_UNTARGET_HUD]			= "./../assets/textures/HUD/Spells/SPELL_UNTARGET.png";
	m[TEXTURE_SPELL_POISON_HUD]				= "./../assets/textures/HUD/Spells/SPELL_POISON.png";
	m[TEXTURE_SPELL_DEFENSE_HUD]			= "./../assets/textures/HUD/Spells/SPELL_DEFENSE.png";
	m[TEXTURE_SPELL_TELEPORT_HUD]			= "./../assets/textures/HUD/Spells/SPELL_TELEPORT.png";
	m[TEXTURE_SPELL_INVISIBILITY_HUD]		= "./../assets/textures/HUD/Spells/SPELL_INVISIBILITY.png";
	m[TEXTURE_BLOOD]						= "./../assets/textures/overlays/BLOOD.png";
	m[TEXTURE_FUZZY]						= "./../assets/textures/overlays/FUZZY.png";
	m[TEXTURE_HITLANDED]					= "./../assets/textures/overlays/HITLANDED.png";
	m[TEXTURE_decal]						= "./../assets/textures/decal.png";
	m[TEXTURE_skydome]						= "./../assets/textures/skymap/skydome.jpg";
	m[TEXTURE_SPELL_WALL]					= "./../assets/textures/projectils/SPELL_WALL.png";
	m[TEXTURE_SPELL_THUNDER]				= "./../assets/textures/projectils/SPELL_THUNDER.png";
	m[TEXTURE_SPELL_FIRE]					= "./../assets/textures/projectils/SPELL_FIRE.png";
	m[TEXTURE_SPELL_POISON]					= "./../assets/textures/projectils/SPELL_POISON.png";
	m[TEXTURE_SPELL_PROJECTILE]				= "./../assets/textures/projectils/SPELL_PROJECTILE.png";
	m[TEXTURE_gold]							= "./../assets/textures/gold.jpg";
	m[TEXTURE_grass]						= "./../assets/textures/grass.jpg";
	m[TEXTURE_bookcase]						= "./../assets/textures/bookcase.png";
	m[TEXTURE_white_floor]					= "./../assets/textures/white_floor.jpg";
	m[TEXTURE_gardenwall]					= "./../assets/textures/gardenwall.jpg";
	m[TEXTURE_rock]							= "./../assets/textures/rock.jpg";
	m[TEXTURE_none]							= "./../assets/textures/none.png";
	m[TEXTURE_Switch]						= "./../assets/textures/switch.png";
	m[TEXTURE_2897]							= "./../assets/textures/2897.jpg";
	m[TEXTURE_doorblocked]					= "./../assets/textures/doorblocked.jpg";
	m[TEXTURE_Wizard]						= "./../assets/textures/Wizard.png";
	m[TEXTURE_marbre5]						= "./../assets/textures/marbre5.jpg";
	m[TEXTURE_negro]						= "./../assets/textures/negro.jpg";
	m[TEXTURE_yellow]						= "./../assets/textures/yellow.jpg";
	m[TEXTURE_madera]						= "./../assets/textures/madera.jpg";
	m[TEXTURE_BUTTON]						= "./../assets/textures/GUI/Menus/Common/button.png";
	m[TEXTURE_BUTTON_HOVER]					= "./../assets/textures/GUI/Menus/Common/button_hover.png";
	m[TEXTURE_BUTTON_PRESSED]				= "./../assets/textures/GUI/Menus/Common/button_pressed.png";
	m[TEXTURE_SHOP_BACKGROUND]				= "./../assets/textures/GUI/Menus/ShopMenu/shop_background.png";
	m[TEXTURE_SHOP_SLOT]					= "./../assets/textures/GUI/Menus/ShopMenu/slot.png";
	m[TEXTURE_ALLIANCE_BACKGROUND]			= "./../assets/textures/GUI/Menus/AllianceMenu/alliance_background.png";
	m[TEXTURE_BOOK_BACKGROUND]				= "./../assets/textures/GUI/Menus/MainMenu/book_background.png";
	m[TEXTURE_GUI_CURSOR]					= "./../assets/textures/GUI/cursor.png";
	m[TEXTURE_GUI_CURSOR_GLOW]				= "./../assets/textures/GUI/cursor_glow.png";
	m[TEXTURE_WARLOCK_ALLIANCE]				= "./../assets/textures/GUI/Menus/AllianceMenu/warlock_alliance.png";
	m[TEXTURE_WIZARD_ALLIANCE]				= "./../assets/textures/GUI/Menus/AllianceMenu/wizard_alliance.png";
	m[TEXTURE_WARLOCK_ALLIANCE_HOVER]		= "./../assets/textures/GUI/Menus/AllianceMenu/warlock_alliance_hover.png";
	m[TEXTURE_WIZARD_ALLIANCE_HOVER]		= "./../assets/textures/GUI/Menus/AllianceMenu/wizard_alliance_hover.png";
	m[TEXTURE_OSPELLS_BANNER]				= "./../assets/textures/GUI/Menus/ShopMenu/ofensive_spells_banner.png";
	m[TEXTURE_DSPELLS_BANNER]				= "./../assets/textures/GUI/Menus/ShopMenu/defensive_spells_banner.png";
	m[TEXTURE_TSPELLS_BANNER]				= "./../assets/textures/GUI/Menus/ShopMenu/tactic_spells_banner.png";
	m[TEXTURE_TRAPS_BANNER]					= "./../assets/textures/GUI/Menus/ShopMenu/traps_banner.png";
	m[TEXTURE_MENU_TITLE]					= "./../assets/textures/GUI/Menus/MainMenu/titulo.png";
	m[TEXTURE_ORB_BACK]						= "./../assets/textures/HUD/Orb/orb_back.png";
	m[TEXTURE_ORB_FRONT]					= "./../assets/textures/HUD/Orb/orb_front.png";
	m[TEXTURE_ORB_FILL]						= "./../assets/textures/HUD/Orb/orb_fill.png";
	m[TEXTURE_ORB_SCROLL_FILL]				= "./../assets/textures/HUD/Orb/orb_scroll_fill.png";
	m[TEXTURE_ORB_SCROLL_LIP]				= "./../assets/textures/HUD/Orb/orb_scroll_lip.png";
	m[TEXTURE_ORB_SCROLL_FILL_MASK]			= "./../assets/textures/HUD/Orb/orb_scroll_fill_mask.png";
	m[TEXTURE_SPELL_SLOT]					= "./../assets/textures/HUD/Orb/spell_slot.png";
	m[TEXTURE_ITEM_SLOT]					= "./../assets/textures/HUD/Orb/item_slot.png";
	m[TEXTURE_ITEM_SLOT_MASK]				= "./../assets/textures/HUD/Orb/item_slot_mask.png";
	return m;
}

static std::map<GAMEMESHES, std::string> CREATE_MESHMAP(){
	std::map<GAMEMESHES, std::string> m;
	
	m[MESH_grail]		= "./../assets/modelos/grail.obj";
	m[MESH_WarlockArm]	= "./../assets/modelos/WarlockArm.obj";
	m[MESH_Wizard]		= "./../assets/modelos/Wizard.obj";
	m[MESH_door]		= "./../assets/modelos/door.obj";
	m[MESH_fountain]	= "./../assets/modelos/fountain.obj";
	m[MESH_button]		= "./../assets/modelos/button.obj";
	m[MESH_WizardArm]	= "./../assets/modelos/WizardArm.obj";
	m[MESH_potion]		= "./../assets/modelos/potion.obj";
	m[MESH_bomb]		= "./../assets/modelos/bomb.obj";
	m[MESH_Warlock]		= "./../assets/modelos/Warlock.obj";
	m[MESH_npc]			= "./../assets/modelos/npc.obj";
	m[MESH_bookcase]	= "./../assets/modelos/bookcase.obj";

	return m;
}

static std::vector<std::string> CREATE_ANIMATIONMAP(){
	std::vector<std::string> m;

	// INSERT ALL ANIMATIONS
	m.insert( m.end(), AWIZARD_TSHOOT1.begin(), AWIZARD_TSHOOT1.end() );
    m.insert( m.end(), AWIZARD_TSHOOT2.begin(), AWIZARD_TSHOOT2.end() );
    m.insert( m.end(), AWIZARD_TSHOOT3.begin(), AWIZARD_TSHOOT3.end() );
    m.insert( m.end(), AWIZARD_TDRINK.begin(), AWIZARD_TDRINK.end() );
    m.insert( m.end(), AWIZARD_TINTERACT.begin(), AWIZARD_TINTERACT.end() );
	
    m.insert( m.end(), AWIZARD_TIDLE.begin(), AWIZARD_TIDLE.end() );
    m.insert( m.end(), AWIZARD_BIDLE.begin(), AWIZARD_BIDLE.end() );
    m.insert( m.end(), AWIZARD_TWALK.begin(), AWIZARD_TWALK.end() );
    m.insert( m.end(), AWIZARD_BWALK.begin(), AWIZARD_BWALK.end() );
    m.insert( m.end(), AWIZARD_TRUN.begin(), AWIZARD_TRUN.end() );
    m.insert( m.end(), AWIZARD_BRUN.begin(), AWIZARD_BRUN.end() );

    m.insert( m.end(), AWIZARD_ARMWALK.begin(), AWIZARD_ARMWALK.end() );
    m.insert( m.end(), AWIZARD_ARMIDLE.begin(), AWIZARD_ARMIDLE.end() );
    m.insert( m.end(), AWIZARD_ARMSHOOT1.begin(), AWIZARD_ARMSHOOT1.end() );

	return m;
}

static std::map<GAMETEXTURES, std::string> TEXTUREMAP = CREATE_TEXTUREMAP();
static std::map<GAMEMESHES, std::string> MESHMAP = CREATE_MESHMAP();
static std::vector<std::string> ANIMATIONMAP = CREATE_ANIMATIONMAP();

#endif