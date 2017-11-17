#include <irrlicht/irrlicht.h>
#include "TEventReceiver.h"
#include "TVideoDriver.h"


class TravelersDevice{

public:
    /**
     * Returns Device of the engine. If it not created, this method initializes it.
    */
    static TravelersDevice* getTravelersDevice(TEventReceiver* receiver);
    TVideoDriver* getVideoDriver();

private:
    TravelersDevice();
    TravelersDevice(TEventReceiver* receiver);

    /** Irrlicht provisional device*/
    static irr::IrrlichtDevice* privateDevice;

};