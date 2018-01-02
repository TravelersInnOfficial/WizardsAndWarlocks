#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <iostream>
#include <vector>
#include <string>
#include <Keycodes.h>
#include <Actions.h>
#include "./../GraphicEngine/GraphicEngine.h"

class Key_player {
public:
	~Key_player();
	Key_player(TKEY_CODE k, ACTION_ENUM a);
  	void SetStatus(keyStatesENUM);
  	void SetAction(ACTION_ENUM action);
  	ACTION_ENUM GetAction();
  	keyStatesENUM GetStatus();
  	TKEY_CODE GetKey();
private:
	TKEY_CODE key;
	keyStatesENUM status;
	ACTION_ENUM action;
};

class PlayerController{
public:
	PlayerController();
	~PlayerController();
	bool AddAction(TKEY_CODE key, ACTION_ENUM ac);
	bool SetStatus(ACTION_ENUM ac, keyStatesENUM st);
	bool IsKeyDown(int n);
	bool IsKeyDown(ACTION_ENUM a);
	bool IsKeyPressed(int n);
	bool IsKeyPressed(ACTION_ENUM a);
	bool IsKeyReleased(int n);
	bool IsKeyReleased(ACTION_ENUM a);
	bool IsKeyUp(int n);
	bool IsKeyUp(ACTION_ENUM a);
	void Update();
	void UpdateOwnStatus();
	void SwapActions(ACTION_ENUM a, ACTION_ENUM b);
	void SetAllStatus(keyStatesENUM status);
private:
	std::vector<Key_player*> keys;
};

#endif