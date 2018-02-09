#ifndef NETWORKENGINE_H
#define NETWORKENGINE_H

#include <iostream>
#include <NetworkStructs.h>
#include "RakNetIncludes.h"
#include "NetworkObject.h"
#include "Server.h"
#include "Client.h"

class NetworkEngine{

public:

	// Returns Instance of the engine. If it not created, this method initializes it.
	static NetworkEngine* GetInstance(bool serverCreatedFromGame = false);

	~NetworkEngine();
	
	// Start a Server, must declare port before
	// Or
	// Start a Client, must declare IP && port before
	void StartServer(bool serverCreatedFromGame = false);
	void StartClient();

	// We retrieve our SERVER or the CLIENT to operate
	Server* GetServer();
	Client* GetClient();

	// We recieve the packages of our peer
	void Update(float deltaTime = 0, bool isLobby = false);

	// Manage the IP we're going to send the packages to
	// "127.0.0.1" format
	void SetIp(std::string newIp);
	std::string GetIp();

	// Manage the port we're listening or talking to
	void SetPort(int newPort);
	int GetPort();

	// We return the server/clietn started status
	bool IsServerInit();
	bool IsClientInit();

	// We end the Server or Client service
	void EndService();

private:
	
	static const int maxClients = 8;	// Max clients on the game (4v4 max)

	std::string serverIp;				// IP to connect to (LAN Broadcast by default)
	int serverPort;						// Port to lisen and talk to (60000 by default)
	bool isServer;
	bool isClient;

	Server* server;						// Server variable (uninitialized)
	Client* client;						// Client variable (uninitialized)

	NetworkEngine(bool serverCreatedFromGame = false);

};

#endif