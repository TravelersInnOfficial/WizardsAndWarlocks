#ifndef PLAYERHUD_H
#define PLAYERHUD_H

#include <string>

//FAST FORWARD DECLARATIONS
class GraphicEngine;
class Player;
class Potion;
class GSprite;
class GRect;
//-------------------------

class PlayerHUD{
public:
    PlayerHUD(Player* p);
    ~PlayerHUD();
    void InitHUD();
    void Draw();
    void Erase();

private:
    
	struct HUD_Orb{
		GSprite* bkg;
		GSprite* front;
		GSprite* fill;
		GSprite* scroll_fill;
		GSprite* scroll_lip;
        float xPos;
        float yPos;
        float width;
        float height;

		HUD_Orb();
        ~HUD_Orb();
		void SetHeight(float v);
		void Update(float vel);
        void SetColor(int alliance);
	};

    struct ItemSlot{
        GSprite* bkg;
        GSprite* item;
        float xPos;
        float yPos;
        float width;
        float height;

        ItemSlot();
        ~ItemSlot();
        void AddItem(std::string path);
        void RemoveItem();
    };

    Player* m_player;
    int p_alliance;
    Potion* p_potion;
    GraphicEngine* g_engine;
    HUD_Orb* health_orb;
	HUD_Orb* mana_orb;
    GRect* stamina_bar;
    GSprite* spell_slot;
    ItemSlot* potion_slot;
    ItemSlot* trap_slot;
    ItemSlot* trap_usings_slot;

	float m_orb_height;
    float m_stamina_bar_width;
    float m_spell_size;
    float m_spell_space;
    int m_num_spells;
    int m_trap_usings;
    float m_stamina_xPos;
    float m_stamina_yPos;

    void p_initPlayerOrbs();
    void p_initStaminaBar();
    void p_initPlayerSpellSelector();
    void p_initPlayerPotion();
    void p_initPlayerTrap();

    void p_drawPlayerOrbs() const;
    void p_drawStaminaBar();
    void p_drawPlayerSpellSelector() const;
    void p_drawPlayerPotion();
    void p_drawPlayerTrap();

    void p_erasePlayerOrbs();
    void p_eraseStaminaBar();
    void p_erasePlayerSpellSelector();
    void p_erasePlayerPotion();
    void p_erasePlayerTrap();

};
#  endif