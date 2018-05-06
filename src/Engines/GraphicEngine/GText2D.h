#ifndef GTEXT2D_H
#define GTEXT2D_H

#include <string>
class GraphicEngine;
class TF2DText;
template<typename T> class TOEvector2d;   
typedef TOEvector2d<float> TOEvector2df;

template<typename T> class vector2d;
typedef vector2d<float> vector2df;

class GText2D{
    friend class GraphicEngine;
public:
    ~GText2D();
    
    void SetText(std::string text);
    void SetPosition(float x, float y);
    void SetTextSize(float siz);
    vector2df GetSize();

private:
    GText2D(TF2DText* text);
    TF2DText* m_tf2dtext;
};

#endif