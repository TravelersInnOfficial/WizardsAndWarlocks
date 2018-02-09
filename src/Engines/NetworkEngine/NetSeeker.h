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
	std::vector<std::string> GetList();

private:
	std::vector<std::string> lobbys;

	RakNet::RakPeerInterface *peer;
	RakNet::Packet *packet;
	RakNet::SocketDescriptor descriptor;

	float timeToUpdateList;
	float currentTimeLeft;

	void Ask();
	void Recieve();
	void RegisterIp(std::vector<std::string> ips);
	std::string TreatIp(std::string ip);
	int Find(std::string toFind, std::vector<std::string> where);
	void SendShutdown();

};

#endif