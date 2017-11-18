#include "Device.h"

static Device* instance;
static VideoDriver* driverInstance;
static SceneManager* sceneManagerInstance;
static GUIEnvironment* guiEnvironmentInstance;

Device::Device(){
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

Device::Device(EventReceiver* receiver){
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

Device* Device::getDevice(EventReceiver* receiver){
    if(instance == 0){
        instance = new Device(receiver);
    }
    return instance;
}

VideoDriver* Device::getVideoDriver(){
    if(driverInstance == 0){
        driverInstance = new VideoDriver(privateDevice->getVideoDriver());
    }

    return driverInstance;
}

SceneManager* Device::getSceneManager(){
    if(sceneManagerInstance == 0){
        sceneManagerInstance = new SceneManager(privateDevice->getSceneManager());
    }

    return sceneManagerInstance;
}

GUIEnvironment* Device::getGUIEnvironment(){
    if(guiEnvironmentInstance == 0){
        guiEnvironmentInstance = new GUIEnvironment(privateDevice->getGUIEnvironment());
    }

    return guiEnvironmentInstance;
}

bool Device::run(){
    return privateDevice->run();
}

bool Device::drop(){
    return privateDevice->drop();
}

void Device::setCursorVisible(bool visible){
    privateDevice->getCursorControl()->setVisible(visible);
}