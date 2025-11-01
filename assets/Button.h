#pragma once
#include "EventHandler.h"
#include <Siv3D.hpp>

class Button
{
public:
	Button();
	Button(const Vec2 _pos, double _rotation, const RectF _collider, const Texture _backgroundTex);
	Button(const Vec2 _pos, double _rotation, const RectF _collider, const Color _color);
	Button(const Vec2 _pos, double _rotation, const RectF _collider, const Texture _backgroundTex, const String _text = U"", const Font _font = Font(40), const Color _textcolor = Palette::Black);
	Button(const Vec2 _pos, double _rotation, const RectF _collider, const Circle _backgroundCircle, const Color _backcolor, const String _text = U"", const Font _font = Font(40), const Color _textcolor = Palette::Black);
	Button(const Vec2 _pos, double _rotation, const RectF _collider, const Color _backcolor, const String _text = U"", const Font _font = Font(40), const Color _textcolor = Palette::Black);
	void setPos(const Vec2 _pos);
	void setText(const String _text);
	RectF getCollider() const;
	void update();
	void draw() const;
	EventHandlerNoArgs onMouseEnter;
	EventHandlerNoArgs onMouseLeave;
	EventHandlerNoArgs onMouseStay;
	EventHandlerNoArgs onClicked;

private:
	Vec2 m_pos;
	double m_rotation;
	String m_text;
	Font m_font;
	Texture m_backgroundTex;
	Circle m_backgroundCircle;
	RectF m_ColliderRect;
	Color m_color;
	Color m_textColor;
	bool m_isMouseOver = false;

	enum class BackgroundType
	{
		None,
		Texture,
		Circle,
		RectF
	} m_backgroundType;
};
