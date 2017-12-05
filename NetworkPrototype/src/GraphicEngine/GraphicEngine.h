#ifndef GRAPHICENGINE_H
#define GRAPHICENGINE_H

#include <irrlicht/irrlicht.h>
#include "GBody.h"
#include "GCamera.h"
#include "GEntity.h"
#include "GGUIElement.h"
#include "EventReceiver.h"
#include "Keycodes.h"
#include "../vector3d.h"
#include "../vector4d.h"

class GraphicEngine{

public:
    /// DEVICE FUNCTIONS

    /**
     * Returns Instance of the engine. If it not created, this method initializes it.
    */
    static GraphicEngine* getInstance();
    
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

    void drawAim();

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
     * Adds a mesh to scene and returns body
     * path: path to .obj mesh
    */
    GBody* addObjMeshSceneNode(std::string path);

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
     * Adds a camera scene node with an animator appropriate for FPS.
     * rotateSpeed: Speed in degress with which the camera is rotated
     * moveSpeed: Speed in units per millisecond with which the camera is moved
    */
    GCamera* addCameraSceneNodeFPS(float rotateSpeed, float moveSpeed);

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
     * Adds a static text
     * text: Text to be displayed
     * position: Rectangle specifying the borders of the static text
     * border: Set to true if the static text should have a 3d border
     * wordWrap: Enable if the text should wrap into multiple lines
     * parent: Parent item of the element
    */
    GGUIElement* addStaticText(std::wstring text, vector4di position, bool border = false, bool wordWrap = true, GGUIElement * parent = 0);
    
    /**
     * Creates a window at the right of the screen
    */
    GGUIElement* createDebugWindowControl();

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

    /**
     * Returns true is given code is held down
    */
    bool IsKeyPressed(TKEY_CODE code);

    /**
     * Returns true is given code is held down
    */
    bool IsLeftButtonPressed();
    bool IsLeftButtonDown();

private:
    GraphicEngine();

    /** Irrlicht provisional devices*/
    irr::IrrlichtDevice*        privateDevice;
    irr::video::IVideoDriver*   privateDriver;
    irr::scene::ISceneManager*  privateSManager;
    irr::gui::IGUIEnvironment*  privateGUIEnv;
    EventReceiver* privateReceiver;
    GCamera* privateCamera;

};

#endif