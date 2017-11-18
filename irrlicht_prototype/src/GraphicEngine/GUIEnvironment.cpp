#include "GUIEnvironment.h"

GUIEnvironment::GUIEnvironment(){

}

GUIEnvironment::GUIEnvironment(irr::gui::IGUIEnvironment* environment){
    if(environment){
        privateEnvironment = environment;
    }
}
