#pragma once
#include "ShareData.h"
#include "Character.h"
#include "Button.h"
#include <Siv3D.hpp>

class ResultScene : public App::Scene {
	public:
		ResultScene(const InitData& _init);
		~ResultScene();
		void update() override;
		void draw() const override;

	private:
		int m_totalEvolveCount = 0;
		std::shared_ptr<Character> m_playerCharacter;
		Button m_titleButton;
		Font m_characterNameFont{ 35 };
		Font m_TitleFont = Font(50);
		Font m_dataFont = Font(30);
		String m_name;

		void moveTitle();
		const String s_ResultBack = U"ResultBack";
		const String s_BaseValueFont = U"BaseValueFont";
		const String s_ParameterFont = U"ParameterValueFont";
};
