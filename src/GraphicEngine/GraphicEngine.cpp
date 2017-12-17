#include "GraphicEngine.h"

static GraphicEngine* instance;

GraphicEngine::GraphicEngine(){
	privateReceiver = new EventReceiver();

	/*irr::IrrlichtDevice *nulldevice = irr::createDevice(irr::video::EDT_NULL);
	irr::core::dimension2d<irr::u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();
	nulldevice -> drop();*/

	privateDevice = irr::createDevice(
		irr::video::EDT_OPENGL,                             //Driver
		irr::core::dimension2d<unsigned int>(900,600),      //Size of window
		16,                                                 //bits
		false,                                              //fullscreen
		false,                                              //stencil buffer
		true,                                               //vsync
		privateReceiver                                     //event receiver
	);

	if(!privateDevice)
		exit(1);

	//caption of the window
	privateDevice->setWindowCaption(L"Wizards And Warlocks Master v1.0");

	//Initialize private pointers
	privateDriver = privateDevice->getVideoDriver();
	privateSManager = privateDevice->getSceneManager();
	privateGUIEnv = privateDevice->getGUIEnvironment();
}

GraphicEngine* GraphicEngine::getInstance(){
	//singleton constructor
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

void GraphicEngine::ChangeWindowName(std::wstring newName){
	privateDevice->setWindowCaption(newName.c_str());
}

void GraphicEngine::ToggleMenu(bool newState){
	irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*) privateCamera->privateNode;
	if(cam != NULL) cam->setInputReceiverEnabled(!newState);
    privateDevice->getCursorControl()->setVisible(newState);
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

void GraphicEngine::drawManaAndHealth(int h, int m){
	irr::u32 W = (irr::u32) privateDriver->getScreenSize().Width;
	irr::u32 H = (irr::u32) privateDriver->getScreenSize().Height;

	float size = 20;

	float xInit = W/20;
	float xEnd =  W/3;

	float yInitH = H * 0.05;
	float yInitM = H * 0.09;

	// float yInitH = H - (H * 0.14);
	// float yInitM = H - (H * 0.10);

	float yEndH = yInitH + size;
	float yEndM = yInitM + size;

	float hP = h/100.0f;
	float mP = m/100.0f;


	// Black Bar
	irr::video::SColor color = irr::video::SColor(255, 0, 0, 0);
	privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(xInit, yInitH, xEnd, yEndH));
	privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(xInit, yInitM, xEnd, yEndM));
	
	// Helath & Mana Bar
	color = irr::video::SColor(255, 255, 0, 0);
	privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(xInit, yInitH, xInit + (xEnd - xInit) * hP, yEndH));
	color = irr::video::SColor(255, 0, 0, 255);
	privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(xInit, yInitM, xInit + (xEnd - xInit) * mP, yEndM));
}

// SMANAGER FUNCTIONS
void GraphicEngine::drawAll(){
	privateSManager->drawAll();
}

GBody* GraphicEngine::addCube2Scene(vector3df p, vector3df r, vector3df s, float size, int id){
	GBody * gb = new GBody(
		privateSManager->addCubeSceneNode(
			size,   //size
			0,      //parent
			id,     //id
			irr::core::vector3df(p.X, p.Y, p.Z),    //position
			irr::core::vector3df(0,0, 0),    		//rotation
			irr::core::vector3df(s.X, s.Y, s.Z)     //scale
	));

	gb->Rotate(vector3df(r.X, r.Y, 0));
	gb->Rotate(vector3df(0, 0, r.Z));

	return gb;
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

	irr::scene::ICameraSceneNode* oldCamera = privateSManager->getActiveCamera();
	if (oldCamera){
		privateSManager->setActiveCamera(0);
		oldCamera->remove();
		privateCamera = NULL;
	}

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

int GraphicEngine::ReadMenu(){
	return(privateReceiver->ReadMenu());
}

std::string GraphicEngine::ReadText(int id){
	irr::gui::IGUIElement* textElem;
	textElem = privateGUIEnv->getRootGUIElement()->getElementFromId(id, true);

	const wchar_t *text = textElem->getText();
	std::wstring ws(text);
	std::string text_str(ws.begin(), ws.end());

	return (text_str);
}

void GraphicEngine::addStaticText(vector4di p, std::wstring text, bool border, bool wordWrap){
	irr::gui::IGUIStaticText* ge = privateGUIEnv->addStaticText(text.c_str(), irr::core::rect<irr::s32>(p.X, p.Y, p.X + p.X2, p.Y + p.Y2), border, wordWrap, 0);
	ge->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
	ge->setDrawBorder(false);
}

void GraphicEngine::addButton(vector4di p, std::wstring text, std::wstring infoText, int id){
	privateGUIEnv->addButton(
		irr::core::rect<irr::s32>(p.X, p.Y, p.X + p.X2, p.Y + p.Y2),	//position
		0,													            //parent
		id,													            //id
		text.c_str(),										            //display text
		infoText.c_str()									            //tooltip text
	);
}

void GraphicEngine::addEditBox(vector4di p, std::wstring text, int id){
	irr::gui::IGUIEditBox* ge = privateGUIEnv->addEditBox(
									text.c_str(),
									irr::core::rect<irr::s32>(p.X, p.Y, p.X + p.X2, p.Y + p.Y2),
									false,
									0,
									(irr::s32)id
								);
	ge->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
}

irr::scene::ITriangleSelector* GraphicEngine::AddTriangleSelector(irr::scene::ISceneNode* node){
	irr::scene::ITriangleSelector* selector = privateSManager->createTriangleSelectorFromBoundingBox(node);
	return selector;
}

std::map<int,std::vector<vector3df>> GraphicEngine::Raycast(vector3df Start, vector3df End){
	std::map<int,std::vector<vector3df>> NodePointData;
	std::vector<vector3df> PointData;

	irr::core::vector3df point;
	irr::core::triangle3df triangle;
	irr::scene::ISceneNode *node = 0;
	irr::scene::ISceneCollisionManager* collisionManager = privateSManager->getSceneCollisionManager();
	irr::scene::ITriangleSelector* selector = 0;

	//First we need to get the cursor position in the 2D space
	//irr::core::position2d<irr::s32> pos = privateDevice->getCursorControl()->getPosition();

	// we need to get the 3D vector from it.
	//const irr::core::line3d<irr::f32> ray = collisionManager->getRayFromScreenCoordinates(pos);

	const irr::core::line3d<irr::f32> ray(Start.X,Start.Y,Start.Z,End.X,End.Y,End.Z);

	if(collisionManager->getSceneNodeAndCollisionPointFromRay(ray,point,triangle)){
		node = collisionManager->getSceneNodeAndCollisionPointFromRay(ray,point,triangle);
		selector = node->getTriangleSelector();
	}
	if(collisionManager->getCollisionPoint(ray,selector,point,triangle,node)){
		irr::core::vector3df triangleN = triangle.getNormal().getHorizontalAngle();
		//irr::core::vector3df triangleN = triangle.getNormal();
		vector3df collisionPoint(point.X,point.Y,point.Z);
		vector3df normalVector(triangleN.X, triangleN.Y, triangleN.Z);

		int nodeID = node->getID();
		PointData.push_back(normalVector);
		PointData.push_back(collisionPoint);

		NodePointData.insert(std::pair<int,std::vector<vector3df>>(nodeID, PointData));
	}

	return NodePointData;
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

keyStatesENUM GraphicEngine::GetKeyStatus(TKEY_CODE code){
	return privateReceiver->GetKeyStatus((irr::EKEY_CODE)code);
}

void GraphicEngine::SetKeyStatus(TKEY_CODE code, keyStatesENUM status){
	privateReceiver->setKeyStatus((irr::EKEY_CODE)code, status);
}

