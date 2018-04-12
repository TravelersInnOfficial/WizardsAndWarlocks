#ifndef PLAYERHUD_H
#define PLAYERHUD_H

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
		GSprite* m_orb_front;
		GSprite* m_orb_back;
		GSprite* m_orb_fill;
		GSprite* m_orb_scroll_lip;
		GSprite* m_orb_scroll_fill;

		HUD_Orb();
        ~HUD_Orb();
		void SetHeight(float v);
		void Update(float vel);
        void SetColor(int alliance);
	};

    Player* m_player;
    int p_alliance;
    Potion* p_potion;
    GraphicEngine* g_engine;
    HUD_Orb* health_orb;
	HUD_Orb* mana_orb;
    GRect* stamina_bar;
    GSprite* spell_slot;

	float m_orb_height;
    float m_stamina_bar_width;
    float m_spell_size;
    float m_spell_space;
    int m_num_spells;

    void p_drawPlayerOrbs() const;
    void p_drawPlayerSpellSelector() const;
    void p_drawPlayerPotion();
    void p_drawPlayerTrap();

    void p_erasePlayerOrbs();
    void p_erasePlayerSpellSelector();
    void p_erasePlayerPotion();
    void p_erasePlayerTrap();

};
#  endif