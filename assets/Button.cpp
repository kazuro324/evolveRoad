#include "Button.h"

Button::Button()
{
	m_pos = Vec2{ 0,0 };
	m_rotation = 0.0;
	m_ColliderRect = RectF{ 0,0,0,0 };
	m_backgroundTex = Texture{};
	m_color = Palette::White;
	m_backgroundCircle = Circle{ 0,0,0 };
	m_text = U"";
	m_font = Font(0);
	m_backgroundType = BackgroundType::None;
}

Button::Button(const Vec2 _pos, double _rotation, const RectF _collider, const Texture _backgroundTex)
{
	m_pos = _pos;
	m_rotation = _rotation;
	m_ColliderRect = _collider;
	m_backgroundTex = _backgroundTex;
	m_backgroundType = BackgroundType::Texture;
}

Button::Button(const Vec2 _pos, double _rotation, const RectF _collider, const Color _color)
{
	m_pos = _pos;
	m_rotation = _rotation;
	m_ColliderRect = _collider;
	m_color = _color;
	m_backgroundType = BackgroundType::RectF;
}

Button::Button(const Vec2 _pos, double _rotation, const RectF _collider, const Texture _backgroundTex, const String _text, const Font _font, const Color _textcolor)
{
	m_pos = _pos;
	m_rotation = _rotation;
	m_ColliderRect = _collider;
	m_backgroundTex = _backgroundTex;
	m_text = _text;
	m_font = _font;
	m_color = Palette::White;
	m_backgroundCircle = Circle{ 0,0,0 };
	m_textColor = _textcolor;
	m_backgroundType = BackgroundType::Texture;
}

Button::Button(const Vec2 _pos, double _rotation, const RectF _collider, const Circle _backgroundCircle, const Color _backcolor, const String _text, const Font _font, const Color _textcolor)
{
	m_pos = _pos;
	m_rotation = _rotation;
	m_ColliderRect = _collider;
	m_text = _text;
	m_font = _font;
	m_backgroundCircle = _backgroundCircle;
	m_textColor = _textcolor;
	m_backgroundType = BackgroundType::Circle;
}

Button::Button(const Vec2 _pos, double _rotation, const RectF _collider, const Color _backcolor, const String _text, const Font _font, const Color _textcolor)
{
	m_pos = _pos;
	m_rotation = _rotation;
	m_ColliderRect = _collider;
	m_text = _text;
	m_font = _font;
	m_color = _backcolor;
	m_textColor = _textcolor;
	m_backgroundType = BackgroundType::RectF;
}

void Button::setPos(const Vec2 _pos)
{
	m_pos = _pos;
}

void Button::setText(const String _text)
{
	m_text = _text;
}

void Button::update()
{
	//マウスオーバー判定
	if (m_ColliderRect.contains(Cursor::Pos()))
	{
		if (!m_isMouseOver)
		{
			m_isMouseOver = true;
			onMouseEnter.Fire();
		}
		onMouseStay.Fire();
		if (MouseL.down())
		{
			onClicked.Fire();
		}
	}
	else
	{
		if (m_isMouseOver)
		{
			m_isMouseOver = false;
			onMouseLeave.Fire();
		}
	}
}

void Button::draw() const
{
	switch (m_backgroundType)
	{
		case Button::BackgroundType::Texture:
			m_backgroundTex.drawAt(m_pos);
			break;
		case Button::BackgroundType::Circle:
			m_backgroundCircle.draw();
			break;
		case Button::BackgroundType::RectF:
			m_ColliderRect.rounded(40).draw(m_color);
			break;
		default:
			break;
	}

	//テキスト描画
	m_font(m_text).drawAt(m_pos, m_textColor);
}

/// @brief ボタン判定取得
RectF Button::getCollider() const
{
	return m_ColliderRect;
}
