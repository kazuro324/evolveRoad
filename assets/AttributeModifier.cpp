#include "AttributeModifier.h"

AttributeModifier::AttributeModifier()
{
	m_targetType = AttributeType::Health;
	m_value = 0;
}

AttributeModifier::AttributeModifier(AttributeType _type, int _value)
{
	m_targetType = _type;
	m_value = _value;
}

void AttributeModifier::ApplyModify(Attribute *_attribute)
{
	if (m_value > 0)
	{
		_attribute->addValue(m_value);
	}
	else
	{
		_attribute->removeValue(-m_value);
	}
}

AttributeType AttributeModifier::getAttributeType() const
{
	return m_targetType;
}
