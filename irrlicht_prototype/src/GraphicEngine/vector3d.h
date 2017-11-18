#include <irrlicht/irrlicht.h>

class vector3df{

public:

    vector3df();
    vector3df(float x, float y, float z);
    

private:
    irr::core::vector3df privateVector3df;

};