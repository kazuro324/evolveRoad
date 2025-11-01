struct NumberEffect : IEffect
{
	Vec2 m_start;

	int m_score;

	Font m_font;

	Color m_color;

	NumberEffect(const Vec2& start, int score, const Font& font, Color color)
		: m_start{ start }
		, m_score{ score }
		, m_font{ font }
		, m_color{ color }{
	}

	bool update(double t) override
	{

		m_font(m_score).drawAt(TextStyle::Outline(0.2, ColorF{ 0.0, static_cast<double>(m_color.a) }),
			60, m_start.movedBy(0, t * -120), m_color);

		return (t < 0.5);
	}
};
