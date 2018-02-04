#include "GUIEngine.h"

GUIEngine* GUIEngine::instance = 0;

GUIEngine::GUIEngine(){
    g_engine = GraphicEngine::getInstance();
    pDevice = g_engine->GetIrrlichtDevice();
    m_EventReceiver = g_engine->GetMenuReceiver();
    
    // Create GUI object
    m_GUIHandler = IrrIMGUI::createIMGUI(pDevice, m_EventReceiver);
}

GUIEngine* GUIEngine::GetInstance(){
	if(instance == 0) instance = new GUIEngine();
	return instance;
}

GUIEngine::~GUIEngine(){
    //m_GUIHandler->drop();
}

void GUIEngine::ToggleMenu(){
    
}

void GUIEngine::update(){
    // create the GUI elements
    m_GUIHandler->startGUI();
}

void GUIEngine::draw(){
   m_GUIHandler->drawAll();
}

IrrIMGUI::IIMGUIHandle * GUIEngine::GetGuiHandler(){return m_GUIHandler;}
irr::IrrlichtDevice* GUIEngine::GetPDevice(){return pDevice;}