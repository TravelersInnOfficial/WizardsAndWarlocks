#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "./PhysicsEngine/BT_Body.h"
#include <irrlicht/irrlicht.h>
using namespace std;

class Player{
	public:

		Player();

		void CreatePlayer(irr::scene::ISceneManager*, irr::IrrlichtDevice*, irr::video::IVideoDriver*);
		void DeletePlayer();
		void Update(irr::scene::ISceneManager*);
		void Jump();
		float GetPosX();
		float GetPosY();
		float GetWidth();
		float GetHeight();
		float GetLength();
		void MoveX(int);
		void MoveZ(int);

		~Player();

	private:

		float m_posX;
		float m_posY;
		float m_posZ;
		float m_width;
		float m_height;
		float m_length;
		float m_mass;

		BT_Body* bt_player;

		irr::scene::IAnimatedMesh* m_playerMesh;
		irr::scene::ISceneNode* m_playerNode;
};

#endif