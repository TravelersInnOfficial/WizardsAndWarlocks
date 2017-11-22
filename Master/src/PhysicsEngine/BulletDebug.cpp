#include "BulletDebug.h"

void BulletDebug::setDriver(IVideoDriver *d){
	driver = d;
}

void BulletDebug::drawLine( const btVector3& from, const btVector3& to, const btVector3& color){
	// Codigo para pintar una linea
	SColorf fromC;
	fromC.set(color[0],color[1],color[2],color[3]);

	vector3df a(from.getX(), from.getY(), from.getZ());
	vector3df b(to.getX(), to.getY(), to.getZ());

	matrix4 id;
    id.makeIdentity();
    driver->setTransform(video::ETS_WORLD, id);
    driver->draw3DLine(a, b, fromC.toSColor());
}


void BulletDebug::drawContactPoint(const btVector3& PointOnB, const btVector3&  normalOnB, btScalar distance, int lifeTime, const btVector3& color){

}

void BulletDebug::reportErrorWarning(const char* warningString){

}

void BulletDebug::draw3dText(const btVector3& location, const char* textString){

}

void BulletDebug::setDebugMode(int debugMode){

}

int BulletDebug::getDebugMode() const {
		return 1;
}