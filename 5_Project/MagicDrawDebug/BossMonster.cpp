#include "../RRYEngine/framework.h"
#include "../RRYEngine/Vector.h"
#include "BossMonster.h"
#include "System.h"
#include "UnitManager.h"
#include "SoundManager.h"
#include "GameProc.h"

BossMonster::BossMonster()
{
	m_AttackSprite = new ANISPRITE();
	m_Spr = new SPRITE();
	m_AniSprite = new ANISPRITE();
	m_HitSprite = new ANISPRITE();
	m_SummonSprite = new ANISPRITE();
	m_DeadAniSprite = new ANISPRITE();

	for (int i = 0; i < 6; i++)
	{
		m_EyesSprite[i] = new ANISPRITE();
	}
	m_CurrPatternCount = 0;
	m_Type = m_Pattern[m_CurrPatternCount];
	m_IsEyeOpenFinish = true;
}

bool BossMonster::Attack()
{
	m_IsAttacking = true;
	return false;
}

bool BossMonster::AttackAnimation(float dTime)
{
	return DrawAnimation(dTime, 12, m_Pos.x, m_Pos.y, m_AttackSprite);
}

bool BossMonster::EyeOpen(float dTime)
{
	int boolCount = 0;
	switch (m_Type)
	{
	case 2:
	{
		//눈 모양 그리기
		for (int i = 0; i < 3; i++)
			if (DrawAnimation(dTime, 1, m_Pos.x, m_Pos.y, m_EyesSprite[m_TriPattern[m_CurrPatternEyeCount][i]]))
				boolCount++;

		if (boolCount == 3) return true;
	}
	break;
	case 3:
	{
		//눈 모양 그리기
		for (int i = 0; i < 4; i++)
			if (DrawAnimation(dTime, 1, m_Pos.x, m_Pos.y, m_EyesSprite[m_RectPattern[m_CurrPatternEyeCount][i]]))
				boolCount++;

		if (boolCount == 4) return true;
	}
	break;
	}
	return false;
}

bool BossMonster::HitAnimation(float dTime)
{
	return DrawAnimation(dTime, 4, m_Pos.x, m_Pos.y, m_HitSprite);
}

bool BossMonster::SummonAnimation(float dTime)
{
	if (!m_IsSummonMonster && m_SummonSprite->aniFrame >= 3)
	{
		//4종 중 1개 랜덤 생성
		//확률 보정
		int r = rand() % 100;
		int type = 0;

		if (r < 20) type = 0;
		else if (r < 55) type = 1;
		else if (r < 85) type = 2;
		else type = 3;

		UnitManager::GetInstance()->MakeEnemy(type, 0, GetPos().x + 150, GetPos().y + 420);
		m_IsSummonMonster = true;
		m_IsCollision = false;
	}

	return DrawAnimation(dTime, 8, m_Pos.x, m_Pos.y, m_SummonSprite);
}

bool BossMonster::DeadAnimation(float dTime)
{
	return DrawAnimation(dTime, 11, m_Pos.x, m_Pos.y, m_DeadAniSprite);
}

void BossMonster::SetEyes(ANISPRITE eye, int num)
{
	memcpy(m_EyesSprite[num], &eye, sizeof(ANISPRITE));
	m_EyesSprite[num]->loopCount = 1;
}

void BossMonster::SetSprites(ANISPRITE idle, ANISPRITE attack, ANISPRITE summon, ANISPRITE hit, ANISPRITE dead)
{
	memcpy(m_AniSprite, &idle, sizeof(ANISPRITE));
	memcpy(m_Spr, &(m_AniSprite->spr), sizeof(SPRITE));

	memcpy(m_AttackSprite, &attack, sizeof(ANISPRITE));
	m_AttackSprite->loopCount = 1;

	memcpy(m_SummonSprite, &summon, sizeof(ANISPRITE));
	m_SummonSprite->loopCount = 1;

	memcpy(m_HitSprite, &hit, sizeof(ANISPRITE));
	m_HitSprite->loopCount = 1;

	memcpy(m_DeadAniSprite, &dead, sizeof(ANISPRITE));
	m_DeadAniSprite->loopCount = 1;
}

bool BossMonster::Init()
{
	m_Hp = 10;
	m_Velo = 150;
	m_Pos = { 1400, m_Pos.y };
	m_CurrPatternCount = 0;
	m_Type = m_Pattern[m_CurrPatternCount];
	m_CurrPatternEyeCount = rand() % 5;

	m_InitMoveFinish = false;
	m_IsAttacking = false;
	m_PatternTime = 0;
	m_IsMiss = false;
	m_IsHit = false;
	m_IsSummonning = false;
	m_IsSummonMonster = false;

	return true;
}

void BossMonster::ChangePattern()
{
	m_CurrPatternCount++;
	m_CurrPatternCount %= 6;
	m_Type = m_Pattern[m_CurrPatternCount];

	m_CurrPatternEyeCount = rand() % 5;
	m_PatternTime = 0;
}

bool BossMonster::Init(int hp, POSITION pos, float velo, Vector2 dir)
{
	m_Pos = { 1400, m_Pos.y };
	m_IsDead = false;
	m_Hp = hp;
	m_Pos = pos;
	m_Velo = velo;
	m_Dir = dir;

	m_CurrPatternCount = 0;
	m_Type = m_Pattern[m_CurrPatternCount];
	m_CurrPatternEyeCount = rand() % 5;

	return true;
}

//보스 몬스터의 경우.. 뒤로 밀려남 구현
bool BossMonster::Hit(int pattern)
{
	if (m_InitMoveFinish)
	{
		m_MoveStart = true;
		m_InitMoveFinish = false;
		m_Velo /= 15;
	}

	if (m_MoveStart)
	{
		if (m_Type == pattern)
		{
			if (!m_IsCollision)
			{
				m_IsCollision = true;

				m_Hp -= 1;
				m_IsHit = true;

				ChangePattern();
				m_IsEyeOpenFinish = false;
				SoundManager::GetInstance()->PlayOnce(SoundList::BOSS_GROWL);

				//만약 죽었으면,
			}
			if (m_Hp <= 0)
			{
				m_IsDead = true;
				SoundManager::GetInstance()->PlayOnce(SoundList::BOSS_DIE);
				SetBBox(m_BBox);
				return true;
			}
		}
		else
		{
			m_IsMiss = true;
			return false;
		}
	}
	else return false;
}

bool BossMonster::Draw(float dTime, float alpha)
{
	if (m_IsDead)
	{
		if (DeadAnimation(dTime))
		{
			return false;
		}
	}
	else if (m_IsHit)
	{
		if (HitAnimation(dTime))
		{
			m_IsSummonning = true;
			SoundManager::GetInstance()->PlayOnce(SoundList::BOSS_SUMMON);
			m_IsHit = false;
		}
	}
	else if (m_IsSummonning)
	{
		if (SummonAnimation(dTime))
		{
			m_IsSummonMonster = false;
			m_IsSummonning = false;
		}
	}
	//타입별로 그려줄 스프라이트나 애니메이션이 다름
	else if (m_IsAttacking)
	{
		if (AttackAnimation(dTime))
		{
			m_IsAttacking = false;
			m_IsMiss = false;
		}
	}
	else
	{
		DrawAnimation(dTime, 4, m_Pos.x, m_Pos.y, m_AniSprite);
	}

	if (!m_IsDead)
	{
		if (m_IsEyeOpenFinish)
		{
			switch (m_Type)
			{
			case 2:
			{
				//눈 모양 그리기
				for (int i = 0; i < 3; i++)
					DrawAlphaSprite(backBufferDC, m_Pos.x, m_Pos.y, m_Spr->Width, m_Spr->Height,
						&m_EyesSprite[m_TriPattern[m_CurrPatternEyeCount][i]]->spr, m_Spr->Width, 0);
			}
			break;
			case 3:
			{
				//눈 모양 그리기
				for (int i = 0; i < 4; i++)
					DrawAlphaSprite(backBufferDC, m_Pos.x, m_Pos.y, m_Spr->Width, m_Spr->Height,
						&m_EyesSprite[m_RectPattern[m_CurrPatternEyeCount][i]]->spr, m_Spr->Width, 0);
			}
			break;
			}
		}
		else
		{
			if (EyeOpen(dTime))
				m_IsEyeOpenFinish = true;
		}
	}

	//디버그 모드
	if (System::GetInstance()->m_ShowBBox)
	{
		DrawBBox();
		PrintText((int)m_Pos.x, (int)m_Pos.y + 200, RGB(0, 255, 0), "%d", m_Type);
	}


	return true;
	//그 외에는 기본 몬스터의 애니메이션을 따름
	//return Monster::Draw(dTime, alpha);
}

bool BossMonster::Move(float dTime)
{
	if (m_MoveStart)
	{
		if (m_IsHit)
		{
			m_PatternTime = 0;
		}
		else if (m_IsSummonning)
		{

		}

		else if (m_IsMiss)
		{
			Attack();
		}
		else
		{
			m_PatternTime += dTime;
			if (m_PatternTime >= rand() % 5 + 10)
			{
				ChangePattern();
				m_IsEyeOpenFinish = false;

			}
			return Monster::Move(dTime);
		}
	}
	else
	{
		int w = m_Spr->Width;
		int h = m_Spr->Height;

		if (m_Pos.x > System::GetInstance()->m_ScreenSize.cx - ((w * 6) / 7) - 25)
		{
			return Monster::Move(dTime);
		}
		else
		{
			m_InitMoveFinish = true;
		}
	}
}

void BossMonster::Release()
{
	ReleaseSprite(&m_AttackSprite);
	ReleaseSprite(&m_HitSprite);
	ReleaseSprite(&m_SummonSprite);

	for (int i = 0; i < 6; i++)
	{
		ReleaseSprite(&m_EyesSprite[i]);
	}

	Monster::Release();
}
