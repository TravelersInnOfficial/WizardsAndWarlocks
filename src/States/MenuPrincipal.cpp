#include "./MenuPrincipal.h"
#include <GraphicEngine/GraphicEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <GraphicEngine/MenuManager.h>

MenuPrincipal::MenuPrincipal(){
	g_engine = GraphicEngine::getInstance();
	g_engine->setCursorVisible(false);
	g_engine->ToggleMenu(true);
	g_engine->InitReceiver();
	selectedOption = NO_OPT;
	createSoundEvent();
	playMenuMusic();
	MenuManager::GetInstance()->CreateMenu(MAIN_M);

	netSeeker = new NetSeeker();
}

MenuPrincipal::~MenuPrincipal(){
	if (menuMusic->isPlaying()) menuMusic->stop();
	menuMusic->release();
	delete menuMusic;
	
	g_engine->setCursorVisible(false);
	delete netSeeker;

	MenuManager::GetInstance()->~MenuManager();
}

bool MenuPrincipal::Input(){
	if(!g_engine->run()) return true;
	if(g_engine->EscPressed()) return true;

	selectedOption = g_engine->ReadButtonPressed();
	return false;
}

void MenuPrincipal::Update(float deltaTime){
	UpdateSeeker(deltaTime);
	SoundSystem::getInstance()->Update();

	// En el caso de que se haya cambiado de opcion
	if(selectedOption != NO_OPT){
		std::cout<<"SELECTED OPTION: "<<selectedOption<<std::endl;
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
				#ifdef _WIN64
				//path = "START /B WizardsAndWarlocks.exe -i"+server_name;
					path = "START /B WizardsAndWarlocks.exe -i";
				#endif
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
	}

	MenuManager::GetInstance()->Update(deltaTime);
}

void MenuPrincipal::UpdateSeeker(float deltaTime){
	//netSeeker->Update(deltaTime);
	/*
	std::vector<ServerData> newServerList = netSeeker->GetList();
	if(serverList.size() != newServerList.size()){
		std::cout<<"----------------------"<<std::endl;
		std::cout<<"NEW LIST OF SERVERS: "<<std::endl;
		
		if(newServerList.size() == 0) std::cout<<"No server available"<<std::endl;
		else{
			for(int i = 0; i < newServerList.size(); i++){
				std::cout<<newServerList.at(i).name<<" - IP: "<<newServerList.at(i).ip;
				std::cout<<":60000. Players: "<<newServerList.at(i).playerCount<<"/8";
				std::cout<<". Game Started: "<<!newServerList.at(i).lobbyState<<"."<<std::endl;
			}
		}
		
		std::cout<<"----------------------"<<std::endl;
		serverList = newServerList;
	}*/
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
	n_engine->SetIp(g_engine->ReadText(MAIN_M_IP));
	n_engine->StartClient(proprietary);
	n_engine->GetClient()->SetClientName(g_engine->ReadText(MAIN_M_NAME));
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