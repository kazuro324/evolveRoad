#pragma once
#include "Character.h"
#include "Attribute.h"
#include "CardDataHolder.h"

struct CharacterCategory
{
	public:
		CharacterCategory(AttributeType _type, int _evolveTiming);
		AttributeType getAttributeType() const;
		int getEvolveTiming() const;
		bool operator<(const CharacterCategory& other) const;

	private:
		AttributeType m_type;
		int m_evolveTiming;

};

class CharacterDataHolder
{
	public:
		static CharacterDataHolder& Instance();
		CharacterDataHolder();
		Character getData(int _index);
		Character getData(CharacterCategory _category);
		Character getData(CharacterQuality _quality);

	private:
		static CharacterDataHolder* instance;
		std::map<CharacterCategory, Character> m_datas;

		CharacterQuality stringToQuality(String _qualityStr);

		const String s_Common = U"Common";
		const String s_Rare = U"Rare";
		const String s_Epic = U"Epic";
		const String s_Chimera = U"NotEnemy";
};
