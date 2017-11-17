#include "TravelersDevice.h"

static TravelersDevice* instance;
static TVideoDriver* driverInstance;

TravelersDevice::TravelersDevice(){
    privateDevice = irr::createDevice(
        irr::video::EDT_OPENGL,
        irr::core::dimension2d<irr::u32>(640, 480),
        16,
        false,
        false,
        false,
        0
    );
}

TravelersDevice::TravelersDevice(TEventReceiver* receiver){
    privateDevice = irr::createDevice(
        irr::video::EDT_OPENGL,
        irr::core::dimension2d<irr::u32>(640, 480),
        16,
        false,
        false,
        false,
        receiver
    );
}

TravelersDevice* TravelersDevice::getTravelersDevice(TEventReceiver* receiver){
    if(instance == 0){
        instance = new TravelersDevice(receiver);
    }
    return instance;
}

TVideoDriver* TravelersDevice::getVideoDriver(){
    if(driverInstance == 0){
        privateDevice->getVideoDriver();
    }

    return driverInstance;
}