#include "./MenuPrincipal.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GSprite.h>
#include <NetworkEngine/NetworkEngine.h>
#include "./../Managers/StateManager.h"
#include <GraphicEngine/MenuManager.h>
#include <SoundEngine/SoundSystem.h>
#include <NetworkStructs.h>
#include <Menus.h>

MenuPrincipal::MenuPrincipal(){
	background = "./../assets/textures/GUI/Menus/MainMenu/bkg/frame_";
	bkg_frame = 0;
	total_bkg_frames = 90;

	g_engine = GraphicEngine::getInstance();
	g_engine->ResetScene();
	g_engine->setCursorVisible(false);
	g_engine->ToggleMenu(true);
	g_engine->InitReceiver();
	//selectedOption = NO_OPT;
	createSoundEvent();
	playMenuMusic();
	MenuManager::GetInstance()->CreateMenu(MAIN_M);
	bkg = nullptr;
}

MenuPrincipal::~MenuPrincipal(){
	if (menuMusic->isPlaying()) menuMusic->stop();
	menuMusic->release();
	delete menuMusic;
	
	g_engine->setCursorVisible(false);

	MenuManager::GetInstance()->ClearMenu();
	
	if(bkg!=nullptr){
		delete bkg;
		bkg = nullptr;
	}
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
	if(bkg_frame>total_bkg_frames) bkg_frame = 0;
	if(bkg!=nullptr) bkg->SetTexture(background + std::to_string(bkg_frame) + ".jpg");
}

void MenuPrincipal::Draw(){
	g_engine->BeginDraw();
	MenuManager::GetInstance()->Draw();
	if(bkg == nullptr){
		vector2df dims(g_engine->GetScreenWidth(),g_engine->GetScreenHeight());
		
		bkg = GraphicEngine::getInstance()->addSprite(background + "0.jpg", vector2df(0,0), dims);
	}
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