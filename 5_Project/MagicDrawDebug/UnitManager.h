#pragma once

#define ENEMY_MAX_ 80

struct BBOX;

class Hero;
class Monster;
class BossMonster;

class UnitManager
{
	static UnitManager*		m_Instance;
	int						m_CreateCount[3][4][2] =
							{	{1, 0, 1, 0, 1, 0, 1, 0},
								{7, 0, 6, 4, 0, 0, 0, 0},
								{6, 0, 5, 3, 3, 4, 4, 2}
							};
	int						m_CurrentMonsterCount[3][4][2] = { 0, };
	float					m_createTimeInFunction[4][2] = { 0, };
	int						m_TutorialCount = 0;
public:
	static int				m_Score[4][2];
	float					m_CreateTimer[4][2] = { 4, 1, 6.3, 7, 10.9, 11, 9.6, 12 };
	int						m_MonsterSpeed[4][2] = { 170, 100, 232, 102, 160, 110, 110, 150};

	static int				KillCount;

	SPRITE					m_HpSprite;
	ANISPRITE				m_HeroAniSprite;
	ANISPRITE				m_HeroNormalHitSprite;
	ANISPRITE				m_HeroBossHitSprite;
	ANISPRITE				m_HeroAttackSprite;
	Hero*					m_Hero = nullptr;

	ANISPRITE				m_MonsterAniSprites[5];
	ANISPRITE				m_MonsterDeadAniSprite[5];
	ANISPRITE				m_BossAttackSprite;

	ANISPRITE				m_BossHitSprite;
	ANISPRITE				m_BossSummonSprite;
	ANISPRITE				m_BossEyeSprite[6];

	Monster*				m_Monsters[4][ENEMY_MAX_ / 4] = { nullptr, };

	BossMonster*			m_BossMonster = nullptr;

	UnitManager();
	static UnitManager*		GetInstance();

	void	IncSpeed(int type, int num) { m_MonsterSpeed[type][num] += 10; }
	void	DecSpeed(int type, int num) { m_MonsterSpeed[type][num] -= 10; }

	int		GetSpeed(int type, int num) { return m_MonsterSpeed[type][num]; }

	void	IncTime(int type, int num) { m_CreateTimer[type][num] += .1f; }
	void	DecTime(int type, int num) { m_CreateTimer[type][num] -= .1f; }
	
	float	GetTimer(int type, int num) { return m_CreateTimer[type][num]; }

	void	IncCount(int type, int num) { m_CreateCount[2][type][num] += 1; }
	void	DecCount(int type, int num) { m_CreateCount[2][type][num] -= 1; }

	int		GetCount(int type, int num) { return m_CreateCount[2][type][num]; }

	void	Init();
	void	Release();

	void	CreateInitEnemy();

	void	MoveMonster();
	void	MoveBoss();

	void	DrawHero();
	void	DrawMonster();
	void	DrawBoss();

	int		CreateHero();
	int		CreateBoss();
	int		CheckCreateTime(float dTime);
	void	MakeEnemy(int type, int number, float x, float y);
	void	CheckStage();

	bool	CheckCollision(BBOX left, BBOX right);
	bool	CheckHeroCollision();
};

#define STAGE_1_COUNT 4
#define STAGE_2_COUNT 17
#define STAGE_3_COUNT 25

#define STAGE_COUNT(i) ((i == 0) ? STAGE_1_COUNT : ((i == 1) ? STAGE_2_COUNT : STAGE_3_COUNT))

