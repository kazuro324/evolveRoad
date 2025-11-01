#pragma once
#include "Button.h"
#include "AttributeModifier.h"
#include "AudioManager.h"
#include "Character.h"


class Card
{
	public:
		Card();
		Card(const Card& _clone);
		Card(String _name, std::vector<AttributeModifier>);
		~Card();
		void update();
		void draw() const;
		void use();
		void blockModifier();
		void unBlockModifier();
		void setPos(const Vec2 _pos);
		Card* clone();
		void setTarget(std::shared_ptr<Character> _target);
		std::vector<AttributeModifier>& getModifiers();
		EventHandlerNoArgs onClicked;
		EventHandlerNoArgs onMouseHover;
		EventHandler<Card*> onMouseEnter;
		EventHandlerNoArgs onMouseExit;
		EventHandler<Card*> onUsed;

	private:
		Vec2 m_position;
		RectF m_collider;
		double m_rotation;
		Font m_font;
		String m_title;
		String m_description;
		Texture m_iconTex;
		const Vec2 m_size{ 100,140 };
		bool m_isBlockModifier = false;
		bool m_isMouseOver = false;
		void onEnter(Card* _card);
		void onExit();
		void modifierToDraw(int _index, AttributeModifier _mod) const;
		double m_scale = 1.0;
		std::shared_ptr<Character> m_targetCharacter;
		std::vector<AttributeModifier> m_attributeModifiers;

		const String s_healthText = U"体力";
		const String s_strengthText = U"攻撃";
		const String s_speedText = U"速度";
		const String s_ParameterFont = U"ParameterValueFont";
		const FilePath s_CardPickAudio = U"CardPick";
		const FilePath s_CardSelectAudio = U"CardSelect";
};
