#include "MenuManager.h"
#include "./../Players/Player.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"
#include <TrapCodes.h>

MenuManager* MenuManager::instance = 0;

MenuManager::MenuManager(){
	g_engine = GraphicEngine::getInstance();
	irr::core::dimension2du res = g_engine->privateDriver->getScreenSize();
	screenSize.X = res.Width;
	screenSize.Y = res.Height;
	netDebugWindow = NULL;
}

MenuManager::~MenuManager(){
	ClearMenu();
}

MenuManager* MenuManager::GetInstance(){
	if(instance == 0) instance = new MenuManager();
	return instance;
}

void MenuManager::CreateMenu(MenuType type, int option){
	switch(type){
		case(MAIN_M):{
			CreateMain();
			break;
		}
		case(ALLIANCE_M):{
			CreateAlliance();
			break;
		}
		case(ENDMATCH_M):{
			CreateMatchEnded(option);
			break;
		}
		case(NETDEBUG_M):{
			if(netDebugWindow == NULL) CreateNetDebug();
			else ClearMenu();
			break;
		}
		case(SELLER_M):{
			CreateSeller();
			break;
		}
		default:{
			break;
		}
	}
}

void MenuManager::ClearMenu(){
	int size = loadedOptions.size();
	for(int i=0; i<size; i++){
		irr::gui::IGUIElement* elem;
		elem = g_engine->privateGUIEnv->getRootGUIElement()->getElementFromId(loadedOptions[i], true);
		elem->remove();
	}
	loadedOptions.clear();
	netDebugWindow = NULL;
}

void MenuManager::ClearElement(MenuOption elementID){
	irr::gui::IGUIElement* elem;
	elem = g_engine->privateGUIEnv->getRootGUIElement()->getElementFromId(elementID, true);
	if(elem!= NULL) elem->remove();
}

void MenuManager::CreateMain(){

	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X/30,screenSize.Y/30,screenSize.X - screenSize.X/30, screenSize.Y - screenSize.Y/30);
	vector2di menuSize = vector2di(menuWindow.getWidth(), menuWindow.getHeight());

 	irr::gui::IGUIWindow* window = g_engine->privateGUIEnv->addWindow(
        menuWindow,
        false,
        L"Start your Game",
		0,
		MAIN_M_WINDOW
	);
	window->getCloseButton()->setVisible(false);
	window->setDraggable(false);
	loadedOptions.push_back(MAIN_M_WINDOW);

	vector4di rect = vector4di(menuSize.X/10.0f,menuSize.Y/3,menuSize.X/4.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Single Player", L"To play single player", MAIN_M_SINGLE, window);

	rect = vector4di(menuSize.X/1.59f,menuSize.Y/3.75f,menuSize.X/9,menuSize.Y/30);
	g_engine->addStaticText(rect, L"Multi Player", true, false, MAIN_M_TEXT_1, window);

	rect = vector4di(menuSize.X/2.3f,menuSize.Y/3,menuSize.X/4.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Conect to Game", L"To start a client (FILL IP)", MAIN_M_CLIENT, window);

	rect = vector4di(menuSize.X/1.4f,menuSize.Y/3,menuSize.X/4.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Host a Game", L"To start a server on this computer", MAIN_M_SERVER, window);

	rect = vector4di(menuSize.X/2.3f,menuSize.Y/1.4f,menuSize.X/4.5f,menuSize.Y/30);
	g_engine->addEditBox(rect, L"127.0.0.1", MAIN_M_IP, window);

	rect = vector4di(menuSize.X/2.3f,menuSize.Y/1.3f,menuSize.X/4.5f,menuSize.Y/30);
	g_engine->addEditBox(rect, L"Player Name", MAIN_M_NAME, window);
}

void MenuManager::CreateAlliance(){
	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X/30,screenSize.Y/30,screenSize.X - screenSize.X/30, screenSize.Y - screenSize.Y/30);
	vector2di menuSize = vector2di(menuWindow.getWidth(), menuWindow.getHeight());

 	irr::gui::IGUIWindow* window = g_engine->privateGUIEnv->addWindow(
        menuWindow,
        false,
        L"Select Alliance",
		0,
		ALLIANCE_M_WINDOW
	);
	window->setDraggable(false);
	window->getCloseButton()->setVisible(false);
	loadedOptions.push_back(ALLIANCE_M_WINDOW);

	vector4di rect = vector4di(menuSize.X/2-((menuSize.X/7.0f)/2),menuSize.Y/4,menuSize.X/7.0f,menuSize.Y/30);
	g_engine->addStaticText(rect, L"Select your Alliance, Human.", true, false, ALLIANCE_M_TEXT_1, window);
	
	rect = vector4di(menuSize.X/9.0f,menuSize.Y/3,menuSize.X/3.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Play as a Wizard", L"Be a Wizard and get back the Grail", ALLIANCE_M_WIZARD, window);

	rect = vector4di(menuSize.X/1.67f,menuSize.Y/3,menuSize.X/3.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Play as a Warlock", L"Be a Warlock and protect the Grail", ALLIANCE_M_WARLOCK, window);
}

void MenuManager::CreateSeller(){
	
	Player* playerOne = PlayerManager::GetInstance()->GetPlayerOne();
	if(playerOne == NULL) return;

	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X/30,screenSize.Y/30,screenSize.X - screenSize.X/30, screenSize.Y - screenSize.Y/30);
	vector2di menuSize = vector2di(menuWindow.getWidth(), menuWindow.getHeight());

 	irr::gui::IGUIWindow* window = g_engine->privateGUIEnv->addWindow(
        menuWindow,
        false,
        L"Select Spells",
		0,
		SELLER_M_WINDOW
	);
	window->setDraggable(false);
	window->getCloseButton()->setVisible(false);
	loadedOptions.push_back(SELLER_M_WINDOW);

	vector4di rect = vector4di(menuSize.X/2-((menuSize.X/2.0f)/2),menuSize.Y/15,menuSize.X/2.0f,menuSize.Y/30);
	g_engine->addStaticText(rect, L"Select the SPELLS and TRAPS you will fight with.", true, false, SELLER_M_TEXT_1, window);

	// BUTTONS FOR OUR PLAYER SPELLS
	float W =			menuSize.X;		// Ancho
	float H =			menuSize.Y;		// Alto
	float sizeBox =		W * 0.075;		// Tamanyo de los cuadrados del hechizo
	float xInit =		W * 0.252;		// X inicial del primer hechizo
	float yInit =		H * 0.14;		// Y inicial
	float space =		W * 0.03;		// Espacio entre hechizos
	float outline =		5;				// Borde de los hechizo

	std::vector<Hechizo*> spells = SpellManager::GetInstance()->GetSpells(playerOne);
	std::vector<MenuOption> menuOptions;
	menuOptions.push_back(SELLER_M_PS_2); menuOptions.push_back(SELLER_M_PS_3); menuOptions.push_back(SELLER_M_PS_4);
	for(int i = 1; i < spells.size(); i++){
		float xInitSpell = xInit + (sizeBox + space) * i;
		vector4df sizeImage(xInitSpell + outline, yInit + outline, sizeBox - outline, sizeBox - outline);
		vector4di finalSizeImage(sizeImage.X, sizeImage.Y, sizeImage.X2, sizeImage.Y2);
		std::string texturePath = spells[i]->GetHUDTexturePath();
		g_engine->addButton(finalSizeImage, L"", L"CURRENT SPELL", menuOptions.at(i-1), window, texturePath);
	}

	// BUTTONS FOR ALL THE SPELLS AVALIABLE
	sizeBox =	W * 0.075;
	xInit =		W * 0.147;
	yInit =		H * 0.28;
	space =		W * 0.03;
	outline =	5;

	menuOptions.clear();
	menuOptions.push_back(SELLER_SPEED); menuOptions.push_back(SELLER_DEFENSE); menuOptions.push_back(SELLER_INVISIBILITY); menuOptions.push_back(SELLER_UNTARGET); menuOptions.push_back(SELLER_FIRE); menuOptions.push_back(SELLER_POISON); menuOptions.push_back(SELLER_THUNDER); menuOptions.push_back(SELLER_TELEPORT); menuOptions.push_back(SELLER_CLEANSE); menuOptions.push_back(SELLER_WALL); menuOptions.push_back(SELLER_DUMMY); menuOptions.push_back(SELLER_TELEPORTBASE); menuOptions.push_back(SELLER_BLIZZAR);

	std::vector<SPELLCODE> kinds_spell;
	kinds_spell.push_back(SPELL_SPEED); kinds_spell.push_back(SPELL_DEFENSE); kinds_spell.push_back(SPELL_INVISIBILITY); kinds_spell.push_back(SPELL_UNTARGET); kinds_spell.push_back(SPELL_FIRE); kinds_spell.push_back(SPELL_POISON); kinds_spell.push_back(SPELL_THUNDER); kinds_spell.push_back(SPELL_TELEPORT); kinds_spell.push_back(SPELL_CLEANSE); kinds_spell.push_back(SPELL_WALL); kinds_spell.push_back(SPELL_DUMMY); kinds_spell.push_back(SPELL_TELEPORTBASE); kinds_spell.push_back(SPELL_BLIZZARD);


	int cut = 7;
	for(int i = 0; i < menuOptions.size(); i++){
		float xInitSpell = xInit + (sizeBox + space) * i;

		if(i == cut) yInit = yInit + sizeBox + space/2;
		if(i >= cut) xInitSpell = xInit + (sizeBox + space) * (i - cut);

		vector4di finalSizeImage = vector4di(xInitSpell + outline, yInit + outline, sizeBox - outline, sizeBox - outline);
		std::string texturePath = SpellManager::GetInstance()->GetPathFromEnum(kinds_spell.at(i));
		g_engine->addButton(finalSizeImage, L"", L"NEW SPELL", menuOptions.at(i), window, texturePath);
	}

	// BUTTON FOR OUR PLAYER TRAPS
	xInit =		W * 0.5 - sizeBox/2;		// X inicial del primer hechizo
	yInit =		H * 0.57;					// Y inicial

	TrapManager* trapManager = TrapManager::GetInstance();
	std::string texturePath = trapManager->GetPathFromEnum(trapManager->getPlayerTrap(playerOne));

	vector4di finalSizeImage = vector4di(xInit + outline, yInit + outline, sizeBox - outline, sizeBox - outline);
	g_engine->addButton(finalSizeImage, L"", L"CURRENT TRAP", SELLER_M_TRAP, window, texturePath);

	// BUTTON FOR ALL TRAPS AVALIABLE
	sizeBox =	W * 0.075;
	xInit =		W * 0.2;
	yInit =		H * 0.7;
	space =		W * 0.03;
	outline =	5;

	menuOptions.clear();
	menuOptions.push_back(SELLER_DEATH_CLAWS); menuOptions.push_back(SELLER_SPIRITS); menuOptions.push_back(SELLER_SILENCE); menuOptions.push_back(SELLER_TAXES); menuOptions.push_back(SELLER_DISTURBANCE); menuOptions.push_back(SELLER_EXPLOSIVE);
	
	std::vector<TrapEnum> kinds;
	kinds.push_back(TENUM_DEATH_CLAWS); kinds.push_back(TENUM_SPIRITS); kinds.push_back(TENUM_SILENCE); kinds.push_back(TENUM_TAXES); kinds.push_back(TENUM_DISTURBANCE); kinds.push_back(TENUM_EXPLOSIVE);

	for(int i = 0; i < menuOptions.size(); i++){
		float xInitSpell = xInit + (sizeBox + space) * i;
		vector4di finalSizeImage = vector4di(xInitSpell + outline, yInit + outline, sizeBox - outline, sizeBox - outline);
		std::string texturePath = TrapManager::GetInstance()->GetPathFromEnum(kinds.at(i));
		g_engine->addButton(finalSizeImage, L"", L"NEW TRAP", menuOptions.at(i), window, texturePath);
	}

	// BUTTON FOR ACEPTAR
	rect = vector4di(menuSize.X/2-((menuSize.X/2.0f)/2),menuSize.Y*0.85,menuSize.X/2.0f,menuSize.Y*0.1);
	g_engine->addButton(rect, L"ACCEPT", L"Accept and go back to Lobby", SELLER_M_ACCEPT, window);


}

void MenuManager::CreateMatchEnded(int option){
	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X/30,screenSize.Y/30,screenSize.X - screenSize.X/30, screenSize.Y - screenSize.Y/30);
	vector2di menuSize = vector2di(menuWindow.getWidth(), menuWindow.getHeight());

 	irr::gui::IGUIWindow* window = g_engine->privateGUIEnv->addWindow(
        menuWindow,
        false,
        L"Match Ended",
		0,
		ENDMATCH_M_WINDOW
	);
	window->setDraggable(false);
	window->getCloseButton()->setVisible(false);
	loadedOptions.push_back(ENDMATCH_M_WINDOW);

	std::wstring winner = L"THE WIZARDS WON!";
	if (option != (int)ALLIANCE_WIZARD) winner = L"THE WARLOCKS WON!";

	vector4di rect = vector4di(menuSize.X/2-((menuSize.X/9.0f)/2),menuSize.Y/4,menuSize.X/9.0f,menuSize.Y/30);
	g_engine->addStaticText(rect, winner, true, false, ENDMATCH_M_TEXT_1, window);

	rect = vector4di(menuSize.X/2-((menuSize.X/3.0f)/2),menuSize.Y/3,menuSize.X/3.0f,menuSize.Y/3);
	g_engine->addButton(rect, L"Accept", L"Go Back to the Lobby", ENDMATCH_M_CONFIRM, window);
}

void MenuManager::CreateNetDebug(){
	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X - 300, 0, screenSize.X, 200);

 	netDebugWindow = g_engine->privateGUIEnv->addWindow(
        menuWindow,
        false,
        L"Net Debug",
		0,
		NETDEBUG_M_WINDOW
	);
	netDebugWindow->setDraggable(false);
	netDebugWindow->getCloseButton()->setVisible(false);
	loadedOptions.push_back(NETDEBUG_M_WINDOW);

	UpdateNetDebug();
}

void MenuManager::UpdateNetDebug(){

	std::vector<Player*> players = PlayerManager::GetInstance()->GetAllPlayers();

	if(netDebugWindow != NULL){

		ClearElement(NETDEBUG_M_P1); ClearElement(NETDEBUG_M_P2); ClearElement(NETDEBUG_M_P3); ClearElement(NETDEBUG_M_P4); ClearElement(NETDEBUG_M_P5); ClearElement(NETDEBUG_M_P6); ClearElement(NETDEBUG_M_P7); ClearElement(NETDEBUG_M_P8);	
		vector2di menuSize = vector2di(netDebugWindow->getClientRect().getWidth(), netDebugWindow->getClientRect().getHeight());
		Player* player = NULL;

		std::vector<std::wstring> players_ws;
		std::wstring player1; std::wstring player2; std::wstring player3; std::wstring player4; std::wstring player5; std::wstring player6; std::wstring player7; std::wstring player8;
		players_ws.push_back(player1); players_ws.push_back(player2); players_ws.push_back(player3); players_ws.push_back(player4); players_ws.push_back(player5); players_ws.push_back(player6); players_ws.push_back(player7); players_ws.push_back(player8);

		std::vector<MenuOption> players_mo;
		players_mo.push_back(NETDEBUG_M_P1); players_mo.push_back(NETDEBUG_M_P2); players_mo.push_back(NETDEBUG_M_P3); players_mo.push_back(NETDEBUG_M_P4); players_mo.push_back(NETDEBUG_M_P5); players_mo.push_back(NETDEBUG_M_P6); players_mo.push_back(NETDEBUG_M_P7); players_mo.push_back(NETDEBUG_M_P8);

		for(int i = 0; i < 8; i++){
			if(players.size() > i){
				player = players.at(i);
				if(player != NULL){
					std::string ps = GetStringFromPlayer(player);
					players_ws.at(i) = std::wstring(ps.length(), L' ');
					std::copy(ps.begin(), ps.end(), players_ws.at(i).begin());
				}
			}
			else players_ws.at(i) = L"PLAYER NOT CONNECTED";
			vector4di rect = vector4di(0, menuSize.Y/8 * (i + 1), menuSize.X, menuSize.Y/8);
			g_engine->addStaticText(rect, players_ws.at(i), true, false, players_mo.at(i), netDebugWindow);
		}
	}
}

std::string MenuManager::GetStringFromPlayer(Player* player){
	std::string toRet = player->GetName()
						+ "     |     "
						+ std::to_string((int)player->GetHP())
						+" HP / "
						+ std::to_string((int)player->GetMP())
						+" MP /"
						+ std::to_string((int)player->GetSP())
						+" SP.     |     Char: "
						+ std::to_string(player->GetHasCharacter())
						+ "     |     N_ID: "
						+ std::to_string(player->GetNetworkObject()->GetObjId());
	return(toRet);
}