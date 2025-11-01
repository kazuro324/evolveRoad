#include "CardGetScene.h"

CardGetScene::CardGetScene(const InitData& _init) : IScene(_init)
{
	TextureAsset::Load(s_CardGetBack);
	AudioAsset::Load(s_CardPickAudio);
	AudioAsset::Load(s_CardSelectAudio);

	std::vector<Card> cards = CardDataHolder::Instance().getCards();

	for (int i = 0; i < 3; i++)
	{
		Card* card = cards[Random<int>(3, cards.size() - 1)].clone();
		card->onClicked.addEvent(std::bind(&CardGetScene::moveTraining, this));
		card->blockModifier();
		card->onUsed.addEvent(std::bind(&CardGetScene::addCard, this, std::placeholders::_1));
		m_pickList.push_back(card);
	}

	RePositionCards();
}

CardGetScene::~CardGetScene()
{
	getData().battleCount++;
	for (auto& card : m_pickList)
	{
		card->unBlockModifier();
		card->onUsed.removeEvent(std::bind(&CardGetScene::addCard, this, std::placeholders::_1));
		card->onClicked.removeEvent(std::bind(&CardGetScene::moveTraining, this));
	}

	TextureAsset::Release(s_CardGetBack);
	AudioAsset::Release(s_CardPickAudio);
	AudioAsset::Release(s_CardSelectAudio);
}

void CardGetScene::draw() const
{
	TextureAsset(s_CardGetBack).drawAt(640, 360);

	m_titleFont(U"報酬").drawAt(640, 50, Palette::Black);

	for (auto card : m_pickList)
	{
		card->draw();
	}
}

void CardGetScene::update()
{
	for (auto card : m_pickList)
	{
		card->update();
	}
}

void CardGetScene::RePositionCards()
{
	const double totalWidth = (m_pickList.size() - 1) * CardSpace;
	const double startX = Scene::Center().x - (totalWidth / 2);
	for (size_t i = 0; i < m_pickList.size(); i++)
	{
		m_pickList[i]->setPos(Vec2{ startX + i * CardSpace, 360 });
	}
}

void CardGetScene::moveTraining()
{
	changeScene(State::Training, 0.5s);
}

void CardGetScene::addCard(Card* _pickCard)
{
	getData().m_deck.push_back(std::make_shared<Card>(*_pickCard));
}
