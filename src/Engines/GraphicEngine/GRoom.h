#ifndef GROOM_H
#define GROOM_H

#include "GEntity.h"
#include <vector3d.h>

class GPortal;
class TFRoom;
class TFPortal;

class GRoom: public GEntity{
public:
	GRoom(int id, vector3df position, vector3df rotation, vector3df scale);
	~GRoom();

	int GetId();
	float GetDistance(vector3df position);
	bool AddChild(GEntity* element);
	bool RemoveChild(GEntity* element);
	GPortal* AddConnection(GRoom* room, vector3df position, vector3df rotation, vector3df scale);
	bool DeletePortal(GPortal* portal);
private:
	int m_id;
	std::vector<GPortal*> m_portals;
};

#endif