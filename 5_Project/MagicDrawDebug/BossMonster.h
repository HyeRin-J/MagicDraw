#pragma once
#include "Monster.h"
class BossMonster :
	public Monster
{
	int m_TriPattern[5][3] = { {0, 3, 4}, {1, 2, 5}, {0, 1, 2}, {3, 4, 5}, {2, 3, 4} };
	int m_RectPattern[5][4] = { {0, 1, 2, 4}, {1, 2, 3, 4}, {0, 2, 3, 5}, {2, 3, 4, 5}, {0, 1, 5, 4} };

	int m_Pattern[6] = { 3, 2, 2, 3, 2, 3 };
	int m_CurrPatternCount = 0;
	int m_CurrPatternEyeCount = 0;
	bool m_IsEyeOpenFinish = false;
public:
	PANISPRITE m_HitSprite;
	PANISPRITE m_SummonSprite;
	PANISPRITE m_AttackSprite;
	PANISPRITE m_EyesSprite[6];

	bool m_InitMoveFinish = false;
	bool m_MoveStart = false;
	bool m_IsAttacking = false;
	float m_PatternTime = 0;
	bool m_IsMiss = false;
	bool m_IsHit = false;
	bool m_IsSummonning = false;
	bool m_IsSummonMonster = false;

	BossMonster();

	bool Attack();
	bool AttackAnimation(float dTime);
	bool EyeOpen(float dTime);
	bool HitAnimation(float dTime);
	bool SummonAnimation(float dTime);
	bool DeadAnimation(float dTime);

	void SetEyes(ANISPRITE eye, int num);

	void SetSprites(ANISPRITE idle, ANISPRITE attack, ANISPRITE summon, ANISPRITE hit, ANISPRITE dead);

	bool Init();

	void ChangePattern();

	virtual bool Init(int hp, POSITION pos, float velo, Vector2 dir);
	virtual bool Hit(int pattern);         //맞았는가
	virtual bool Draw(float dTime, float alpha = 1.0f);		//유닛 그리기
	virtual bool Move(float dTime);	    //유닛 움직이기

	virtual void Release();
};

