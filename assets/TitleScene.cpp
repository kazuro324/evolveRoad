#include "TitleScene.h"

TitleScene::TitleScene(const InitData& init) : IScene(init)
{
	TextureAsset::Load(s_License);
	TextureAsset::Load(s_ResultBack);
	FontAsset::Load(s_LogoFont);

	m_startButton = Button{ Vec2{640,480}, 0.0, RectF{Arg::center(640,480),400,200}, Palette::White, U"ゲームスタート", Font{FontMethod::MSDF, 48}, Palette::Black};
	m_exitButton = Button{ Vec2(1180, 670), 0.0, RectF(Arg::bottomRight(1280, 720),200,100), Palette::White, U"終了" };

	m_startButton.onClicked.addEvent(std::bind(&TitleScene::moveTrainingScene, this));
	m_exitButton.onClicked.addEvent(std::bind(&TitleScene::exit, this));

	AudioAsset(s_TitleAudio).setVolume(AudioManager::Instance().volume);
	AudioAsset(s_TitleAudio).play();

	//初期デッキ
	for (int i = 0; i < 5; i++)
	{
		getData().m_deck.push_back(std::make_shared<Card>(CardDataHolder::Instance().getRandomCard()));
	}

	Character _myCharacter = CharacterDataHolder::Instance().getData(0);
	getData().myCharacter = std::make_shared<Character>(_myCharacter);
}

TitleScene::~TitleScene()
{
	TextureAsset::Release(s_ResultBack);
	FontAsset::Release(s_LogoFont);
}

void TitleScene::update()
{
	TextureAsset(s_ResultBack).drawAt(640, 360);
	AudioAsset(s_TitleAudio).setVolume(AudioManager::Instance().volume);
	m_exitButton.update();
	m_startButton.update();

	if (SimpleGUI::Button(U"\U000F0FC3", Vec2(25, 25)))
	{
		if (m_isLicenseOpen)
		{
			m_isLicenseOpen = false;
		}
		else
		{
			m_isLicenseOpen = true;
		}
	}
}

void TitleScene::draw() const
{
	m_exitButton.draw();
	m_startButton.draw();
	FontAsset(s_LogoFont)(U"神化道").drawAt(Vec2(640, 150), Palette::Black);

	//音量スライダー
	SimpleGUI::Slider((0.5 < AudioManager::Instance().volume) ? U"\U000F057E"
			: (0.0 < AudioManager::Instance().volume) ? U"\U000F0580" : U"\U000F0581", AudioManager::Instance().volume, Vec2{ 100, 600 }, 30, 170);

	if (m_isLicenseOpen)
	{
		TextureAsset(s_License).drawAt(640, 360);
	}
}

void TitleScene::moveTrainingScene()
{
	changeScene(State::Training, 0.5s);
}

void TitleScene::exit()
{
	System::Exit();
}
