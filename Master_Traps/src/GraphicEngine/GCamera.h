#ifndef GCamera_H
#define GCamera_H

#include <irrlicht/irrlicht.h>
#include <iostream>
#include "GEntity.h"

class GCamera: public GEntity{

public:

    GCamera();

    friend class GraphicEngine;

private:

    GCamera(irr::scene::ISceneNode* node);    
  
};

#endif