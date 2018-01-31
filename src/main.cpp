/*#include <exception>
#include <iostream>
#include <IrrIMGUI/IncludeIrrlicht.h>
#include <IrrIMGUI/IncludeIMGUI.h>
#include <IrrIMGUI/IrrIMGUI.h>
#include <IrrIMGUI/IrrIMGUIDebug.h>

#define _TOSTR(x) #x
#define TOSTR(x) _TOSTR(x)
#define FASSERT(expr) if (!(expr)) { throw IrrIMGUI::Debug::ExAssert(__FILE__ "[" TOSTR(__LINE__) "] Assertion failed: \'" TOSTR(expr) "'\n"); }

void runScene(void) {
	using namespace IrrIMGUI;
	using namespace irr;

	CIMGUIEventReceiver EventReceiver;

	SIrrlichtCreationParameters IrrlichtParams;
	IrrlichtParams.DriverType    = video::EDT_OPENGL;
	IrrlichtParams.WindowSize    = core::dimension2d<u32>(1024, 800);
	IrrlichtParams.Bits          = 32;
	IrrlichtParams.Fullscreen    = false;
	IrrlichtParams.Stencilbuffer = true;
	IrrlichtParams.AntiAlias     = 16;
	IrrlichtParams.Vsync         = false;
	IrrlichtParams.EventReceiver = &EventReceiver;

	IrrlichtDevice * const pDevice = createDeviceEx(IrrlichtParams);
	FASSERT(pDevice)

	IIMGUIHandle * const pGUI = createIMGUI(pDevice, &EventReceiver);

	video::IVideoDriver  * const pDriver       = pDevice->getVideoDriver();
	scene::ISceneManager * const pSceneManager = pDevice->getSceneManager();

	pDriver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);
	
	irr::scene::ISceneNode * const pSkybox = pSceneManager->addSkyBoxSceneNode(
		pDriver->getTexture("../../media/Background.jpg"),
		pDriver->getTexture("../../media/Background.jpg"),
		pDriver->getTexture("../../media/Background.jpg"),
		pDriver->getTexture("../../media/Background.jpg"),
		pDriver->getTexture("../../media/Background.jpg"),
		pDriver->getTexture("../../media/Background.jpg")
	);
	
	pDriver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);

	scene::IMeshSceneNode * const pMoon = pSceneManager->addSphereSceneNode(5.0f, 128);
	FASSERT(pMoon);
	pMoon->setPosition(irr::core::vector3df(0,0,25));
	pMoon->setMaterialTexture(0, pDriver->getTexture("../../media/Phobos.jpg"));
	pMoon->setMaterialFlag(irr::video::EMF_LIGHTING, false);

	pSceneManager->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0,0,5));

	s32 LastFPS  = 0;
	u32 LastTime = pDevice->getTimer()->getRealTime();
	f32 Rotation = 0.0;
	f32 const RotPerSec = 0.01f;

	while(pDevice->run()) {
		pDriver->beginScene(true, true, irr::video::SColor(255,100,101,140));

		pGUI->startGUI();
		
		ImGui::Begin("Picture sources", NULL, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Background picture from Manuel Tellur / pixelio.de (Image-ID: 642831)");
		ImGui::Text("Moon (Phobos) texture from http://nasa3d.arc.nasa.gov");
		
		if (ImGui::Button("Exit", ImVec2(40, 20))) pDevice->closeDevice();
		ImGui::End();

		pSceneManager->drawAll();
		pGUI->drawAll();

		pDriver->endScene();

		u32 const Time = pDevice->getTimer()->getRealTime();
		u32 const DeltaTime = Time - LastTime;
		
		if (DeltaTime > 0) {
			Rotation += (360.0f * RotPerSec) * (DeltaTime / 1000.0f);
			LastTime = Time;
		}

		pMoon->setRotation(irr::core::vector3df(0,Rotation,0));

		s32 const FPS = pDriver->getFPS();
		if (FPS != LastFPS) {
			LastFPS = FPS;
			core::stringw TempString = L"Hello World, A simple IMGUI example - FPS: ";
			TempString += LastFPS;
			pDevice->setWindowCaption(TempString.c_str());
		}
	}

	pDevice->drop();
	pGUI->drop();
}

int main(void) {
	try{ runScene(); }
	catch(std::exception &rEx){ std::cout << rEx.what() << std::flush; }
	return 0;
}
*/

#include <iostream>
#include "./Managers/StateManager.h"


int main() {

	// MAIN MENU
	StateManager* state = StateManager::GetInstance();
	while(state->Update()){}

	delete state;
	
	return 0;
}