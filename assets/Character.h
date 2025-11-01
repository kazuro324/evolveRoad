#pragma once
#include "Attribute.h"

enum class ActType
{
	None,
	Attack,
	Damaged
};

enum class CharacterQuality
{
	Common,
	Rare,
	Epic,
	NotEnemy
};

class Character
{
	public:
		Character();
		Character(String _texPath, String _name, CharacterQuality _quality, Attribute _health, Attribute _attack, Attribute _speed, AttributeType _chimeraType);
		Character(Character* _character);
		~Character();
		Character* clone();
		void draw() const;
		Attribute* getAttribute(AttributeType type);
		String getName() const;
		Vec2 getPos() const;
		AttributeType getChimeraType() const;
		CharacterQuality getQuality() const;
		void setPos(const Vec2 _pos);
		void setAct(ActType _type);
		Texture getTexture() const;
		void evolve(Character* _evolveToCharacter);

	private:
		Vec2 m_position;
		CharacterQuality m_quality;
		double m_rotation;
		String m_name;
		Texture m_avatarTex;
		std::map<AttributeType, Attribute> m_Attributes;
		AttributeType m_chimeraType = AttributeType::Chimera;
};
