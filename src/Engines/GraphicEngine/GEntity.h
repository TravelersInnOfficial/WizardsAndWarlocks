#ifndef GENTITY_H
#define GENTITY_H


#include <vector3d.h>
#include <TravelersOcularEngine/src/TOcularEngine/TOcularEngine.h>

//fast forward declaration
class TFNode;
class GRoom;

class GEntity{
	friend class GRoom;
public:
	
	/**
	 * Set body position
	 * pos: vector x,y,z of body position
	*/
	void setPosition(vector3df pos);

	/**
	 * Set body rotation
	 * rot: vector x,y,z of body rotation
	*/
	void setRotation(vector3df rot);

	/**
	 * Rotates body
	 * rot: vector x,y,z of body rotation
	*/
	void Rotate(vector3df rot);
	
	/**
	 * Get body scale
	*/
	void setScale(vector3df scale);
	
	/**
	 * Get body position
	*/
	vector3df getPosition();

	/**
	 * Get body rotation
	*/
	vector3df getRotation();

	/**
	 * Set body scale
	*/
	vector3df getScale();

	/**
	 * Updates the absolute position based on the relative and the parents position of camera
	*/
	void updateAbsolutePosition();

	/**
	 * Enables or disables automatic culling based on the bounding box.
	*/
	void setAutomaticCulling();

protected:
	TFNode* privateNode;
	void Erase();
  
};

#endif