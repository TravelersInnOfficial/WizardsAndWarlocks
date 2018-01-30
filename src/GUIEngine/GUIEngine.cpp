#include "GUIEngine.h"

static GUIEngine* instance = NULL;

GUIEngine* GUIEngine::GetInstance(){
	if(instance == NULL) instance = new GUIEngine();
	return instance;
}

GUIEngine::GUIEngine(){
    irr::IrrlichtDevice* myIrrlichtDevice = GraphicEngine::getInstance()->GetIrrlichtDevice();

    CEGUI::IrrlichtRenderer::bootstrapSystem(*myIrrlichtDevice);
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*> (CEGUI::System::getSingleton().getResourceProvider());
    
	rp->setResourceGroupDirectory("schemes", "./../assets/GUIAssets/schemes/");
	rp->setResourceGroupDirectory("imagesets", "./../assets/GUIAssets/imagesets/");
	rp->setResourceGroupDirectory("fonts", "./../assets/GUIAssets/fonts/");
	rp->setResourceGroupDirectory("layouts", "./../assets/GUIAssets/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "./../assets/GUIAssets/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "./../assets/GUIAssets/lua_scripts/");

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
}

GUIEngine::~GUIEngine(){
    for(int i = 0; i < allWindows.size(); i++){
        GUIWindow* w = allWindows.at(i);
        if(w != NULL) delete w;
    }
    allWindows.clear();
    instance = NULL;
}

void GUIEngine::Draw(){
    CEGUI::System::getSingleton().renderAllGUIContexts();
}

void GUIEngine::CreateFromFile(GUIAssetType assetType, std::string name){
    switch(assetType){
        case(GUI_SCHEMES):{
            CEGUI::SchemeManager::getSingleton().createFromFile(name);
            break;
        }
        case(GUI_FONTS):{
            CEGUI::FontManager::getSingleton().createFromFile(name);
            break;
        }
        default:{
            break;
        }
    }
}

void GUIEngine::ChangeCursor(std::string cursorImage){
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(cursorImage);
}

GUIWindow* GUIEngine::CreateLayoutWindow(std::string route){
    CEGUI::Window* myRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(route);
    GUIWindow* toReturn = new GUIWindow(myRoot);
    allWindows.push_back(toReturn);
    return(toReturn);
}

void GUIEngine::SetRoot(GUIWindow* newRootWindow){
    CEGUI::Window* newRoot = newRootWindow->myWindow;
    if(newRoot != NULL) CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(newRoot);
}

GUIWindow* GUIEngine::GetWindow(std::string windowName){
    GUIWindow* toRet = NULL;

    CEGUI::Window* toRetWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
    if(windowName.length() > 0 && toRetWindow != NULL) toRetWindow = toRetWindow->getChild(windowName);

    if(toRetWindow != NULL){
        toRet = new GUIWindow(toRetWindow);
        allWindows.push_back(toRet);
    }

    return toRet;
}

void GUIEngine::DestroyWindow(GUIWindow* windowToDestroy){
    CEGUI::Window* searchFor = windowToDestroy->myWindow;
    bool found = false;

    if(searchFor != NULL){
        for(int i = allWindows.size(); i < 0; i--){
            GUIWindow* w = allWindows.at(i);
            if(w != NULL){
                CEGUI::Window* checking = w->myWindow;
                if(checking == searchFor){
                    allWindows.erase(allWindows.begin() + i);
                    delete checking;
                    found = true;
                }
            }
        }
    }
    
    if(!found) CEGUI::WindowManager::getSingleton().destroyWindow(searchFor);

}