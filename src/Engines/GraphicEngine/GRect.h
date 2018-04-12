#ifndef GRECT_H
#define GRECT_H

#include <vector2d.h>

class GraphicEngine;
class TFRect;

class GRect{
	friend class GraphicEngine;
public:
	~GRect();
	void SetColor(float r, float g, float b);
	void SetWidth(float width);
	void SetHeight(float height);

	float GetWidth();

private:
	GRect(TFRect* rect);

	TFRect* m_rect;
};


#endif