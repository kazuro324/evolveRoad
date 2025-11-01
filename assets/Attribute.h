#pragma once

struct Attribute
{
	public:
		Attribute();
		Attribute(int _baseValue, int _maxValue, int _evolveIncrease, double _valueMultiplier);
		void addValue(int _value);
		void removeValue(int _value);
		void Evolve(const int _evolveMaxValue, const int _evolveIncrease, const double _nextMultiplier);
		int getBaseValue() const;
		int getValue() const;
		int getEvolveCount() const;
		int getMaxValue() const;
		int getEvolveIncrease() const;
		void addEvolveCount();
		double getMultiplier() const;
		void resetValue();
		int getChangedValue() const;
		double getValuePercentage() const;
		int getTotalValue() const;

	private:
		int firstValue;
		int baseValue;
		int value;
		int maxValue;
		int evolutionCount;
		int evolveIncrease;
		double valueMultiplier;
		int totalValue = 0;
};

enum class AttributeType
{
	Health = 0,
	Strength = 1,
	Speed = 2,
	Chimera = 3
};
