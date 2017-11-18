#include "VideoDriver.h"

VideoDriver::VideoDriver(){

}

VideoDriver::VideoDriver(irr::video::IVideoDriver* driver){
    if (driver){
        privateDriver = driver;
    }
}

