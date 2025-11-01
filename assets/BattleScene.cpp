#include "BattleScene.h"
#include "NumberEffect.cpp"

BattleScene::BattleScene(const InitData& _init)
	: IScene(_init)
{
	TextureAsset::Load(s_BattleBack);
	AudioAsset::Load(s_AttackAudio);
	AudioAsset(s_BattleBGM).setVolume(AudioManager::Instance().volume);
	AudioAsset(s_BattleBGM).play();
	FontAsset::Load(s_ParameterFont);

	m_player = getData().myCharacter;
	m_enemy = getData().nextBattleCharacter;

	m_player->setPos(Vec2{ 640, 490 });
	m_enemy->setPos(Vec2{ 640, 210 });

	m_playerHealth = m_player->getAttribute(AttributeType::Health)->getChangedValue();
	m_enemyHealth = m_enemy->getAttribute(AttributeType::Health)->getChangedValue();

	m_playerMaxHealth = m_playerHealth;
	m_enemyMaxHealth = m_enemyHealth;

	m_chimeraHealthDamage = m_playerMaxHealth * s_ChimeraHealthDamagePercentage;

	m_playerAttackDamage = m_player->getAttribute(AttributeType::Strength)->getChangedValue();
	m_enemyAttackDamage = m_enemy->getAttribute(AttributeType::Strength)->getChangedValue();

	m_playerAttackCooldown = m_player->getAttribute(AttributeType::Speed)->getChangedValue();
	m_enemyAttackCooldown = m_enemy->getAttribute(AttributeType::Speed)->getChangedValue();

	m_playerAttackCooldown = s_SpeedReverse - (m_playerAttackCooldown / (m_playerAttackCooldown + m_attackSpeedMultiplier));
	m_enemyAttackCooldown = s_SpeedReverse - (m_enemyAttackCooldown / (m_enemyAttackCooldown + m_attackSpeedMultiplier));

	if (m_player->getChimeraType() == AttributeType::Strength)
	{
		m_playerAttackCooldown *= s_ChimeraSpeedDeBuff;
	}

	m_playerAttackCooldown *= m_attackSpeedMultiplier;
	m_enemyAttackCooldown *= m_attackSpeedMultiplier;

	m_isChimeraHealth = m_player->getChimeraType() == AttributeType::Health;

	m_battleOverFont = Font(50);
	m_battleOverCount = 0.0;
}

BattleScene::~BattleScene()
{
	AudioAsset(s_BattleBGM).stop();

	TextureAsset::Release(s_BattleBack);
}

void BattleScene::update()
{
	if (m_isBattleOver)
	{
		m_battleOverCount += Scene::DeltaTime();
		if (m_battleOverCount >= s_BattleOverTime)
		{
			if (m_playerHealth <= 0 || getData().battleCount >= 7)
			{
				changeScene(State::Result, 1s);
				return;
			}
			changeScene(State::CardGet, 1s);
		}
	}

	if (m_isBattleOver)
	{
		return;
	}

	//攻撃クールダウンカウント
	m_playerAttackCount += Scene::DeltaTime();
	m_enemyAttackCount += Scene::DeltaTime();

	//キメラ体力処理
	if (m_isChimeraHealth)
	{
		if (m_chimeraHealthCount <= s_ChimeraHealthTime)
		{
			m_chimeraHealthCount += Scene::DeltaTime();
		}
		else
		{
			m_chimeraHealthCount = 0.0;
			Attack(m_chimeraHealthDamage, &m_playerHealth);
		}
	}

	//行動処理
	if (m_playerAttackCount >= m_playerAttackCooldown)
	{
		m_playerAttackCount = s_CountReset;
		Attack(m_playerAttackDamage, &m_enemyHealth);
		if (m_player->getChimeraType() == AttributeType::Speed)
		{
			Attack(5, &m_playerHealth);
		}
	}

	if (m_enemyAttackCount >= m_enemyAttackCooldown)
	{
		m_enemyAttackCount = s_CountReset;
		Attack(m_enemyAttackDamage, &m_playerHealth);
	}

	CheckDeath();
}

void BattleScene::draw() const
{
	TextureAsset(s_BattleBack).drawAt(640, 360);

	//キャラ描画
	m_player->draw();
	m_enemy->draw();

	m_battleOverFont(U"{}"_fmt(m_player->getName())).drawAt(640, 660);
	m_battleOverFont(U"{}"_fmt(m_enemy->getName())).drawAt(640, 40);

	//体力バー背景
	RectF( Arg::center(640, 600), 300, 30 ).draw(Palette::Black);
	RectF( Arg::center(640, 100), 300, 30 ).draw(Palette::Black);

	//体力バー
	RectF(Arg::leftCenter(490, 600), 300 * ((double)m_playerHealth / m_playerMaxHealth), 30).draw(Palette::Green);
	RectF(Arg::leftCenter(490, 100), 300 * ((double)m_enemyHealth / m_enemyMaxHealth), 30).draw(Palette::Green);

	FontAsset(s_ParameterFont)(U"{}/{}"_fmt(m_enemyHealth, m_enemyMaxHealth)).draw(Arg::center(640, 100), Palette::White);
	FontAsset(s_ParameterFont)(U"{}/{}"_fmt(m_playerHealth, m_playerMaxHealth)).draw(Arg::center(640, 600), Palette::White);

	//攻撃時間背景
	RectF( Arg::center(640, 570), 300, 20 ).draw(Palette::Black);
	RectF( Arg::center(640, 130), 300, 20 ).draw(Palette::Black);

	//攻撃時間バー
	RectF( Arg::leftCenter(490, 570), 300 * (m_playerAttackCount / m_playerAttackCooldown), 20 ).draw(Palette::Orange);
	RectF( Arg::leftCenter(490, 130), 300 * (m_enemyAttackCount / m_enemyAttackCooldown), 20 ).draw(Palette::Orange);

	effect.update();

	//戦闘判定文字表示
	if (m_isBattleOver)
	{
		m_battleOverFont(m_winText).drawAt(640, 360, Palette::Blue);
	}
}

void BattleScene::Attack(int _attack, int* _target)
{
	*_target -= _attack;
	AudioAsset(s_AttackAudio).playOneShot(AudioManager::Instance().volume);

	if (*_target <= 0)
	{
		*_target = 0;
	}
	if (_target == &m_playerHealth)
	{
		effect.add<NumberEffect>(Vec2(640, 490), _attack, Font(30), Palette::Red);
	}
	else
	{
		effect.add<NumberEffect>(Vec2(640, 190), _attack, Font(30), Palette::Red);
	}
}

void BattleScene::CheckDeath()
{
	if (m_enemyHealth <= 0)
	{
		m_isBattleOver = true;
		m_winText = U"勝利";
	}

	if (m_playerHealth <= 0)
	{
		m_isBattleOver = true;
		m_winText = U"敗北";
	}
}
