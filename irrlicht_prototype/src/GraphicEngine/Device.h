#include <irrlicht/irrlicht.h>
#include "EventReceiver.h"
#include "VideoDriver.h"
#include "SceneManager.h"
#include "GUIEnvironment.h"
class Device{

public:
    /**
     * Returns Device of the engine. If it not created, this method initializes it.
     * receiver: event receiver to interact with device
    */
    static Device* getDevice(EventReceiver* receiver);
    
    /**
     * Gets pointer to video driver
    */
    VideoDriver* getVideoDriver();

    /**
     * Gets pointer to scene manager
    */
    SceneManager* getSceneManager();
    
    /**
     * Gets pointer to gui environment
    */
    GUIEnvironment* getGUIEnvironment();

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

private:
    Device();
    Device(EventReceiver* receiver);

    /** Irrlicht provisional device*/
    static irr::IrrlichtDevice* privateDevice;

};