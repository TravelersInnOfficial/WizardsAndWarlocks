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

void GraphicEngine::addCameraSceneNodeFPS(){
    privateSManager->addCameraSceneNodeFPS();
}

int GraphicEngine::getTime(){
    return privateDevice->getTimer()->getTime();
}

bool GraphicEngine::beginScene(){
    return privateDriver->beginScene();
}

bool GraphicEngine::beginSceneDefault(){
    return privateDriver->beginScene(true, true, irr::video::SColor(255,113,113,133));
}

bool GraphicEngine::endScene(){
    return privateDriver->endScene();
}

void GraphicEngine::drawAll(){
    privateSManager->drawAll();
}

void GraphicEngine::drawAllGUI(){
    privateGUIEnv->drawAll();
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

GBody* GraphicEngine::addSphere2Scene(vector3df p, vector3df r, vector3df s, float radius, int id){
    return new GBody(
        privateSManager->addSphereSceneNode(
            radius,     //size
            16,         //polycount
            0,          //parent
            id,         //id
            irr::core::vector3df(p.X, p.Y, p.Z),    //position
            irr::core::vector3df(r.X, r.Y, r.Z),    //rotation
            irr::core::vector3df(s.X, s.Y, s.Z)     //scale
    ));
}

void GraphicEngine::setTextureToBody(GBody* body, std::string s){
    body->privateNode->setMaterialTexture(0, privateDriver->getTexture("../media/t351sml.jpg"));
}

void GraphicEngine::setTextureFlag(GBody* body, std::string flag, bool value){
    irr::video::E_MATERIAL_FLAG videoFlag;
    if (flag == "lightning"){
        videoFlag = irr::video::EMF_LIGHTING;
    }
    else if(flag == "wired"){
        videoFlag = irr::video::EMF_WIREFRAME;
    }
    else{
        return;
    }
    body->privateNode->setMaterialFlag(videoFlag, value); 
}