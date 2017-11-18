#include <irrlicht/irrlicht.h>

class GUIEnvironment{

public:

    GUIEnvironment();
    GUIEnvironment(irr::gui::IGUIEnvironment* environment);

private:

    /**Irrlicht provisional scene manager*/
    irr::gui::IGUIEnvironment* privateEnvironment;
};