#include "../RRYEngine/framework.h"
#include "../RRYEngine/Vector.h"
#include "Hero.h"
#include "System.h"

//애니 스프라이트 Unit.h로 옮길 예정..
Hero::Hero()
{
	m_IdleSprite = new ANISPRITE();
	m_NormalHitSprite = new ANISPRITE();
	m_AttackSprite = new ANISPRITE();
	m_BossHitSprite = new ANISPRITE();
}

void Hero::Init()
{
	m_Hp = 5;
	m_IsCollision = false;
	Unit::Init(5, { -100, 200 }, 0, { 0, 0 }, m_IdleSprite->spr);
}

bool Hero::BossHit()
{
	if (!m_IsBossAttack)
	{
		m_IsBossAttack = true;
		m_Hp--;
		if (m_Hp <= 0) return true;
	}
	return false;
}

void Hero::SetSprites(ANISPRITE attackSpr, ANISPRITE hitSprite, ANISPRITE bossHitSprite)
{
	memcpy(m_AttackSprite, &attackSpr, sizeof(ANISPRITE));
	m_AttackSprite->loopCount = 1;

	memcpy(m_NormalHitSprite, &hitSprite, sizeof(ANISPRITE));
	m_NormalHitSprite->loopCount = 1;

	memcpy(m_BossHitSprite, &bossHitSprite, sizeof(ANISPRITE));
	m_BossHitSprite->loopCount = 1;
}

bool Hero::NormalHitAnimation(float dTime)
{
	return DrawAnimation(dTime, 30, m_Pos.x, m_Pos.y, m_NormalHitSprite);
}

bool Hero::BossHitAnimation(float dTime)
{
	return DrawAnimation(dTime, 64, m_Pos.x, m_Pos.y, m_BossHitSprite);
}

bool Hero::AttackAnimation(float dTime)
{
	return DrawAnimation(dTime, 30, m_Pos.x, m_Pos.y, m_AttackSprite);
}

bool Hero::Init(SPRITE spr, int CutX, int CutY)
{
	return Unit::Init(5, { -100, 200 }, 0, { 0, 0 }, spr);
}

bool Hero::Hit()
{
	if (!m_IsCollision)
	{
		m_IsCollision = true;
		m_Hp--;
		if (m_Hp <= 0) return true;
	}
	
	return false;
}

bool Hero::Draw(float dTime, float alpha)
{
	//애니메이션 대응용
	//DrawSprite(m_AniSprite->spr.dc, 0, 0, &m_AniSprite->spr, 2, 2);
	if (m_IsCollision)
	{
		if (NormalHitAnimation(dTime))
		{
			m_IsCollision = false;
		}
	}
	else if (m_IsBossAttack)
	{
		if (BossHitAnimation(dTime))
		{
			m_IsBossAttack = false;
		}
	}
	else if (m_IsAttacking)
	{
		if (AttackAnimation(dTime))
		{
			m_IsAttacking = false;
		}
	}
	else
	{
		DrawAnimation(dTime, 3, m_Pos.x, m_Pos.y, m_IdleSprite);
	}

	//DrawAlphaSprite(backBufferDC, m_Pos.x, m_Pos.y, m_Spr->Width, m_Spr->Height, m_Spr);
	//DrawTransSprite(backBufferDC, m_Pos.x, m_Pos.y, m_Spr);

	if (System::GetInstance()->m_ShowBBox)
	{
		DrawBBox();
	}

	//return Unit::Draw(dTime, alpha);

	return true;
}

//히어로는 움직이지 않는다!
bool Hero::Move(float dTime)
{
	return false;
}

void Hero::Release()
{
	ReleaseSprite(&m_IdleSprite);
	ReleaseSprite(&m_AttackSprite);
	ReleaseSprite(&m_NormalHitSprite);
	ReleaseSprite(&m_BossHitSprite);

	Unit::Release();
}
