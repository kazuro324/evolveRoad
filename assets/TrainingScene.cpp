#include "TrainingScene.h"
#include "NumberEffect.cpp"
#include "GatherEffect.cpp"
#include "GrowEffect.cpp"

TrainingScene::TrainingScene(const InitData& init) : IScene(init)
{
	m_isHelp = false;
	TextureAsset::Load(s_TrainingBack);
	TextureAsset::Load(s_Help);
	FontAsset::Load(s_BaseValueFont);
	FontAsset::Load(s_ParameterFont);
	AudioAsset::Load(s_CardPickAudio);
	AudioAsset::Load(s_CardSelectAudio);
	AudioAsset::Load(s_TrainingAudio);
	AudioAsset(s_TrainingAudio).setVolume(AudioManager::Instance().volume);
	AudioAsset(s_TrainingAudio).play();

	m_battleStartButton = Button(Vec2(1100, 550), 0_deg, RectF(Arg::center(1100, 550), 400, 200), Palette::White, U"戦闘開始");

	m_playerCharacter = getData().myCharacter;
	m_playerCharacter->setPos(Scene::Center());

	Character _enemyCharacter;
	switch (getData().battleCount)
	{
		case 0:
		case 1:
		case 2:
			_enemyCharacter = CharacterDataHolder::Instance().getData(CharacterQuality::Common);
			break;

		case 3:
		case 4:
			_enemyCharacter = CharacterDataHolder::Instance().getData(CharacterQuality::Rare);
			break;

		case 5:
		case 6:
			_enemyCharacter = CharacterDataHolder::Instance().getData(CharacterQuality::Epic);
			break;

		default:
			_enemyCharacter = CharacterDataHolder::Instance().getData(CharacterQuality::Common);
			break;
	}
	
	getData().nextBattleCharacter = std::make_shared<Character>(_enemyCharacter);

	m_battleStartButton.onClicked.addEvent(std::bind(&TrainingScene::moveBattle, this));

	m_decks.clear();

	for (auto _card : getData().m_deck)
	{
		m_decks.push_back(_card->clone());
	}

	for (int i = 0; i < 3; i++)
	{
		pickCard();
	}
}

TrainingScene::~TrainingScene()
{
	AudioAsset(s_TrainingAudio).stop();
	FontAsset::Release(s_BaseValueFont);
	FontAsset::Release(s_ParameterFont);
	AudioAsset::Release(s_CardPickAudio);
	AudioAsset::Release(s_CardSelectAudio);
	TextureAsset::Release(s_TrainingBack);
	TextureAsset::Release(s_Help);
}

void TrainingScene::update()
{
	TextureAsset(s_TrainingBack).drawAt(640, 360);

	if (m_pickCardCount >= s_PickMax)
	{
		HideCardPrediction();
	}
	m_battleStartButton.update();
	if (SimpleGUI::Button(U"?", Vec2(25, 25)))
	{
		if (m_isHelp)
		{
			m_isHelp = false;
		}
		else
		{
			m_isHelp = true;
		}
	}

	if (m_pickCardCount < s_PickMax && !m_isEvolveUpdate)
	{
		for (auto& card : m_cards)
		{
			card->update();
		}
	}

	if (m_isReplaceFlag)
	{
		for (auto card : m_cards)
		{
			RemoveCard(card);
			m_decks.push_back(card);
		}

		for (int i = 0; i < 3; i++)
		{
			pickCard();
		}
		m_isReplaceFlag = false;
	}

	evolveUpdate();
}


void TrainingScene::draw() const
{
	RectF(Arg::center(225, 300), 400, 400).rounded(40).draw(Palette::Gray);

	RectF(Arg::center(640, 40), 650, 80).rounded(10).draw(ColorF(Palette::Gray, 0.95));
	m_dataFont(U"カードを使用してキャラクターを強化しよう").drawAt(640, 40, Palette::White);

	RectF(Arg::center(1100, 150), 400, 100).rounded(20).draw(Palette::Gray);
	m_dataFont(U"残りカード使用回数 : {}"_fmt(4 - m_pickCardCount)).drawAt(1100,150, Palette::White);

	m_characterNameFont(m_playerCharacter->getName()).drawAt(220,150, Palette::White);

	FontAsset(U"TitleFont")(U"育成ターン").drawAt(Vec2(640, 50), Palette::Black);

	showData();


	for (const auto& card : m_cards)
	{
		card->draw();
	}
	m_battleStartButton.draw();
	m_playerCharacter->draw();
	
	m_effects.update();

	showHelp();
}

/// @brief ランダムなカードを手札に加える
void TrainingScene::pickCard()
{
	if (m_decks.size() <= 0)
	{
		return;
	}

	//カードの追加
	int _randomPickIndex = Random<int>(0, m_decks.size() - 1);
	Card* _pickedCard = m_decks[_randomPickIndex];
	m_decks.erase(std::remove(m_decks.begin(), m_decks.end(), _pickedCard), m_decks.end());
	m_cards.push_back(_pickedCard);
	RePositionCards();

	//カード初期設定
	_pickedCard->setTarget(m_playerCharacter);
	_pickedCard->onClicked.addEvent(std::bind(&TrainingScene::rePlace, this));
	_pickedCard->onUsed.addEvent(std::bind(&TrainingScene::ShowModifierNumber, this, std::placeholders::_1));
	_pickedCard->onUsed.addEvent(std::bind(&TrainingScene::RemoveCard, this, std::placeholders::_1));
	_pickedCard->onUsed.addEvent(std::bind(&TrainingScene::CheckEvolve, this, std::placeholders::_1));
	_pickedCard->onMouseEnter.addEvent(std::bind(&TrainingScene::ShowCardPrediction, this, std::placeholders::_1));
	_pickedCard->onMouseExit.addEvent(std::bind(&TrainingScene::HideCardPrediction, this));
	_pickedCard->onClicked.addEvent(std::bind(&TrainingScene::HideCardPrediction, this));
}

/// @brief カード位置を再配置
void TrainingScene::RePositionCards()
{
	const double totalWidth = (m_cards.size() - 1) * CardSpacing;
	const double startX = Scene::Center().x - (totalWidth / 2);
	for (size_t i = 0; i < m_cards.size(); ++i)
	{
		m_cards[i]->setPos(Vec2{ startX + i * CardSpacing, 640 });
	}
}

void TrainingScene::RemoveCard(Card* _card)
{
	m_cards.erase(std::remove(m_cards.begin(), m_cards.end(), _card), m_cards.end());
	RePositionCards();
}

void TrainingScene::ShowModifierNumber(Card* _card)
{
	for (auto& mod : _card->getModifiers())
	{
		String _text;
		Color _showColor;

		int currentValue = m_playerCharacter->getAttribute(mod.getAttributeType())->getValue();

		if (currentValue - mod.m_value < 0)
		{
			mod.m_value -= currentValue;
		}

		if (mod.m_value > 0)
		{
			_text = U"+{}"_fmt(mod.m_value);
			_showColor = Palette::Green;
		}
		else
		{
			_text = U"{}"_fmt(mod.m_value);
			_showColor = Palette::Red;
		}

		m_effects.add<NumberEffect>(Vec2(85, 225 + (100 * (int)mod.getAttributeType())), mod.m_value, Font(40), _showColor);
	}
}

void TrainingScene::moveBattle()
{
	changeScene(State::Battle, 0.5s);
}

void TrainingScene::CheckEvolve(Card* _card)
{
	for (int i = 0; i < s_AttributeCount; i++)
	{
		if (m_playerCharacter->getAttribute((AttributeType)i)->getValuePercentage() < s_AttributeOver)
		{
			continue;
		}

		int evolveCount = m_playerCharacter->getAttribute((AttributeType)i)->getEvolveCount();
		if (evolveCount >= 3)
		{
			evolveCount = 3;
		}
		Character evolveCharacter;

		//キメラに進化するかしないか
		if (Random<double>(0.0, 1.0) <= s_chimeraPercentage)
		{
			evolveCharacter = CharacterDataHolder::Instance().getData(CharacterCategory(AttributeType::Chimera, i));
		}
		else
		{
			evolveCharacter = CharacterDataHolder::Instance().getData(CharacterCategory((AttributeType)i, evolveCount + 1));
		}

		m_playerCharacter->getAttribute((AttributeType)i)->addEvolveCount();
		m_evolveToCharacter = evolveCharacter;
		m_isEvolveUpdate = true;
		return;
	}
}

void TrainingScene::evolveUpdate()
{
	if (!m_isEvolveUpdate)
	{
		return;
	}

	m_evolveTimeCount += Scene::DeltaTime();

	if (m_evolveTimeCount <= 3.0)
	{
		m_effects.add<GatherEffect>(Scene::Center(), 700);
		return;
	}

	if (!m_isAddGrowEffect)
	{
		m_effects.add<GrowEffect>(Scene::Center(), 400, 900);
		m_isAddGrowEffect = true;
	}

	if (m_evolveTimeCount > 4)
	{
		m_playerCharacter->evolve(&m_evolveToCharacter);
		m_evolveTimeCount = 0.0;
		m_isEvolveUpdate = false;
	}
}

void TrainingScene::ShowCardPrediction(Card* _card)
{
	for (auto mod : _card->getModifiers())
	{
		if (m_playerCharacter->getAttribute(mod.getAttributeType())->getValue() <= 0 && mod.m_value <= 0)
		{
			continue;
		}

		switch (mod.getAttributeType())
		{
			case AttributeType::Health:
				m_healthIncreasePrediction = mod.m_value;
				break;

			case AttributeType::Strength:
				m_strengthIncreasePrediction = mod.m_value;
				break;

			case AttributeType::Speed:
				m_speedIncreasePrediction = mod.m_value;
				break;

			default:
				break;
		}
	}
}

void TrainingScene::HideCardPrediction()
{
	m_healthIncreasePrediction = 0;
	m_strengthIncreasePrediction = 0;
	m_speedIncreasePrediction = 0;
}

void TrainingScene::rePlace()
{
	m_pickCardCount++;
	m_isReplaceFlag = true;
}

void TrainingScene::showData() const
{
	RoundRect predictionBarRect;

	for (int i = 0; i < 3; i++)
	{
		int currentValue = m_playerCharacter->getAttribute((AttributeType)i)->getValue();
		int maxValue = m_playerCharacter->getAttribute((AttributeType)i)->getMaxValue();
		int addValue = 0;
		switch (i)
		{
		case 0:
			addValue = m_healthIncreasePrediction;
			break;
		case 1:
			addValue = m_strengthIncreasePrediction;
			break;

		case 2:
			addValue = m_speedIncreasePrediction;
		}

		if (currentValue + addValue > maxValue && addValue > 0)
		{
			addValue = maxValue - currentValue;
		}

		if (currentValue + addValue < 0 && addValue < 0)
		{
			addValue = -currentValue;
		}

		RectF(Arg::leftCenter(50, 240 + (100 * i)), 350, 5).rounded(40).draw(Palette::Black);
		predictionBarRect = RectF(Arg::leftCenter(50, 240 + (100 * i)), m_playerCharacter->getAttribute((AttributeType)i)->getValuePercentage() * 350, 5).rounded(40).draw(Palette::Green);
		if (addValue > 0)
		{
			RectF(Arg::leftCenter(predictionBarRect.rightCenter()), ((double)addValue / maxValue) * 350, 5).draw(Palette::Lightgreen);
		}
		else if (addValue < 0)
		{
			RectF(Arg::rightCenter(predictionBarRect.rightCenter()), ((double)-addValue / maxValue) * 350, 5).draw(Palette::Red);
		}
	}

	RectF predictionNumberRect;
	int health = m_playerCharacter->getAttribute(AttributeType::Health)->getValue();
	int strength = m_playerCharacter->getAttribute(AttributeType::Strength)->getValue();
	int speed = m_playerCharacter->getAttribute(AttributeType::Speed)->getValue();

	int healthDif = health + m_healthIncreasePrediction;
	if (health + m_healthIncreasePrediction < 0)
	{
		healthDif = 0;
	}

	int strengthDif = strength + m_strengthIncreasePrediction;
	if (strength + m_strengthIncreasePrediction < 0)
	{
		strengthDif = 0;
	}

	int speedDif = speed + m_speedIncreasePrediction;
	if (speed + m_speedIncreasePrediction < 0)
	{
		speedDif = 0;
	}

	m_dataFont(U"体力").draw(Arg::leftCenter(50, 220));

	//体力表示
	FontAsset(s_BaseValueFont)(U"{}"_fmt(m_playerCharacter->getAttribute(AttributeType::Health)->getBaseValue())).draw(Arg::center(200, 210));
	predictionNumberRect = FontAsset(s_ParameterFont)(U"/{}"_fmt(m_playerCharacter->getAttribute(AttributeType::Health)->getMaxValue())).draw(Arg::rightCenter(400, 220), Palette::White);
	if (m_healthIncreasePrediction > 0)
	{
		FontAsset(s_ParameterFont)(U"{}"_fmt(healthDif))
			.draw(Arg::rightCenter(predictionNumberRect.pos.x, 220), Palette::Lightgreen);
	}
	else if (m_healthIncreasePrediction < 0)
	{
		FontAsset(s_ParameterFont)(U"{}"_fmt(healthDif))
			.draw(Arg::rightCenter(predictionNumberRect.pos.x, 220), Palette::Red);
	}
	else
	{
		FontAsset(s_ParameterFont)(U"{}"_fmt(healthDif))
			.draw(Arg::rightCenter(predictionNumberRect.pos.x, 220), Palette::White);
	}

	//攻撃力表示
	m_dataFont(U"攻撃").draw(Arg::leftCenter(50, 320));
	FontAsset(s_BaseValueFont)(U"{}"_fmt(m_playerCharacter->getAttribute(AttributeType::Strength)->getBaseValue())).draw(Arg::center(200, 310));
	predictionNumberRect = FontAsset(s_ParameterFont)(U"/{}"_fmt(m_playerCharacter->getAttribute(AttributeType::Strength)->getMaxValue())).draw(Arg::rightCenter(400, 320), Palette::White);
	if (m_strengthIncreasePrediction > 0)
	{
		FontAsset(s_ParameterFont)(U"{}"_fmt(strengthDif))
			.draw(Arg::rightCenter(predictionNumberRect.pos.x, 320), Palette::Lightgreen);
	}
	else if (m_strengthIncreasePrediction < 0)
	{
		FontAsset(s_ParameterFont)(U"{}"_fmt(strengthDif))
			.draw(Arg::rightCenter(predictionNumberRect.pos.x, 320), Palette::Red);
	}
	else
	{
		FontAsset(s_ParameterFont)(U"{}"_fmt(strengthDif))
			.draw(Arg::rightCenter(predictionNumberRect.pos.x, 320), Palette::White);
	}

	//速度表示
	m_dataFont(U"速度").draw(Arg::leftCenter(50, 420));
	FontAsset(s_BaseValueFont)(U"{}"_fmt(m_playerCharacter->getAttribute(AttributeType::Speed)->getBaseValue())).draw(Arg::center(200, 410));
	predictionNumberRect = FontAsset(s_ParameterFont)(U"/{}"_fmt(m_playerCharacter->getAttribute(AttributeType::Speed)->getMaxValue())).draw(Arg::rightCenter(400, 420), Palette::White);
	if (m_speedIncreasePrediction > 0)
	{
		FontAsset(s_ParameterFont)(U"{}"_fmt(speedDif))
			.draw(Arg::rightCenter(predictionNumberRect.pos.x, 420), Palette::Lightgreen);
	}
	else if (m_speedIncreasePrediction < 0)
	{
		FontAsset(s_ParameterFont)(U"{}"_fmt(speedDif))
			.draw(Arg::rightCenter(predictionNumberRect.pos.x, 420), Palette::Red);
	}
	else
	{
		FontAsset(s_ParameterFont)(U"{}"_fmt(speedDif))
			.draw(Arg::rightCenter(predictionNumberRect.pos.x, 420), Palette::White);
	}
}

void TrainingScene::showHelp() const
{
	if (!m_isHelp)
	{
		return;
	}

	TextureAsset(s_Help).drawAt(640, 360);
}
