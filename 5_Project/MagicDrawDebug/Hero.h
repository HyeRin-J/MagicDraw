#pragma once
#include "Unit.h"
class Hero :
	public Unit
{

public:
	PANISPRITE m_IdleSprite;
	PANISPRITE m_AttackSprite;
	PANISPRITE m_NormalHitSprite;
	PANISPRITE m_BossHitSprite;

	bool m_IsBossAttack = false;
	bool m_IsAttacking = false;

	Hero();

	void Init();
	bool BossHit();

	void SetSprites(ANISPRITE attackSpr, ANISPRITE hitSprite, ANISPRITE bossHitSprite);
	bool NormalHitAnimation(float dTime);
	bool BossHitAnimation(float dTime);

	bool AttackAnimation(float dTime);

	virtual bool Init(SPRITE spr, int CutX, int CutY);
	virtual bool Hit();
	virtual bool Draw(float dTime, float alpha = 1.0f);
	virtual bool Move(float dTime);
	virtual void Release();
};

