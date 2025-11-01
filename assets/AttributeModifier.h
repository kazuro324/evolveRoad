#pragma once
#include "Attribute.h"

struct AttributeModifier
{
	public:
		AttributeModifier();
		AttributeModifier(AttributeType _type, int _value);
		void ApplyModify(Attribute *_attribute);
		AttributeType getAttributeType() const;
		int m_value;


	private:
		AttributeType m_targetType;
};
