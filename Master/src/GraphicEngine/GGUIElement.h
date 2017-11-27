#ifndef GGUIELEMENT_H
#define GGUIELEMENT_H

#include <irrlicht/irrlicht.h>
#include <iostream>

class GGUIElement{

public:

    GGUIElement();

    /**
     * Set text of GGUIElement
     * text: new text to be display
    */
    void setText(std::wstring text);

    friend class GraphicEngine;

private:

    GGUIElement(irr::gui::IGUIElement* element);
    irr::gui::IGUIElement* privateElement;
};

#endif