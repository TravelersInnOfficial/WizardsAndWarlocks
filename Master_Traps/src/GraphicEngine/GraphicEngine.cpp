#include "GraphicEngine.h"

static GraphicEngine* instance;

GraphicEngine::GraphicEngine(){
   	privateReceiver = new EventReceiver();

    irr::IrrlichtDevice *nulldevice = irr::createDevice(irr::video::EDT_NULL);
	irr::core::dimension2d<irr::u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();
	nulldevice -> drop();

    privateDevice = irr::createDevice(
        irr::video::EDT_OPENGL,
        deskres,
        16,
        true,
        false,
        true,
        privateReceiver
    );

    if(!privateDevice)
        exit(1);

    privateDevice->setWindowCaption(L"Wizards And Warlocks Master v1.0");

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

GCamera* GraphicEngine::addCameraSceneNodeFPS(float rotateSpeed, float moveSpeed){
    privateCamera = new GCamera(privateSManager->addCameraSceneNodeFPS(0, rotateSpeed, moveSpeed));
    return privateCamera;
}

int GraphicEngine::getTime(){
    return privateDevice->getTimer()->getTime();
}

bool GraphicEngine::beginScene(){
    return privateDriver->beginScene();
}

bool GraphicEngine::beginSceneDefault(){
    return privateDriver->beginScene(true, true, irr::video::SColor(255,113,113,255));
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

GBody* GraphicEngine::addObjMeshSceneNode(std::string path){
    return new GBody(privateSManager->addAnimatedMeshSceneNode(privateSManager->getMesh(path.c_str())));
}

void GraphicEngine::setTextureToBody(GBody* body, int layer, std::string s){
    body->privateNode->setMaterialTexture(0, privateDriver->getTexture(s.c_str()));
}

void GraphicEngine::setTextureFlag(GBody* body, std::string flag, bool value){
    irr::video::E_MATERIAL_FLAG videoFlag;
    if (flag == "lighting"){
        videoFlag = irr::video::EMF_LIGHTING;
    }
    else if(flag == "wired"){
        videoFlag = irr::video::EMF_WIREFRAME;
    }
    else if(flag == "normalize"){
        videoFlag = irr::video::EMF_NORMALIZE_NORMALS;
    }
    else{
        return;
    }
    body->privateNode->setMaterialFlag(videoFlag, value); 
}

void GraphicEngine::setAnimationFlyStraight(GBody* body, vector3df initialPos, vector3df finalPos, float time, bool loop, bool pingpong){
    irr::scene::ISceneNodeAnimator* anim = privateSManager->createFlyStraightAnimator(
        irr::core::vector3df(initialPos.X, initialPos.Y, initialPos.Z), 
        irr::core::vector3df(  finalPos.X,   finalPos.Y,   finalPos.Z), 
        time, 
        loop, 
        pingpong);
    
    if (anim)
    {
        body->privateNode->addAnimator(anim);
        anim->drop();
    }
}

bool GraphicEngine::IsKeyDown(TKEY_CODE code){
    return privateReceiver->IsKeyDown((irr::EKEY_CODE)code);
}

GCamera* GraphicEngine::getActiveCamera(){
    privateCamera->privateNode = privateSManager->getActiveCamera();
    return privateCamera;
}
