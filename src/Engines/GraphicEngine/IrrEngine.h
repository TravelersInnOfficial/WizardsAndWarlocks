#ifndef IRRENGINE_H
#define IRRENGINE_H

#include <irrlicht/irrlicht.h>
#include "GBody.h"
#include "GCamera.h"
#include "GEntity.h"
#include "EventReceiver.h"
#include "MenuReceiver.h"
#include <Keycodes.h>
#include <vector2d.h>
#include <vector3d.h>
#include <vector4d.h>
#include <map>
#include <vector>
#include <OverlayCodes.h>
#include "GraphicEngine.h"

class IrrEngine : public GraphicEngine{
    friend class MenuManager;
    friend class GUIEngine;
    friend class GEntity;
    friend class GBody;
    
public:
    /// DEVICE FUNCTIONS
    static IrrEngine* getInstance(bool isServer = false);
    
    ~IrrEngine();
   
    bool run();
    bool drop();
    void setCursorVisible(bool visible);
    int getTime();
    void ChangeWindowName(std::wstring newName);
    void ToggleMenu(bool newState);
    void ToggleCameraMovement(bool newState);

    /// DRIVER FUNCTIONS
    bool beginScene();
    bool beginSceneDefault();
    bool endScene();
    void setTextureToBody(GBody* body, int layer, std::string path);
    void paintLineDebug(vector3df, vector3df, vector3df);
    void drawAim(bool playerMoving);
    void drawGrailGUI(float currentValue, float maxValue);
    void drawOverlays(OverlayCodes type);
    int GetScreenHeight();
    int GetScreenWidth();
    void draw2DImage(std::string texturePath, vector4df rect);
    void draw2DRectangle(vector3df color, float xInit, float yInit, float xEnd, float yEnd);

    /// SMANAGER FUNCTIONS
    void drawAll();

    GBody* addCube2Scene(
        vector3df position = vector3df(0,0,0), 
        vector3df rotation = vector3df(0,0,0), 
        vector3df scale = vector3df(1,1,1),
        float size = 1.f,
        int id = -1
        );
    
    GBody* addSphere2Scene(
        vector3df position = vector3df(0,0,0), 
        vector3df rotation = vector3df(0,0,0), 
        vector3df scale = vector3df(1,1,1),
        float radius = 0.5f,
        int id = -1
        );

    GBody* addObjMeshSceneNode(std::string path);
    GBody* addObjMeshSceneNode(std::string path, vector3df position, vector3df rotation, vector3df scale = vector3df(1,1,1));
    GCamera* addCameraSceneNodeFPS(float rotateSpeed, float moveSpeed);
    GCamera* addCameraSceneNode(vector3df position, vector3df lookat);
    GCamera* getActiveCamera();

    /// GUIENV FUNCTIONS
    MenuOption ReadButtonPressed();

    bool EscPressed();
    std::string ReadText(MenuOption id);
    vector2di GetCursorPosition();
    void SetCursorPosition(vector2di cursor);
    
    /// RECEIVER FUNCTIONS
    bool IsKeyDown(TKEY_CODE code);
    bool IsKeyReleased(TKEY_CODE code);
    bool IsKeyUp(TKEY_CODE code);
    void UpdateReceiver();
    void InitReceiver();
    bool IsKeyPressed(TKEY_CODE code);
    keyStatesENUM GetKeyStatus(TKEY_CODE code);
    void SetKeyStatus(TKEY_CODE code, keyStatesENUM status);

private:
    IrrEngine(bool isServer = false);
    bool m_isServer;

    /** Irrlicht provisional devices*/
    irr::IrrlichtDevice*        privateDevice;
    irr::video::IVideoDriver*   privateDriver;
    irr::scene::ISceneManager*  privateSManager;
    irr::gui::IGUIEnvironment*  privateGUIEnv;
    EventReceiver* privateReceiver;
    MenuReceiver* privateMenuReceiver;
    GCamera* privateCamera;    

    // PRIVATE FUNCIONTS
    void addStaticText(vector4di position, std::wstring text, bool border = false, bool wordWrap = true, int id = -1, irr::gui::IGUIWindow* parent = nullptr);
    irr::scene::IBillboardTextSceneNode* addBillboardText(std::string text, irr::scene::ISceneNode* parent, vector3df position = vector3df(0,0,0), int id = -1);

    /**
     * Returns the irrlicht device of the motor graphic ---> ONLY FOR GUI ENGINE INITIALIZATION <---
    */
    irr::IrrlichtDevice* GetIrrlichtDevice();
    MenuReceiver* GetMenuReceiver();
};

#endif