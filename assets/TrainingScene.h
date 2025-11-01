#pragma once
#include "ShareData.h"
#include "Card.h"
#include "Character.h"
#include "CharacterDataHolder.h"
#include "Button.h"

class TrainingScene : public App::Scene
{
	public:
		TrainingScene(const InitData& _init);
		~TrainingScene();
		void update() override;
		void draw() const override;
		void pickCard();

	private:
		std::vector<Card*> m_cards;
		std::vector<Card*> m_decks;
		std::shared_ptr<Character> m_playerCharacter;
		Font m_characterNameFont{ 50 };
		Font m_dataFont{30};
		Button m_messageBox;
		Effect m_effects;
		Button m_battleStartButton;
		Character m_evolveToCharacter;
		double m_evolveTimeCount = 0.0;
		bool m_isAddGrowEffect = false;
		bool m_isEvolveUpdate = false;
		int m_healthIncreasePrediction = 0;
		int m_strengthIncreasePrediction = 0;
		int m_speedIncreasePrediction = 0;
		bool m_isReplaceFlag = false;
		int m_pickCardCount = 0;
		bool m_isHelp = false;

		void RePositionCards();
		void RemoveCard(Card* _card);
		void ShowModifierNumber(Card* _card);
		void moveBattle();
		void CheckEvolve(Card* _card);
		void evolveUpdate();
		void ShowCardPrediction(Card* _card);
		void HideCardPrediction();
		void rePlace();
		void showData() const;
		void showHelp() const;

		const String s_CardPickAudio = U"CardPick";
		const String s_CardSelectAudio = U"CardSelect";
		const String s_BaseValueFont = U"BaseValueFont";
		const String s_ParameterFont = U"ParameterValueFont";
		const String s_TrainingAudio = U"TrainingBGM";

		const double CardSpacing = 150;
		const int s_AttributeCount = 3;
		const double s_AttributeOver = 1.0;
		const double s_chimeraPercentage = 0.2;
		const int s_PickMax = 4;
		const String s_TrainingBack = U"StatusBack";
		const String s_Help = U"TrainingHelp";
};
