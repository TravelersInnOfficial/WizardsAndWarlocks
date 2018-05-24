#include "./GRoom.h"
#include "./GPortal.h"
#include "GraphicEngine.h"
#include <TOcularEngine/TOcularEngine.h>

GRoom::GRoom(int id, vector3df position, vector3df rotation, vector3df scale){
	m_id = id;
	 TOEvector3df pos(position.X, position.Y, position.Z);
	 TOEvector3df rot(rotation.X, rotation.Y, rotation.Z);
	 TOEvector3df sca(scale.X, scale.Y, scale.Z);
	privateNode = GraphicEngine::getInstance()->privateSManager->AddRoom(pos, rot, sca);
}

GRoom::~GRoom(){
	int size = m_portals.size();
	for(int i=0; i<size; i++){
		delete m_portals[i];
	}
	m_portals.clear();

	GraphicEngine::getInstance()->privateSManager->DeleteRoom(privateNode);
}

int GRoom::GetId(){
	return m_id;
}

float GRoom::GetDistance(vector3df position){
	 TOEvector3df pos(position.X, position.Y, position.Z);
	float toRet = ((TFRoom*)privateNode)->GetDistance(pos);
	return toRet;
}

bool GRoom::AddChild(GEntity* entity){
	TFNode* node = entity->privateNode;
	return ((TFRoom*)privateNode)->AddChild(node);
}

bool GRoom::RemoveChild(GEntity* entity){
	return ((TFRoom*)privateNode)->RemoveChild(entity->privateNode);
}

GPortal* GRoom::AddConnection(GRoom* room, vector3df position, vector3df rotation, vector3df scale){
	TFRoom* connectRoom = (TFRoom*)room->privateNode;
	 TOEvector3df pos(position.X, position.Y, position.Z);
	 TOEvector3df rot(rotation.X, rotation.Y, rotation.Z);
	 TOEvector3df sca(scale.X, scale.Y, scale.Z);

	TFPortal* portal = ((TFRoom*)privateNode)->AddConnection(connectRoom, pos, rot, sca);
	GPortal* tfportal = new GPortal(portal);

	m_portals.push_back(tfportal);

	return tfportal;
}

bool GRoom::DeletePortal(GPortal* portal){
	int size = m_portals.size();
	for(int i=0; i<size; i++){
		if(portal == m_portals[i]){

			// Eliminamos el TFPortal
			TFRoom* room = (TFRoom*)privateNode;
			bool output = room->DeletePortal(portal->m_portal);

			// Eliminamos el GPortal
			delete m_portals[i];
			m_portals.erase(m_portals.begin() + i);

			return output;
		}
	}
	return false;
}

