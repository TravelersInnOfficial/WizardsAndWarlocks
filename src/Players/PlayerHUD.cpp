#include "PlayerHUD.h"
#include "Player.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Objects/Potion.h"
#include <Assets.h>

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
    
    m_orb_height = 0.0f;
    m_stamina_bar_width = 0.0f;

    g_engine = GraphicEngine::getInstance();
}

PlayerHUD::~PlayerHUD(){
    g_engine = nullptr;
    p_potion = nullptr;
    
    delete health_orb;
    delete mana_orb;
    delete stamina_bar;
    delete spell_slot;
}

void PlayerHUD::InitHUD(){
    if(health_orb == nullptr && mana_orb == nullptr && stamina_bar == nullptr){
		int W = g_engine->GetScreenWidth();		
		int H = g_engine->GetScreenHeight();

		float size = 20.0f;			// Height of the bar

		float xInit = W/20.0f;		// Calculate the Init and End of the bar on X axis
		float xEnd =  W/3.0f;		

		float yInitH = H - size*1.5;	// Calculate the Init of the bar on Y axis
		float yInitM = H - size*3;
		float yInitS = H - size*4.5;

		float yEndH = yInitH + size;	// Calculate the End of the bar on Y axis with the size
		float yEndM = yInitM + size;
		float yEndS = yInitS + size;

		stamina_bar =g_engine->add2DRect(vector2df(xInit,yInitS),vector2df(xEnd-xInit, yEndS-yInitS));
		stamina_bar->SetColor(0.5,0.5,0.5);

		m_stamina_bar_width =  stamina_bar->GetWidth();

		/*************************************ORBS*************************************/
		toe::core::TOEvector2di tex_dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_ORB_BACK]);

        //g_engine->GetVideoDriver()->GetWindowResolution();
		float ratio = (W/H);
		float new_width = W/5.0f;
		float new_height = ratio * new_width;
		vector2df orb_dims = vector2df(new_width,new_height);
		if(orb_dims.X > tex_dims.X) orb_dims = vector2df(tex_dims.X,tex_dims.Y);

		vector2df pos = vector2df(0,0);
		vector2df pos2 = vector2df(g_engine->GetScreenWidth()-orb_dims.X,0);

		//HEALTH
		health_orb = new HUD_Orb();
		health_orb->m_orb_back = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_BACK],pos,orb_dims);
		
		health_orb->m_orb_fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FILL],pos,orb_dims);
		health_orb->m_orb_fill->SetColor(1,0,0);
		
		health_orb->m_orb_scroll_fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL],pos,orb_dims);
		health_orb->m_orb_scroll_fill->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);
		health_orb->m_orb_scroll_fill->SetColor(0.5,0,0,0.8);

		health_orb->m_orb_scroll_lip = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_LIP],pos,orb_dims);
		health_orb->m_orb_scroll_lip->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);
		health_orb->m_orb_scroll_lip->SetColor(0.3,0,0,0.8);

		health_orb->m_orb_front = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FRONT],pos,orb_dims);

		//MANA
		mana_orb = new HUD_Orb();
		mana_orb->m_orb_back = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_BACK],pos2,orb_dims);
		
		mana_orb->m_orb_fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FILL],pos2,orb_dims);
		mana_orb->m_orb_fill->SetColor(0,0,1);
		
		mana_orb->m_orb_scroll_fill = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL],pos2,orb_dims);
		mana_orb->m_orb_scroll_fill->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);
		mana_orb->m_orb_scroll_fill->SetColor(0,0,0.5,0.5);

		mana_orb->m_orb_scroll_lip = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_SCROLL_LIP],pos2,orb_dims);
		mana_orb->m_orb_scroll_lip->SetMask(TEXTUREMAP[TEXTURE_ORB_SCROLL_FILL_MASK]);
		mana_orb->m_orb_scroll_lip->SetColor(0,0,0.3);

		mana_orb->m_orb_front = g_engine->addSprite(TEXTUREMAP[TEXTURE_ORB_FRONT],pos2,orb_dims);

		m_orb_height = health_orb->m_orb_fill->GetHeight();

        //SPELL SLOT
        tex_dims = toe::GetTextureDims(TEXTUREMAP[TEXTURE_SPELL_SLOT]);
        vector2df slot_pos = vector2df(W-orb_dims.X, 0);
        vector2df slot_dims = vector2df(tex_dims.X,tex_dims.Y);

        //spell_slot = g_engine->addSprite(TEXTUREMAP[TEXTURE_SPELL_SLOT],slot_pos,slot_dims);
	}
    else{
        health_orb->SetHeight(m_orb_height);
	    mana_orb->SetHeight(m_orb_height);
    	stamina_bar->SetWidth(m_stamina_bar_width);
    }
}

void PlayerHUD::Draw(){
    p_drawPlayerOrbs();
    p_drawPlayerSpellSelector();
    p_drawPlayerPotion();
    p_drawPlayerTrap();
}

void PlayerHUD::Erase(){
    p_erasePlayerOrbs();
    p_erasePlayerSpellSelector();
    p_erasePlayerPotion();
    if(p_alliance != ALLIANCE_WARLOCK) p_erasePlayerTrap();
}

//--------------------------------------------------------------------------------------//
//---------------------------------------PRIVATE----------------------------------------//
//--------------------------------------------------------------------------------------//

void PlayerHUD::p_drawPlayerOrbs() const{
    if(health_orb != nullptr && mana_orb != nullptr && stamina_bar != nullptr){
        float HP = m_player->GetHP();
        float MP = m_player->GetMP();
        float SP = m_player->GetSP();

		health_orb->SetHeight(m_orb_height*(HP/100));
		mana_orb->SetHeight(m_orb_height*(MP/100));

		health_orb->Update(0.005);
		mana_orb->Update(0.005);

		stamina_bar->SetWidth(m_stamina_bar_width*(SP/100));
	}
}

void PlayerHUD::p_drawPlayerSpellSelector() const{
    SpellManager::GetInstance()->DrawHUDSpells(m_player, m_player->GetCurrentSpell());
}

void PlayerHUD::p_drawPlayerPotion(){
    if(p_potion != m_player->GetPotion()){
        if(p_potion != nullptr) p_potion->EraseHUD();
        p_potion = m_player->GetPotion();
    }
    if(p_potion != nullptr) p_potion->DrawHUD();
}

void PlayerHUD::p_drawPlayerTrap(){
    if(p_alliance == -1) p_alliance = m_player->GetAlliance();
    if(p_alliance != m_player->GetAlliance()){
        p_erasePlayerTrap();
        p_alliance = m_player->GetAlliance();
    }
    if(p_alliance == ALLIANCE_WARLOCK) TrapManager::GetInstance()->DrawHUD(m_player);
}

void PlayerHUD::p_erasePlayerOrbs(){
    if(health_orb != nullptr){ delete health_orb; health_orb = nullptr;} 
	if(mana_orb != nullptr){ delete mana_orb; mana_orb = nullptr;}
	if(stamina_bar != nullptr){ delete stamina_bar; stamina_bar = nullptr;}
}

void PlayerHUD::p_erasePlayerSpellSelector(){
    if(m_player!= nullptr) SpellManager::GetInstance()->EraseHUDSpells(m_player);
}

void PlayerHUD::p_erasePlayerPotion(){
    if(p_potion != nullptr) p_potion->EraseHUD();
    p_potion = nullptr;
}

void PlayerHUD::p_erasePlayerTrap(){
    TrapManager::GetInstance()->EraseHUD(m_player);
}

//--------------------------------------------------------------------------------------//
//------------------------------------HUD ORB STRUCT------------------------------------//
//--------------------------------------------------------------------------------------//

PlayerHUD::HUD_Orb::HUD_Orb(){
    m_orb_front = nullptr;
    m_orb_back = nullptr;
    m_orb_fill = nullptr;
    m_orb_scroll_lip = nullptr;
    m_orb_scroll_fill = nullptr;
}

PlayerHUD::HUD_Orb::~HUD_Orb(){
	delete m_orb_front;
	delete m_orb_back;
	delete m_orb_fill;
	delete m_orb_scroll_lip;
	delete m_orb_scroll_fill;

	m_orb_front = nullptr;
	m_orb_back = nullptr;
	m_orb_fill = nullptr;
	m_orb_scroll_lip = nullptr;
	m_orb_scroll_fill = nullptr;
	m_orb_scroll_lip = nullptr;
}

void PlayerHUD::HUD_Orb::SetHeight(float v){
	float lip_height = 3;
	m_orb_fill->SetRect(0,m_orb_fill->GetTextureHeight()-v,m_orb_fill->GetWidth(),v);
	m_orb_scroll_fill->SetRect(0,m_orb_scroll_fill->GetTextureHeight()-v,m_orb_scroll_fill->GetWidth(),v);
	m_orb_scroll_lip->SetRect(0,m_orb_scroll_lip->GetTextureHeight()-v,m_orb_scroll_lip->GetWidth(),v - (v - lip_height));
	m_orb_scroll_lip->SetPosition(m_orb_fill->GetPosX(),m_orb_fill->GetHeight() - lip_height);
}

void PlayerHUD::HUD_Orb::Update(float vel){
	m_orb_scroll_fill->ScrollV(vel);
	m_orb_scroll_lip->ScrollH(-vel);
}
