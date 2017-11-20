#include "GraphicEngine.h"
#include "EventReceiver.h"

static GraphicEngine* instance;

GraphicEngine::GraphicEngine(){
   	EventReceiver *receiver = new EventReceiver();

    privateDevice = irr::createDevice(
        irr::video::EDT_OPENGL,
        irr::core::dimension2d<irr::u32>(640, 480),
        16,
        false,
        false,
        false,
        receiver
    );

    privateDriver = privateDevice->getVideoDriver();
    privateSManager = privateDevice->getSceneManager();
    privateGUIEnv = privateDevice->getGUIEnvironment();
}

GraphicEngine* GraphicEngine::getInstance(){
    if(instance == 0){
        instance = new GraphicEngine();
    }
    return instance;
}

bool GraphicEngine::run(){
    return privateDevice->run();
}

bool GraphicEngine::drop(){
    return privateDevice->drop();
}

void GraphicEngine::setCursorVisible(bool visible){
    privateDevice->getCursorControl()->setVisible(visible);
}

GBody* GraphicEngine::addCube2Scene(vector3df p, vector3df r, vector3df s, float size, int id){
    return new GBody(
        privateSManager->addCubeSceneNode(
            size,   //size
            0,      //parent
            id,     //id
            irr::core::vector3df(p.X, p.Y, p.Z),    //position
            irr::core::vector3df(r.X, r.Y, r.Z),    //rotation
            irr::core::vector3df(s.X, s.Y, s.Z)     //scale
    ));
}