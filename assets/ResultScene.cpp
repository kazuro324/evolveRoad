#include "ResultScene.h"

ResultScene::ResultScene(const InitData& _init) : IScene(_init)
{
	TextureAsset::Load(s_ResultBack);

	m_titleButton = Button(Vec2(640, 600), 0, RectF(Arg::center(640, 600), 400, 200), Palette::White, U"タイトルに戻る", m_TitleFont, Palette::Black);
	m_titleButton.onClicked.addEvent(std::bind(&ResultScene::moveTitle, this));
	m_playerCharacter = getData().myCharacter;
	for (int i = 0; i < 3; i++)
	{
		m_totalEvolveCount += m_playerCharacter->getAttribute((AttributeType)i)->getEvolveCount();
	}

	getData().myCharacter->setPos(Vec2(1160, 170));
	m_name = getData().myCharacter->getName();
}

ResultScene::~ResultScene()
{
	getData().battleCount = 0;
	getData().m_deck.clear();
	getData().myCharacter = NULL;
	getData().nextBattleCharacter = NULL;
	TextureAsset::Release(s_ResultBack);
}

void ResultScene::update()
{
	m_titleButton.update();
}

void ResultScene::draw() const
{
	TextureAsset(s_ResultBack).drawAt(640, 360);

	m_titleButton.draw();
	Rect(Arg::center(200, 240), 300, 230).rounded(20).draw(Palette::Gray);
	Rect(Arg::center(200, 540), 300, 300).rounded(20).draw(Palette::Gray);
	Rect(Arg::center(1050, 330), 350, 450).rounded(20).draw(Palette::Gray);

	getData().myCharacter->draw();
	//m_characterNameFont(m_name).drawAt(1000, 170);
	m_characterNameFont(U"攻撃型決めラ").drawAt(1000, 170);


	for (int i = 0; i < 3; i++)
	{
		int currentValue = m_playerCharacter->getAttribute((AttributeType)i)->getValue();
		int maxValue = m_playerCharacter->getAttribute((AttributeType)i)->getMaxValue();

		RectF(Arg::leftCenter(900, 290 + (100 * i)), 300, 5).rounded(40).draw(Palette::Black);
		RectF(Arg::leftCenter(900, 290 + (100 * i)), m_playerCharacter->getAttribute((AttributeType)i)->getValuePercentage() * 300, 5).rounded(40).draw(Palette::Green);
	}

	int health = m_playerCharacter->getAttribute(AttributeType::Health)->getValue();
	int strength = m_playerCharacter->getAttribute(AttributeType::Strength)->getValue();
	int speed = m_playerCharacter->getAttribute(AttributeType::Speed)->getValue();

	m_dataFont(U"体力").draw(Arg::leftCenter(900, 270));

	//体力表示
	FontAsset(s_BaseValueFont)(U"{}"_fmt(m_playerCharacter->getAttribute(AttributeType::Health)->getBaseValue())).draw(Arg::center(1020, 260));
	FontAsset(s_ParameterFont)(U"{}/{}"_fmt(health, m_playerCharacter->getAttribute(AttributeType::Health)->getMaxValue())).draw(Arg::rightCenter(1200, 270), Palette::White);


	//攻撃力表示
	m_dataFont(U"攻撃").draw(Arg::leftCenter(900, 370));
	FontAsset(s_BaseValueFont)(U"{}"_fmt(m_playerCharacter->getAttribute(AttributeType::Strength)->getBaseValue())).draw(Arg::center(1020, 360));
	FontAsset(s_ParameterFont)(U"{}/{}"_fmt(strength, m_playerCharacter->getAttribute(AttributeType::Strength)->getMaxValue())).draw(Arg::rightCenter(1200, 370), Palette::White);


	//速度表示
	m_dataFont(U"速度").draw(Arg::leftCenter(900, 470));
	FontAsset(s_BaseValueFont)(U"{}"_fmt(m_playerCharacter->getAttribute(AttributeType::Speed)->getBaseValue())).draw(Arg::center(1020, 460));
	FontAsset(s_ParameterFont)(U"{}/{}"_fmt(speed, m_playerCharacter->getAttribute(AttributeType::Speed)->getMaxValue())).draw(Arg::rightCenter(1200, 470), Palette::White);

	Rect(Arg::center(640, 100), 300, 150).rounded(20).draw(Palette::Gray);
	Rect(Arg::center(640, 360), 400, 150).rounded(20).draw(Palette::Gray);

	if (getData().battleCount >= 7)
	{
		m_TitleFont(U"踏破結果").drawAt(640, 100);
		m_TitleFont(U"踏破おめでとう").drawAt(640, 360);
	}
	else
	{
		m_TitleFont(U"結果").drawAt(640, 100);
		m_TitleFont(U"もう一度頑張ろう").drawAt(640, 360);
	}

	m_dataFont(U"進化回数:{}"_fmt(m_totalEvolveCount)).drawAt(200, 200);
	m_dataFont(U"勝利回数:{}"_fmt(getData().battleCount)).drawAt(200, 300);
	int totalChange = m_playerCharacter->getAttribute(AttributeType::Health)->getTotalValue();
	m_dataFont(U"体力合計変化量:{}"_fmt(totalChange)).drawAt(200, 440);
	totalChange = m_playerCharacter->getAttribute(AttributeType::Strength)->getTotalValue();
	m_dataFont(U"攻撃合計変化量:{}"_fmt(totalChange)).drawAt(200, 540);
	totalChange = m_playerCharacter->getAttribute(AttributeType::Speed)->getTotalValue();
	m_dataFont(U"速度合計変化量:{}"_fmt(totalChange)).drawAt(200, 640);
}

void ResultScene::moveTitle()
{
	changeScene(State::Title, 0.5s);
}
