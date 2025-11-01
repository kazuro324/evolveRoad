#include <Siv3D.hpp>
#include "Button.h"
#include "AudioManager.h"
#include "ShareData.h"
#include "TitleScene.h"
#include "TrainingScene.h"
#include "BattleScene.h"
#include "CardGetScene.h"
#include "ResultScene.h"
#include "NumberEffect.cpp"

void Main()
{
	Window::SetTitle(U"Siv3D Sample");

	TextureAsset::Register(U"StatusBack", Resource(U"resources/textures/Status_Back_Ground.png"));
	TextureAsset::Register(U"ResultBack", Resource(U"resources/textures/Result_Back_Ground.png"));
	TextureAsset::Register(U"BattleBack", Resource(U"resources/textures/Battle_Back_Ground.png"));
	TextureAsset::Register(U"TrainingHelp", Resource(U"resources/textures/trainingHelp.png"));
	TextureAsset::Register(U"License", Resource(U"resources/textures/license.png"));
	FontAsset::Register(U"LogoFont", FontMethod::MSDF, 200, Resource(U"resources/fonts/Gisshiri.ttf"));
	FontAsset::Register(U"BaseValueFont", FontMethod::MSDF, 45, Resource(U"resources/fonts/Xolonium.ttf"));
	FontAsset::Register(U"ParameterValueFont", FontMethod::MSDF, 25, Resource(U"resources/fonts/Xolonium.ttf"));
	AudioAsset::Register(U"CardPick", Resource(U"resources/sounds/cardPick.mp3"));
	AudioAsset::Register(U"CardSelect", Resource(U"resources/sounds/cardSelect.mp3"));
	AudioAsset::Register(U"Attack", Resource(U"resources/sounds/attack.mp3"));
	AudioAsset::Register(U"BattleBGM", Resource(U"resources/sounds/battle.mp3"));
	AudioAsset::Register(U"TrainingBGM", Resource(U"resources/sounds/training.mp3"));
	AudioAsset::Load(U"BattleBGM");
	AudioAsset::Load(U"TrainingBGM");

	//ウィンドウサイズ変更
	Window::Resize(1280, 720);
	Scene::SetBackground(Palette::Beige);

	auto& _instance = CharacterDataHolder::Instance();
	auto& _data = CharacterDataHolder::Instance();
	auto& _audio = AudioManager::Instance();

	App manager;

	manager.add<TitleScene>(State::Title);
	manager.add<TrainingScene>(State::Training);
	manager.add<BattleScene>(State::Battle);
	manager.add<CardGetScene>(State::CardGet);
	manager.add<ResultScene>(State::Result);

	//シーンごとのアップデート処理
	while (System::Update())
	{
		if (not manager.update()) {
			break;
		}
	}
}
