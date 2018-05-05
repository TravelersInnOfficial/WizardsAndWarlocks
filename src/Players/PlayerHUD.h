#ifndef PLAYERHUD_H
#define PLAYERHUD_H

#include <string>
#include <vector2d.h>
#include <iostream>
#include <vector>
#include <map>

//FAST FORWARD DECLARATIONS
class GraphicEngine;
class Player;
class Potion;
class GSprite;
class GRect;
class GText2D;
class Trap;
//-------------------------

class PlayerHUD{
public:
    PlayerHUD(Player* p);
    ~PlayerHUD();
    void InitHUD();
    void Draw();
    void Erase();
    void ShowEnemyInMap(Player* p);

private:
    
	struct HUD_Orb{
		GSprite* bkg;
		GSprite* front;
		GSprite* fill;
		GSprite* scroll_fill;
		GSprite* scroll_lip;
        GText2D* info;
        float xPos;
        float yPos;
        float width;
        float height;

		HUD_Orb();
        ~HUD_Orb();
		void SetHeight(float v);
		void Update(float vel, std::string inf);
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

    struct HUD_Minimap{
        Player*                    m_player;

        // Map Info
        GSprite*                   m_mapImage;
        float                      m_originalSize;
        float                      m_sizeMap;
        float                      m_zoom;
        std::string                m_mapPath;
        vector2df                  m_position;
        vector2df                  m_size;
        float                      m_rotation;
        
        std::vector<GSprite*>      m_players;
        vector2df                  m_spriteSize;
        std::map<Player*,float>    p_enemies;
        std::vector<GSprite*>      m_enemies;
        std::vector<GSprite*>      m_traps;

        HUD_Minimap(Player* p);
        ~HUD_Minimap();
        void DrawPlayers();
        void ZoomMap(float value);
        void SetMapSize(float size);
        void SetRotation(float rot);
        void RecalculatePlayerSprites(int playerSize);
        void RecalculateTrapSprites(int trapsSize);
        void SetTexture(std::string path);
        void UpdateScroll();
        void ChangeMap(std::string path, float size);

        GSprite* CreatePlayerSprite();
        void CalculatePositionSprite(int id, Player* p);
        void CalculatePositionSprite(int id, Trap* t);
        void SetStyleSprite(int id, Player* p);

        void AlivePoint(GSprite* sprite);
        void DeadPoint(GSprite* sprite);
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

    HUD_Minimap* m_minimap;


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
    void p_drawMinimap();

    void p_erasePlayerOrbs();
    void p_eraseStaminaBar();
    void p_erasePlayerSpellSelector();
    void p_erasePlayerPotion();
    void p_erasePlayerTrap();

};
#  endif