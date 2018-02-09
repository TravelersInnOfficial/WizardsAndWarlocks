#include "./MenuPrincipal.h"
#include <GraphicEngine/GraphicEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <GraphicEngine/MenuManager.h>


MenuPrincipal::MenuPrincipal(){
	g_engine = GraphicEngine::getInstance();
	g_engine->setCursorVisible(true);
	g_engine->ToggleMenu(true);
	g_engine->InitReceiver();
	selectedOption = NO_OPT;
	MenuManager::GetInstance()->CreateMenu(MAIN_M);
	netSeeker = new NetSeeker();
}

MenuPrincipal::~MenuPrincipal(){
	g_engine->setCursorVisible(false);
	delete netSeeker;
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
		// Dependiendo de la seleccion hacemos una cosa u otra
		switch(selectedOption){
			case MAIN_M_CLIENT:{
				PrepareClient();
				break;
			}
			case MAIN_M_SERVER:{
				std::string path = "./WizardsAndWarlocks -i &";
				#ifdef _WIN64
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
		MenuManager::GetInstance()->ClearMenu();
		g_engine->ToggleMenu(false);
	}
}

void MenuPrincipal::UpdateSeeker(float deltaTime){
	netSeeker->Update(deltaTime);
	std::vector<std::string> newLobbyList = netSeeker->GetList();
	if(lobbyList.size() != newLobbyList.size()){
		std::cout<<"----------------------"<<std::endl;
		std::cout<<"NEW LIST OF LOBBYS: "<<std::endl;
		
		if(newLobbyList.size() == 0) std::cout<<"Currently there's no Lobbys"<<std::endl;
		else{
			for(int i = 0; i < newLobbyList.size(); i++){
				std::cout<<"LOBBY Nº "<< i+1 <<" - IP: "<<newLobbyList.at(i)<<std::endl;
			}
		}
		
		std::cout<<"----------------------"<<std::endl;
		lobbyList = newLobbyList;
	}
}

void MenuPrincipal::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAllGUI();
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