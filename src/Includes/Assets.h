#ifndef ASSETS_H
#define ASSETS_H

#include <iostream>
#include <vector>
#include <map>
#include "Animation_Strings.h"

enum GAMETEXTURES {
	TEXTURE_none,
	TEXTURE_Wizard,
	TEXTURE_Warlock,
	TEXTURE_game_icon,
	TEXTURE_cero,
	TEXTURE_uno,
	TEXTURE_dos,
	TEXTURE_tres,
	TEXTURE_cuatro,
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
	TEXTURE_SPELL_WALL,
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
	TEXTURE_SPELL_SLOT_BKG,
	TEXTURE_ITEM_SLOT,
	TEXTURE_ITEM_SLOT_MASK,
	TEXTURE_MINIMAP_COMPASS,
	TEXTURE_SPELL_SEP,
	TEXTURE_SPELL_SEP_L,
	TEXTURE_SPELL_SEP_R,
	TEXTURE_STAMINA_BKG,
	TEXTURE_STAMINA_GRID,
	TEXTURE_STAMINA_LIGHT,
	TEXTURE_SPELL_SLOT_MASK,
	TEXTURE_NPC_GENIE,
	TEXTURE_NPC_TARANTULA
};

enum GAMEMESHES {
	MESH_Wizard,
	MESH_WizardArm,
	MESH_Warlock,
	MESH_WarlockArm,
	MESH_grail,
	MESH_door,
	MESH_fountain,
	MESH_button,
	MESH_potion,
	MESH_bomb,
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

	m[TEXTURE_none]							= "./../assets/textures/none.png";
	m[TEXTURE_Warlock]						= "./../assets/textures/Warlock.png";
	m[TEXTURE_Wizard]						= "./../assets/textures/Wizard.png";
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
	m[TEXTURE_SPELL_PROJECTILE_WARLOCK_HUD]	= "./../assets/textures/HUD/Spells/SPELL_PROJECTILE_WARLOCK.png";
	m[TEXTURE_SPELL_BLIZZARD_HUD]			= "./../assets/textures/HUD/Spells/SPELL_BLIZZARD.png";
	m[TEXTURE_SPELL_WALL_HUD]				= "./../assets/textures/HUD/Spells/SPELL_WALL.png";
	m[TEXTURE_SPELL_TELEPORTBASE_HUD]		= "./../assets/textures/HUD/Spells/SPELL_TELEPORTBASE.png";
	m[TEXTURE_SPELL_THUNDER_HUD]			= "./../assets/textures/HUD/Spells/SPELL_THUNDER.png";
	m[TEXTURE_SPELL_FIRE_HUD]				= "./../assets/textures/HUD/Spells/SPELL_FIRE.png";
	m[TEXTURE_SPELL_UNTARGET_HUD]			= "./../assets/textures/HUD/Spells/SPELL_UNTARGET.png";
	m[TEXTURE_SPELL_POISON_HUD]				= "./../assets/textures/HUD/Spells/SPELL_POISON.png";
	m[TEXTURE_SPELL_DEFENSE_HUD]			= "./../assets/textures/HUD/Spells/SPELL_DEFENSE.png";
	m[TEXTURE_SPELL_TELEPORT_HUD]			= "./../assets/textures/HUD/Spells/SPELL_TELEPORT.png";
	m[TEXTURE_SPELL_INVISIBILITY_HUD]		= "./../assets/textures/HUD/Spells/SPELL_INVISIBILITY.png";
	m[TEXTURE_SPELL_SLOT]					= "./../assets/textures/HUD/Spells/spell_slot.png";
	m[TEXTURE_SPELL_SLOT_BKG]				= "./../assets/textures/HUD/Spells/spell_selector_bkg.png";
	m[TEXTURE_SPELL_SLOT_MASK]				= "./../assets/textures/HUD/Spells/spell_mask.png";
	m[TEXTURE_SPELL_SEP]					= "./../assets/textures/HUD/Spells/decorator.png";
	m[TEXTURE_SPELL_SEP_L]					= "./../assets/textures/HUD/Spells/decorator_left.png";
	m[TEXTURE_SPELL_SEP_R]					= "./../assets/textures/HUD/Spells/decorator_right.png";
	m[TEXTURE_BLOOD]						= "./../assets/textures/overlays/BLOOD.png";
	m[TEXTURE_FUZZY]						= "./../assets/textures/overlays/FUZZY.png";
	m[TEXTURE_HITLANDED]					= "./../assets/textures/overlays/HITLANDED.png";
	m[TEXTURE_decal]						= "./../assets/textures/decal.png";
	m[TEXTURE_SPELL_WALL]					= "./../assets/textures/SPELL_WALL.png";
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
	m[TEXTURE_ITEM_SLOT]					= "./../assets/textures/HUD/Orb/item_slot.png";
	m[TEXTURE_ITEM_SLOT_MASK]				= "./../assets/textures/HUD/Orb/item_slot_mask.png";
	m[TEXTURE_MINIMAP_COMPASS]				= "./../assets/textures/HUD/Minimap/compass.png";
	m[TEXTURE_STAMINA_BKG]					= "./../assets/textures/HUD/StaminaBar/bkg.png";
	m[TEXTURE_STAMINA_GRID]					= "./../assets/textures/HUD/StaminaBar/grid.png";
	m[TEXTURE_STAMINA_LIGHT]				= "./../assets/textures/HUD/StaminaBar/highlight.png";
	m[TEXTURE_NPC_GENIE]					= "./../assets/textures/genie.png";
	m[TEXTURE_NPC_TARANTULA]				= "./../assets/textures/tarantula.png";
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

	return m;
}

static std::vector<std::string> CREATE_ANIMATIONMAP(){
	std::vector<std::string> m;

	// INSERT ALL WIZARD ANIMATIONS
	m.insert( m.end(), AWIZARD_ARMIDLE.begin(), 		AWIZARD_ARMIDLE.end() );
    m.insert( m.end(), AWIZARD_BIDLE.begin(), 			AWIZARD_BIDLE.end() );
    m.insert( m.end(), AWIZARD_TIDLE.begin(), 			AWIZARD_TIDLE.end() );

    m.insert( m.end(), AWIZARD_ARMWALK.begin(), 		AWIZARD_ARMWALK.end() );
    m.insert( m.end(), AWIZARD_TWALK.begin(), 			AWIZARD_TWALK.end() );
    m.insert( m.end(), AWIZARD_BWALK.begin(), 			AWIZARD_BWALK.end() );

    m.insert( m.end(), AWIZARD_ARMRUN.begin(), 			AWIZARD_ARMRUN.end() );
    m.insert( m.end(), AWIZARD_TRUN.begin(), 			AWIZARD_TRUN.end() );
    m.insert( m.end(), AWIZARD_BRUN.begin(), 			AWIZARD_BRUN.end() );
	
	// TOP ONLY
	m.insert( m.end(), AWIZARD_ARMSHOOT1.begin(), 		AWIZARD_ARMSHOOT1.end() );
	m.insert( m.end(), AWIZARD_TSHOOT1.begin(), 		AWIZARD_TSHOOT1.end() );

	m.insert( m.end(), AWIZARD_ARMSHOOT2.begin(), 		AWIZARD_ARMSHOOT2.end() );
    m.insert( m.end(), AWIZARD_TSHOOT2.begin(), 		AWIZARD_TSHOOT2.end() );

    m.insert( m.end(), AWIZARD_ARMDRINK.begin(), 		AWIZARD_ARMDRINK.end() );
    m.insert( m.end(), AWIZARD_TDRINK.begin(), 			AWIZARD_TDRINK.end() );

    m.insert( m.end(), AWIZARD_ARMINTERACT.begin(), AWIZARD_ARMINTERACT.end() );
    m.insert( m.end(), AWIZARD_TINTERACT.begin(), 	AWIZARD_TINTERACT.end() );

	/* TOP ANIMATIONS NOT USED YET
		m.insert( m.end(), AWIZARD_ARMSHOOT3.begin(), 	AWIZARD_ARMSHOOT3.end() );
		m.insert( m.end(), AWIZARD_TSHOOT3.begin(), 	AWIZARD_TSHOOT3.end() );
	*/

	// JUMP ANIMATION
    m.insert( m.end(), AWIZARD_TJUMPSTART.begin(), 	AWIZARD_TJUMPSTART.end() );
    m.insert( m.end(), AWIZARD_BJUMPSTART.begin(), 	AWIZARD_BJUMPSTART.end() );
	m.insert( m.end(), AWIZARD_TJUMPFALL.begin(), 	AWIZARD_TJUMPFALL.end() );
    m.insert( m.end(), AWIZARD_BJUMPFALL.begin(), 	AWIZARD_BJUMPFALL.end() );
    m.insert( m.end(), AWIZARD_TJUMPEND.begin(), 	AWIZARD_TJUMPEND.end() );
    m.insert( m.end(), AWIZARD_BJUMPEND.begin(), 	AWIZARD_BJUMPEND.end() );

	//DANCE ANIMATIONS
	m.insert( m.end(), AWIZARD_TCIRCLE.begin(), AWIZARD_TCIRCLE.end() );
    m.insert( m.end(), AWIZARD_BCIRCLE.begin(), AWIZARD_BCIRCLE.end() );
	
    m.insert( m.end(), AWIZARD_TDAB.begin(), 	AWIZARD_TDAB.end() );
    m.insert( m.end(), AWIZARD_BDAB.begin(), 	AWIZARD_BDAB.end() );

    m.insert( m.end(), AWIZARD_ARMDAB.begin(), 			AWIZARD_ARMDAB.end() );	
	m.insert( m.end(), AWIZARD_TDANCE.begin(), 	AWIZARD_TDANCE.end() );
    m.insert( m.end(), AWIZARD_BDANCE.begin(), 	AWIZARD_BDANCE.end() );

    m.insert( m.end(), AWIZARD_TWIN.begin(), 	AWIZARD_TWIN.end() );
    m.insert( m.end(), AWIZARD_BWIN.begin(), 	AWIZARD_BWIN.end() );

	/// INSERT ALL WARLOCK ANIMATIONS
	m.insert( m.end(), AWARLOCK_ARMIDLE.begin(), 	AWARLOCK_ARMIDLE.end() );
    m.insert( m.end(), AWARLOCK_BIDLE.begin(), 		AWARLOCK_BIDLE.end() );
    m.insert( m.end(), AWARLOCK_TIDLE.begin(), 		AWARLOCK_TIDLE.end() );

    m.insert( m.end(), AWARLOCK_ARMWALK.begin(), 	AWARLOCK_ARMWALK.end() );
    m.insert( m.end(), AWARLOCK_TWALK.begin(), 		AWARLOCK_TWALK.end() );
    m.insert( m.end(), AWARLOCK_BWALK.begin(), 		AWARLOCK_BWALK.end() );

    m.insert( m.end(), AWARLOCK_ARMRUN.begin(), 	AWARLOCK_ARMRUN.end() );
    m.insert( m.end(), AWARLOCK_TRUN.begin(), 		AWARLOCK_TRUN.end() );
    m.insert( m.end(), AWARLOCK_BRUN.begin(), 		AWARLOCK_BRUN.end() );

	// TOP ONLY
	m.insert( m.end(), AWARLOCK_ARMSHOOT1.begin(), 		AWARLOCK_ARMSHOOT1.end() );
	m.insert( m.end(), AWARLOCK_TSHOOT1.begin(), 		AWARLOCK_TSHOOT1.end() );

	m.insert( m.end(), AWARLOCK_ARMSHOOT2.begin(), 		AWARLOCK_ARMSHOOT2.end() );
    m.insert( m.end(), AWARLOCK_TSHOOT2.begin(),		AWARLOCK_TSHOOT2.end() );

    m.insert( m.end(), AWARLOCK_ARMDRINK.begin(), 		AWARLOCK_ARMDRINK.end() );
    m.insert( m.end(), AWARLOCK_TDRINK.begin(), 		AWARLOCK_TDRINK.end() );

    m.insert( m.end(), AWARLOCK_ARMINTERACT.begin(), 	AWARLOCK_ARMINTERACT.end() );
    m.insert( m.end(), AWARLOCK_TINTERACT.begin(), 		AWARLOCK_TINTERACT.end() );

	/* TOP ANIMATIONS NOT USED YET
		m.insert( m.end(), AWARLOCK_ARMSHOOT3.begin(), 		AWARLOCK_ARMSHOOT3.end() );
		m.insert( m.end(), AWARLOCK_TSHOOT3.begin(), 		AWARLOCK_TSHOOT3.end() );
	*/	

	// JUMP ANIMATION
	m.insert( m.end(), AWARLOCK_TJUMPSTART.begin(), 	AWARLOCK_TJUMPSTART.end() );
	m.insert( m.end(), AWARLOCK_BJUMPSTART.begin(), 	AWARLOCK_BJUMPSTART.end() );
	m.insert( m.end(), AWARLOCK_TJUMPFALL.begin(), 		AWARLOCK_TJUMPFALL.end() );
	m.insert( m.end(), AWARLOCK_BJUMPFALL.begin(), 		AWARLOCK_BJUMPFALL.end() );
	m.insert( m.end(), AWARLOCK_TJUMPEND.begin(), 		AWARLOCK_TJUMPEND.end() );
	m.insert( m.end(), AWARLOCK_BJUMPEND.begin(), 		AWARLOCK_BJUMPEND.end() );

	//DANCE ANIMATIONS
	m.insert( m.end(), AWARLOCK_TCIRCLE.begin(), 	AWARLOCK_TCIRCLE.end() );
	m.insert( m.end(), AWARLOCK_BCIRCLE.begin(), 	AWARLOCK_BCIRCLE.end() );
	
	m.insert( m.end(), AWARLOCK_TDAB.begin(), 		AWARLOCK_TDAB.end() );
	m.insert( m.end(), AWARLOCK_BDAB.begin(), 		AWARLOCK_BDAB.end() );

	//m.insert( m.end(), AWARLOCK_ARMDAB.begin(), 	AWARLOCK_ARMDAB.end() );	
	m.insert( m.end(), AWARLOCK_TDANCE.begin(), 	AWARLOCK_TDANCE.end() );
	m.insert( m.end(), AWARLOCK_BDANCE.begin(), 	AWARLOCK_BDANCE.end() );

	m.insert( m.end(), AWARLOCK_TWIN.begin(), 		AWARLOCK_TWIN.end() );
	m.insert( m.end(), AWARLOCK_BWIN.begin(), 		AWARLOCK_BWIN.end() );

	// INSERT ALL NPC ANIMATIONS
	m.insert( m.end(), AGENIE_DEFAULT.begin(), 		AGENIE_DEFAULT.end() );
	m.insert( m.end(), ATARANTULA_DEFAULT.begin(), 	ATARANTULA_DEFAULT.end() );

	return m;
}

static std::map<GAMETEXTURES, std::string> TEXTUREMAP = CREATE_TEXTUREMAP();
static std::map<GAMEMESHES, std::string> MESHMAP = CREATE_MESHMAP();
static std::vector<std::string> ANIMATIONMAP = CREATE_ANIMATIONMAP();

#endif