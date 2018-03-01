#ifndef IRRENGINE_H
#define IRRENGINE_H

#include <irrlicht/irrlicht.h>
#include "IrrGBody.h"
#include "IrrGCamera.h"
#include "IrrGEntity.h"
#include "IrrEventReceiver.h"
#include "IrrMenuReceiver.h"
#include <Keycodes.h>
#include <vector2d.h>
#include <vector3d.h>
#include <vector4d.h>
#include <map>
#include <vector>
#include <OverlayCodes.h>

class IrrEngine{
    friend class IrrMenuManager;
    friend class GUIEngine;
    friend class IrrGEntity;
    friend class IrrGBody;
    
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
    void setTextureToBody(IrrGBody* body, int layer, std::string path);
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

    IrrGBody* addCube2Scene(
        vector3df position = vector3df(0,0,0), 
        vector3df rotation = vector3df(0,0,0), 
        vector3df scale = vector3df(1,1,1),
        float size = 1.f,
        int id = -1
        );
    
    IrrGBody* addSphere2Scene(
        vector3df position = vector3df(0,0,0), 
        vector3df rotation = vector3df(0,0,0), 
        vector3df scale = vector3df(1,1,1),
        float radius = 0.5f,
        int id = -1
        );

    IrrGBody* addObjMeshSceneNode(std::string path);
    IrrGBody* addObjMeshSceneNode(std::string path, vector3df position, vector3df rotation, vector3df scale = vector3df(1,1,1));
    IrrGCamera* addCameraSceneNodeFPS(float rotateSpeed, float moveSpeed);
    IrrGCamera* addCameraSceneNode(vector3df position, vector3df lookat);
    IrrGCamera* getActiveCamera();

    /// GUIENV FUNCTIONS
    bool EscPressed();
    //std::string ReadText(MenuOption id);
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
    IrrEventReceiver* privateReceiver;
    IrrMenuReceiver* privateMenuReceiver;
    IrrGCamera* privateCamera;    

    // PRIVATE FUNCIONTS
    irr::scene::IBillboardTextSceneNode* addBillboardText(std::string text, irr::scene::ISceneNode* parent, vector3df position = vector3df(0,0,0), int id = -1);

    /**
     * Returns the irrlicht device of the motor graphic ---> ONLY FOR GUI ENGINE INITIALIZATION <---
    */
    irr::IrrlichtDevice* GetIrrlichtDevice();
    IrrMenuReceiver* GetMenuReceiver();
};

#endif