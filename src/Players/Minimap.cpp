#include "./Minimap.h"

#include <Assets.h>
#include "Player.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Objects/Trap.h"
#include "./../Managers/StateManager.h"
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GSprite.h>
#include <Constants.h>
#include <cmath>

HUD_Minimap::HUD_Minimap(Player* p){
    m_player = p;

    m_zoom = 1.0f;
    m_sizeMap = 50.0f; // Ejemplo del Lobby2
    m_originalSize = 50.0f;
    m_mapPath = "./../assets/textures/HUD/Minimap/Mapa.jpg";

    GraphicEngine* g_engine = GraphicEngine::getInstance();

    float ratio = g_engine->GetAspectRatio();
    float W = g_engine->GetScreenWidth();
    float H = g_engine->GetScreenHeight();
    m_rotation = 0.0f;

    m_center = vector2df(0,0.5f);
    m_size = vector2df(W/5 , (W/5)*ratio);
    m_position = vector2df(W-m_size.X, H-m_size.Y);
    m_mapImage = GraphicEngine::getInstance()->addSprite(m_mapPath, m_position, m_size);
    m_mapImage->SetMask("./../assets/textures/HUD/Minimap/mask.jpg");
    m_compassImg = GraphicEngine::getInstance()->addSprite(TEXTUREMAP[TEXTURE_MINIMAP_COMPASS],m_position,m_size);
}

HUD_Minimap::~HUD_Minimap(){
    delete m_mapImage;
    delete m_compassImg;

    int size = m_players.size();
    for(int i=0; i<size; i++){
        delete m_players[i];
    }
    m_players.clear();


    size = m_enemies.size();
    for(int i=0; i<size; i++){
        delete m_enemies[i];
    }
    m_enemies.clear();
    p_enemies.clear();

    size = m_traps.size();
    for(int i=0; i<size; i++){
        delete m_traps[i];
    }
    m_traps.clear();
}

void HUD_Minimap::ZoomMap(float value){
    m_zoom = value;

    m_sizeMap = m_originalSize;

    float sizeRect = 1.0f / value;
    float pos = 0.5 - sizeRect/2;

    m_mapImage->SetTextureRect(pos, pos, sizeRect, sizeRect);
}

void HUD_Minimap::AlivePoint(GSprite* sprite){
    std::string path = TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK];

    sprite->SetColor(0, 1, 0);
    sprite->SetTexture(path);
}

void HUD_Minimap::DeadPoint(GSprite* sprite){
    std::string path = TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK];

    sprite->SetColor(1, 0, 0);
    sprite->SetTexture(path);
}

void HUD_Minimap::SetMapSize(float size){
    m_originalSize = size;
    m_sizeMap = size;
}

void HUD_Minimap::SetRotation(float rot){
    rot = rot * 180 / M_PI;
    m_rotation = rot;
    m_mapImage->SetRotation(-rot);
    m_compassImg->SetRotation(-rot);
}

void HUD_Minimap::SetTexture(std::string path){
    if(m_mapPath.compare(path) != 0){
        m_mapPath = path;
        m_mapImage->SetTexture(path);
    }
}

void HUD_Minimap::ChangeMap(std::string path, float size){
    SetTexture(path);
    SetMapSize(size);
}

void HUD_Minimap::UpdateScroll(){
    vector3df position = m_player->GetPos();

    // La distancia Z se corresponde con el scrollV
    float xValue = (position.X / (m_sizeMap * 2)) + m_center.X/2;
    float zValue = (position.Z / (m_sizeMap * 2)) + m_center.Y/2;

    m_mapImage->SetScrollH(xValue);
    m_mapImage->SetScrollV(-zValue);
}

GSprite* HUD_Minimap::CreatePlayerSprite(){
    GSprite* output = nullptr;

    m_spriteSize = vector2df(10,10);
    vector2df pos(0,0);

    output = GraphicEngine::getInstance()->addSprite("", pos, m_spriteSize);
    return output;
}

void HUD_Minimap::RecalculatePlayerSprites(int playerSize){
    int spriteSize = m_players.size();

    // En el caso de que hayan menos sprites que players, vamos creando sprites
    while(spriteSize < playerSize){
        GSprite* newSprite = CreatePlayerSprite();
        m_players.push_back(newSprite);
        spriteSize++;
    }

    // En el caso de que hayan sprites de más, vamos eliminano sprites
    while(spriteSize > playerSize){
        delete m_players[spriteSize-1];
        m_players.erase(m_players.begin() + spriteSize - 1);
        spriteSize--;
    }
}

void HUD_Minimap::RecalculateTrapSprites(int trapSize){
    int spriteSize = m_traps.size();

    while(spriteSize < trapSize){
        std::string path = TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK];
        
        GSprite* newSprite = CreatePlayerSprite();
        newSprite->SetColor(1,1,0);
        newSprite->SetTexture(path);
        
        m_traps.push_back(newSprite);
        spriteSize++;
    }

    while(spriteSize > trapSize){
        delete m_traps[spriteSize-1];
        m_traps.erase(m_traps.begin() + spriteSize - 1);
        spriteSize--;
    }
}

void HUD_Minimap::DrawPlayers(){
    std::vector<Player*> players = PlayerManager::GetInstance()->GetAllPlayers(m_player->GetAlliance());
    int playerSize = players.size();
    RecalculatePlayerSprites(playerSize);

    //PLAYERS
    int spriteSize = m_players.size();
    for(int i=0; i<spriteSize; i++){
        Player* currentPlayer = players[i];
        SetStyleSprite(i, currentPlayer);
        CalculatePositionSprite(i, currentPlayer);
    }

    //ENEMIES
    if(!p_enemies.empty()){
        std::map<Player*,float>::iterator it = p_enemies.begin();
        std::vector<GSprite*>::iterator spr_it = m_enemies.begin();

        for(; it!=p_enemies.end() && spr_it!=m_enemies.end() ; ++it, ++spr_it){
            float time_left = it->second;
            int id = spr_it - m_enemies.begin();

            if(time_left>0){
                time_left -= StateManager::GetInstance()->GetDeltaTime();
                it->second = time_left;
                Player* currentPlayer = (Player*) it->first;
                CalculatePositionSprite(id, currentPlayer);
            }
            else{
                p_enemies.erase(it);
                delete m_enemies[id];
                m_enemies.erase(spr_it);
            }
        }
    }

    //TRAPS
    if(m_player->GetAlliance() == ALLIANCE_WARLOCK){
        std::vector<Trap*> traps = TrapManager::GetInstance()->GetAllTraps();
        int trapSize = traps.size();
        RecalculateTrapSprites(trapSize);

        spriteSize = m_traps.size();
        for(int i = 0; i<spriteSize; i++){
            CalculatePositionSprite(i, traps[i]);
        }
    }
}

void HUD_Minimap::SetStyleSprite(int id, Player* p){
    GSprite* currentSprite = nullptr;
    if(p->GetAlliance() == m_player->GetAlliance() ) currentSprite = m_players[id];
    else currentSprite = m_enemies[id];

    if(currentSprite != nullptr){
        if(p->IsDead()){
            DeadPoint(currentSprite);
        }else{
            AlivePoint(currentSprite);
        }
    }
}

void HUD_Minimap::CalculatePositionSprite(int id, Player* p){
    float ratio = GraphicEngine::getInstance()->GetAspectRatio();

    vector3df position = m_player->GetPos();
    vector3df otherPosition = p->GetPos();

    vector3df diff = otherPosition - position;

    float xValue = (diff.X * (m_size.X)) / (m_sizeMap * 2);
    float zValue = (diff.Z * (m_size.Y)) / (m_sizeMap * 2);

    xValue *= m_zoom;
    zValue *= m_zoom;

    // -- ROTATE THE POINT -----------------------------------
    float radRotation = m_rotation * M_PI/180;

    float cosRot = cos(radRotation);
    float sinRot = sin(radRotation);

    float xValueRot = (xValue * cosRot) + (zValue * -sinRot);
    float zValueRot = (xValue * sinRot) + (zValue *  cosRot);
    zValueRot *= ratio;

    // -------------------------------------------------------
    // -- CHECK IF OUTSIDE -----------------------------------

    float checkX = (xValueRot * xValueRot) / (m_size.X/2 * m_size.X/2);
    float checkY = (zValueRot * zValueRot) / (m_size.Y/2 * m_size.Y/2);

    if(checkX + checkY > 1){
        // Esta fuera del circulo del minimapa
        if(m_player->GetAlliance() == p->GetAlliance()) m_players[id]->SetColor(0,0,0,0);
        else m_enemies[id]->SetColor(0,0,0,0);
        return;
    }

    // -------------------------------------------------------
    xValueRot -= m_spriteSize.X/2;
    zValueRot -= m_spriteSize.Y/2;

    xValueRot += m_position.X + m_size.X/2;
    zValueRot += m_position.Y + m_size.Y/2;

    if(m_player->GetAlliance() == p->GetAlliance()) m_players[id]->SetPosition(xValueRot, zValueRot);
    else m_enemies[id]->SetPosition(xValueRot, zValueRot);
}

void HUD_Minimap::CalculatePositionSprite(int id, Trap* t){
    float ratio = GraphicEngine::getInstance()->GetAspectRatio();

    vector3df position = m_player->GetPos();
    vector3df otherPosition = t->GetPosition();

    vector3df diff = otherPosition - position;

    float xValue = (diff.X * (m_size.X)) / (m_sizeMap * 2);
    float zValue = (diff.Z * (m_size.Y)) / (m_sizeMap * 2);

    xValue *= m_zoom;
    zValue *= m_zoom;

    // -- ROTATE THE POINT -----------------------------------
    float radRotation = m_rotation * M_PI/180;

    float cosRot = cos(radRotation);
    float sinRot = sin(radRotation);

    float xValueRot = (xValue * cosRot) + (zValue * -sinRot);
    float zValueRot = (xValue * sinRot) + (zValue *  cosRot);
    zValueRot *= ratio;

    // -------------------------------------------------------
    // -- CHECK IF OUTSIDE -----------------------------------

    float checkX = (xValueRot * xValueRot) / (m_size.X/2 * m_size.X/2);
    float checkY = (zValueRot * zValueRot) / (m_size.Y/2 * m_size.Y/2);

    if(m_player->GetAlliance() != ALLIANCE_WARLOCK) m_traps[id]->SetAlpha(0);
    else m_traps[id]->SetAlpha(1);

    if(checkX + checkY > 1){
        // Esta fuera del circulo del minimapa
        m_traps[id]->SetAlpha(0);
        return;
    }
    else m_traps[id]->SetAlpha(1);

    // -------------------------------------------------------
    xValueRot -= m_spriteSize.X/2;
    zValueRot -= m_spriteSize.Y/2;

    xValueRot += m_position.X + m_size.X/2;
    zValueRot += m_position.Y + m_size.Y/2;

    m_traps[id]->SetPosition(xValueRot, zValueRot);
}