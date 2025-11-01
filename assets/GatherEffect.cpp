# include <Siv3D.hpp>

struct GatherEffect : IEffect
{
	Vec2 m_center;

	struct Bubble
	{
		Vec2 startPos;
		double startTime;
		double scale;
		ColorF color;
	};

	Array<Bubble> m_bubbles;

	GatherEffect(const Vec2& center, const double spread)
		: m_center(center)
	{
		for (int32 i = 0; i < 5; i++)
		{
			Bubble bubble{
				.startPos = center + RandomVec2(Circle{ spread }),
				.startTime = Random(-0.3, 0.3),
				.scale = Random(0.3, 3.0),
				.color = Palette::White
			};
			m_bubbles << bubble;
		}
	}

	bool update(double t) override
	{
		for (const auto& bubble : m_bubbles)
		{
			const double localT = bubble.startTime + t;

			if (not InRange(localT, 0.0, 1.0))
			{
				continue;
			}

			const Vec2 pos = Math::Lerp(bubble.startPos, m_center, EaseInOutCubic(localT));

			const double radius = 10.0 * bubble.scale * (1.0 - localT);
			const double alpha = localT - 1.0;

			Circle{ pos, radius }.drawShadow(Vec2::Zero(),20, 20, Palette::White);
		}

		return (t < 3);
	}
};
