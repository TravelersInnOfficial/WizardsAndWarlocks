#include "./MenuPrincipal.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GSprite.h>
#include <NetworkEngine/NetworkEngine.h>
#include "./../Managers/StateManager.h"
#include <GraphicEngine/MenuManager.h>
#include <SoundEngine/SoundSystem.h>
#include <NetworkStructs.h>
#include <Menus.h>
#include <Assets.h>

MenuPrincipal::MenuPrincipal(){

	g_engine = GraphicEngine::getInstance();
	g_engine->ResetScene();
	g_engine->setCursorVisible(false);
	g_engine->ToggleMenu(true);
	g_engine->InitReceiver();
	//selectedOption = NO_OPT;
	createSoundEvent();
	playMenuMusic();
	
	bkg_frame = (int) TEXTURE_MAINMENU_BKG_0;
	total_bkg_frames = bkg_frame + 89;
	vector2df dims(g_engine->GetScreenWidth(),g_engine->GetScreenHeight());	
	bkg = g_engine->addSprite(TEXTUREMAP[(GAMETEXTURES) bkg_frame], vector2df(0,0), dims);
	bkg_frame++;

	MenuManager::GetInstance()->CreateMenu(MAIN_M);
}

MenuPrincipal::~MenuPrincipal(){
	if (menuMusic->isPlaying()) menuMusic->stop();
	menuMusic->release();
	delete menuMusic;
	
	g_engine->setCursorVisible(false);

	MenuManager::GetInstance()->ClearMenu();
	
	delete bkg;
}

bool MenuPrincipal::Input(){
	if(!g_engine->run()) return true;
	if(g_engine->IsKeyPressed(Key_Escape)) {
		return true;
	}
	return false;
}

void MenuPrincipal::Update(float deltaTime){
	SoundSystem::getInstance()->Update();
	MenuManager::GetInstance()->Update(deltaTime);

	bkg_frame++;
	if(bkg_frame>total_bkg_frames) bkg_frame = (int) TEXTURE_MAINMENU_BKG_0;
	bkg->SetTexture(TEXTUREMAP[ (GAMETEXTURES) bkg_frame]);
}

void MenuPrincipal::Draw(){
	g_engine->BeginDraw();
	MenuManager::GetInstance()->Draw();
	g_engine->EndDraw();
}

void MenuPrincipal::PrepareGame(){
	StateManager::GetInstance()->PrepareStatus(STATE_GAME);
}

void MenuPrincipal::PrepareClient(bool proprietary){
	NetworkEngine* n_engine;
	n_engine = NetworkEngine::GetInstance();
	n_engine->StartClient(proprietary);
	StateManager::GetInstance()->PrepareStatus(STATE_NETGAME_CLIENT);
}

void MenuPrincipal::PrepareServer(){
	StateManager::GetInstance()->PrepareStatus(STATE_NETGAME_SERVER);
}

void MenuPrincipal::createSoundEvent() {
	menuMusic = SoundSystem::getInstance()->createEvent("event:/Music/Menu Music");
}

void MenuPrincipal::playMenuMusic() {
	SoundSystem::getInstance()->playEvent(menuMusic);
}