#include "BulletDebug.h"
#include <GraphicEngine/GraphicEngine.h>

void BulletDebug::drawLine( const btVector3& from, const btVector3& to, const btVector3& color){
	vector3df f(from.getX(), from.getY(), from.getZ());
	vector3df t(to.getX(), to.getY(), to.getZ());
	vector3df c(color.getX(), color.getY(), color.getZ());

	//std::cout<<from.getX()<<from.getY()<<from.getZ()<<std::endl;

    GraphicEngine::getInstance()->paintLineDebug(f, t, c);
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