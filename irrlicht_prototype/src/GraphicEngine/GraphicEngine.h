#ifndef GRAPHICENGINE_H
#define GRAPHICENGINE_H

#include <irrlicht/irrlicht.h>
#include "GBody.h"
#include "vector3d.h"

class GraphicEngine{

public:
    /**
     * Returns Device of the engine. If it not created, this method initializes it.
     * receiver: event receiver to interact with device
    */
    static GraphicEngine* getInstance();
    
    /**
     * Executed in main while loop of the game
    */
    bool run();
    
    /**
     * Drops the device to finish program
    */
    bool drop();

    /**
     * Hides or shows mouse cursor
    */
    void setCursorVisible(bool visible);

    /**
     * Adds a cube to scene and returns body
    */
    GBody* addCube2Scene(
        vector3df position = vector3df(0,0,0), 
        vector3df rotation = vector3df(0,0,0), 
        vector3df scale = vector3df(1,1,1),
        float size = 10.f,
        int id = -1
        );


private:
    GraphicEngine();

    /** Irrlicht provisional device*/
    irr::IrrlichtDevice*        privateDevice;
    irr::video::IVideoDriver*   privateDriver;
    irr::scene::ISceneManager*  privateSManager;
    irr::gui::IGUIEnvironment*  privateGUIEnv;

};

#endif