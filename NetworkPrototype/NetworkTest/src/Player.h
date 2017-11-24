#ifndef PLAYER_H
#define PLAYER_H

#include <btBulletDynamicsCommon.h>
#include <SFML/Graphics.hpp>
#include <iostream>

class Player{
public:

	Player();

	void CreatePlayer(btDiscreteDynamicsWorld* );
	void DeletePlayer(btDiscreteDynamicsWorld* );
	void Update();
	void Jump();
	float GetPosX();
	float GetPosY();
	float GetWidth();
	float GetHeight();
	float GetLength();
	void SetPosX(float);
	void SetPosY(float);

	~Player();

private:

	float m_posX;
	float m_posY;
	float m_width;
	float m_height;
	float m_length;
	sf::RectangleShape* m_shape;
	btRigidBody* m_playerRigidBody;
	btCollisionShape* m_playerShape;
	btScalar playerMass;
};

#endif