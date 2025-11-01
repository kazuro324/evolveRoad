#include "Character.h"

Character::Character()
{
	m_position = Vec2{ 0,0 };
	m_rotation = 0.0;
	m_name = U"カブトムシ";
	m_quality = CharacterQuality::Common;
	m_avatarTex = Texture{ U"😀"_emoji, TextureDesc::Unmipped };
	m_Attributes.clear();
	m_Attributes[AttributeType::Health] = Attribute{ 50, 100, 5, 0.2 };
	m_Attributes[AttributeType::Strength] = Attribute{ 10, 50, 5, 0.2 };
	m_Attributes[AttributeType::Speed] = Attribute{ 5, 50, 5, 0.2 };
}

Character::Character(Character* _character)
{
	m_position = _character->m_position;
	m_name = _character->m_name;
	m_quality = _character->m_quality;
	m_rotation = _character->m_rotation;
	m_avatarTex = _character->m_avatarTex;
	m_Attributes = _character->m_Attributes;
	m_chimeraType = _character->m_chimeraType;
}

Character::Character(String _texPath, String _name, CharacterQuality _quality, Attribute _health, Attribute _attack, Attribute _speed, AttributeType _chimeraType)
{
	m_position = Vec2{ 0,0 };
	m_quality = _quality;
	m_rotation = 0.0;
	m_name = _name;
	m_avatarTex = Texture{ Resource(_texPath), TextureDesc::Unmipped};
	m_Attributes.clear();
	m_Attributes[AttributeType::Health] = _health;
	m_Attributes[AttributeType::Strength] = _attack;
	m_Attributes[AttributeType::Speed] = _speed;
	m_chimeraType = _chimeraType;
}

Character::~Character()
{
	m_Attributes.clear();
}

Character* Character::clone()
{
	return new Character(*this);
}

void Character::draw() const
{
	m_avatarTex.drawAt(m_position);
}

Attribute* Character::getAttribute(AttributeType _type)
{
	return &m_Attributes[_type];
}

String Character::getName() const
{
	return m_name;
}

Vec2 Character::getPos() const
{
	return m_position;
}

AttributeType Character::getChimeraType() const
{
	return m_chimeraType;
}

CharacterQuality Character::getQuality() const
{
	return m_quality;
}

void Character::setPos(const Vec2 _pos)
{
	m_position = _pos;
}

void Character::setAct(ActType _type)
{
	switch (_type)
	{
		case ActType::Attack:
			
			break;

		case ActType::Damaged:
			break;

		default:
			break;
	}
}

Texture Character::getTexture() const
{
	return m_avatarTex;
}

void Character::evolve(Character* _evolveToCharacter)
{
	m_name = _evolveToCharacter->getName();
	m_quality = _evolveToCharacter->getQuality();
	for (int i = 0; i < 3; i++)
	{
		Attribute* _evolveAtr = _evolveToCharacter->getAttribute((AttributeType)i);
		int	maxValue = _evolveAtr->getMaxValue();
		int evolveIncrease = _evolveAtr->getEvolveIncrease();
		double valueMultiplier = _evolveAtr->getMultiplier();
		getAttribute((AttributeType)i)->Evolve(maxValue, evolveIncrease, valueMultiplier);
	}
	m_avatarTex = _evolveToCharacter->getTexture();
	m_chimeraType = _evolveToCharacter->getChimeraType();
}
