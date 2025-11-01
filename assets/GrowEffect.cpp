#include <Siv3D.hpp>

struct GrowEffect : IEffect
{
	Vec2 m_position;
	double m_innerRadius;
	double m_outerRadius;

	GrowEffect(const Vec2 _pos,double innerRadius, double outerRadius)
	{
		m_position = _pos;
		m_innerRadius = innerRadius;
		m_outerRadius = outerRadius;
	}

	bool update(double t) override
	{
		double scale;
		if (t < 0.5)
		{
			scale = EaseOutBack(t);
		}
		if (t >= 0.5)
		{
			scale = EaseOutBack(1 - (t - 1));
		}

		Circle(m_position, m_outerRadius * scale).drawShadow(Vec2::Zero(), 30, 30, Palette::White);
		Circle(m_position, m_innerRadius * scale).drawShadow(Vec2::Zero(), 30, 30, Palette::Whitesmoke);

		return (t < 1);
	}
};
