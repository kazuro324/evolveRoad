#include "CardDataHolder.h"

CardDataHolder* CardDataHolder::instance = nullptr;

CardDataHolder::CardDataHolder()
{
	CSV _loadFile = CSV(Resource(U"resources/csv/cardData.csv"));
	//CSV _loadFile = CSV(U"resources/csv/cardData.csv");

	if (not _loadFile)
	{
		throw Error(U"キャラクターデータcsvファイルの読み取りに失敗しました");
	}

	for (size_t row = 1; row < _loadFile.rows(); row++)
	{
		if (_loadFile[row].size() % 2 == 0)
		{
			Print << U"{}列目のデータが読み取れませんでした。データが抜けている可能性があります"_fmt(row + 1);
			continue;
		}

		const String name = _loadFile[row][0];
		std::vector<AttributeModifier> _modifiers;

		for (int i = 1; i < _loadFile[row].size(); i += 2)
		{
			AttributeType _atr = stringToAttributeType(_loadFile[row][i]);
			_modifiers.push_back(AttributeModifier(_atr, Parse<int>(_loadFile[row][i + 1])));
		}

		Card _cardData(name, _modifiers);
		m_datas.push_back(_cardData);
	}
}

Card CardDataHolder::getData(int id)
{
	return m_datas[id];
}

Card CardDataHolder::getRandomCard()
{
	return m_datas[Random<int>(0, m_datas.size() - 1)];
}

std::vector<Card> CardDataHolder::getCards() const
{
	return m_datas;
}

CardDataHolder& CardDataHolder::Instance()
{
	if (instance == NULL)
	{
		instance = new CardDataHolder();
	}

	return *instance;
}

//文字列をAttributeTypeへ変化
AttributeType CardDataHolder::stringToAttributeType(String _value)
{
	//stringではswitchが使えないためifを使用
	if (_value == s_Health)
		return AttributeType::Health;
	if (_value == s_Strength)
		return AttributeType::Strength;
	if (_value == s_Speed)
		return AttributeType::Speed;
	if (_value == s_Chimera)
		return AttributeType::Chimera;
}

int CardDataHolder::getSize() const
{
	return m_datas.size();
}
