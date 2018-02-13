#ifndef NETSEEKER_H
#define NETSEEKER_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <NetworkStructs.h>
#include "RakNetIncludes.h"

class NetSeeker{

public:
	NetSeeker(float timeToUpdateList = 1.0f);
	~NetSeeker();

	void Update(float deltaTime = 0);
	std::vector<ServerData> GetList();

private:
	std::vector<ServerData> servers;

	RakNet::RakPeerInterface *peer;
	RakNet::Packet *packet;
	RakNet::SocketDescriptor descriptor;

	float timeToUpdateList;
	float currentTimeLeft;

	void Ask();
	void Recieve();
	void RegisterServers(std::vector<ServerData> newServers);
	std::string TreatIp(std::string ip);
	int Find(ServerData toFind, std::vector<ServerData> where);
	void SendShutdown();

};

#endif