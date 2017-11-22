#ifndef GCamera_H
#define GBODY_H

#include <irrlicht/irrlicht.h>
#include <iostream>
#include "vector3d.h"
#include "GEntity.h"

class GCamera: public GEntity{

public:

    GCamera();

    friend class GraphicEngine;

private:

    GCamera(irr::scene::ISceneNode* node);    
  
};

#endif