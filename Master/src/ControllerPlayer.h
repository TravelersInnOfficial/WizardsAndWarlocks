#ifndef CONTROLLERPLAYER_H
#define CONTROLLERPLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include <Keycodes.h>
#include "./GraphicEngine/GraphicEngine.h"

class Key_player {
public:
	~Key_player();
	Key_player(TKEY_CODE k, std::string a);
  	void SetStatus(keyStatesENUM);
  	std::string GetAction();
  	keyStatesENUM GetStatus();
  	TKEY_CODE GetKey();
private:
	TKEY_CODE key;
	keyStatesENUM status;
	std::string action;
};

class ControllerPlayer{
public:
	ControllerPlayer();
	~ControllerPlayer();
	bool AddAction(TKEY_CODE key, std::string ac);
	bool IsKeyDown(int n);
	bool IsKeyDown(std::string a);
	bool IsKeyPressed(int n);
	bool IsKeyPressed(std::string a);
	bool IsMouseDown(int n);
	bool IsMouseDown(std::string a);
	void Update();
private:
	std::vector<Key_player*> keys;
};

#endif