#include "GraphicEngine.h"
#include <TravelersOcularEngine/src/TOcularEngine/TOcularEngine.h>
#include "./../GUIEngine/GUIEngine.h"

#include <ShaderTypes.h>
#include <EntityTypes.h>

GraphicEngine::GraphicEngine(bool isServer){
	ShadowState = true;
	ParticleState = true;
	VideoDriver::m_assetsPath = "./../src/Engines/TravelersOcularEngine/assets";
	privateDriver = toe::GetVideoDriver();
	
	if(isServer){
		privateDriver->CreateWindows("Wizards&Warlocks", toe::core::TOEvector2di(1,1), false);
		privateDriver->Minimize();
	}
	else
	privateDriver->CreateWindows("Wizards&Warlocks",toe::core::TOEvector2di(800,600));
	//privateDriver->CreateWindows("Wizards&Warlocks", privateDriver->GetScreenResolution(), true);
	
	privateDriver->SetClearScreenColor(toe::core::TOEvector4df(0.7, 0.7, 1, 1));
	privateSManager = privateDriver->GetSceneManager();

	privateReceiver = new EventReceiver();
	privateDriver->SetIODriver(privateReceiver);

	m_actual_overlay = nullptr;
	privateCamera = nullptr;

	privateDriver->EnableClipping();
	privateSManager->SetAmbientLight(toe::core::TOEvector3df(0.45f,0.45f,0.45f));

	m_currentRoom = nullptr;
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

void GraphicEngine::ResetScene(){
	if(privateSManager != nullptr) privateSManager->ResetManager();
	
	for(int i = 0; i<m_aim.size(); i++){
		m_aim[i] = nullptr;
	}
	m_aim.clear();
	
	m_actual_overlay = nullptr;
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

void GraphicEngine::ToggleMenu(bool newState){
	if(privateDriver != nullptr){
		if(newState) GUIEngine::GetInstance()->InitReceiver();
		else privateDriver->SetReceiver();
	}
}

void GraphicEngine::ToggleCameraMovement(bool newState){
	/*if (privateCamera != nullptr){
		irr::scene::ICameraSceneNode* cam = (irr::scene::ICameraSceneNode*) privateCamera->privateNode;
		if(cam != nullptr) cam->setInputReceiverEnabled(newState);
	}*/
}

void GraphicEngine::setTextureToBody(GBody* body, int layer, std::string s){
	if(privateDriver != nullptr){
		TFMesh* auxMesh = (TFMesh*) body->privateNode;
		auxMesh->SetTexture(s.c_str());
	}
}

void GraphicEngine::paintLineDebug(vector3df f, vector3df t, vector3df c){
	if(privateSManager != nullptr)
		privateSManager->DrawLine(toe::core::TOEvector3df(f.X, f.Y, f.Z), toe::core::TOEvector3df(t.X, t.Y, t.Z), toe::core::TOEvector3df(c.X, c.Y, c.Z));
}

void GraphicEngine::CreateAim(){
	if(m_aim.size() == 0){
		int size_w = 15;
		int size_h = 2;

		float cenW =  privateDriver->GetWindowDimensions().X * 0.5;
		float cenH =  privateDriver->GetWindowDimensions().Y * 0.5;

		float rDist = 4;
		
		for(int i=0; i<5; i++){
			m_aim.push_back(toe::Add2DRect());
			m_aim[i]->SetColor(0.8f,0.0f,0.8f);
		}

		//Draw Crosshair
		m_aim[0]->SetPosition(cenW, cenH);
		m_aim[0]->SetSize(0,0);

		m_aim[1]->SetPosition(cenW					, cenH - rDist - size_w	); //down
		m_aim[1]->SetSize(size_h					, size_w					);

		m_aim[2]->SetPosition(cenW + rDist + size_h	, cenH 	 				); //right
		m_aim[2]->SetSize(size_w					, size_h				);

		m_aim[3]->SetPosition(cenW 					, cenH + rDist + size_h	); //up
		m_aim[3]->SetSize(size_h					, size_w);

		m_aim[4]->SetPosition(cenW - rDist - size_w	, cenH 					); //left
		m_aim[4]->SetSize(size_w					, size_h				);
	}
}

void GraphicEngine::SetAimColor(float r, float g, float b){
	for(int i = 0; i<m_aim.size(); i++) m_aim[i]->SetColor(r,g,b);
}
void GraphicEngine::drawAim(bool moving){
	CreateAim();

	if(moving != moving_aim){
		if(privateDriver != nullptr){
		
			int size_w = 15;
			int size_h = 2;

			float cenW =  privateDriver->GetWindowDimensions().X * 0.5;
			float cenH =  privateDriver->GetWindowDimensions().Y * 0.5;

			float rDist = 0;
			if(moving){
				rDist = 30;
				//center of screen
				m_aim[0]->SetSize(0, 0);
			}
			else{
				rDist = 4;
				//center of screen
				m_aim[0]->SetSize(2, 2);
			}

			//Draw Crosshair
			m_aim[1]->SetPosition(cenW					, cenH - rDist - size_w	); //down
			m_aim[2]->SetPosition(cenW + rDist + size_h	, cenH 	 				); //right
			m_aim[3]->SetPosition(cenW 					, cenH + rDist + size_h	); //up
			m_aim[4]->SetPosition(cenW - rDist - size_w	, cenH 					); //left
		}
		moving_aim = moving;
	}
}

void GraphicEngine::drawGrailGUI(float currentValue, float maxValue){
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
void GraphicEngine::ClearOverlay(){
	if(m_actual_overlay!=nullptr) m_actual_overlay->SetTexture("");
	
}

void GraphicEngine::SetOverlayTransparency(float t){
	if(m_actual_overlay!= nullptr) m_actual_overlay->SetAlpha(t);
}

void GraphicEngine::drawOverlays(OverlayCodes type){
	if(privateDriver != nullptr){
		std::string overlayTexture = OverlayPath[type];
		if(m_actual_overlay == nullptr){
			float W = GetScreenWidth();
			float H = GetScreenHeight();

			m_actual_overlay = toe::AddSprite("", toe::core::TOEvector2df(0,0),toe::core::TOEvector2df(W,H));
		}
		else if(overlayTexture.length() > 0 && m_actual_overlay->GetTexture() != overlayTexture){
			m_actual_overlay->SetTexture(overlayTexture);
		}
	}
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

float GraphicEngine::GetAspectRatio(){
	int W = GetScreenWidth();		
	int H = GetScreenHeight();
	return H/(float)W;
}

void GraphicEngine::draw2DImage(std::string texturePath, vector4df rect){
	/*if(privateDriver != nullptr){
		irr::video::ITexture* spellTexture = privateDriver->getTexture(texturePath.c_str());
		irr::core::rect<irr::s32> destRect = irr::core::rect<irr::s32>(rect.X, rect.Y, rect.X2, rect.Y2);
		const irr::core::dimension2d<irr::u32> size = spellTexture->getSize();
		irr::core::rect<irr::s32> imgRect = irr::core::rect<irr::s32>(0, 0, size.Width, size.Height);
		privateDriver->draw2DImage(spellTexture, destRect, imgRect, 0, 0, true);
	}*/
}

void GraphicEngine::draw2DRectangle(vector3df c, float xInit, float yInit, float xEnd, float yEnd){
	/*if(privateDriver != nullptr){
		irr::video::SColor color = irr::video::SColor(255, c.X, c.Y, c.Z);
		privateDriver->draw2DRectangle(color, irr::core::rect<irr::s32>(xInit, yInit, xEnd, yEnd));
	}*/
}

// SMANAGER FUNCTIONS
void GraphicEngine::drawAll(){
	if(privateDriver != nullptr){ 
		privateDriver->BeginDraw();
		privateDriver->EndDraw();
	}
}

void GraphicEngine::BeginDraw(){
	if(privateDriver != nullptr) privateDriver->BeginDraw();
}

void GraphicEngine::EndDraw(){
	if(privateDriver != nullptr) privateDriver->EndDraw();
}

GBody* GraphicEngine::addCube2Scene(vector3df p, vector3df r, vector3df s, float size, int id){
	GBody* gb = nullptr;

	if(privateSManager != nullptr){
		gb = new GBody(
			toe::AddCube(
				toe::core::TOEvector3df(p.X, p.Y, p.Z),
				toe::core::TOEvector3df(r.X, r.Y, r.Z),
				toe::core::TOEvector3df(s.X * size, s.Y * size, s.Z * size)
			)
		);
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

GAnimation* GraphicEngine::addAnimatedMeshSceneNode(vector3df position, vector3df rotation, vector3df scale){
	GAnimation* ga = nullptr;
	toe::core::TOEvector3df position_TOE = toe::core::TOEvector3df(position.X, position.Y, position.Z);
	toe::core::TOEvector3df rotation_TOE = toe::core::TOEvector3df(rotation.X, rotation.Y, rotation.Z);
	toe::core::TOEvector3df scale_TOE = toe::core::TOEvector3df(scale.X, scale.Y, scale.Z);
	
	ga = new GAnimation(
		privateSManager->AddAnimation(position_TOE, rotation_TOE, scale_TOE)
	);
	return ga;
}

GCamera* GraphicEngine::addCameraSceneNodeFPS(float rotateSpeed, float moveSpeed){
	// DESTRUIMOS NUESTRA PROPIA CAMARA
	if(privateCamera != nullptr){
		delete privateCamera;
		privateCamera = nullptr;
	}

	// DESTRUIMOS LA CAMARA DEL MOTOR
	TFCamera* oldCamera = privateSManager->GetMainCamera();
	if (oldCamera != nullptr) privateSManager->DeleteCamera(oldCamera);
	
	// CREAMOS UNA NUEVA CAMARA EN EL MOTOR Y EN EL JUEGO
	toe::core::TOEvector3df defaultdata = toe::core::TOEvector3df(0, 0, 0);
	privateCamera = new GCamera(privateSManager->AddCamera(defaultdata, defaultdata, true));

	// LA DEVOLVEMOS
	return privateCamera;
}

GCamera* GraphicEngine::addCameraSceneNode(vector3df position, vector3df lookat){
	// DESTRUIMOS NUESTRA PROPIA CAMARA
	if(privateCamera != nullptr){
		delete privateCamera;
		privateCamera = nullptr;
	}

	// DESTRUIMOS LA CAMARA DEL MOTOR
	TFCamera* oldCamera = privateSManager->GetMainCamera();
	if (oldCamera != nullptr) privateSManager->DeleteCamera(oldCamera);
	
	// CREAMOS UNA NUEVA CAMARA EN EL MOTOR Y EN EL JUEGO
	toe::core::TOEvector3df position_TOE = toe::core::TOEvector3df(position.X, position.Y, position.Z);
	toe::core::TOEvector3df lookat_TOE = toe::core::TOEvector3df(lookat.X, lookat.Y, lookat.Z);
	privateCamera = new GCamera(privateSManager->AddCamera(position_TOE, lookat_TOE, true));

	// LA DEVOLVEMOS
	return privateCamera;
}

GSprite* GraphicEngine::addSprite(std::string texture, vector2df position, vector2df size){
	toe::core::TOEvector2df pos(position.X, position.Y);
	toe::core::TOEvector2df dims(size.X, size.Y);
	TFSprite* sprite = toe::AddSprite(texture, pos, dims);

	GSprite* output = new GSprite(sprite);
	return output;
}

GRect* GraphicEngine::add2DRect(vector2df position, vector2df size){
	toe::core::TOEvector2df pos(position.X, position.Y);
	toe::core::TOEvector2df dim(size.X, size.Y);
	TFRect* rect = toe::Add2DRect(pos, dim);

	GRect* output = new GRect(rect);
	return output;
}


GCamera* GraphicEngine::getActiveCamera(){
	return privateCamera;
}

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
	return privateReceiver->keyPressed(code);
}

keyStatesENUM GraphicEngine::GetKeyStatus(KeyboardKey code){
	return privateReceiver->GetKeyStatus(code);
}

void GraphicEngine::SetKeyStatus(KeyboardKey code, keyStatesENUM status){
	privateReceiver->setKeyStatus(code, status);
}

void GraphicEngine::AddDome(){
	privateSManager->AddDome();
}

void GraphicEngine::EnableClipping(){
	privateDriver->EnableClipping();
}

void GraphicEngine::DisableClipping(){
	privateDriver->DisableClipping();
}

void GraphicEngine::ChangeMeshShader(SHADERTYPE shader){
	privateDriver->ChangeShader(shader, TMESH_ENTITY);
}

GRoom* GraphicEngine::AddRoom(int id, vector3df position, vector3df rotation, vector3df scale){
	GRoom* room = nullptr;
	room = new GRoom(id, position, rotation, scale);
	m_rooms.push_back(room);
	return room;
}

void GraphicEngine::SetCurrentRoom(GRoom* room){
	m_currentRoom = room;
}

GPortal* GraphicEngine::AddConnection(int firstID, int secondID, vector3df position, vector3df rotation, vector3df scale){
	GPortal* output = nullptr;

	GRoom* firstRoom = nullptr;
	GRoom* secondRoom = nullptr;

	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		int ID = m_rooms[i]->GetId();

		if(ID == firstID) firstRoom = m_rooms[i];
		else if(ID == secondID) secondRoom = m_rooms[i];
	}

	if(firstRoom != nullptr && secondRoom != nullptr){
		output = firstRoom->AddConnection(secondRoom, position, rotation, scale);
	}
	return output;
}

void GraphicEngine::CleanRooms(){
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		delete m_rooms[i];
	}
	m_rooms.clear();
}

bool* GraphicEngine::GetShadowState(){
	return &ShadowState;
}
	
bool* GraphicEngine::GetParticleState(){
	return &ParticleState;
}

bool GraphicEngine::GetShadowActive(){
	return ShadowState;
}

bool GraphicEngine::GetParticleActive(){
	return ParticleState;
}