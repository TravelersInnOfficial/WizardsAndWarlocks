#include <irrlicht/irrlicht.h>
#include <iostream>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main(){

    irr::video::E_DRIVER_TYPE driverType;

    std::cout<<"Please select the driver you want for this example:\n"\
    " (a) OpenGL 1.5\n (b) Direct3D 9.0c\n (c) Direct3D 8.1\n"\
    " (d) Burning's Software Renderer\n (e) Software Renderer\n"\
    " (f) NullDevice\n (otherKey) exit\n\n";

    char c;
    std::cin>>c;

    switch(c){
        case 'a': driverType = irr::video::EDT_OPENGL;   break;
		case 'b': driverType = irr::video::EDT_DIRECT3D9;break;
		case 'c': driverType = irr::video::EDT_DIRECT3D8;break;
		case 'd': driverType = irr::video::EDT_BURNINGSVIDEO;break;
		case 'e': driverType = irr::video::EDT_SOFTWARE; break;
		case 'f': driverType = irr::video::EDT_NULL;     break;
		default: return 1;
    }

    // create device that will create game window
    irr::IrrlichtDevice *device = irr::createDevice(        
        driverType,                                             //Type of the device
        irr::core::dimension2d<irr::u32>(640, 480),             //Size of the window
        16,                                                     //Bits per pixel
        false,                                                  //Fullscreen
        false,                                                  //stencil buffer
        false,                                                  //vertical syncronisation
        0                                                       //user created event receiver
    );

    if (device == 0)
    return 1; // could not create selected driver.

    // Set window title
    device->setWindowCaption(L"Irrlicht test for the lulz");

    // Video and scene pointers
    irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::scene::ISceneManager* smgr = device->getSceneManager();

    // load pk3 (kind of zip file)
    //device->getFileSystem()->addFileArchive("../models/map-20kdm2.pk3");

    // obtain mesh from executable directory specified in makefile 
    irr::scene::IMesh* mesh = smgr->getMesh("../models/train.obj");
    irr::scene::ISceneNode* node = 0;
    
    // create an OctTree scene node to optimize only visible geometry
    //if (mesh)
    //node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
    if(mesh){
        node = smgr ->addMeshSceneNode(
            mesh,                                   // Object
            node,                                   // parent
            0,                                      // id
            irr::core::vector3df(0,0,0),            // Position
            irr::core::vector3df(0,0,0),            // rotation
            irr::core::vector3df(0,0,0),            // scale
            false                                   // add pointer
        );                     

    }

    // change position
    if (node)
    node->setPosition(irr::core::vector3df(0,0,20));

    // add camera fps style
    smgr->addCameraSceneNodeFPS();
    device->getCursorControl()->setVisible(false);    

    int lastFPS = -1;

    while(device->run())
    {
        if (device->isWindowActive())
		{
			driver->beginScene(true, true, irr::video::SColor(255,200,200,200));
			smgr->drawAll();
			driver->endScene();

			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
			    irr::core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
			    str += driver->getName();
			    str += "] FPS:";
                str += fps;
			    device->setWindowCaption(str.c_str());
			    lastFPS = fps;
		    }
        }  
        else 
            device->yield();
    }
    device->drop();

    return 0;
}