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
	TEXTURE_HITLANDED,
	TEXTURE_decal,
	TEXTURE_SPELL_WALL,
	TEXTURE_SHOP_BACKGROUND,
	TEXTURE_ALLIANCE_BACKGROUND,
	TEXTURE_BUTTON,
	TEXTURE_BUTTON_HOVER,
	TEXTURE_BUTTON_PRESSED,
	TEXTURE_GUI_CURSOR,
	TEXTURE_BLACK_BKG_FONT,
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
	TEXTURE_NPC_TARANTULA,
	TEXTURE_MATCH_DEFEAT,
	TEXTURE_MATCH_VICTORY,
	TEXTURE_LOADING_BAR_BKG,
	TEXTURE_LOADING_BAR_MASK,
	TEXTURE_LOADING_SCREEN_WIZARD,
	TEXTURE_LOADING_SCREEN_WARLOCK,
	TEXTURE_WARLOCK_ICON,
	TEXTURE_WIZARD_ICON,
	TEXTURE_default_skydome,
	TEXTURE_normal_mainwall,
	TEXTURE_specular_mainwall,
	TEXTURE_mainwall,
	TEXTURE_invisible_texture,
	TEXTURE_normal_stone_floor,
	TEXTURE_specular_stone_floor,
	TEXTURE_stone_floor,
	TEXTURE_normal_ceilling,
	TEXTURE_specular_ceilling,
	TEXTURE_ceilling,
	TEXTURE_torch,
	TEXTURE_book3,
	TEXTURE_book1,
	TEXTURE_default_particle,
	TEXTURE_NeutralParticle,
	TEXTURE_default_font,
	TEXTURE_mask,
	TEXTURE_FireParticle,
	TEXTURE_DefenseParticles,
	TEXTURE_normal_wood_floor,
	TEXTURE_specular_wood_floor,
	TEXTURE_wood_floor,
	TEXTURE_normal_grass_floor,
	TEXTURE_specular_grass_floor,
	TEXTURE_grass_floor,
	TEXTURE_open_book4,
	TEXTURE_open_book2,
	TEXTURE_shelf,
	TEXTURE_open_book3,
	TEXTURE_book2,
	TEXTURE_open_book1,
	TEXTURE_book4,

	TEXTURE_ConfusedParticle,
	TEXTURE_DefenseParticles2,
	TEXTURE_ElectricParticle,
	TEXTURE_FreezeParticle,
	TEXTURE_InvisibleParticle,
	TEXTURE_PoisonParticle,
	TEXTURE_SilencedParticles,
	TEXTURE_SnareParticle,
	TEXTURE_TrapParticle,

	TEXTURE_MAINMENU_BKG_0,
	TEXTURE_MAINMENU_BKG_1,
	TEXTURE_MAINMENU_BKG_2,
	TEXTURE_MAINMENU_BKG_3,
	TEXTURE_MAINMENU_BKG_4,
	TEXTURE_MAINMENU_BKG_5,
	TEXTURE_MAINMENU_BKG_6,
	TEXTURE_MAINMENU_BKG_7,
	TEXTURE_MAINMENU_BKG_8,
	TEXTURE_MAINMENU_BKG_9,
	TEXTURE_MAINMENU_BKG_10,
	TEXTURE_MAINMENU_BKG_11,
	TEXTURE_MAINMENU_BKG_12,
	TEXTURE_MAINMENU_BKG_13,
	TEXTURE_MAINMENU_BKG_14,
	TEXTURE_MAINMENU_BKG_15,
	TEXTURE_MAINMENU_BKG_16,
	TEXTURE_MAINMENU_BKG_17,
	TEXTURE_MAINMENU_BKG_18,
	TEXTURE_MAINMENU_BKG_19,
	TEXTURE_MAINMENU_BKG_20,
	TEXTURE_MAINMENU_BKG_21,
	TEXTURE_MAINMENU_BKG_22,
	TEXTURE_MAINMENU_BKG_23,
	TEXTURE_MAINMENU_BKG_24,
	TEXTURE_MAINMENU_BKG_25,
	TEXTURE_MAINMENU_BKG_26,
	TEXTURE_MAINMENU_BKG_27,
	TEXTURE_MAINMENU_BKG_28,
	TEXTURE_MAINMENU_BKG_29,
	TEXTURE_MAINMENU_BKG_30,
	TEXTURE_MAINMENU_BKG_31,
	TEXTURE_MAINMENU_BKG_32,
	TEXTURE_MAINMENU_BKG_33,
	TEXTURE_MAINMENU_BKG_34,
	TEXTURE_MAINMENU_BKG_35,
	TEXTURE_MAINMENU_BKG_36,
	TEXTURE_MAINMENU_BKG_37,
	TEXTURE_MAINMENU_BKG_38,
	TEXTURE_MAINMENU_BKG_39,
	TEXTURE_MAINMENU_BKG_40,
	TEXTURE_MAINMENU_BKG_41,
	TEXTURE_MAINMENU_BKG_42,
	TEXTURE_MAINMENU_BKG_43,
	TEXTURE_MAINMENU_BKG_44,
	TEXTURE_MAINMENU_BKG_45,
	TEXTURE_MAINMENU_BKG_46,
	TEXTURE_MAINMENU_BKG_47,
	TEXTURE_MAINMENU_BKG_48,
	TEXTURE_MAINMENU_BKG_49,
	TEXTURE_MAINMENU_BKG_50,
	TEXTURE_MAINMENU_BKG_51,
	TEXTURE_MAINMENU_BKG_52,
	TEXTURE_MAINMENU_BKG_53,
	TEXTURE_MAINMENU_BKG_54,
	TEXTURE_MAINMENU_BKG_55,
	TEXTURE_MAINMENU_BKG_56,
	TEXTURE_MAINMENU_BKG_57,
	TEXTURE_MAINMENU_BKG_58,
	TEXTURE_MAINMENU_BKG_59,
	TEXTURE_MAINMENU_BKG_60,
	TEXTURE_MAINMENU_BKG_61,
	TEXTURE_MAINMENU_BKG_62,
	TEXTURE_MAINMENU_BKG_63,
	TEXTURE_MAINMENU_BKG_64,
	TEXTURE_MAINMENU_BKG_65,
	TEXTURE_MAINMENU_BKG_66,
	TEXTURE_MAINMENU_BKG_67,
	TEXTURE_MAINMENU_BKG_68,
	TEXTURE_MAINMENU_BKG_69,
	TEXTURE_MAINMENU_BKG_70,
	TEXTURE_MAINMENU_BKG_71,
	TEXTURE_MAINMENU_BKG_72,
	TEXTURE_MAINMENU_BKG_73,
	TEXTURE_MAINMENU_BKG_74,
	TEXTURE_MAINMENU_BKG_75,
	TEXTURE_MAINMENU_BKG_76,
	TEXTURE_MAINMENU_BKG_77,
	TEXTURE_MAINMENU_BKG_78,
	TEXTURE_MAINMENU_BKG_79,
	TEXTURE_MAINMENU_BKG_80,
	TEXTURE_MAINMENU_BKG_81,
	TEXTURE_MAINMENU_BKG_82,
	TEXTURE_MAINMENU_BKG_83,
	TEXTURE_MAINMENU_BKG_84,
	TEXTURE_MAINMENU_BKG_85,
	TEXTURE_MAINMENU_BKG_86,
	TEXTURE_MAINMENU_BKG_87,
	TEXTURE_MAINMENU_BKG_88,
	TEXTURE_MAINMENU_BKG_89,
	TEXTURE_MAINMENU_BKG_90
};

enum GAMEMESHES {
	MESH_banner,
	MESH_barrel,
	MESH_bath,
	MESH_bed,
	MESH_bench,
	MESH_book,
	MESH_book_table,
	MESH_bottle,
	MESH_candle,
	MESH_casserole,
	MESH_chair,
	MESH_chimney,
	MESH_cup,
	MESH_death_fountain,
	MESH_dish,
	MESH_door2,
	MESH_door_frame,
	MESH_double_door,
	MESH_electricball,
	MESH_fireball,
	MESH_fire,
	MESH_girder,
	MESH_grail,
	MESH_hand_candle,
	MESH_handcuffs,
	MESH_hp,
	MESH_ice,
	MESH_lamp,
	MESH_mp,
	MESH_open_book,
	MESH_poison,
	MESH_pot_ice,
	MESH_potion_hex,
	MESH_potion_squad,
	MESH_potion_tri,
	MESH_regular,
	MESH_restore_fountain,
	MESH_shelf,
	MESH_shelving01,
	MESH_shelving02,
	MESH_shelving03,
	MESH_shelving04,
	MESH_shelving_k,
	MESH_shield,
	MESH_skull_painting,
	MESH_smoking_pipe,
	MESH_table02,
	MESH_table,
	MESH_thunder,
	MESH_torch,
	MESH_water_fountain,
	MESH_wc,
	MESH_bomb,

	MESH_WAR_body,
	MESH_WAR_head,
	MESH_WAR_leftArm,
	MESH_WAR_leftLeg,
	MESH_WAR_ornament1,
	MESH_WAR_ornament2,
	MESH_WAR_rightArm,
	MESH_WAR_rightLeg,
	MESH_WAR_staff,

	MESH_WIZ_body,
	MESH_WIZ_head,
	MESH_WIZ_leftArm,
	MESH_WIZ_leftLeg,
	MESH_WIZ_ornament1,
	MESH_WIZ_ornament2,
	MESH_WIZ_rightArm,
	MESH_WIZ_rightLeg,
	MESH_WIZ_staff
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
	m[TEXTURE_HITLANDED]					= "./../assets/textures/overlays/HITLANDED.png";
	m[TEXTURE_decal]						= "./../assets/textures/decal.png";
	m[TEXTURE_SPELL_WALL]					= "./../assets/textures/SPELL_WALL.png";
	m[TEXTURE_BUTTON]						= "./../assets/textures/GUI/Menus/Common/button.png";
	m[TEXTURE_BUTTON_HOVER]					= "./../assets/textures/GUI/Menus/Common/button_hover.png";
	m[TEXTURE_BUTTON_PRESSED]				= "./../assets/textures/GUI/Menus/Common/button_pressed.png";
	m[TEXTURE_SHOP_BACKGROUND]				= "./../assets/textures/GUI/Menus/ShopMenu/shop_background.png";
	m[TEXTURE_SHOP_SLOT]					= "./../assets/textures/GUI/Menus/ShopMenu/slot.png";
	m[TEXTURE_ALLIANCE_BACKGROUND]			= "./../assets/textures/GUI/Menus/AllianceMenu/alliance_background.png";
	m[TEXTURE_GUI_CURSOR]					= "./../assets/textures/GUI/cursor.png";
	m[TEXTURE_BLACK_BKG_FONT]				= "./../assets/textures/GUI/black_bkg_font.png";
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
	m[TEXTURE_MATCH_DEFEAT]					= "./../assets/textures/GUI/Menus/EndMatchMenu/defeat.png";
	m[TEXTURE_MATCH_VICTORY]				= "./../assets/textures/GUI/Menus/EndMatchMenu/victory.png";
	m[TEXTURE_LOADING_BAR_BKG] 				= "./../assets/textures/GUI/Menus/LoadingScreen/loading_bar_bkg.png";
	m[TEXTURE_LOADING_BAR_MASK] 			= "./../assets/textures/GUI/Menus/LoadingScreen/loading_bar_mask.png";
	m[TEXTURE_LOADING_SCREEN_WIZARD] 		= "./../assets/textures/GUI/Menus/LoadingScreen/bkg_wizard.png";
	m[TEXTURE_LOADING_SCREEN_WARLOCK] 		= "./../assets/textures/GUI/Menus/LoadingScreen/bkg_warlock.png";
	m[TEXTURE_WARLOCK_ICON]				= "./../assets/textures/GUI/warlockalliance.png";
	m[TEXTURE_WIZARD_ICON]				= "./../assets/textures/GUI/wizardalliance.png";

	m[TEXTURE_default_skydome] = "../src/Engines/TravelersOcularEngine/assets/textures/default_skydome.jpg";
	m[TEXTURE_normal_mainwall] = "../assets/textures/maptextures/normal_mainwall 1.png";
	m[TEXTURE_specular_mainwall] = "../assets/textures/maptextures/specular_mainwall 1.png";
	m[TEXTURE_mainwall] = "../assets/textures/maptextures/mainwall 1.png";
	m[TEXTURE_invisible_texture] = "../src/Engines/TravelersOcularEngine/assets/textures/invisible_texture.png";
	m[TEXTURE_normal_stone_floor] = "../assets/textures/maptextures/normal_stone_floor.png";
	m[TEXTURE_specular_stone_floor] = "../assets/textures/maptextures/specular_stone_floor.png";
	m[TEXTURE_stone_floor] = "../assets/textures/maptextures/stone_floor.png";
	m[TEXTURE_normal_ceilling] = "../assets/textures/maptextures/normal_ceilling.png";
	m[TEXTURE_specular_ceilling] = "../assets/textures/maptextures/specular_ceilling.png";
	m[TEXTURE_ceilling] = "../assets/textures/maptextures/ceilling.png";
	m[TEXTURE_torch] = "../assets/textures/torch.png";
	m[TEXTURE_book3] = "../assets/textures/book3.png";
	m[TEXTURE_book1] = "../assets/textures/book1.png";
	m[TEXTURE_default_font] = "../src/Engines/TravelersOcularEngine/assets/textures/default_font.png";
	m[TEXTURE_mask] = "../assets/textures/HUD/Minimap/mask.jpg";
	m[TEXTURE_normal_wood_floor] = "../assets/textures/maptextures/normal_wood_floor.png";
	m[TEXTURE_specular_wood_floor] = "../assets/textures/maptextures/specular_wood_floor.png";
	m[TEXTURE_wood_floor] = "../assets/textures/maptextures/wood_floor.png";
	m[TEXTURE_normal_grass_floor] = "../assets/textures/maptextures/normal_grass_floor.png";
	m[TEXTURE_specular_grass_floor] = "../assets/textures/maptextures/specular_grass_floor.png";
	m[TEXTURE_grass_floor] = "../assets/textures/maptextures/grass_floor.png";
	m[TEXTURE_open_book4] = "../assets/textures/open_book4.png";
	m[TEXTURE_open_book2] = "../assets/textures/open_book2.png";
	m[TEXTURE_shelf] = "../assets/textures/shelf.png";
	m[TEXTURE_open_book3] = "../assets/textures/open_book3.png";
	m[TEXTURE_book2] = "../assets/textures/book2.png";
	m[TEXTURE_open_book1] = "../assets/textures/open_book1.png";
	m[TEXTURE_book4] = "../assets/textures/book4.png";

	m[TEXTURE_default_particle] = "../src/Engines/TravelersOcularEngine/assets/textures/default_particle.png";
	m[TEXTURE_NeutralParticle] = "../assets/textures/particles/NeutralParticle.png";
	m[TEXTURE_FireParticle] = "../assets/textures/particles/FireParticle.png";
	m[TEXTURE_DefenseParticles] = "../assets/textures/particles/DefenseParticles.png";
	m[TEXTURE_ConfusedParticle] = "../assets/textures/particles/ConfusedParticle.png";
	m[TEXTURE_DefenseParticles2] = "../assets/textures/particles/DefenseParticles2.png";
	m[TEXTURE_ElectricParticle] = "../assets/textures/particles/ElectricParticle.png";
	m[TEXTURE_FreezeParticle] = "../assets/textures/particles/FreezeParticle.png";
	m[TEXTURE_InvisibleParticle] = "../assets/textures/particles/InvisibleParticle.png";
	m[TEXTURE_PoisonParticle] = "../assets/textures/particles/PoisonParticle.png";
	m[TEXTURE_SilencedParticles] = "../assets/textures/particles/SilencedParticles.png";
	m[TEXTURE_SnareParticle] = "../assets/textures/particles/SnareParticle.png";
	m[TEXTURE_TrapParticle] = "../assets/textures/particles/TrapParticle.png";

	m[TEXTURE_MAINMENU_BKG_0]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_0.jpg";
	m[TEXTURE_MAINMENU_BKG_1]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_1.jpg";
	m[TEXTURE_MAINMENU_BKG_2]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_2.jpg";
	m[TEXTURE_MAINMENU_BKG_3]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_3.jpg";
	m[TEXTURE_MAINMENU_BKG_4]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_4.jpg";
	m[TEXTURE_MAINMENU_BKG_5]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_5.jpg";
	m[TEXTURE_MAINMENU_BKG_6]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_6.jpg";
	m[TEXTURE_MAINMENU_BKG_7]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_7.jpg";
	m[TEXTURE_MAINMENU_BKG_8]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_8.jpg";
	m[TEXTURE_MAINMENU_BKG_9]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_9.jpg";
	m[TEXTURE_MAINMENU_BKG_10]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_10.jpg";
	m[TEXTURE_MAINMENU_BKG_11]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_11.jpg";
	m[TEXTURE_MAINMENU_BKG_12]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_12.jpg";
	m[TEXTURE_MAINMENU_BKG_13]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_13.jpg";
	m[TEXTURE_MAINMENU_BKG_14]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_14.jpg";
	m[TEXTURE_MAINMENU_BKG_15]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_15.jpg";
	m[TEXTURE_MAINMENU_BKG_16]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_16.jpg";
	m[TEXTURE_MAINMENU_BKG_17]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_17.jpg";
	m[TEXTURE_MAINMENU_BKG_18]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_18.jpg";
	m[TEXTURE_MAINMENU_BKG_19]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_19.jpg";
	m[TEXTURE_MAINMENU_BKG_20]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_20.jpg";
	m[TEXTURE_MAINMENU_BKG_21]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_21.jpg";
	m[TEXTURE_MAINMENU_BKG_22]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_22.jpg";
	m[TEXTURE_MAINMENU_BKG_23]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_23.jpg";
	m[TEXTURE_MAINMENU_BKG_24]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_24.jpg";
	m[TEXTURE_MAINMENU_BKG_25]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_25.jpg";
	m[TEXTURE_MAINMENU_BKG_26]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_26.jpg";
	m[TEXTURE_MAINMENU_BKG_27]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_27.jpg";
	m[TEXTURE_MAINMENU_BKG_28]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_28.jpg";
	m[TEXTURE_MAINMENU_BKG_29]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_29.jpg";
	m[TEXTURE_MAINMENU_BKG_30]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_30.jpg";
	m[TEXTURE_MAINMENU_BKG_31]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_31.jpg";
	m[TEXTURE_MAINMENU_BKG_32]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_32.jpg";
	m[TEXTURE_MAINMENU_BKG_33]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_33.jpg";
	m[TEXTURE_MAINMENU_BKG_34]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_34.jpg";
	m[TEXTURE_MAINMENU_BKG_35]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_35.jpg";
	m[TEXTURE_MAINMENU_BKG_36]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_36.jpg";
	m[TEXTURE_MAINMENU_BKG_37]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_37.jpg";
	m[TEXTURE_MAINMENU_BKG_38]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_38.jpg";
	m[TEXTURE_MAINMENU_BKG_39]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_39.jpg";
	m[TEXTURE_MAINMENU_BKG_40]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_40.jpg";
	m[TEXTURE_MAINMENU_BKG_41]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_41.jpg";
	m[TEXTURE_MAINMENU_BKG_42]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_42.jpg";
	m[TEXTURE_MAINMENU_BKG_43]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_43.jpg";
	m[TEXTURE_MAINMENU_BKG_44]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_44.jpg";
	m[TEXTURE_MAINMENU_BKG_45]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_45.jpg";
	m[TEXTURE_MAINMENU_BKG_46]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_46.jpg";
	m[TEXTURE_MAINMENU_BKG_47]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_47.jpg";
	m[TEXTURE_MAINMENU_BKG_48]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_48.jpg";
	m[TEXTURE_MAINMENU_BKG_49]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_49.jpg";
	m[TEXTURE_MAINMENU_BKG_50]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_50.jpg";
	m[TEXTURE_MAINMENU_BKG_51]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_51.jpg";
	m[TEXTURE_MAINMENU_BKG_52]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_52.jpg";
	m[TEXTURE_MAINMENU_BKG_53]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_53.jpg";
	m[TEXTURE_MAINMENU_BKG_54]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_54.jpg";
	m[TEXTURE_MAINMENU_BKG_55]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_55.jpg";
	m[TEXTURE_MAINMENU_BKG_56]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_56.jpg";
	m[TEXTURE_MAINMENU_BKG_57]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_57.jpg";
	m[TEXTURE_MAINMENU_BKG_58]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_58.jpg";
	m[TEXTURE_MAINMENU_BKG_59]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_59.jpg";
	m[TEXTURE_MAINMENU_BKG_60]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_60.jpg";
	m[TEXTURE_MAINMENU_BKG_61]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_61.jpg";
	m[TEXTURE_MAINMENU_BKG_62]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_62.jpg";
	m[TEXTURE_MAINMENU_BKG_63]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_63.jpg";
	m[TEXTURE_MAINMENU_BKG_64]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_64.jpg";
	m[TEXTURE_MAINMENU_BKG_65]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_65.jpg";
	m[TEXTURE_MAINMENU_BKG_66]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_66.jpg";
	m[TEXTURE_MAINMENU_BKG_67]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_67.jpg";
	m[TEXTURE_MAINMENU_BKG_68]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_68.jpg";
	m[TEXTURE_MAINMENU_BKG_69]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_69.jpg";
	m[TEXTURE_MAINMENU_BKG_70]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_70.jpg";
	m[TEXTURE_MAINMENU_BKG_71]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_71.jpg";
	m[TEXTURE_MAINMENU_BKG_72]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_72.jpg";
	m[TEXTURE_MAINMENU_BKG_73]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_73.jpg";
	m[TEXTURE_MAINMENU_BKG_74]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_74.jpg";
	m[TEXTURE_MAINMENU_BKG_75]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_75.jpg";
	m[TEXTURE_MAINMENU_BKG_76]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_76.jpg";
	m[TEXTURE_MAINMENU_BKG_77]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_77.jpg";
	m[TEXTURE_MAINMENU_BKG_78]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_78.jpg";
	m[TEXTURE_MAINMENU_BKG_79]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_79.jpg";
	m[TEXTURE_MAINMENU_BKG_80]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_80.jpg";
	m[TEXTURE_MAINMENU_BKG_81]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_81.jpg";
	m[TEXTURE_MAINMENU_BKG_82]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_82.jpg";
	m[TEXTURE_MAINMENU_BKG_83]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_83.jpg";
	m[TEXTURE_MAINMENU_BKG_84]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_84.jpg";
	m[TEXTURE_MAINMENU_BKG_85]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_85.jpg";
	m[TEXTURE_MAINMENU_BKG_86]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_86.jpg";
	m[TEXTURE_MAINMENU_BKG_87]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_87.jpg";
	m[TEXTURE_MAINMENU_BKG_88]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_88.jpg";
	m[TEXTURE_MAINMENU_BKG_89]= "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_89.jpg";

	return m;
}

static std::map<GAMEMESHES, std::string> CREATE_MESHMAP(){
	std::map<GAMEMESHES, std::string> m;
	
	m[MESH_banner] = "./../assets/modelos/banner.obj";
	m[MESH_barrel] = "./../assets/modelos/barrel.obj";
	m[MESH_bath] = "./../assets/modelos/bath.obj";
	m[MESH_bed] = "./../assets/modelos/bed.obj";
	m[MESH_bench] = "./../assets/modelos/bench.obj";
	m[MESH_book] = "./../assets/modelos/book.obj";
	m[MESH_book_table] = "./../assets/modelos/book_table.obj";
	m[MESH_bottle] = "./../assets/modelos/bottle.obj";
	m[MESH_candle] = "./../assets/modelos/candle.obj";
	m[MESH_casserole] = "./../assets/modelos/casserole.obj";
	m[MESH_chair] = "./../assets/modelos/chair.obj";
	m[MESH_chimney] = "./../assets/modelos/chimney.obj";
	m[MESH_cup] = "./../assets/modelos/cup.obj";
	m[MESH_death_fountain] = "./../assets/modelos/death_fountain.obj";
	m[MESH_dish] = "./../assets/modelos/dish.obj";
	m[MESH_door2] = "./../assets/modelos/door2.obj";
	m[MESH_door_frame] = "./../assets/modelos/door_frame.obj";
	m[MESH_double_door] = "./../assets/modelos/double_door.obj";
	m[MESH_electricball] = "./../assets/modelos/electricball.obj";
	m[MESH_fireball] = "./../assets/modelos/fireball.obj";
	m[MESH_fire] = "./../assets/modelos/fire.obj";
	m[MESH_girder] = "./../assets/modelos/girder.obj";
	m[MESH_grail] = "./../assets/modelos/grail.obj";
	m[MESH_hand_candle] = "./../assets/modelos/hand_candle.obj";
	m[MESH_handcuffs] = "./../assets/modelos/handcuffs.obj";
	m[MESH_hp] = "./../assets/modelos/hp.obj";
	m[MESH_ice] = "./../assets/modelos/ice.obj";
	m[MESH_lamp] = "./../assets/modelos/lamp.obj";
	m[MESH_mp] = "./../assets/modelos/mp.obj";
	m[MESH_open_book] = "./../assets/modelos/open_book.obj";
	m[MESH_poison] = "./../assets/modelos/poison.obj";
	m[MESH_pot_ice] = "./../assets/modelos/pot_ice.obj";
	m[MESH_potion_hex] = "./../assets/modelos/potion_hex.obj";
	m[MESH_potion_squad] = "./../assets/modelos/potion_squad.obj";
	m[MESH_potion_tri] = "./../assets/modelos/potion_tri.obj";
	m[MESH_regular] = "./../assets/modelos/regular.obj";
	m[MESH_restore_fountain] = "./../assets/modelos/restore_fountain.obj";
	m[MESH_shelf] = "./../assets/modelos/shelf.obj";
	m[MESH_shelving01] = "./../assets/modelos/shelving01.obj";
	m[MESH_shelving02] = "./../assets/modelos/shelving02.obj";
	m[MESH_shelving03] = "./../assets/modelos/shelving03.obj";
	m[MESH_shelving04] = "./../assets/modelos/shelving04.obj";
	m[MESH_shelving_k] = "./../assets/modelos/shelving_k.obj";
	m[MESH_shield] = "./../assets/modelos/shield.obj";
	m[MESH_skull_painting] = "./../assets/modelos/skull_painting.obj";
	m[MESH_smoking_pipe] = "./../assets/modelos/smoking_pipe.obj";
	m[MESH_table02] = "./../assets/modelos/table02.obj";
	m[MESH_table] = "./../assets/modelos/table.obj";
	m[MESH_thunder] = "./../assets/modelos/thunder.obj";
	m[MESH_torch] = "./../assets/modelos/torch.obj";
	m[MESH_water_fountain] = "./../assets/modelos/water_fountain.obj";
	m[MESH_wc] = "./../assets/modelos/wc.obj";
	m[MESH_bomb] = "./../assets/modelos/bomb.obj";

	m[MESH_WAR_body] = "./../assets/modelos/PlayerParts/Warlock/body.obj";
	m[MESH_WAR_head] = "./../assets/modelos/PlayerParts/Warlock/head.obj";
	m[MESH_WAR_leftArm] = "./../assets/modelos/PlayerParts/Warlock/leftArm.obj";
	m[MESH_WAR_leftLeg] = "./../assets/modelos/PlayerParts/Warlock/leftLeg.obj";
	m[MESH_WAR_ornament1] = "./../assets/modelos/PlayerParts/Warlock/ornament1.obj";
	m[MESH_WAR_ornament2] = "./../assets/modelos/PlayerParts/Warlock/ornament2.obj";
	m[MESH_WAR_rightArm] = "./../assets/modelos/PlayerParts/Warlock/rightArm.obj";
	m[MESH_WAR_rightLeg] = "./../assets/modelos/PlayerParts/Warlock/rightLeg.obj";
	m[MESH_WAR_staff] = "./../assets/modelos/PlayerParts/Warlock/staff.obj";

	m[MESH_WIZ_body] = "./../assets/modelos/PlayerParts/Wizards/body.obj";
	m[MESH_WIZ_head] = "./../assets/modelos/PlayerParts/Wizards/head.obj";
	m[MESH_WIZ_leftArm] = "./../assets/modelos/PlayerParts/Wizards/leftArm.obj";
	m[MESH_WIZ_leftLeg] = "./../assets/modelos/PlayerParts/Wizards/leftLeg.obj";
	m[MESH_WIZ_ornament1] = "./../assets/modelos/PlayerParts/Wizards/ornament1.obj";
	m[MESH_WIZ_ornament2] = "./../assets/modelos/PlayerParts/Wizards/ornament2.obj";
	m[MESH_WIZ_rightArm] = "./../assets/modelos/PlayerParts/Wizards/rightArm.obj";
	m[MESH_WIZ_rightLeg] = "./../assets/modelos/PlayerParts/Wizards/rightLeg.obj";
	m[MESH_WIZ_staff] = "./../assets/modelos/PlayerParts/Wizards/staff.obj";

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