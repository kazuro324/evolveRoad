#pragma once
#include "Card.h"
#include "Attribute.h"
#include "AttributeModifier.h"

class CardDataHolder
{
	public:
		static CardDataHolder& Instance();
		CardDataHolder();
		Card getData(int id);
		Card getRandomCard();
		std::vector<Card> getCards() const;
		AttributeType stringToAttributeType(String _value);
		int getSize() const;



	private:
		static CardDataHolder* instance;
		std::vector<Card> m_datas;

		const String s_Health = U"Health";
		const String s_Chimera = U"Chimera";
		const String s_Strength = U"Strength";
		const String s_Speed = U"Speed";
};
