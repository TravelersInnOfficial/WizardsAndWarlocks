#ifndef GRAPHICENGINE_H
#define GRAPHICENGINE_H

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

class GraphicEngine{
    friend class MenuManager;
    friend class GUIEngine;

public:
    /// DEVICE FUNCTIONS

    /**
     * Returns Instance of the engine. If it not created, this method initializes it.
    */
    static GraphicEngine* getInstance();
    
    ~GraphicEngine();
    /**
     * Executed in main while loop of the game
    */
    bool run();
    
    /**
     * Drops the device to finish program
    */
    bool drop();

    /**
     * Hides or shows mouse cursor
     * visible: value(true/false)
    */
    void setCursorVisible(bool visible);

    /**
     * Returns current virtual time in milliseconds
    */
    int getTime();

    /**
     * Sets the window name
    */
    void ChangeWindowName(std::wstring newName);

    /**
     * Activates / Deactivates the main camera input and sets the cursor
    */
    void ToggleMenu(bool newState);

    void ToggleCameraMovement(bool newState);

    /// DRIVER FUNCTIONS

    /**
     * Application must call this method before performing any rendering.
    */
    bool beginScene();

    /**
     * Application must call this method before performing any rendering (default begin).
    */
    bool beginSceneDefault();

    /**
     * Presents the rendered image to the screen.
    */
    bool endScene();
    
    /**
     * Sets a texture to the giben body
     * layer: layer of texture
     * body: pointer to body where to apply texture
     * path: relative path to texture
    */
    void setTextureToBody(GBody* body, int layer, std::string path);
    //
    void paintLineDebug(vector3df, vector3df, vector3df);

    /**
     * Draws hud aiming square
    */
    void drawAim(bool playerMoving);

    /**
     * Draws GUI for the grail
    */
    void drawGrailGUI(float currentValue, float maxValue);

    /**
     * Draws all the efects activated
    */
    void drawOverlays(int type = -1);

    /**
     * Returns the height of the screen
    */
    int GetScreenHeight();

    /**
     * Returns the width of the screen
    */
    int GetScreenWidth();


    void draw2DImage(std::string texturePath, vector4df rect);
    /**
     * Draws a rectangle
     */
    void draw2DRectangle(vector3df color, float xInit, float yInit, float xEnd, float yEnd);

    /// SMANAGER FUNCTIONS

    /**
     * Draws all the scene nodes.
    */
    void drawAll();

    /**
     * Adds a sphere to scene and returns body
     * position: vector which contains position of the body 
     * rotation: vector which contains rotation of the body 
     * scale: vector which contains scale of the body
     * id: id of node
    */
    GBody* addCube2Scene(
        vector3df position = vector3df(0,0,0), 
        vector3df rotation = vector3df(0,0,0), 
        vector3df scale = vector3df(1,1,1),
        float size = 1.f,
        int id = -1
        );

    /**
     * Adds a sphere to scene and returns body
     * position: vector which contains position of the body 
     * rotation: vector which contains rotation of the body 
     * scale: vector which contains scale of the body
     * radius: radius of sphere
     * id: id of node
    */
    GBody* addSphere2Scene(
        vector3df position = vector3df(0,0,0), 
        vector3df rotation = vector3df(0,0,0), 
        vector3df scale = vector3df(1,1,1),
        float radius = 0.5f,
        int id = -1
        );

    /**
     * @brief Adds a mesh to scene and returns body
     * @param path: path to .obj mesh
    */
    GBody* addObjMeshSceneNode(std::string path);

    /**
     * @brief Adds a mesh to scene and returns body
     * 
     * @param path: path to .obj mesh
     * @param position: vector of object position
     * @param rotation: vector of object rotation
     * @param scale:    vector of object scale
     * @return GBody* Graphic body
     */
    GBody* addObjMeshSceneNode(std::string path, vector3df position, vector3df rotation, vector3df scale = vector3df(1,1,1));

    /**
     * Creates a fly straight animator, which lets the attached scene node fly or move along a line between two points
     * body: pointer to body to set animation
     * initialPos: Start point of the line
     * finalPos: End point of the line
     * time: Time in milli seconds how long the node should need to move
     * loop: If set to false, the node stops when the end point is reached
     * pinpong: Flag to set whether the animator should fly back from back to start again
    */
    void setAnimationFlyStraight(GBody* body,
        vector3df initialPos, 
        vector3df finalPos, 
        float time, 
        bool loop = true, 
        bool pingpong = true);

    /**
     * @brief Adds a camera scene node with an animator appropriate for FPS.
     * 
     * @param rotateSpeed: Speed in degress with which the camera is rotated
     * @param moveSpeed: Speed in units per millisecond with which the camera is moved
     * @return GCamera*: pointer to the camera created
     */
    GCamera* addCameraSceneNodeFPS(float rotateSpeed, float moveSpeed);

    /**
     * @brief Adds a free camera node with no event handler 
     * 
     * @param position: position in units where camera will be spawned
     * @param lookat: position of the target in unit where camera will be looking
     * @return GCamera*: pointer to the camera added
     */
    GCamera* addCameraSceneNode(vector3df position, vector3df lookat);
    /**
     * Returns active camera in scene
    */
    GCamera* getActiveCamera();

    //
    void addToDeletionQueue(irr::scene::ISceneNode*);
    
        /// GUIENV FUNCTIONS

    /**
     * Draws all gui elements by traversing the GUI environment starting at the root node.
    */
    void drawAllGUI();

    /**
     * Reads a GUI Menu from the reciever
    */
    MenuOption ReadButtonPressed();

    bool EscPressed();

    /**
     * Reads a GUI EditBox from the reciever
    */
    std::string ReadText(MenuOption id);

    /**
     * Returns a triangle selector for applying to a node
     * */
    irr::scene::ITriangleSelector* AddTriangleSelector(irr::scene::ISceneNode*);

    /**
     * Detects Irrlicht collisions with node triangles. 
     * Returns:
     *      - the node ID
     *      - vector(raycast collision point, normal vector of the triangle hitted)
     */
    std::map<int,std::vector<vector3df>> Raycast(vector3df, vector3df);

    /**
     * @brief Returns mouse position in the screen
     * 
     * @return vector2di: position in the screen
     */
    vector2di GetCursorPosition();
    
    /**
     * @brief Set mouse position in the screen
     * 
     * @param cursor position
     */
    void SetCursorPosition(vector2di cursor);
    
    /// RECEIVER FUNCTIONS

    /**
     * Returns true is given code is pressed
    */
    bool IsKeyDown(TKEY_CODE code);

    /**
     * Returns true is given code is released
    */
    bool IsKeyReleased(TKEY_CODE code);

    /**
     * Returns true is given code is released
    */
    bool IsKeyUp(TKEY_CODE code);

    /*
     Updates the reciever so we can controll if a key is pressed down
    */
    void UpdateReceiver();

    /*
     Init the reciever
    */
    void InitReceiver();

    /**
     * Returns true is given code is held down
    */
    bool IsKeyPressed(TKEY_CODE code);

    //
    keyStatesENUM GetKeyStatus(TKEY_CODE code);

    //
    void SetKeyStatus(TKEY_CODE code, keyStatesENUM status);

    void Raycast(vector3df Start, vector3df End, vector3df* point, vector3df* normal);

    irr::scene::IBillboardTextSceneNode* addBillboardText(std::string text, irr::scene::ISceneNode* parent, vector3df position = vector3df(0,0,0), int id = -1);

private:
    GraphicEngine();

    /** Irrlicht provisional devices*/
    irr::IrrlichtDevice*        privateDevice;
    irr::video::IVideoDriver*   privateDriver;
    irr::scene::ISceneManager*  privateSManager;
    irr::gui::IGUIEnvironment*  privateGUIEnv;
    EventReceiver* privateReceiver;
    MenuReceiver* privateMenuReceiver;
    GCamera* privateCamera;

    /**
     * Adds a static text
     * text: Text to be displayed
     * position: Rectangle specifying the borders of the static text
     * border: Set to true if the static text should have a 3d border
     * wordWrap: Enable if the text should wrap into multiple lines
     * id: id to easily access the button
    */
    void addStaticText(vector4di position, std::wstring text, bool border = false, bool wordWrap = true, int id = -1, irr::gui::IGUIWindow* parent = NULL);
    
    /**
     * Adds a button to hud
     * p: position (x, y) and size (x2,y2) of the button
     * text: text to be displayed
     * infoText: text to help the user
     * id: id to easily access the button
    */
    void addButton(vector4di p, std::wstring text = L"Button", std::wstring infoText = L"", int id = -1, irr::gui::IGUIWindow* parent = NULL, std::string texturePath = "");

    /**
     * Adds an editable text box to given position
     * p: position (x, y) and size (x2,y2) of the button
     * text: text to initally display in the box
    */
    void addEditBox(vector4di p, std::wstring text = L"", int id = -1, irr::gui::IGUIWindow* parent = NULL);

    /**
     * Returns the irrlicht device of the motor graphic ---> ONLY FOR GUI ENGINE INITIALIZATION <---
    */
    irr::IrrlichtDevice* GetIrrlichtDevice();
    MenuReceiver* GetMenuReceiver();
};

#endif