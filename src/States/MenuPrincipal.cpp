#include "./MenuPrincipal.h"
#include <GraphicEngine/GraphicEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <GraphicEngine/MenuManager.h>

MenuPrincipal::MenuPrincipal(){
	g_engine = GraphicEngine::getInstance();
	g_engine->setCursorVisible(false);
	//g_engine->ToggleMenu(true);
	g_engine->InitReceiver();
	//selectedOption = NO_OPT;
	createSoundEvent();
	playMenuMusic();
	MenuManager::GetInstance()->CreateMenu(MAIN_M);
}

MenuPrincipal::~MenuPrincipal(){
	if (menuMusic->isPlaying()) menuMusic->stop();
	menuMusic->release();
	delete menuMusic;
	
	g_engine->setCursorVisible(false);

	MenuManager::GetInstance()->~MenuManager();
}

bool MenuPrincipal::Input(){
	if(!g_engine->run()) return true;
	if(g_engine->EscPressed()) return true;

	if(g_engine->IsKeyPressed(Key_A)) std::cout<<"funciona"<<std::endl;

	//selectedOption = g_engine->ReadButtonPressed();
	return false;
}

void MenuPrincipal::Update(float deltaTime){
	SoundSystem::getInstance()->Update();

	// En el caso de que se haya cambiado de opcion
	/*if(selectedOption != NO_OPT){
		// Dependiendo de la seleccion hacemos una cosa u otra
		if (menuMusic->isPlaying()) menuMusic->stop();
		switch(selectedOption){
			case MAIN_M_CLIENT:{
				PrepareClient();
				break;
			}
			case MAIN_M_SERVER:{
				//std::string path = "./WizardsAndWarlocks -i "+server_name +" &";
				std::string path = "./WizardsAndWarlocks -i &";
				--ifdef _WIN64
				path = "START /B WizardsAndWarlocks.exe -i"+server_name;
					path = "START /B WizardsAndWarlocks.exe -i";
				--endif
				std::system(path.c_str());
				PrepareClient(true);
				break;
			}
			default:{
				PrepareGame();
				break;
			}
		}
		g_engine->ToggleMenu(false);
	}*/

	MenuManager::GetInstance()->Update(deltaTime);
}

void MenuPrincipal::Draw(){
	g_engine->beginSceneDefault();
	
	//BACKGROUND IMAGE
    g_engine->draw2DImage(background, vector4df(0,0,g_engine->GetScreenWidth(),g_engine->GetScreenHeight()));

	MenuManager::GetInstance()->Draw();
	g_engine->endScene();
}

void MenuPrincipal::PrepareGame(){
	StateManager::GetInstance()->PrepareStatus(STATE_GAME);
}

void MenuPrincipal::PrepareClient(bool proprietary){
	NetworkEngine* n_engine;
	n_engine = NetworkEngine::GetInstance();
	//n_engine->SetIp(g_engine->ReadText(MAIN_M_IP));
	n_engine->StartClient(proprietary);
	//n_engine->GetClient()->SetClientName(g_engine->ReadText(MAIN_M_NAME));
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