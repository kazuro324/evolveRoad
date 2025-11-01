#include "Card.h"

Card::Card()
{
	m_title = U"";
	m_font = Font{ 20 };
	setPos(Vec2::Zero());

	onClicked.addEvent(std::bind(&Card::use, this));
	onClicked.addEvent(std::bind(&Card::onExit, this));
	onMouseEnter.addEvent(std::function<void(Card*)>(std::bind(&Card::onEnter, this, std::placeholders::_1)));
	onMouseExit.addEvent(std::bind(&Card::onExit, this));
}

Card::Card(const Card& _clone)
{
	setPos(Vec2::Zero());
	m_title = _clone.m_title;
	m_rotation = _clone.m_rotation;
	m_attributeModifiers = _clone.m_attributeModifiers;
	m_collider = _clone.m_collider;
	m_font = _clone.m_font;
	m_scale = _clone.m_scale;
	m_targetCharacter = _clone.m_targetCharacter;

	onClicked.addEvent(std::bind(&Card::use, this));
	onClicked.addEvent(std::bind(&Card::onExit, this));
	onMouseEnter.addEvent(std::function<void(Card*)>(std::bind(&Card::onEnter, this, std::placeholders::_1)));
	onMouseExit.addEvent(std::bind(&Card::onExit, this));
}


Card::Card(String _name, std::vector<AttributeModifier> _modifier) : Card()
{
	setPos(Vec2::Zero());
	m_title = _name;
	for (auto _mod : _modifier)
	{
		m_attributeModifiers.push_back(_mod);
	}
}

Card::~Card()
{
	onClicked.clearEvents();
	onMouseEnter.clearEvents();
	onMouseExit.clearEvents();
	onUsed.clearEvents();
}

void Card::use()
{
	//キャラに効果を適用
	AudioAsset(s_CardPickAudio).playOneShot(AudioManager::Instance().volume);

	if (!m_isBlockModifier)
	{
		for (auto _modifier : m_attributeModifiers)
		{
			AttributeType _targetType = _modifier.getAttributeType();
			Attribute* _applyAttribute = m_targetCharacter->getAttribute(_targetType);
			if (!_applyAttribute)
			{
				return;
			}
			_modifier.ApplyModify(_applyAttribute);
		}
	}
	onUsed.Fire(this);
}

void Card::blockModifier()
{
	m_isBlockModifier = true;
}

void Card::unBlockModifier()
{
	m_isBlockModifier = false;
}

void Card::setPos(const Vec2 _pos)
{
	m_position = _pos;
	m_collider = RectF{ Arg::center = _pos, m_size };
}

Card* Card::clone()
{
	return new Card(*this);
}

void Card::setTarget(std::shared_ptr<Character> _target)
{
	m_targetCharacter = _target;
}

std::vector<AttributeModifier>& Card::getModifiers()
{
	return m_attributeModifiers;
}

void Card::update()
{
	if (m_collider.contains(Cursor::Pos()))
	{
		onMouseHover.Fire();
		if (!m_isMouseOver)
		{
			m_isMouseOver = true;
			onMouseEnter.Fire(this);
		}
		if (MouseL.down())
		{
			onClicked.Fire();
		}
	}
	else
	{
		if (m_isMouseOver)
		{
			m_isMouseOver = false;
			onMouseExit.Fire();
		}
	}
}

void Card::draw() const
{
	m_collider.scaled(m_scale).drawShadow(Vec2{10,10}, 0.9);
	m_collider.scaled(m_scale).draw(Palette::White);
	m_font(m_title).drawAt( 15 * m_scale, m_position + Vec2(0, -50), Palette::Black);
	
	for (int i = 0; i < m_attributeModifiers.size(); i++)
	{
		modifierToDraw(i, m_attributeModifiers[i]);
	}
}

void Card::onEnter(Card* _card)
{
	AudioAsset(s_CardSelectAudio).playOneShot(AudioManager::Instance().volume);
	m_scale = 1.2;
}

void Card::onExit()
{
	m_scale = 1;
}

void Card::modifierToDraw(int _index, AttributeModifier _mod) const
{
	String modName = U"";
	switch (_mod.getAttributeType())
	{
		case AttributeType::Health:
			modName += s_healthText;
			break;

		case AttributeType::Strength:
			modName += s_strengthText;
			break;

		case AttributeType::Speed:
			modName += s_speedText;
			break;

		default:
			break;
	}

	m_font(modName).draw(15 * m_scale, Arg::rightCenter = m_position + Vec2(0, -5 + 20 * _index), Palette::Black);
	if (_mod.m_value > 0)
	{
		FontAsset(s_ParameterFont)(U"+{}"_fmt(_mod.m_value)).draw(15 * m_scale, Arg::leftCenter = m_position + Vec2(0, -5 + 20 * _index), Palette::Green);
		return;
	}
	FontAsset(s_ParameterFont)(U"{}"_fmt(_mod.m_value)).draw(15 * m_scale, Arg::leftCenter = m_position + Vec2(0, -5 + 20 * _index), Palette::Red);

}
