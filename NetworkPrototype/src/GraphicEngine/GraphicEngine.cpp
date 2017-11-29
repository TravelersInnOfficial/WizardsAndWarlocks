#include "GraphicEngine.h"

static GraphicEngine* instance;

GraphicEngine::GraphicEngine(){
    privateReceiver = new EventReceiver();

    irr::IrrlichtDevice *nulldevice = irr::createDevice(irr::video::EDT_NULL);
    irr::core::dimension2d<irr::u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();
    nulldevice -> drop();

    privateDevice = irr::createDevice(
        irr::video::EDT_OPENGL,
        irr::core::dimension2d<unsigned int>(900,600),
        16,
        false,
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

int GraphicEngine::getTime(){
    return privateDevice->getTimer()->getTime();
}

// DRIVER FUNCTIONS
bool GraphicEngine::beginScene(){
    return privateDriver->beginScene();
}

bool GraphicEngine::beginSceneDefault(){
    return privateDriver->beginScene(true, true, irr::video::SColor(255,113,113,255));
}

bool GraphicEngine::endScene(){
    return privateDriver->endScene();
}

void GraphicEngine::setTextureToBody(GBody* body, int layer, std::string s){
    body->privateNode->setMaterialTexture(0, privateDriver->getTexture(s.c_str()));
}

void GraphicEngine::paintLineDebug(vector3df f, vector3df t, vector3df c){
    irr::video::SColorf fromC;
    fromC.set(1.0f, c.X, c.Y, c.Z); //(a, r, g, b)

    irr::core::vector3df from(f.X, f.Y, f.Z);
    irr::core::vector3df to(t.X, t.Y, t.Z);

    irr::core::matrix4 id;
    id.makeIdentity();
    privateDriver->setTransform(irr::video::ETS_WORLD, id);
    privateDriver->draw3DLine(from, to, fromC.toSColor());
}

void GraphicEngine::drawAim(){
    irr::video::SColor color = irr::video::SColor(255, 255, 0, 0);
    irr::u32 size = 15;
    irr::u32 cenW = (irr::u32) (privateDriver->getScreenSize().Width * 0.5);
    irr::u32 cenH = (irr::u32) (privateDriver->getScreenSize().Height * 0.5);

    //Draw crosshair
    privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - 1, cenH - size, cenW + 1, cenH - 4)); //above
    privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW + 4, cenH - 1, cenW + size, cenH + 1)); //right
    privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - 1, cenH + 4, cenW + 1, cenH + size)); //down
    privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - size, cenH - 1, cenW - 4, cenH + 1)); //left
    privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - 1, cenH - 1, cenW + 1, cenH + 1)); //center of screen
}

// SMANAGER FUNCTIONS
void GraphicEngine::drawAll(){
    privateSManager->drawAll();
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

GCamera* GraphicEngine::addCameraSceneNodeFPS(float rotateSpeed, float moveSpeed){
    irr::SKeyMap keyMap[4];
    keyMap[0].Action = irr::EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = irr::KEY_KEY_W;
    keyMap[1].Action = irr::EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = irr::KEY_KEY_S;
    keyMap[2].Action = irr::EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = irr::KEY_KEY_A;
    keyMap[3].Action = irr::EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = irr::KEY_KEY_D;

    privateCamera = new GCamera(privateSManager->addCameraSceneNodeFPS(0, rotateSpeed, moveSpeed, -1, keyMap, 4));
    
    return privateCamera;
}

GCamera* GraphicEngine::getActiveCamera(){
    privateCamera->privateNode = privateSManager->getActiveCamera();
    return privateCamera;
}

void GraphicEngine::addToDeletionQueue(irr::scene::ISceneNode* g){
    privateSManager->addToDeletionQueue(g);
}

// GUIENV FUNCTIONS
void GraphicEngine::drawAllGUI(){
    privateGUIEnv->drawAll();
}

GGUIElement* GraphicEngine::addStaticText(std::wstring text, vector4di p, bool border, bool wordWrap, GGUIElement * parent){
    return new GGUIElement(privateGUIEnv->addStaticText(text.c_str(), irr::core::rect<irr::s32>(p.X, p.Y, p.X2, p.Y2), border, wordWrap, parent!=0? parent->privateElement: 0));
}

GGUIElement* GraphicEngine::createDebugWindowControl(){
    // create the toolbox window
    irr::gui::IGUIWindow* wnd = privateGUIEnv->addWindow(irr::core::rect<irr::s32>(600,45,800,480),
        false, L"Toolset", 0, 0x10000);

    // create tab control and tabs
    irr::gui::IGUITabControl* tab = privateGUIEnv->addTabControl(
        irr::core::rect<irr::s32>(2,20,800-602,480-7), wnd, true, true);

    irr::gui::IGUITab* t1 = tab->addTab(L"Config");
    return new GGUIElement(t1);
}

// RECEIVER FUNCTIONS
void GraphicEngine::UpdateReceiver(){
    privateReceiver->Update();
}

bool GraphicEngine::IsKeyDown(TKEY_CODE code){
    return privateReceiver->keyDown((irr::EKEY_CODE)code);
}

bool GraphicEngine::IsKeyReleased(TKEY_CODE code){
    return privateReceiver->keyRelease((irr::EKEY_CODE)code);
}

bool GraphicEngine::IsKeyUp(TKEY_CODE code){
    return privateReceiver->keyUp((irr::EKEY_CODE)code);
}

bool GraphicEngine::IsKeyPressed(TKEY_CODE code){
    return privateReceiver->keyPressed((irr::EKEY_CODE)code);
}

bool GraphicEngine::IsLeftButtonPressed(){
    return privateReceiver->leftMousePressed();
}

bool GraphicEngine::IsLeftButtonDown(){
    return privateReceiver->leftMouseDown();
}
