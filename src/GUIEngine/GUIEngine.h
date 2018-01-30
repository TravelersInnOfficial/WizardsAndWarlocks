#ifndef GUIENGINE_H
#define GUIENGINE_H

#include <iostream>
#include <./../GraphicEngine/GraphicEngine.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Irrlicht/Renderer.h>
#include <CEGUI/SchemeManager.h>

#include "GUIEnums.h"
#include "GUIWindow.h"

class GUIEngine{
public:
    static GUIEngine* GetInstance();
    ~GUIEngine();

    void Draw();
    void CreateFromFile(GUIAssetType assetType, std::string name);
    void ChangeCursor(std::string cursorImage);
    GUIWindow* CreateLayoutWindow(std::string route);
    void SetRoot(GUIWindow* newRootWindow);

    void DestroyWindow(GUIWindow* windowToDestroy);

    // Buscar vacio para ROOT
    GUIWindow* GetWindow(std::string windowName = "");

private:
    GUIEngine();
    std::vector<GUIWindow*> allWindows;

};

#endif