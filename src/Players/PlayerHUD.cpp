#include "PlayerHUD.h"
#include "Player.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Objects/Potion.h"
#include <Assets.h>
#include <vector>
#include <string>

//--------------------------------------------------------------------------------------//
//---------------------------------------PUBLIC-----------------------------------------//
//--------------------------------------------------------------------------------------//

PlayerHUD::PlayerHUD(Player* p){
    m_player = p;
    p_alliance = -1;
    p_potion = nullptr;

    health_orb = nullptr;
    mana_orb = nullptr;
    stamina_bar = nullptr;
    spell_slot = nullptr;
    potion_slot = nullptr;
    trap_slot = nullptr;
    trap_usings_slot = nullptr;
    
    m_orb_height = 0.0f;
    m_stamina_bar_width = 0.0f;

    m_minimap = nullptr;

    g_engine = GraphicEngine::getInstance();
}

PlayerHUD::~PlayerHUD(){
	Erase();

    g_engine = nullptr;
    p_potion = nullptr;

    delete spell_slot;
}

void PlayerHUD::InitHUD(){
    if(health_orb == nullptr && mana_orb == nullptr && stamina_bar == nullptr){
        p_initPlayerOrbs();
        if(health_orb != nullptr) p_initPlayerPotion();
        if(mana_orb != nullptr){ 
            p_initPlayerSpellSelector();
            if(p_alliance != ALLIANCE_WIZARD) p_initPlayerTrap();
        }
        if(spell_slot!= nullptr && health_orb!=nullptr && mana_orb != nullptr) p_initStaminaBar();


        /************************************MINIMAP*************************************/
        if(m_minimap == nullptr) m_minimap = new HUD_Minimap(m_player);

	}
    else{
        health_orb->SetHeight(m_orb_height);
	    mana_orb->SetHeight(m_orb_height);
    	stamina_bar->SetWidth(m_stamina_bar_width);
    }
}

void PlayerHUD::Draw(){
    p_drawPlayerOrbs();
    p_drawStaminaBar();
    p_drawPlayerSpellSelector();
    p_drawPlayerPotion();
    p_drawMinimap();

    if(p_alliance == -1) p_alliance = m_player->GetAlliance();
    if(p_alliance != m_player->GetAlliance()){
        p_erasePlayerTrap();
        p_alliance = m_player->GetAlliance();
        mana_orb->SetColor(p_alliance);
        if(p_alliance == ALLIANCE_WARLOCK) g_engine->SetAimColor(0.8f,0.0f,0.8f);
        else g_engine->SetAimColor(0.0f,0.0f,1.0f);
    }
    if(p_alliance == ALLIANCE_WARLOCK) p_drawPlayerTrap();
}

void PlayerHUD::Erase(){
    p_erasePlayerOrbs();
    p_eraseStaminaBar();
    p_erasePlayerSpellSelector();
    p_erasePlayerPotion();
    if(p_alliance != ALLIANCE_WARLOCK) p_erasePlayerTrap();
    if(spell_slot != nullptr){ delete spell_slot; spell_slot = nullptr; }
    if(m_minimap != nullptr){ delete m_minimap; m_minimap = nullptr; }
}

//--------------------------------------------------------------------------------------//
//---------------------------------------PRIVATE----------------------------------------//
//--------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------//
//----------------------------------------INITS-----------------------------------------//
//--------------------------------------------------------------------------------------//


void PlayerHUD::p_initPlayerOrbs(){
    int W = g_engine->GetScreenWidth();		
    float ratio = g_engine->GetAspectRatio();
    toe::core::TOEvector2di tex_dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_ORB_BACK]);

    //orb dimensions
    float new_width = W/5.0f;
    float new_height = ratio * new_width;
    vector2df orb_dims = vector2df(new_width,new_height);
    if(orb_dims.X > tex_dims.X) orb_dims = vector2df(tex_dims.X,tex_dims.Y);

    //orb position
    vector2df pos = vector2df(0,0);

    //HEALTH
    health_orb = new HUD_Orb();
    health_orb->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_BACK],pos,orb_dims);
    
    health_orb->fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FILL],pos,orb_dims);
    health_orb->fill->SetColor(1,0,0);
    
    health_orb->scroll_fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL],pos,orb_dims);
    health_orb->scroll_fill->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);
    health_orb->scroll_fill->SetColor(0.5,0,0,0.8);

    health_orb->scroll_lip = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_LIP],pos,orb_dims);
    health_orb->scroll_lip->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);
    health_orb->scroll_lip->SetColor(0.3,0,0,0.8);

    health_orb->front = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FRONT],pos,orb_dims);
    
    health_orb->xPos = pos.X;
    health_orb->yPos = pos.Y;
    health_orb->width = orb_dims.X;
    health_orb->height = orb_dims.Y;

    //MANA
    pos = vector2df(g_engine->GetScreenWidth()-orb_dims.X,0);
    mana_orb = new HUD_Orb();
    mana_orb->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_BACK],pos,orb_dims);
    
    mana_orb->fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FILL],pos,orb_dims);
    
    mana_orb->scroll_fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL],pos,orb_dims);
    mana_orb->scroll_fill->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);

    mana_orb->scroll_lip = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_LIP],pos,orb_dims);
    mana_orb->scroll_lip->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);

    mana_orb->front = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FRONT],pos,orb_dims);

    mana_orb->SetColor(ALLIANCE_WARLOCK);

    mana_orb->xPos = pos.X;
    mana_orb->yPos = pos.Y;
    mana_orb->width = orb_dims.X;
    mana_orb->height = orb_dims.Y;

    //INITIAL ORB FILL HEIGHT
    m_orb_height = health_orb->fill->GetHeight();
}

void PlayerHUD::p_initStaminaBar(){
    int W = g_engine->GetScreenWidth();	
    float size = 10.0f;			// Height of the bar

    float xInit = health_orb->width;		// Calculate the Init and End of the bar on X axis
    float xEnd =  W - mana_orb->width;		

    float yInit = spell_slot->GetHeight();
    float yEnd = yInit + size;

    stamina_bar =g_engine->add2DRect(vector2df(xInit,yInit),vector2df(xEnd-xInit, yEnd-yInit));
    stamina_bar->SetColor(0.5,1.0,0.5);

    m_stamina_bar_width = stamina_bar->GetWidth();
    m_stamina_xPos = xInit;
    m_stamina_yPos = yInit;
}

void PlayerHUD::p_initPlayerSpellSelector(){
    int W = g_engine->GetScreenWidth();	
    m_spell_size = W * 0.055;
    m_num_spells = SpellManager::GetInstance()->GetNumSpells();
    m_spell_space = 10;

    float new_height = m_spell_size+12;
    float new_width = m_num_spells* (m_spell_size + m_spell_space) + 5;
    vector2df slot_dims = vector2df(new_width,new_height);

    toe::core::TOEvector2di tex_dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_SPELL_SLOT]);
    if(slot_dims.Y > tex_dims.Y) slot_dims = vector2df(tex_dims.X,tex_dims.Y);

    vector2df slot_pos = vector2df(W-mana_orb->width-slot_dims.X, 0);

    spell_slot = g_engine->addSprite(TEXTUREMAP[TEXTURE_SPELL_SLOT],slot_pos,slot_dims);
}

void PlayerHUD::p_initPlayerPotion(){
    int W = g_engine->GetScreenWidth();	
    float ratio = g_engine->GetAspectRatio();
    toe::core::TOEvector2di tex_dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_ITEM_SLOT]);

    float new_width = W/10.0f;
    float new_height = ratio * new_width;
    vector2df slot_dims = vector2df(new_width,new_height);
    if(slot_dims.X > tex_dims.X) slot_dims = vector2df(tex_dims.X,tex_dims.Y);

    potion_slot = new ItemSlot();
    potion_slot->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ITEM_SLOT],vector2df(0,health_orb->height),slot_dims);
    
    potion_slot->xPos = 0;
    potion_slot->yPos = health_orb->height;
    potion_slot->width = slot_dims.X;
    potion_slot->height = slot_dims.Y;
}

void PlayerHUD::p_initPlayerTrap(){
    int W = g_engine->GetScreenWidth();	
    
    float ratio = g_engine->GetAspectRatio();
    toe::core::TOEvector2di tex_dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_ITEM_SLOT]);

    float new_width = W/10.0f;
    float new_height = ratio * new_width;
    vector2df slot_dims = vector2df(new_width,new_height);
    if(slot_dims.X > tex_dims.X) slot_dims = vector2df(tex_dims.X,tex_dims.Y);

    //TRAP SLOT
    trap_slot = new ItemSlot();
    trap_slot->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ITEM_SLOT],vector2df(W-slot_dims.X,mana_orb->height),slot_dims);
    
    trap_slot->xPos = W-slot_dims.X;
    trap_slot->yPos = mana_orb->height;
    trap_slot->width = slot_dims.X;
    trap_slot->height = slot_dims.Y;

    //TRAP USINGS SLOT
    slot_dims = vector2df(slot_dims.X/3,slot_dims.Y/3);
    trap_usings_slot = new ItemSlot();
    trap_usings_slot->bkg = g_engine->addSprite(TEXTUREMAP[TEXTURE_ITEM_SLOT],vector2df(W-slot_dims.X,mana_orb->height),slot_dims);
    
    trap_usings_slot->xPos = W-slot_dims.X;
    trap_usings_slot->yPos = mana_orb->height;
    trap_usings_slot->width = slot_dims.X;
    trap_usings_slot->height = slot_dims.Y;
    
    m_trap_usings = TrapManager::GetInstance()->getPlayerUsings(m_player);
}

//--------------------------------------------------------------------------------------//
//----------------------------------------DRAWS-----------------------------------------//
//--------------------------------------------------------------------------------------//


void PlayerHUD::p_drawMinimap(){
    if(m_minimap!=nullptr){
        float rotation = m_player->GetRotY();
        rotation = rotation * 180 / M_PI;
        m_minimap->SetRotation(rotation);

        m_minimap->UpdateScroll();
        m_minimap->DrawPlayers();
    }
}

void PlayerHUD::p_drawPlayerOrbs() const{
    if(health_orb != nullptr && mana_orb != nullptr){
        float HP = m_player->GetHP();
        float MP = m_player->GetMP();

		health_orb->SetHeight(m_orb_height*(HP/100));
		mana_orb->SetHeight(m_orb_height*(MP/100));

		health_orb->Update(0.005);
		mana_orb->Update(0.005);
	}
}

void PlayerHUD::p_drawStaminaBar(){
    if(stamina_bar != nullptr){
        float SP = m_player->GetSP();
        float SP_w = m_stamina_bar_width*(SP/100);
        float colorG = (SP_w/m_stamina_bar_width);
        float colorR = 1-colorG;
        stamina_bar->SetXPos(m_stamina_xPos+(m_stamina_bar_width/2 - SP_w/2));
		stamina_bar->SetWidth(SP_w);
        stamina_bar->SetColor(colorR,colorG - 0.15,0);
    }
}

void PlayerHUD::p_drawPlayerSpellSelector() const{
    if(mana_orb != nullptr){
        float W =  g_engine->GetScreenWidth();
        float xPos = W - mana_orb->bkg->GetWidth();              // X position
        float yPos = 3;                                                 // Y position
    	float outline = 5;			                                    // Borde de los hechizos
        int current = m_player->GetCurrentSpell();                      //m_currentSpell

        std::vector<Hechizo*> hechizos = SpellManager::GetInstance()->GetSpells(m_player);

    	xPos = xPos - (m_spell_size + m_spell_space) * m_num_spells;
    	float xInitSpell = 0.0f;
        if(m_player != nullptr){
            for(int i = 0; i<hechizos.size();i++){
                if(hechizos[i]!=nullptr){
                    bool disabled = hechizos[i]->CheckMP(m_player->GetMP());
                    xInitSpell = xPos + (m_spell_size + m_spell_space)*i;	// Calcula la X inicial de cada hechizo
                    hechizos[i]->DrawHUD(xInitSpell, yPos, m_spell_size, outline, i==current, !disabled, p_alliance);
                }
            }
        }
    }
}

void PlayerHUD::p_drawPlayerPotion(){
    if(p_potion != m_player->GetPotion()){
        if(p_potion != nullptr) potion_slot->RemoveItem();
        p_potion = m_player->GetPotion();

        if(p_potion != nullptr && potion_slot->item == nullptr) potion_slot->AddItem(p_potion->GetHUDTexture());
    }
}

void PlayerHUD::p_drawPlayerTrap(){
    std::string tex = TrapManager::GetInstance()->GetPathFromEnum(TrapManager::GetInstance()->getPlayerTrap(m_player));
    float usings = TrapManager::GetInstance()->getPlayerUsings(m_player);
    //SI NO ESTA INICIALIZADO
    if(trap_slot == nullptr){
        p_initPlayerTrap();
    }
    //SI NO HAY ITEM
    else if(trap_slot->item == nullptr && trap_usings_slot->item == nullptr){
        m_trap_usings = TrapManager::GetInstance()->getPlayerUsings(m_player);
        std::string usingsTex = TrapManager::GetInstance()->GetPathFromUsings(m_trap_usings);

        trap_slot->AddItem(tex);
        trap_usings_slot->AddItem(usingsTex);
    }
    //SI LA TEXTURA HA CAMBIADO
    else if(trap_slot->item->GetTexture() != tex){
        trap_slot->item->SetTexture(tex);
    }
    //SI LOS USOS HAN CAMBIADO
    if(m_trap_usings != usings){
        m_trap_usings = usings;
        std::string usingsTex = TrapManager::GetInstance()->GetPathFromUsings(m_trap_usings);
        trap_usings_slot->item->SetTexture(usingsTex);
    }
}

//--------------------------------------------------------------------------------------//
//---------------------------------------ERASES-----------------------------------------//
//--------------------------------------------------------------------------------------//


void PlayerHUD::p_erasePlayerOrbs(){
    if(health_orb != nullptr){ delete health_orb; health_orb = nullptr;} 
	if(mana_orb != nullptr){ delete mana_orb; mana_orb = nullptr;}
}

void PlayerHUD::p_eraseStaminaBar(){
    if(stamina_bar != nullptr){ delete stamina_bar; stamina_bar = nullptr;}
}

void PlayerHUD::p_erasePlayerSpellSelector(){
    std::vector<Hechizo*> hechizos = SpellManager::GetInstance()->GetSpells(m_player);
    if(m_player != nullptr){
        for(int i = 0; i<hechizos.size();i++){
            if(hechizos[i]!=nullptr) hechizos[i]->EraseHUD();
        }
    }
}

void PlayerHUD::p_erasePlayerPotion(){
    if(p_potion != nullptr) potion_slot->RemoveItem();
    p_potion = nullptr;
}

void PlayerHUD::p_erasePlayerTrap(){
    if(trap_slot != nullptr) delete trap_slot;
    if(trap_usings_slot != nullptr) delete trap_usings_slot;
    
    trap_slot = nullptr;
    trap_usings_slot = nullptr;
}

//--------------------------------------------------------------------------------------//
//------------------------------------HUD ORB STRUCT------------------------------------//
//--------------------------------------------------------------------------------------//

PlayerHUD::HUD_Orb::HUD_Orb(){
    front = nullptr;
    bkg = nullptr;
    fill = nullptr;
    scroll_lip = nullptr;
    scroll_fill = nullptr;
}

PlayerHUD::HUD_Orb::~HUD_Orb(){
	delete front;
	delete bkg;
	delete fill;
	delete scroll_lip;
	delete scroll_fill;

	front = nullptr;
	bkg = nullptr;
	fill = nullptr;
	scroll_lip = nullptr;
	scroll_fill = nullptr;
	scroll_lip = nullptr;
}

void PlayerHUD::HUD_Orb::SetHeight(float v){
	float lip_height = 3;
	fill->SetRect(0,fill->GetTextureHeight()-v,fill->GetWidth(),v);
	scroll_fill->SetRect(0,scroll_fill->GetTextureHeight()-v,scroll_fill->GetWidth(),v);
	scroll_lip->SetRect(0,scroll_lip->GetTextureHeight()-v,scroll_lip->GetWidth(),v - (v - lip_height));
	scroll_lip->SetPosition(fill->GetPosX(),fill->GetHeight() - lip_height);
}

void PlayerHUD::HUD_Orb::SetColor(int alliance){
    if(alliance == ALLIANCE_WARLOCK){
		fill->SetColor(1.0f,0,1.0f);
		scroll_fill->SetColor(0.0f,0.0f,0.5f,0.5f);
		scroll_lip->SetColor(0.3f,0.0f,0.3f,0.3f);
    }
    else{
        fill->SetColor(0.0f,0.0f,1.0f);
        scroll_fill->SetColor(0.0f,0.0f,0.5f,0.5f);
        scroll_lip->SetColor(0.0f,0.0f,0.3f,0.3f);
    }   
}   

void PlayerHUD::HUD_Orb::Update(float vel){
	scroll_fill->ScrollV(vel);
	scroll_lip->ScrollH(-vel);
}

PlayerHUD::ItemSlot::ItemSlot(){
    bkg = nullptr;
    item = nullptr;
    xPos = 0.0f;
    yPos = 0.0f;
    width = 0.0f;
    height = 0.0f;
}

PlayerHUD::ItemSlot::~ItemSlot(){
    delete bkg;
    delete item;
    bkg = nullptr;
    item = nullptr;
}

void PlayerHUD::ItemSlot::AddItem(std::string texPath){
    item = GraphicEngine::getInstance()->addSprite(texPath, vector2df(xPos,yPos), vector2df(width,height));
    item->SetMask(TEXTUREMAP[TEXTURE_ITEM_SLOT_MASK]);
}

void PlayerHUD::ItemSlot::RemoveItem(){
    delete item;
    item = nullptr;
}

//--------------------------------------------------------------------------------------//
//------------------------------------HUD MINIMAP---------------------------------------//
//--------------------------------------------------------------------------------------//

PlayerHUD::HUD_Minimap::HUD_Minimap(Player* p){
    m_player = p;

    m_zoom = 1.0f;
    m_sizeMap = 40.0f; // Ejemplo del Lobby2
    m_originalSize = 40.0f;
    m_mapPath = "./../assets/textures/HUD/Minimap/map.jpg";

    GraphicEngine* g_engine = GraphicEngine::getInstance();

    float ratio = g_engine->GetAspectRatio();

    m_rotation = 0.0f;
    m_position = vector2df(600, 300);
    m_size = vector2df(200, 200*ratio);
    m_mapImage = GraphicEngine::getInstance()->addSprite(m_mapPath, m_position, m_size);
    m_mapImage->SetMask("./../assets/textures/HUD/Minimap/mask.jpg");
}

PlayerHUD::HUD_Minimap::~HUD_Minimap(){
    delete m_mapImage;

    int size = m_players.size();
    for(int i=0; i<size; i++){
        delete m_players[i];
    }
    m_players.clear();
}

void PlayerHUD::HUD_Minimap::ZoomMap(float value){
    m_zoom = value;

    m_sizeMap = m_originalSize;

    float sizeRect = 1.0f / value;
    float pos = 0.5 - sizeRect/2;

    m_mapImage->SetTextureRect(pos, pos, sizeRect, sizeRect);
}

void PlayerHUD::HUD_Minimap::AlivePoint(GSprite* sprite){
    std::string path = TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK];

    sprite->SetColor(0, 1, 0);
    sprite->SetTexture(path);
}

void PlayerHUD::HUD_Minimap::DeadPoint(GSprite* sprite){
    std::string path = TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK];

    sprite->SetColor(1, 0, 0);
    sprite->SetTexture(path);
}

void PlayerHUD::HUD_Minimap::SetMapSize(float size){
    m_originalSize = size;
    m_sizeMap = size;
}

void PlayerHUD::HUD_Minimap::SetRotation(float rot){
    m_rotation = rot;
    m_mapImage->SetRotation(-rot);
}

void PlayerHUD::HUD_Minimap::SetTexture(std::string path){
    if(m_mapPath.compare(path) != 0){
        m_mapPath = path;
        m_mapImage->SetTexture(path);
    }
}

void PlayerHUD::HUD_Minimap::ChangeMap(std::string path, float size){
    SetTexture(path);
    SetMapSize(size);
}

void PlayerHUD::HUD_Minimap::UpdateScroll(){
    vector3df position = m_player->GetPos();

    // La distancia Z se corresponde con el scrollV
    float xValue = position.X / (m_sizeMap * 2);
    float zValue = position.Z / (m_sizeMap * 2);

    m_mapImage->SetScrollH(xValue);
    m_mapImage->SetScrollV(-zValue);
}

GSprite* PlayerHUD::HUD_Minimap::CreatePlayerSprite(){
    GSprite* output = nullptr;

    m_spriteSize = vector2df(10,10);
    vector2df pos(0,0);

    output = GraphicEngine::getInstance()->addSprite("", pos, m_spriteSize);
    return output;
}

void PlayerHUD::HUD_Minimap::RecalculatePlayerSprites(int playerSize){
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

void PlayerHUD::HUD_Minimap::DrawPlayers(){
    std::vector<Player*> players = PlayerManager::GetInstance()->GetAllPlayers(m_player->GetAlliance());
    int playerSize = players.size();
    RecalculatePlayerSprites(playerSize);

    int spriteSize = m_players.size();
    for(int i=0; i<spriteSize; i++){
        Player* currentPlayer = players[i];
        SetStyleSprite(i, currentPlayer);
        CalculatePositionSprite(i, currentPlayer);
    }
}

void PlayerHUD::HUD_Minimap::SetStyleSprite(int id, Player* p){
    GSprite* currentSprite = m_players[id];

    if(p->IsDead()){
        DeadPoint(currentSprite);
    }else{
        AlivePoint(currentSprite);
    }

}

void PlayerHUD::HUD_Minimap::CalculatePositionSprite(int id, Player* p){
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
        m_players[id]->SetColor(0,0,0,0);
        return;
    }

    // -------------------------------------------------------
    xValueRot -= m_spriteSize.X/2;
    zValueRot -= m_spriteSize.Y/2;

    xValueRot += m_position.X + m_size.X/2;
    zValueRot += m_position.Y + m_size.Y/2;

    m_players[id]->SetPosition(xValueRot, zValueRot);
}

