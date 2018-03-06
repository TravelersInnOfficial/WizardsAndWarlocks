#include "GraphicEngine.h"
#include <TravelersOcularEngine/src/TOcularEngine/TOcularEngine.h>

GraphicEngine::GraphicEngine(bool isServer){
	VideoDriver::m_assetsPath = "./../src/Engines/TravelersOcularEngine/assets";
	privateDriver = toe::GetVideoDriver();
	privateDriver->CreateWindows("WW",toe::core::TOEvector2df(800,600));
	privateDriver->SetClearScreenColor(toe::core::TOEvector4df(0.7, 0.7, 1, 1));

	privateSManager = privateDriver->GetSceneManager();

	privateReceiver = new EventReceiver();
	privateDriver->SetIODriver(privateReceiver);
}

GraphicEngine::~GraphicEngine(){
}

GraphicEngine* GraphicEngine::getInstance(bool isServer){
	static GraphicEngine instance = GraphicEngine(isServer);
	return &instance;
}

bool GraphicEngine::run(){
	bool toRet = false;
	if(privateDriver != nullptr) toRet = privateDriver->Update();
	return toRet;
}

bool GraphicEngine::drop(){
	if(privateDriver != nullptr) privateDriver->Drop();
	return true;
}

void GraphicEngine::setCursorVisible(bool visible){
	if(privateDriver != nullptr) privateDriver->SetMouseVisibility(visible);
}

int GraphicEngine::getTime(){
	float toRet = 0;
	if(privateDriver != nullptr) toRet = privateDriver->GetTime();
	return toRet;
}

void GraphicEngine::ChangeWindowName(std::string newName){
	if(privateDriver != nullptr) privateDriver->SetWindowName(newName);
}

//####################
//####################
//####################
void GraphicEngine::ToggleMenu(bool newState){
	/*if(privateDevice != nullptr){
		if(newState) privateDevice->setEventReceiver(privateMenuReceiver);
		else privateDevice->setEventReceiver(privateReceiver);
		ToggleCameraMovement(!newState);
		privateDevice->getCursorControl()->setVisible(false);
	}*/
}

//####################
//####################
//####################
void GraphicEngine::ToggleCameraMovement(bool newState){
	/*if (privateCamera != nullptr){
		irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*) privateCamera->privateNode;
		if(cam != nullptr) cam->setInputReceiverEnabled(newState);
	}*/
}

// DRIVER FUNCTIONS
bool GraphicEngine::beginScene(){
	//std::cout<<"En nuestro motor no usamos el BEGIN SCENE"<<std::endl;
	bool toRet = false;
	toRet = true;
	//if(privateDriver != nullptr) toRet = privateDriver->beginScene();
	return toRet;
}

bool GraphicEngine::beginSceneDefault(){
	bool toRet = false;
	//std::cout<<"En nuestro motor no usamos el BEGIN SCENE DEFAULT"<<std::endl;
	toRet = true;
	//if(privateDriver != nullptr) toRet = privateDriver->beginScene(true, true, irr::video::SColor(255,0,0,0));
	return toRet;
}

bool GraphicEngine::endScene(){
	//std::cout<<"En nuestro motor no usamos el END SCENE"<<std::endl;
	bool toRet = false;
	toRet = true;
	//if(privateDriver != nullptr) toRet = privateDriver->endScene();
	return toRet;
}

void GraphicEngine::setTextureToBody(GBody* body, int layer, std::string s){
	if(privateDriver != nullptr){
		TFMesh* auxMesh = (TFMesh*) body->privateNode;
		auxMesh->SetTexture(s.c_str());
	}
}

void GraphicEngine::paintLineDebug(vector3df f, vector3df t, vector3df c){
	//std::cout<<"En nuestro motor no hacemos PAINT LINE DEBUG"<<std::endl;

	/*if(privateDriver != nullptr){
		irr::video::SColorf fromC;
		fromC.set(1.0f, c.X, c.Y, c.Z); //(a, r, g, b)

		irr::core::vector3df from(f.X, f.Y, f.Z);
		irr::core::vector3df to(t.X, t.Y, t.Z);

		irr::core::matrix4 id;
		id.makeIdentity();
		privateDriver->setTransform(irr::video::ETS_WORLD, id);
		privateDriver->draw3DLine(from, to, fromC.toSColor());
	}*/
}

void GraphicEngine::drawAim(bool moving){
	//std::cout<<"En nuestro motor no usamos el DRAW AIM"<<std::endl;

	/*if(privateDriver != nullptr){
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
	}*/
}

void GraphicEngine::drawGrailGUI(float currentValue, float maxValue){
	//std::cout<<"En nuestro motor no usamos el DRAW GRAIL GUI"<<std::endl;

	/*if(privateDriver != nullptr){
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
	}*/
}

void GraphicEngine::drawOverlays(OverlayCodes type){
	//std::cout<<"En nuestro motor no usamos el DRAW OVERLAYS"<<std::endl;

	/*if(privateDriver != nullptr){
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
	}*/
}

int GraphicEngine::GetScreenHeight(){
	int toRet = 0;
	if(privateDriver != nullptr){
		toRet = (int) privateDriver->GetWindowDimensions().Y;
	}
	return toRet;
}

int GraphicEngine::GetScreenWidth(){
	int toRet = 0;
	if(privateDriver != nullptr){
		toRet = (int) privateDriver->GetWindowDimensions().X;
	}
	return toRet;
}

void GraphicEngine::draw2DImage(std::string texturePath, vector4df rect){
	//std::cout<<"En nuestro motor no usamos el DRAW 2D IMAGE"<<std::endl;

	/*if(privateDriver != nullptr){
		irr::video::ITexture* spellTexture = privateDriver->getTexture(texturePath.c_str());
		irr::core::rect<irr::s32> destRect = irr::core::rect<irr::s32>(rect.X, rect.Y, rect.X2, rect.Y2);
		const irr::core::dimension2d<irr::u32> size = spellTexture->getSize();
		irr::core::rect<irr::s32> imgRect = irr::core::rect<irr::s32>(0, 0, size.Width, size.Height);
		privateDriver->draw2DImage(spellTexture, destRect, imgRect, 0, 0, true);
	}*/
}

void GraphicEngine::draw2DRectangle(vector3df c, float xInit, float yInit, float xEnd, float yEnd){
	//std::cout<<"En nuestro motor no usamos el DRAW 2D RECTANGLE"<<std::endl;

	/*if(privateDriver != nullptr){
		irr::video::SColor color = irr::video::SColor(255, c.X, c.Y, c.Z);
		privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(xInit, yInit, xEnd, yEnd));
	}*/
}

// SMANAGER FUNCTIONS
void GraphicEngine::drawAll(){
	if(privateDriver != nullptr) privateDriver->Draw();
}

GBody* GraphicEngine::addCube2Scene(vector3df p, vector3df r, vector3df s, float size, int id){
	GBody* gb = nullptr;

	if(privateSManager != nullptr){
		gb = new GBody(
			toe::AddCube(
				toe::core::TOEvector3df(p.X, p.Y, p.Z),
				toe::core::TOEvector3df(0, 0, 0),
				toe::core::TOEvector3df(s.X * size, s.Y * size, s.Z * size)
			)
		);
		gb->Rotate(vector3df(r.X, r.Y, 0));
		gb->Rotate(vector3df(0, 0, r.Z));
	}

	return gb;
}

GBody* GraphicEngine::addSphere2Scene(vector3df p, vector3df r, vector3df s, float radius, int id){
	GBody* gb = nullptr;

	if(privateSManager != nullptr){
		gb = new GBody(
			toe::AddSphere(
				toe::core::TOEvector3df(p.X, p.Y, p.Z),
				toe::core::TOEvector3df(0, 0, 0),
				toe::core::TOEvector3df(s.X * radius, s.Y * radius, s.Z * radius)
			)
		);
		gb->Rotate(vector3df(r.X, r.Y, 0));
		gb->Rotate(vector3df(0, 0, r.Z));
	}

	return gb;
}

GBody* GraphicEngine::addObjMeshSceneNode(std::string path){
	GBody* gb = nullptr;
	toe::core::TOEvector3df data = toe::core::TOEvector3df(0, 0, 0);
	toe::core::TOEvector3df scale = toe::core::TOEvector3df(1, 1, 1);
	gb = new GBody(
		privateSManager->AddMesh(data, data, scale, path)
	);
	return gb;
}

GBody* GraphicEngine::addObjMeshSceneNode(std::string path, vector3df position, vector3df rotation, vector3df scale){
	GBody* gb = nullptr;
	toe::core::TOEvector3df position_TOE = toe::core::TOEvector3df(position.X, position.Y, position.Z);
	toe::core::TOEvector3df rotation_TOE = toe::core::TOEvector3df(rotation.X, rotation.Y, rotation.Z);
	toe::core::TOEvector3df scale_TOE = toe::core::TOEvector3df(scale.X, scale.Y, scale.Z);
	gb = new GBody(
		privateSManager->AddMesh(position_TOE, rotation_TOE, scale_TOE, path)
	);
	return gb;
}

GCamera* GraphicEngine::addCameraSceneNodeFPS(float rotateSpeed, float moveSpeed){
	GCamera* privateCamera = nullptr;

	TFCamera* oldCamera = privateSManager->GetMainCamera();
	if (oldCamera != nullptr){
		privateSManager->DeleteCamera(oldCamera);
		delete privateCamera;
		privateCamera = nullptr;
	}
	
	toe::core::TOEvector3df defaultdata = toe::core::TOEvector3df(0, 0, 0);
	privateCamera = new GCamera(privateSManager->AddCamera(defaultdata, defaultdata, true));
	return privateCamera;
}

GCamera* GraphicEngine::addCameraSceneNode(vector3df position, vector3df lookat){
	GCamera* privateCamera = nullptr;

	TFCamera* oldCamera = privateSManager->GetMainCamera();
	if (oldCamera != nullptr){
		privateSManager->DeleteCamera(oldCamera);
		delete privateCamera;
		privateCamera = nullptr;
	}
	
	toe::core::TOEvector3df position_TOE = toe::core::TOEvector3df(position.X, position.Y, position.Z);
	toe::core::TOEvector3df lookat_TOE = toe::core::TOEvector3df(lookat.X, lookat.Y, lookat.Z);
	privateCamera = new GCamera(privateSManager->AddCamera(position_TOE, lookat_TOE, true));
	return privateCamera;
}

GCamera* GraphicEngine::getActiveCamera(){
	GCamera* privateCamera = nullptr;
	if(privateCamera != nullptr) privateCamera->privateNode = privateSManager->GetMainCamera();
	return privateCamera;
}

// GUIENV FUNCTIONS
bool GraphicEngine::EscPressed(){
	//return (privateMenuReceiver->EscPressed());
	return false;
}

/*
std::string GraphicEngine::ReadText(MenuOption id){
	irr::gui::IGUIElement* textElem;
	textElem = privateGUIEnv->getRootGUIElement()->getElementFromId((int)id, true);

	const wchar_t *text = textElem->getText();
	std::wstring ws(text);
	std::string text_str(ws.begin(), ws.end());

	return (text_str);
	return "";
}*/

void GraphicEngine::SetCursorPosition(vector2di cursor){
	privateDriver->SetCursorPosition(cursor.X, cursor.Y);
}

vector2di GraphicEngine::GetCursorPosition(){
	toe::core::TOEvector2di auxPos = privateDriver->GetCursorPosition();
	return vector2di(auxPos.X, auxPos.Y);
}

// RECEIVER FUNCTIONS
void GraphicEngine::UpdateReceiver(){
	privateReceiver->Update();
	//privateMenuReceiver->Update();
}

void GraphicEngine::InitReceiver(){
	privateReceiver->InitReceiver();
}

bool GraphicEngine::IsKeyDown(KeyboardKey code){
	return privateReceiver->keyDown(code);
}

bool GraphicEngine::IsKeyReleased(KeyboardKey code){
	return privateReceiver->keyRelease(code);
}

bool GraphicEngine::IsKeyUp(KeyboardKey code){
	return privateReceiver->keyUp(code);
}

bool GraphicEngine::IsKeyPressed(KeyboardKey code){
	//return privateReceiver->keyPressed(code);
	return false;
}

keyStatesENUM GraphicEngine::GetKeyStatus(KeyboardKey code){
	return privateReceiver->GetKeyStatus(code);
}

void GraphicEngine::SetKeyStatus(KeyboardKey code, keyStatesENUM status){
	privateReceiver->setKeyStatus(code, status);
}

/*
irr::scene::IBillboardTextSceneNode* GraphicEngine::addBillboardText(std::string text, irr::scene::ISceneNode* parent, vector3df position, int id){
	irr::core::vector3df auxPos = irr::core::vector3df(0, 0, 0);
	auxPos.X = position.X; auxPos.Y = position.Y; auxPos.Z = position.Z;

	float dimX = text.length() * 0.1;
	float dimY = 0.25f;
	irr::core::dimension2d<irr::f32> dim = irr::core::dimension2d<irr::f32>(dimX, dimY);
	
	std::wstring wText = std::wstring(text.begin(), text.end());

	irr::scene::IBillboardTextSceneNode* board = privateSManager->addBillboardTextSceneNode(0, wText.c_str(), parent, dim, auxPos, id);
	return board;
}

irr::IrrlichtDevice* GraphicEngine::GetIrrlichtDevice(){
	return privateDevice;
}

MenuReceiver* GraphicEngine::GetMenuReceiver(){
	return privateMenuReceiver;
}
*/

void GraphicEngine::LoadMesh(std::string path){
	
}