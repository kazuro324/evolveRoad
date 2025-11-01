#include "CharacterDataHolder.h"
#include <siv3D.hpp>

CharacterDataHolder* CharacterDataHolder::instance = nullptr;

CharacterDataHolder::CharacterDataHolder()
{
	CSV _loadFile = CSV(Resource(U"resources/csv/characterData.csv"));
	//CSV _loadFile = CSV(U"resources/csv/characterData.csv");

	if (not _loadFile)
	{
		throw Error(U"キャラクターデータcsvファイルの読み取りに失敗しました");
	}

	for (size_t row = 1; row < _loadFile.rows(); row++)
	{
		if (_loadFile[row].size() <= 16)
		{
			Print << U"{}列目のデータが読み取れませんでした。行数{}"_fmt(row + 1, _loadFile[row].size());
			continue;
		}

		//データ取得
		const String name = _loadFile[row][0];
		const String texPath = _loadFile[row][1];
		const CharacterQuality quality = stringToQuality(_loadFile[row][2]);
		const int evolveTiming = Parse<int>(_loadFile[row][3]);
		const AttributeType evolveType = CardDataHolder::Instance().stringToAttributeType(_loadFile[row][4]);
		
		Attribute health = Attribute(Parse<int>(_loadFile[row][5]),Parse<int>(_loadFile[row][6]),Parse<int>(_loadFile[row][7]), Parse<double>(_loadFile[row][8]));
		Attribute strength = Attribute(Parse<int>(_loadFile[row][9]), Parse<int>(_loadFile[row][10]), Parse<int>(_loadFile[row][11]), Parse<double>(_loadFile[row][12]));
		Attribute speed = Attribute(Parse<int>(_loadFile[row][13]), Parse<int>(_loadFile[row][14]), Parse<int>(_loadFile[row][15]), Parse<double>(_loadFile[row][16]));

		//データからキャラ構築
		CharacterCategory category(evolveType, evolveTiming);

		Character _characterData;

		if (evolveType == AttributeType::Chimera)
		{
			_characterData = Character(texPath, name, quality, health, strength, speed, (AttributeType)evolveTiming);
		}
		else
		{
			_characterData = Character(texPath, name, quality, health, strength, speed, AttributeType::Chimera);
		}

		m_datas[category] = _characterData;
	}
}

Character CharacterDataHolder::getData(int _index)
{
	auto it = m_datas.begin();
	if (_index > 0)
	{
		std::advance(it, _index);
	}
	return it->second;
}

Character CharacterDataHolder::getData(CharacterCategory _category)
{
	return m_datas[_category];
}

Character CharacterDataHolder::getData(CharacterQuality _quality)
{
	std::vector<Character> sameQualities;
	for (auto& [category, character] : m_datas)
	{
		if (character.getQuality() == _quality)
		{
			sameQualities.push_back(character);
		}
	}

	return sameQualities[Random<int>(0, sameQualities.size() - 1)];
}

CharacterDataHolder& CharacterDataHolder::Instance()
{
	if (instance == NULL)
	{
		instance = new CharacterDataHolder();
	}

	return *instance;
}

CharacterQuality CharacterDataHolder::stringToQuality(String _qualityStr)
{
	if (_qualityStr == s_Common)
		return CharacterQuality::Common;
	if (_qualityStr == s_Rare)
		return CharacterQuality::Rare;
	if (_qualityStr == s_Epic)
		return CharacterQuality::Epic;
	if (_qualityStr == s_Chimera)
		return CharacterQuality::NotEnemy;
}

CharacterCategory::CharacterCategory(AttributeType _type, int _evolveTiming)
{
	m_type = _type;
	m_evolveTiming = _evolveTiming;
}

AttributeType CharacterCategory::getAttributeType() const
{
	return m_type;
}

int CharacterCategory::getEvolveTiming() const
{
	return m_evolveTiming;
}

bool CharacterCategory::operator<(const CharacterCategory& other) const
{
	if (m_type != other.m_type)
		return m_type < other.m_type;
	return m_evolveTiming < other.m_evolveTiming;
}
