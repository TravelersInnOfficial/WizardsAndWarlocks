#include "GUIEngine.h"

GUIEngine* GUIEngine::instance = 0;

GUIEngine::GUIEngine(){
    // Irrlicht OpenGL Settings
    irr::SIrrlichtCreationParameters IrrlichtParams;
    IrrlichtParams.DriverType    = irr::video::EDT_OPENGL;
    IrrlichtParams.WindowSize    = irr::core::dimension2d<irr::u32>(1024, 800);
    IrrlichtParams.Bits          = 32;
    IrrlichtParams.Fullscreen    = false;
    IrrlichtParams.Stencilbuffer = true;
    IrrlichtParams.AntiAlias     = 16;
    IrrlichtParams.Vsync         = false;
    // Create standard event receiver for the IrrIMGUI
    IrrIMGUI::CIMGUIEventReceiver EventReceiver;  
    IrrlichtParams.EventReceiver = &EventReceiver;
    // Create Irrlicht device
    //irr::IrrlichtDevice * const pDevice = irr::createDeviceEx(IrrlichtParams);
    irr::IrrlichtDevice* pDevice = g_engine->GetIrrlichtDevice();
    // Create GUI object
    pGUI = IrrIMGUI::createIMGUI(pDevice, &EventReceiver);
}

GUIEngine::~GUIEngine(){
    pGUI->drop();
}

void GUIEngine::update(){
    // create the GUI elements
    pGUI->startGUI();
    ImGui::Begin("My first Window");
    ImGui::Text("Hello World!");
    if (ImGui::Button("Exit", ImVec2(40, 20)))
    {
    //pDevice->closeDevice();
    }
    ImGui::End();
}

void GUIEngine::draw(){
    // render your Irrlicht scene
    //pSceneManager->drawAll();
    // render the GUI
   pGUI->drawAll();
}