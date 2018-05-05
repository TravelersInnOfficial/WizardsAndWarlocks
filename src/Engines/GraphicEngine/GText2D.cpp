#include "GText2D.h"
#include "GraphicEngine.h"
#include <TOEvector2d.h>
#include <vector2d.h>

GText2D::GText2D(TF2DText* text){
    m_tf2dtext = text;
}

GText2D::~GText2D(){
    GraphicEngine::getInstance()->privateSManager->Delete2Delement(m_tf2dtext);
}

void GText2D::SetPosition(float x, float y){
    m_tf2dtext->SetPosition(x,y);
}

void GText2D::SetText(std::string text){
    if(m_tf2dtext->GetText().compare(text) != 0){
        m_tf2dtext->SetText(text);
    }
}

vector2df GText2D::GetSize(){
    TOEvector2df s = m_tf2dtext->GetSize();
    vector2df toRet(s.X,s.Y);
    return toRet;
}