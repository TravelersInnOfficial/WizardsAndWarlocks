#include "IrrEngine.h"

IrrEngine::IrrEngine(bool isServer){
	m_isServer = isServer;

	privateDevice = nullptr;
	privateDriver = nullptr;
	privateSManager = nullptr;
	privateGUIEnv = nullptr;
	privateReceiver = nullptr;
	privateMenuReceiver = nullptr;
	privateCamera = nullptr;

	privateReceiver = new EventReceiver();
	privateMenuReceiver = new MenuReceiver();

	irr::IrrlichtDevice *nulldevice = irr::createDevice(irr::video::EDT_NULL);
	irr::core::dimension2d<irr::u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();
	nulldevice -> drop();
	
	deskres = irr::core::dimension2d<unsigned int>(900, 600);
	if(isServer) deskres = irr::core::dimension2d<unsigned int>(1, 1);

	privateDevice = irr::createDevice(
		irr::video::EDT_OPENGL,			//Driver
		deskres,						//Size of window
		16,								//bits
		false,							//fullscreen
		false,							//stencil buffer
		true,							//vsync
		privateMenuReceiver				//event receiver
	);

	if(!privateDevice) exit(1);


	//caption of the window
	std::wstring windowCaption = L"Wizards And Warlocks";

	if(isServer){
		privateDevice->minimizeWindow();
		windowCaption = L"Wizards And Warlocks - Server Instance";
	}

	privateDevice->setWindowCaption(windowCaption.c_str());

	// Minimize and change name
	privateDevice->run();

	//Initialize private pointers
	privateDriver = privateDevice->getVideoDriver();
	privateSManager = privateDevice->getSceneManager();
	privateGUIEnv = privateDevice->getGUIEnvironment();

	// Set GUI Alpha
	for (int i = 0; i < irr::gui::EGDC_COUNT; ++i){
		irr::video::SColor col = privateGUIEnv->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR)i);
		col.setAlpha(255);
		privateGUIEnv->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR)i, col);
	}

	// Sky Dome
	privateSManager->addSkyDomeSceneNode(privateDriver->getTexture("./../assets/textures/skymap/skydome.jpg"));
	privateSManager->setAmbientLight(irr::video::SColor(255, 200, 200, 200));

}

IrrEngine::~IrrEngine(){
	drop();
	if(privateReceiver != nullptr) delete privateReceiver;
	if(privateMenuReceiver != nullptr) delete privateMenuReceiver;
}

IrrEngine* IrrEngine::getInstance(bool isServer){
	static IrrEngine instance = IrrEngine(isServer);

	return &instance;
}

bool IrrEngine::run(){
	bool toRet = false;
	if(privateDevice != nullptr) toRet = privateDevice->run();
	return toRet;
}

bool IrrEngine::drop(){
	bool toRet = false;
	if(privateDevice != nullptr) toRet = privateDevice->drop();
	return toRet;
}

void IrrEngine::setCursorVisible(bool visible){
	if(privateDevice != nullptr) privateDevice->getCursorControl()->setVisible(false);
}

int IrrEngine::getTime(){
	float toRet = 0;
	if(privateDevice != nullptr) toRet = privateDevice->getTimer()->getTime();
	return toRet;
}

void IrrEngine::ChangeWindowName(std::wstring newName){
	if(privateDevice != nullptr) privateDevice->setWindowCaption(newName.c_str());
}

void IrrEngine::ToggleMenu(bool newState){
	if(privateDevice != nullptr){
		if(newState) privateDevice->setEventReceiver(privateMenuReceiver);
		else privateDevice->setEventReceiver(privateReceiver);
		ToggleCameraMovement(!newState);
		privateDevice->getCursorControl()->setVisible(false);
	}
}

void IrrEngine::ToggleCameraMovement(bool newState){
	if (privateCamera != nullptr){
		irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*) privateCamera->privateNode;
		if(cam != nullptr) cam->setInputReceiverEnabled(newState);
	}
}

// DRIVER FUNCTIONS
bool IrrEngine::beginScene(){
	bool toRet = false;
	if(privateDriver != nullptr) toRet = privateDriver->beginScene();
	return toRet;
}

bool IrrEngine::beginSceneDefault(){
	bool toRet = false;
	if(privateDriver != nullptr) toRet = privateDriver->beginScene(true, true, irr::video::SColor(255,0,0,0));
	return toRet;
}

bool IrrEngine::endScene(){
	bool toRet = false;
	if(privateDriver != nullptr) toRet = privateDriver->endScene();
	return toRet;
}

void IrrEngine::setTextureToBody(GBody* body, int layer, std::string s){
	if(privateDriver != nullptr) body->privateNode->setMaterialTexture(0, privateDriver->getTexture(s.c_str()));
}

void IrrEngine::paintLineDebug(vector3df f, vector3df t, vector3df c){
	if(privateDriver != nullptr){
		irr::video::SColorf fromC;
		fromC.set(1.0f, c.X, c.Y, c.Z); //(a, r, g, b)

		irr::core::vector3df from(f.X, f.Y, f.Z);
		irr::core::vector3df to(t.X, t.Y, t.Z);

		irr::core::matrix4 id;
		id.makeIdentity();
		privateDriver->setTransform(irr::video::ETS_WORLD, id);
		privateDriver->draw3DLine(from, to, fromC.toSColor());
	}
}

void IrrEngine::drawAim(bool moving){
	if(privateDriver != nullptr){
		irr::video::SColor color = irr::video::SColor(255, 255, 255, 255);
		irr::u32 size = 15;
		irr::u32 cenW = (irr::u32) (privateDriver->getScreenSize().Width * 0.5);
		irr::u32 cenH = (irr::u32) (privateDriver->getScreenSize().Height * 0.5);

		irr::u32 rDist = 0;
		if(moving) rDist = 30;
		
		// Not moving, Draws center point
		else{
			rDist = 4;
			privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - 1, cenH - 1, cenW + 1, cenH + 1)); //center of screen
		}

		//Draw Crosshair
		privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - 1			, cenH - rDist - size	, cenW + 1				, cenH - rDist			)); //above
		privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW + rDist		, cenH - 1	 			, cenW + rDist + size	, cenH + 1				)); //right
		privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - 1			, cenH + rDist			, cenW + 1				, cenH + rDist + size	)); //down
		privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - rDist - size	, cenH - 1	 			, cenW - rDist			, cenH + 1				)); //left
	}
}

void IrrEngine::drawGrailGUI(float currentValue, float maxValue){
	if(privateDriver != nullptr){
		irr::u32 W = (irr::u32) privateDriver->getScreenSize().Width;
		irr::u32 H = (irr::u32) privateDriver->getScreenSize().Height;

		float size = 20;
		float xInit = W/2 - 200;

		float xEnd =  W/2 + 200;
		float yInit = H/2 + H/4;

		float yEnd = yInit + size;
		float hP = currentValue/maxValue;

		// Black Bar
		vector3df color = vector3df(0.0f,0.0f,0.0f);
		draw2DRectangle(color, xInit, yInit, xEnd, yEnd);

		// Helath & Mana Bar
		color = vector3df(51, 171, 249);
		draw2DRectangle(color, xInit, yInit, xInit + (xEnd - xInit) * hP, yEnd);
	}
}

void IrrEngine::drawOverlays(OverlayCodes type){
	if(privateDriver != nullptr){
		std::string overlayTexture = OverlayPath[type];
		irr::video::ITexture* overlay = nullptr;
		overlay = privateDriver->getTexture(overlayTexture.c_str());

		if(overlayTexture.length() > 0){
			const irr::core::dimension2du& size = privateDriver->getScreenSize();
			irr::core::rect<irr::s32> destRect = irr::core::rect<irr::s32>(0, 0, size.Width, size.Height);
			const irr::core::dimension2d<irr::u32> size2 = overlay->getSize();
			irr::core::rect<irr::s32> imgRect = irr::core::rect<irr::s32>(0, 0, size2.Width, size2.Height);
			privateDriver->draw2DImage(overlay, destRect, imgRect, 0, 0, true);
		}
	}
}

int IrrEngine::GetScreenHeight(){
	int toRet = 0;
	if(privateDriver != nullptr){
		irr::u32 W = (irr::u32) privateDriver->getScreenSize().Height;
		toRet = (int)W;
	}
	return toRet;
}

int IrrEngine::GetScreenWidth(){
	int toRet = 0;
	if(privateDriver != nullptr){
		irr::u32 H = (irr::u32) privateDriver->getScreenSize().Width;
		toRet = (int)H;
	}
	return toRet;
}

void IrrEngine::draw2DImage(std::string texturePath, vector4df rect){
	if(privateDriver != nullptr){
		irr::video::ITexture* spellTexture = privateDriver->getTexture(texturePath.c_str());
		irr::core::rect<irr::s32> destRect = irr::core::rect<irr::s32>(rect.X, rect.Y, rect.X2, rect.Y2);
		const irr::core::dimension2d<irr::u32> size = spellTexture->getSize();
		irr::core::rect<irr::s32> imgRect = irr::core::rect<irr::s32>(0, 0, size.Width, size.Height);
		privateDriver->draw2DImage(spellTexture, destRect, imgRect, 0, 0, true);
	}
}

void IrrEngine::draw2DRectangle(vector3df c, float xInit, float yInit, float xEnd, float yEnd){
	if(privateDriver != nullptr){
		irr::video::SColor color = irr::video::SColor(255, c.X, c.Y, c.Z);
		privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(xInit, yInit, xEnd, yEnd));
	}
}

// SMANAGER FUNCTIONS
void IrrEngine::drawAll(){
	if(privateSManager != nullptr) privateSManager->drawAll();
}

GBody* IrrEngine::addCube2Scene(vector3df p, vector3df r, vector3df s, float size, int id){
	GBody* gb = nullptr;

	if(privateSManager != nullptr){
		gb = new GBody(
			privateSManager->addCubeSceneNode(
				size,   //size
				0,      //parent
				id,     //id
				irr::core::vector3df(p.X, p.Y, p.Z),    //position
				irr::core::vector3df(0,0, 0),    		//rotation
				irr::core::vector3df(s.X, s.Y, s.Z)     //scale
			)
		);

		gb->Rotate(vector3df(r.X, r.Y, 0));
		gb->Rotate(vector3df(0, 0, r.Z));
	}

	return gb;
}

GBody* IrrEngine::addSphere2Scene(vector3df p, vector3df r, vector3df s, float radius, int id){
	GBody* gb = nullptr;
	if(privateSManager != nullptr){
		gb = new GBody(
			privateSManager->addSphereSceneNode(
				radius,     //size
				16,         //polycount
				0,          //parent
				id,         //id
				irr::core::vector3df(p.X, p.Y, p.Z),    //position
				irr::core::vector3df(r.X, r.Y, r.Z),    //rotation
				irr::core::vector3df(s.X, s.Y, s.Z)     //scale
			)
		);
	}
	return gb;
}

GBody* IrrEngine::addObjMeshSceneNode(std::string path){
	return new GBody(privateSManager->addAnimatedMeshSceneNode(privateSManager->getMesh(path.c_str())));
}

GBody* IrrEngine::addObjMeshSceneNode(std::string path, vector3df position, vector3df rotation, vector3df scale){
	GBody* body= new GBody(privateSManager->addAnimatedMeshSceneNode(privateSManager->getMesh(path.c_str())));

	body->setPosition(position);
	body->setRotation(rotation);
	body->setScale(scale);

	return body;
}

GCamera* IrrEngine::addCameraSceneNodeFPS(float rotateSpeed, float moveSpeed){

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
		delete privateCamera;
		privateCamera = nullptr;
	}

	privateCamera = new GCamera(privateSManager->addCameraSceneNodeFPS(0, rotateSpeed, moveSpeed, -1, keyMap, 4));

	return privateCamera;
}

GCamera* IrrEngine::addCameraSceneNode(vector3df position, vector3df lookat){

	irr::scene::ICameraSceneNode* oldCamera = privateSManager->getActiveCamera();
	if (oldCamera){
		privateSManager->setActiveCamera(0);
		oldCamera->remove();
		privateCamera = nullptr;
	}

	irr::core::vector3df cameraPosition(position.X, position.Y, position.Z);
	irr::core::vector3df cameraLookat(lookat.X, lookat.Y, lookat.Z);	
	privateCamera = new GCamera(privateSManager->addCameraSceneNode(0, cameraPosition, cameraLookat, -1));

	return privateCamera;
}

GCamera* IrrEngine::getActiveCamera(){
	if(privateCamera != nullptr) privateCamera->privateNode = privateSManager->getActiveCamera();
	return privateCamera;
}

// GUIENV FUNCTIONS
bool IrrEngine::EscPressed(){
	return (privateMenuReceiver->EscPressed());
}

std::string IrrEngine::ReadText(MenuOption id){
	irr::gui::IGUIElement* textElem;
	textElem = privateGUIEnv->getRootGUIElement()->getElementFromId((int)id, true);

	const wchar_t *text = textElem->getText();
	std::wstring ws(text);
	std::string text_str(ws.begin(), ws.end());

	return (text_str);
}

void IrrEngine::addStaticText(vector4di p, std::wstring text, bool border, bool wordWrap, int id, irr::gui::IGUIWindow* parent){
	irr::gui::IGUIStaticText* ge = privateGUIEnv->addStaticText(
		text.c_str(),
		irr::core::rect<irr::s32>(p.X, p.Y, p.X + p.X2, p.Y + p.Y2),
		border,
		wordWrap,
		parent,
		id
	);
	
	ge->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
	ge->setDrawBorder(false);
}

void IrrEngine::SetCursorPosition(vector2di cursor){
	privateDevice->getCursorControl()->setPosition(irr::core::vector2di(cursor.X, cursor.Y));
}

vector2di IrrEngine::GetCursorPosition(){
	irr::core::vector2di ctrlP =  privateDevice->getCursorControl()->getPosition();

	return vector2di(ctrlP.X, ctrlP.Y);
}

// RECEIVER FUNCTIONS
void IrrEngine::UpdateReceiver(){
	privateReceiver->Update();
	privateMenuReceiver->Update();
}

void IrrEngine::InitReceiver(){
	privateReceiver->InitReceiver();
}

bool IrrEngine::IsKeyDown(TKEY_CODE code){
	return privateReceiver->keyDown((irr::EKEY_CODE)code);
}

bool IrrEngine::IsKeyReleased(TKEY_CODE code){
	return privateReceiver->keyRelease((irr::EKEY_CODE)code);
}

bool IrrEngine::IsKeyUp(TKEY_CODE code){
	return privateReceiver->keyUp((irr::EKEY_CODE)code);
}

bool IrrEngine::IsKeyPressed(TKEY_CODE code){
	return privateReceiver->keyPressed((irr::EKEY_CODE)code);
}

keyStatesENUM IrrEngine::GetKeyStatus(TKEY_CODE code){
	return privateReceiver->GetKeyStatus((irr::EKEY_CODE)code);
}

void IrrEngine::SetKeyStatus(TKEY_CODE code, keyStatesENUM status){
	privateReceiver->setKeyStatus((irr::EKEY_CODE)code, status);
}

irr::scene::IBillboardTextSceneNode* IrrEngine::addBillboardText(std::string text, irr::scene::ISceneNode* parent, vector3df position, int id){
	irr::core::vector3df auxPos = irr::core::vector3df(0, 0, 0);
	auxPos.X = position.X; auxPos.Y = position.Y; auxPos.Z = position.Z;

	float dimX = text.length() * 0.1;
	float dimY = 0.25f;
	irr::core::dimension2d<irr::f32> dim = irr::core::dimension2d<irr::f32>(dimX, dimY);
	
	std::wstring wText = std::wstring(text.begin(), text.end());

	irr::scene::IBillboardTextSceneNode* board = privateSManager->addBillboardTextSceneNode(0, wText.c_str(), parent, dim, auxPos, id);
	return board;
}

irr::IrrlichtDevice* IrrEngine::GetIrrlichtDevice(){
	return privateDevice;
}
MenuReceiver* IrrEngine::GetMenuReceiver(){
	return privateMenuReceiver;
}