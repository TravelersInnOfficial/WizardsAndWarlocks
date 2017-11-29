#include "GGUIElement.h"

GGUIElement::GGUIElement(){


}

GGUIElement::GGUIElement(irr::gui::IGUIElement* element){
    privateElement = element;
}

void GGUIElement::setText(std::wstring text){
    privateElement->setText(text.c_str());
}