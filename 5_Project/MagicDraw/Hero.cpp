#include "../RRYEngine/framework.h"
#include "../RRYEngine/Vector.h"
#include "Hero.h"
#include "System.h"
#include "SoundManager.h"

//�ִ� ��������Ʈ Unit.h�� �ű� ����..
Hero::Hero()
{
	m_IdleSprite = new ANISPRITE();
	m_NormalDamageSprite = new ANISPRITE();
	m_AttackSprite = new ANISPRITE();
	m_BossDamageSprite = new ANISPRITE();
	m_JumpingSprite = new ANISPRITE();
}

bool Hero::Init()
{
	m_Hp = 5;
	m_State = 0;
	m_IsBossAttack = false;
	m_IsAttacking = false;
	m_IsFinish = false;
	return Unit::Init(5, { -100, 200 }, 0, { 0, 0 });
}

bool Hero::BossHit()
{
	if (!m_IsBossAttack)
	{
		m_IsBossAttack = true;
		SoundManager::GetInstance()->PlayOnce(SoundList::HERO_DAMAGE);
		m_Hp--;
		if (m_Hp <= 0) return true;
	}
	return false;
}

bool Hero::SetSprites(ANISPRITE idleSpr, ANISPRITE attackSpr, ANISPRITE hitSprite, ANISPRITE bossHitSprite, ANISPRITE jumpingSprite)
{
	memcpy(m_IdleSprite, &idleSpr, sizeof(ANISPRITE));
	
	memcpy(m_AttackSprite, &attackSpr, sizeof(ANISPRITE));
	m_AttackSprite->loopCount = 1;

	memcpy(m_NormalDamageSprite, &hitSprite, sizeof(ANISPRITE));
	m_NormalDamageSprite->loopCount = 1;

	memcpy(m_BossDamageSprite, &bossHitSprite, sizeof(ANISPRITE));
	m_BossDamageSprite->loopCount = 1;

	memcpy(m_JumpingSprite, &jumpingSprite, sizeof(ANISPRITE));
	m_JumpingSprite->loopCount = 2;

	return Unit::SetSprite(m_IdleSprite->spr);
}

bool Hero::NormalHitAnimation(float dTime)
{
	return DrawAnimation(dTime, 30, m_Pos.x, m_Pos.y, m_NormalDamageSprite);
}

bool Hero::BossHitAnimation(float dTime)
{
	return DrawAnimation(dTime, 64, m_Pos.x, m_Pos.y, m_BossDamageSprite);
}

bool Hero::AttackAnimation(float dTime)
{
	return DrawAnimation(dTime, 30, m_Pos.x, m_Pos.y, m_AttackSprite);
}

bool Hero::GameClearAnimation(float dTime)
{
	return DrawAnimation(dTime, 5, m_Pos.x, m_Pos.y, m_JumpingSprite);
}

bool Hero::Hit()
{
	if (!(m_State & IS_COLLISION))
	{
		SoundManager::GetInstance()->PlayOnce(SoundList::HERO_DAMAGE);
		m_State |= IS_COLLISION;
		m_Hp--;
		if (m_Hp <= 0) return true;
	}
	
	return false;
}

bool Hero::Draw(float dTime, float alpha)
{
	//�ִϸ��̼� ������
	//DrawSprite(m_AniSprite->spr.dc, 0, 0, &m_AniSprite->spr, 2, 2);
	if (m_State & IS_COLLISION)
	{
		if (NormalHitAnimation(dTime))
		{
			m_State &= ~IS_COLLISION;
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
	else if (m_IsFinish)
	{
		if (GameClearAnimation(dTime))
		{
			return false;
		}
	}
	else
	{
		DrawAnimation(dTime, 8, m_Pos.x, m_Pos.y, m_IdleSprite);
	}

	//DrawAlphaSprite(backBufferDC, m_Pos.x, m_Pos.y, m_Spr->Width, m_Spr->Height, m_Spr);
	//DrawTransSprite(backBufferDC, m_Pos.x, m_Pos.y, m_Spr);

	if (System::GetInstance()->m_State & SHOWBBOX)
	{
		DrawBBox();
	}

	//return Unit::Draw(dTime, alpha);

	return true;
}

//����δ� �������� �ʴ´�!
bool Hero::Move(float dTime)
{
	return false;
}

void Hero::Release()
{
	ReleaseSprite(&m_IdleSprite);
	ReleaseSprite(&m_AttackSprite);
	ReleaseSprite(&m_NormalDamageSprite);
	ReleaseSprite(&m_BossDamageSprite);
	ReleaseSprite(&m_JumpingSprite);

	Unit::Release();
}
