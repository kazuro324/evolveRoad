#pragma once
#include <siv3D.hpp>
#include "Card.h"
#include "Character.h"

enum class State
{
	Title,
	Training,
	Battle,
	CardGet,
	Result
};

struct GameData
{
	std::shared_ptr<Character> myCharacter = nullptr;
	std::shared_ptr<Character> nextBattleCharacter = nullptr;

	std::vector<std::shared_ptr<Card>> m_deck;

	int battleCount = 0;
};

using App = SceneManager<State, GameData>;
