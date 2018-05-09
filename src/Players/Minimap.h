#ifndef MINIMAP_H
#define MINIMAP_H

#include <vector2d.h>
#include <vector>
#include <map>

class GSprite;
class Player;
class Trap;

class HUD_Minimap{
public:
        Player*                    m_player;

        // Map Info
        GSprite*                   m_mapImage;
        GSprite*                   m_compassImg;
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

#endif