/** Example 004 Movement

This Tutorial shows how to move and animate SceneNodes. The
basic concept of SceneNodeAnimators is shown as well as manual
movement of nodes using the keyboard.  We'll demonstrate framerate
independent movement, which means moving by an amount dependent
on the duration of the last run of the Irrlicht loop.

Example 19.MouseAndJoystick shows how to handle those kinds of input.

As always, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "GraphicEngine/GraphicEngine.h"

using namespace irr;

int main()
{
	GraphicEngine* engine = GraphicEngine::getInstance();

	if (engine == 0)
		return 1; // could not create selected driver.

	/*
	Create the node which will be moved with the WSAD keys. We create a
	sphere node, which is a built-in geometry primitive. We place the node
	at (0,0,30) and assign a texture to it to let it look a little bit more
	interesting. Because we have no dynamic lights in this scene we disable
	lighting for each model (otherwise the models would be black).
	*/
	GBody* node = engine->addSphere2Scene();
	
	if (node)
	{
		node->setPosition(vector3df(0,0,30));
		node->setMaterialTexture("../media/wall.bmp");
		node->setMaterialFlag("lightning", false);
	}

	/*
	Now we create another node, movable using a scene node animator. Scene
	node animators modify scene nodes and can be attached to any scene node
	like mesh scene nodes, billboards, lights and even camera scene nodes.
	Scene node animators are not only able to modify the position of a
	scene node, they can also animate the textures of an object for
	example. We create a cube scene node and attach a 'fly circle' scene
	node animator to it, letting this node fly around our sphere scene node.
	*/
//	scene::ISceneNode* n = smgr->addCubeSceneNode();
	GBody* n = engine->addCube2Scene();

	if (n)
	{
		n->setPosition(vector3df(20,0,30));
		n->setMaterialTexture("../media/t351sml.jpg");
		n->setMaterialFlag("lightning", false);
		engine->setAnimationFlyStraight(n, vector3df(-30, 0, 30), vector3df(30, 0, 30), 2000.0f, true, true);
	}

	/*
	To be able to look at and move around in this scene, we create a first
	person shooter style camera and make the mouse cursor invisible.
	*/
	engine->addCameraSceneNodeFPS();
	engine->setCursorVisible(false);

	/*
	Add a colorful irrlicht logo
	*/
//	device->getGUIEnvironment()->addImage(
//		driver->getTexture("../media/irrlichtlogoalpha2.tga"),
//		core::position2d<s32>(10,20));
//
//	gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(
//		L"", core::rect<s32>(10, 10, 400, 20));
//	diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	int then = engine->getTime();

	// This is the movemen speed in units per second.
	float MOVEMENT_SPEED = 5.f;
	vector3df nodePosition;

	while(engine->run())
	{
		// Work out a frame delta time.
		int now = engine->getTime();
		const float frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		/* Check if keys W, S, A or D are being held down, and move the
		sphere node around respectively. */
		nodePosition = node->getPosition();

		if(engine->IsKeyDown(TKEY_CODE::KEY_KEY_W))
			nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
		else if(engine->IsKeyDown(TKEY_CODE::KEY_KEY_S))
			nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;
		if(engine->IsKeyDown(TKEY_CODE::KEY_KEY_A))
			nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		else if(engine->IsKeyDown(TKEY_CODE::KEY_KEY_D))
			nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

		node->setPosition(nodePosition);

		engine->beginSceneDefault();

		engine->drawAll(); // draw the 3d scene
		engine->drawAllGUI(); // draw the gui environment (the logo)

		engine->endScene();
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	engine->drop();
	
	return 0;
}

/*
That's it. Compile and play around with the program.
**/
