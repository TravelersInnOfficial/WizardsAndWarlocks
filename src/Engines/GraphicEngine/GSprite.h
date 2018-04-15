#ifndef GSPRITE_H
#define GSPRITE_H

#include <vector2d.h>

class GraphicEngine;
class TFSprite;

class GSprite{
	friend class GraphicEngine;
public:
	~GSprite();
	void ToFront();
	void ToBkg();

	vector2df GetPosition();
	float GetPosX();
	float GetPosY();
	float GetHeight();
	float GetWidth();
	std::string GetTexture();
	float GetTextureHeight();

	void ScrollV(float vel);
	void ScrollH(float vel);
	void SetScrollV(float value);
	void SetScrollH(float value);
	void SetWidth(float w);
	void SetHeight(float h);
	void SetMask(std::string path);
	void SetTexture(std::string path);
	void SetPosition(float X, float Y);
	void SetRotation(float rot);
	void SetRect(float x, float y, float w, float h);
	void SetTextureRect(float x, float y, float w, float h);
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	GSprite(TFSprite* sprite);
	TFSprite* m_sprite;

};

#endif