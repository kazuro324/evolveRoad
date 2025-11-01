#pragma once
#include "ShareData.h"
#include "Card.h"
#include "AttributeModifier.h"
#include "Button.h"
#include "CharacterDataHolder.h"
#include "CardDataHolder.h"
#include "AudioManager.h"
#include <Siv3D.hpp>

class TitleScene : public App::Scene
{
	public:
		TitleScene(const InitData& _init);
		~TitleScene();
		void update() override;
		void draw() const override;
	private:
		Button m_startButton;
		Button m_exitButton;
		void moveTrainingScene();
		void exit();
		Effect effect;
		bool m_isLicenseOpen = false;

		const String s_ResultBack = U"ResultBack";
		const String s_LogoFont = U"LogoFont";
		const String s_TitleAudio = U"TrainingBGM";
		const String s_License = U"License";
};
