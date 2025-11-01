#pragma once
#include "ShareData.h"
#include "AudioManager.h"
#include "Attribute.h"
#include <Siv3D.hpp>

class BattleScene : public App::Scene {
public:
	BattleScene(const InitData& _init);
	~BattleScene();
	void update() override;
	void draw() const override;
private:
	void Attack(int _attack, int* _target);
	void CheckDeath();

	std::shared_ptr<Character> m_player;
	std::shared_ptr<Character> m_enemy;

	double m_playerAttackCooldown = 0.0;
	double m_enemyAttackCooldown = 0.0;
	double m_playerAttackCount = 0.0;
	double m_enemyAttackCount = 0.0;

	int m_playerAttackDamage;
	int m_enemyAttackDamage;
	int m_playerHealth;
	int m_enemyHealth;
	int m_playerMaxHealth;
	int m_enemyMaxHealth;

	bool m_isBattleOver = false;
	String m_winText;
	Font m_battleOverFont;
	double m_battleOverCount;
	double m_attackSpeedMultiplier = 8;
	int m_chimeraHealthDamage;

	bool m_isChimeraHealth = false;
	double m_chimeraHealthCount = 0.0;

	Effect effect;

	const double s_SpeedReverse = 1.0;
	const double s_CountReset = 0.0;
	const double s_BattleOverTime = 4;
	const double s_ChimeraSpeedDeBuff = 1.4;
	const double s_ChimeraHealthTime = 3.0;
	const double s_ChimeraHealthDamagePercentage = 0.05;

	const String s_BattleBack = U"BattleBack";
	const String s_AttackAudio = U"Attack";
	const String s_BattleBGM = U"BattleBGM";
	const String s_ParameterFont = U"ParameterValueFont";
};
