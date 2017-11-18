#include <irrlicht/irrlicht.h>

class VideoDriver{

public:

    VideoDriver();
    VideoDriver(irr::video::IVideoDriver* driver);

private:

    /**Irrlicht provisional video driver*/
    irr::video::IVideoDriver* privateDriver;
};