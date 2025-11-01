#include "Attribute.h"

Attribute::Attribute()
	: firstValue(0), baseValue(0), value(0), maxValue(0), evolutionCount(0), evolveIncrease(0)
{

}

Attribute::Attribute(int _baseValue, int _maxValue, int _evolveIncrease, double _valueMultiplier)
	: firstValue(_baseValue), baseValue(_baseValue), value(0), maxValue(_maxValue), evolutionCount(0), evolveIncrease(_evolveIncrease), valueMultiplier(_valueMultiplier)
{

}

void Attribute::addValue(int _value)
{
	if (value > maxValue)
	{
		totalValue += maxValue - value;
		value = maxValue;
		return;
	}
	value += _value;
	totalValue += _value;
}

void Attribute::removeValue(int _value)
{
	if (value - _value < 0)
	{
		totalValue -= value;
		value = 0;
		return;
	}
	value -= _value;
	totalValue -= _value;
}

void Attribute::Evolve(const int _evolveMaxValue, const int _evolveIncrease, const double _nextMultiplier)
{
	baseValue += _evolveIncrease;
	valueMultiplier = _nextMultiplier;
	maxValue = _evolveMaxValue;
	resetValue();
}

int Attribute::getBaseValue() const
{
	return baseValue;
}

int Attribute::getValue() const
{
	return value;
}

int Attribute::getMaxValue() const
{
	return maxValue;
}

int Attribute::getEvolveIncrease() const
{
	return evolveIncrease;
}

void Attribute::addEvolveCount()
{
	evolutionCount++;
}

double Attribute::getMultiplier() const
{
	return valueMultiplier;
}

int Attribute::getEvolveCount() const
{
	return evolutionCount;
}

void Attribute::resetValue()
{
	value = 0;
}

int Attribute::getChangedValue() const
{
	double _percentage = (double)value / maxValue;
	double _multiplierPercentage = 1.0 + (valueMultiplier - 1.0) * _percentage;

	return static_cast<int>(std::floor(baseValue * _multiplierPercentage));
}

double Attribute::getValuePercentage() const
{
	return (double)value / maxValue;
}

int Attribute::getTotalValue() const
{
	return totalValue;
}
