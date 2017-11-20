#include <iostream>
#include "raknet/RakPeerInterface.h"
#include "raknet/MessageIdentifiers.h"
#include "raknet/BitStream.h"
#include "raknet/RakNetTypes.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "./Player.h"

#define MAX_CLIENTS 20
#define SERVER_PORT 60000

using namespace std;

enum GameMessages { //New enums for more events
	ID_PLAYER_JOIN = ID_USER_PACKET_ENUM + 1,
	ID_PLAYER_MOVE = ID_USER_PACKET_ENUM + 2,
	ID_EXISTING_PLAYER = ID_USER_PACKET_ENUM + 3,
	ID_PLAYER_DISCONNECT = ID_USER_PACKET_ENUM + 4,
	ID_PLAYER_CHANGE_COLOR = ID_USER_PACKET_ENUM + 5
};

// We get a random color (SEED MUST BE INITIALIZED!)
sf::Color getRandColor(){
	return(sf::Color(rand() % 255, rand() % 255, rand() % 255));
}

struct World{

	btBroadphaseInterface* broadphase;

	btDefaultCollisionConfiguration* collisionConfiguration;      
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	
	btDiscreteDynamicsWorld* dynamicsWorld;
	
	btCollisionShape* groundShape;

	btDefaultMotionState* groundMotionState;

    btRigidBody* groundRigidBody;

} didneyWorl;

void Create3DWorld(){

	//NEEDS FOR CREATING THE WORLD
	didneyWorl.broadphase = new btDbvtBroadphase();

	didneyWorl.collisionConfiguration = new btDefaultCollisionConfiguration();       
	didneyWorl.dispatcher = new btCollisionDispatcher(didneyWorl.collisionConfiguration);
	didneyWorl.solver = new btSequentialImpulseConstraintSolver;
	
	//INSTANTIATES THE DYNAMICS WORLD
	didneyWorl.dynamicsWorld = new btDiscreteDynamicsWorld(didneyWorl.dispatcher, didneyWorl.broadphase, didneyWorl.solver, didneyWorl.collisionConfiguration);
	
	//SETS THE GRAVITY
	didneyWorl.dynamicsWorld->setGravity(btVector3(0, -200, 0));
	
	//CREATES THE GROUND
	didneyWorl.groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	//GROUND INITIAL MOTION STATE
	didneyWorl.groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	//CREATE GROUND INFO
    btRigidBody::btRigidBodyConstructionInfo
        groundRigidBodyCI(0, didneyWorl.groundMotionState, didneyWorl.groundShape, btVector3(0, 0, 0));

    //AGREGATE THE GROUND INFO TO THE GROUND RIGID BODY
    didneyWorl.groundRigidBody = new btRigidBody(groundRigidBodyCI);

    //ADD THE GROUND TO THE WORLD
    didneyWorl.dynamicsWorld->addRigidBody(didneyWorl.groundRigidBody);

	
}

void Update3DWorld(){
	didneyWorl.dynamicsWorld->stepSimulation(1 / 60.f, 10); //TODO:: CAMBIAR LOS 200 FPS POR 60 FPS
}

void Erase3DWorld(){
  	//Clean up behind ourselves like good little programmers
    didneyWorl.dynamicsWorld->removeRigidBody(didneyWorl.groundRigidBody);
    delete didneyWorl.groundRigidBody->getMotionState();
    delete didneyWorl.groundRigidBody;

    delete didneyWorl.groundShape;

    delete didneyWorl.dynamicsWorld;
    delete didneyWorl.solver;
    delete didneyWorl.collisionConfiguration;
    delete didneyWorl.dispatcher;
    delete didneyWorl.broadphase;

}

int main() {
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet *packet;

	

	// We init the random seed
	srand(time(NULL));

	std::cout << "(C)lient or (S)erver: ";
	std::string input;
	std::cin >> input;

	if (input[0] == 'C' || input[0] == 'c') isServer = false;
	else isServer = true;
	



	if (isServer) {
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);

		peer->Startup(MAX_CLIENTS, &sd, 1);
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
		std::cout << "Server started. \n";
		



		std::map<RakNet::RakNetGUID, sf::Vector2f> players;
		std::map<RakNet::RakNetGUID, sf::Color> playersColor;
		while (1) {
			for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
				switch (packet->data[0]) {
				case ID_NEW_INCOMING_CONNECTION: {

					//CREATE A PLAYER FOR THE 3D WORLD
					//Player* physicPlayer = new Player();
					//physicPlayer->CreatePlayer(didneyWorl.dynamicsWorld);

					players[packet->guid] = sf::Vector2f();
					
					RakNet::BitStream stream; //Send the new player's GUID to other clients due to client~server model
					stream.Write((RakNet::MessageID)ID_PLAYER_JOIN);
					stream.Write(packet->guid);
					peer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true);

					for (auto &row : players) { //Update new player with all pre-existing characters and positions
						if (row.first == packet->guid) continue; //Check if guid is the new player's
						RakNet::BitStream update_plr;
						update_plr.Write((RakNet::MessageID)ID_EXISTING_PLAYER);
						update_plr.Write(row.first);
						update_plr.Write(row.second.x);
						update_plr.Write(row.second.y);

						peer->Send(&update_plr, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false);
					}
					break;

				}
				case ID_PLAYER_MOVE: {
					sf::Vector2f position;

					RakNet::BitStream stream(packet->data, packet->length, false);
					stream.IgnoreBytes(sizeof(RakNet::MessageID));
					stream.Read(position.x);
					stream.Read(position.y);
				
					players.find(packet->guid)->second = position; //Update player position;

					RakNet::BitStream posUpdate;
					posUpdate.Write((RakNet::MessageID)ID_PLAYER_MOVE); //Client getting "ID_PLAYER_MOVE", know's it's a different player
					posUpdate.Write(packet->guid); //Send Raknet ID so the client can update the correct player
					posUpdate.Write(position.x);
					posUpdate.Write(position.y);

					peer->Send(&posUpdate, LOW_PRIORITY, UNRELIABLE, 0, packet->guid, true); //Send to all clients except the one that moved
				
					break;
				}
				
				case ID_PLAYER_CHANGE_COLOR: {
					sf::Color newPlayerColor; 

					RakNet::BitStream stream(packet->data, packet->length, false);
					stream.IgnoreBytes(sizeof(RakNet::MessageID));
					stream.Read(newPlayerColor);
				
					playersColor.find(packet->guid)->second = newPlayerColor; //Update player color;

					RakNet::BitStream colorUpdate;
					colorUpdate.Write((RakNet::MessageID)ID_PLAYER_CHANGE_COLOR); //Client getting "ID_PLAYER_CHANGE_COLOR", know's it's a different player
					colorUpdate.Write(packet->guid); //Send Raknet ID so the client can update the correct player
					colorUpdate.Write(newPlayerColor);

					peer->Send(&colorUpdate, LOW_PRIORITY, UNRELIABLE, 0, packet->guid, true); //Send to all clients except the one that moved
				
					break;
				}

				case ID_CONNECTION_LOST: 
				case ID_DISCONNECTION_NOTIFICATION: {
					
					RakNet::BitStream bitstream;
					bitstream.Write((RakNet::MessageID)ID_PLAYER_DISCONNECT);
					bitstream.Write(packet->guid);

					players.erase(packet->guid);
					
					peer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
					break;
				}
				
				}
			}
		
		}

	}
	else {
		Create3DWorld();
		std::cout << "Client started. \n";

		// We set the IP manually
		std::string ip;
		int ip1, ip2, ip3, ip4;
		
		std::cout << "IP Chunk 1: "; std::cin >> ip1;
		std::cout << "IP Chunk 2: "; std::cin >> ip2;
		std::cout << "IP Chunk 3: "; std::cin >> ip3;
		std::cout << "IP Chunk 4: "; std::cin >> ip4;

		ip = to_string(ip1) + "." + to_string(ip2) + "." + to_string(ip3) + "." + to_string(ip4);
		cout << "FULL IP: "<< ip <<endl;
		// -------------------------------------

		IrrlichtDevice *device = createDevice( video::EDT_SOFTWARE, dimension2d<u32>(900, 600), 16, false, false, false, 0);
		if (!device) return 1;
		device->setWindowCaption(L"Irrlicht + Raknet + Bullet3D Demo");
		IVideoDriver* driver = device->getVideoDriver();
		ISceneManager* smgr = device->getSceneManager();

		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		peer->Connect(ip.c_str(), SERVER_PORT, 0, 0);

		std::map<RakNet::RakNetGUID, sf::RectangleShape> players;

		sf::RectangleShape player({ 40.0f, 40.0f });
		sf::Vector2f previousPos;
		player.setPosition(sf::Vector2f(430,270));

		Player* physicPlayer = new Player();
		physicPlayer->CreatePlayer(didneyWorl.dynamicsWorld);
		physicPlayer->SetPosX(player.getPosition().x);
		physicPlayer->SetPosY(player.getPosition().y);

		// We select its color and create the previous colro variable
		player.setFillColor(getRandColor());
		sf::Color previousColor;

		ISceneNode* node = smgr->addCubeSceneNode();
		if (node) {
			node->setMaterialFlag(EMF_LIGHTING, false);
			node->setMaterialTexture( 0, driver->getTexture("../../media/wall.bmp") );
		}
		smgr->addCameraSceneNode(0, vector3df(0,0,-100), vector3df(0,0,0)); // X(+ = Dcha), Y (+ = Arriba), Z (+ = Delante)


		sf::RenderWindow window(sf::VideoMode(900, 600), "Networking Test");
		window.setFramerateLimit(60);
		while (window.isOpen()){
			sf::Event e;
			while (window.pollEvent(e)) {
				if (e.type == sf::Event::Closed) {
					peer->Shutdown(300);
					window.close();
				}
			}
			for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
				switch (packet->data[0]) {
					case ID_PLAYER_JOIN: {
						RakNet::BitStream bitstream(packet->data, packet->length, false);
						RakNet::RakNetGUID id;


						bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
						bitstream.Read(id);

						sf::RectangleShape otherPlr({40.0f, 40.0f});

						// We put the default color
						otherPlr.setFillColor(sf::Color::Blue);

						players[id] = otherPlr;
						break;
					}
					case ID_EXISTING_PLAYER: {
						RakNet::BitStream bitstream(packet->data, packet->length, false);
						RakNet::RakNetGUID id;
						sf::Vector2f position;

						bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
						bitstream.Read(id);
						bitstream.Read(position.x);
						bitstream.Read(position.y);

						sf::RectangleShape otherPlr({ 40.0f, 40.0f });
						otherPlr.setPosition(position);
						otherPlr.setFillColor(sf::Color::Blue);
						players[id] = otherPlr;
						break;
					}
					case ID_PLAYER_MOVE: {
						RakNet::BitStream bitstream(packet->data, packet->length, false);
						RakNet::RakNetGUID id;
						sf::Vector2f position;

						bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
						bitstream.Read(id);
						bitstream.Read(position.x);
						bitstream.Read(position.y);

						players.find(id)->second.setPosition(position);
						
						break;
					}
					case ID_PLAYER_CHANGE_COLOR: {
						RakNet::BitStream bitstream(packet->data, packet->length, false);
						RakNet::RakNetGUID id;
						sf::Color otherPlayerNewColor;

						bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
						bitstream.Read(id);
						bitstream.Read(otherPlayerNewColor);

						players.find(id)->second.setFillColor(otherPlayerNewColor);
						
						break;
					}
					case ID_PLAYER_DISCONNECT: {
						RakNet::BitStream bitstream(packet->data, packet->length, false);
						RakNet::RakNetGUID id;

						bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
						bitstream.Read(id);
						
						players.erase(id);
						std::cout << "A player has disconnected. \n";
						break;
					}
					case ID_CONNECTION_LOST: {
						window.close();
						break;
					}
				
				}

			}
			if (window.hasFocus()) {

				Update3DWorld();
				physicPlayer->Update();
				player.setPosition(sf::Vector2f(physicPlayer->GetPosX(), (600 - (physicPlayer->GetHeight()*50)) - (physicPlayer->GetPosY())));
				float posY = physicPlayer->GetPosY();
				posY = node->getPosition().Y;
				cout<<posY<<endl;
				node->setPosition(vector3df(node->getPosition().X - 10.0f ,posY,node->getPosition().Z));

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					physicPlayer->Jump();
				}			
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					player.move({ -10.0f, 0.0f });
					physicPlayer->SetPosX(player.getPosition().x);
					node->setPosition(vector3df(node->getPosition().X - 1.0f ,node->getPosition().Y,node->getPosition().Z));
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					player.move({ 10.0f, 0.0f });
					physicPlayer->SetPosX(player.getPosition().x);
					node->setPosition(vector3df(node->getPosition().X + 1.0f ,node->getPosition().Y,node->getPosition().Z));
				}

				// We select its color randomly
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					player.setFillColor(getRandColor());
				}
			
			}

			if (player.getPosition() != previousPos) {
				RakNet::BitStream bitstream;
				bitstream.Write((RakNet::MessageID)ID_PLAYER_MOVE);
				bitstream.Write(player.getPosition().x);
				bitstream.Write(player.getPosition().y);

				peer->Send(&bitstream, LOW_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}
			// We send the new color
			if (player.getFillColor() != previousColor) {
				RakNet::BitStream bitstream;
				bitstream.Write((RakNet::MessageID)ID_PLAYER_CHANGE_COLOR);
				bitstream.Write(player.getFillColor());

				peer->Send(&bitstream, LOW_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}

			driver->beginScene(true, true, SColor(255,200,200,255)); // Color de borrado en ARGB
			smgr->drawAll();
			driver->endScene();

			window.clear(sf::Color::White);
			for (auto &row : players) {
				window.draw(row.second);
			}
			window.draw(player);
			window.display();
			previousPos = player.getPosition();
		}

	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	Erase3DWorld();

	return 0;
}