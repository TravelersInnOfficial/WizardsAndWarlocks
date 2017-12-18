#include <iostream>
#include "./irrlicht/irrlicht.h"

class MenuReceiver : public irr::IEventReceiver {

protected:

    int selectedOption = -1;
    virtual bool OnEvent(const irr::SEvent& event);

public:

    int ReadMenu();
    void Update();
    MenuReceiver();

};
