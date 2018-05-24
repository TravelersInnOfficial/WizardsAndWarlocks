#include "./GPortal.h"
#include <TOcularEngine/Elements/TFPortal.h>

GPortal::GPortal(TFPortal* portal){
	m_portal = portal;
}

GPortal::~GPortal(){
	// El TFPortal se elimina al eliminar el TFRoom
}

void GPortal::SetCenter(vector3df center){
	 TOEvector3df input(center.X, center.Y, center.Z);
	m_portal->SetCenter(input);
}

void GPortal::SetRotation(vector3df rot){
	 TOEvector3df input(rot.X, rot.Y, rot.Z);
	m_portal->SetRotation(input);
}

void GPortal::SetSize(vector3df size){
	 TOEvector3df input(size.X, size.Y, size.Z);
	m_portal->SetSize(input);
}

void GPortal::SetVisible(bool visible){
	m_portal->SetVisible(visible);
}
