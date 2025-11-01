#pragma once
#include <Siv3D.hpp>
#include "ShareData.h"
#include "CardDataHolder.h"

class CardGetScene : public App::Scene
{
	public:
		CardGetScene(const InitData& _init);
		~CardGetScene();
		void draw() const override;
		void update() override;

	private:
		std::vector<Card*> m_pickList;
		std::vector<int> m_indexList;
		void RePositionCards();
		void moveTraining();
		void addCard(Card* _pickCard);
		Font m_titleFont = Font(50);
		double CardSpace = 220;
		const String s_CardGetBack = U"BattleBack";
		const String s_CardPickAudio = U"CardPick";
		const String s_CardSelectAudio = U"CardSelect";
};
