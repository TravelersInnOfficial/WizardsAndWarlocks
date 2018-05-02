#ifndef GRAPHICENGINE_H
#define GRAPHICENGINE_H

#include "GRoom.h"
#include "GBody.h"
#include "GAnimation.h"
#include "GParticle.h"
#include "GCamera.h"
#include "GEntity.h"
#include "GSprite.h"
#include "GRect.h"
#include "EventReceiver.h"
#include "MenuReceiver.h"
#include <EventEnum.h>
#include <vector2d.h>
#include <vector3d.h>
#include <vector4d.h>
#include <map>
#include <vector>
#include <OverlayCodes.h>

class GraphicEngine{
	friend class MenuManager;
	friend class MainMenu;


	friend class GUIEngine;
	friend class GEntity;
	friend class GBody;
	friend class GRoom;
	friend class GSprite;
	friend class GRect;
	friend class GParticle;
	friend class Light;

public:
	/// DEVICE FUNCTIONS

	/**
	 * Returns Instance of the engine. If it not created, this method initializes it.
	*/
	static GraphicEngine* getInstance(bool isServer = false);

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
	void ChangeWindowName(std::string newName);

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
	 * @brief Set the Aim Color
	 * 
	 * @param r 
	 * @param g 
	 * @param b 
	 */
	void SetAimColor(float r, float g, float b);

	/**
	 * Draws all the efects activated
	*/
	void drawOverlays(OverlayCodes type);

	/**
	 * Returns the height of the screen
	*/
	int GetScreenHeight();

	/**
	 * Returns the width of the screen
	*/
	int GetScreenWidth();

	/**
	 * @brief Get the Aspect Ratio
	 * 
	 * @return float 
	 */
	float GetAspectRatio();

	/// SMANAGER FUNCTIONS

	/**
	 * Draws all the scene nodes.
	*/
	void drawAll();

	void BeginDraw();
	void EndDraw();

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
	 * @brief Adds an animated mesh to scene and returns animation
	 * 
	 * @param position 	vector of object position
	 * @param rotation 	vector of object rotation
	 * @param scale 	vector of object scale
	 * @return GAnimation* 
	 */
	GAnimation* addAnimatedMeshSceneNode(vector3df position = vector3df(0,0,0), vector3df rotation = vector3df(0,0,0), vector3df scale = vector3df(1,1,1));

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

	GSprite* addSprite(std::string texture = "", vector2df position = vector2df(0,0), vector2df size = vector2df(10, 10));

	GRect* add2DRect(vector2df position = vector2df(0,0), vector2df size = vector2df(10, 10));

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
	bool IsKeyDown(KeyboardKey code);

	/**
	 * Returns true is given code is released
	*/
	bool IsKeyReleased(KeyboardKey code);

	/**
	 * Returns true is given code is released
	*/
	bool IsKeyUp(KeyboardKey code);

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
	bool IsKeyPressed(KeyboardKey code);

	keyStatesENUM GetKeyStatus(KeyboardKey code);

	void SetKeyStatus(KeyboardKey code, keyStatesENUM status);

	void ResetScene();

	void AddDome();

	void ClearOverlay();

	void SetOverlayTransparency(float t);

	void EnableClipping();

	void DisableClipping();

	void ChangeMeshShader(SHADERTYPE shader);

	GRoom* AddRoom(int id, vector3df position, vector3df rotation, vector3df scale);

	void SetCurrentRoom(GRoom* room = nullptr);

	GPortal* AddConnection(int firstID, int secondID, vector3df position, vector3df rotation, vector3df scale);

	void CleanRooms();

	bool* GetShadowState();
	bool* GetParticleState();
	bool GetShadowActive();
	bool GetParticleActive();

private:
	GraphicEngine(bool isServer = false);
	bool m_isServer;
	bool ShadowState;
	bool ParticleState;

	VideoDriver*   privateDriver;
	SceneManager*  privateSManager;
	EventReceiver* privateReceiver;
	GCamera* privateCamera;
	std::vector<TFRect*> m_aim;
	bool moving_aim = true;

	std::vector<GRoom*> m_rooms;		// Habitaciones actuales en el motor grafico
	GRoom* m_currentRoom; 				// Habitacion actual al que se anyadiran los modelos

	TFSprite* m_actual_overlay;

	void CreateAim();
};

#endif