#ifndef GPORTAL_H
#define GPORTAL_H

#include <vector3d.h>

class TFPortal;

class GPortal{
	friend class GRoom;
public:
	void SetVisible(bool visible);
	void SetCenter(vector3df center);
	void SetRotation(vector3df rot);
	void SetSize(vector3df size);
private:
	TFPortal* m_portal;

	GPortal(TFPortal* portal);
	~GPortal();
};

#endif