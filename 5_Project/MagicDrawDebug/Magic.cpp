#include "../RRYEngine/framework.h"
#include "../RRYEngine/Vector.h"
#include "Unit.h"
#include "Monster.h"
#include "BossMonster.h"
#include "UnitManager.h"
#include "Magic.h"
#include "System.h"
#include "GameProc.h"

int Magic::DrawBBox()
{
	return Unit::DrawBBox();
}

bool Magic::Hit(BBOX colBox)
{
	return false;
}

bool Magic::Explosion(float dTime, float alpha)
{
	return DrawAnimation(dTime, 10, m_Pos.x, m_Pos.y, m_ExploAniSprite, alpha);
}

void Magic::Init()
{
	m_Pos = { -100, 450 };

	m_IsFire = false;
	m_IsCollision = false;

	Unit::SetBBox(m_BBox);
}

bool Magic::Init(int hp, POSITION pos, float velo, Vector2 dir, ANISPRITE spr, ANISPRITE aniSpr, int type)
{
	m_Type = type;

	m_FireSprite = new ANISPRITE();
	memcpy(m_FireSprite, &spr, sizeof(ANISPRITE));
	m_Spr = new SPRITE();
	memcpy(m_Spr, &m_FireSprite->spr, sizeof(SPRITE));

	m_ExploAniSprite = new ANISPRITE();
	memcpy(m_ExploAniSprite, &aniSpr, sizeof(ANISPRITE));
	m_ExploAniSprite->loopCount = 1;

	m_Hp = hp;
	m_Pos = pos;
	m_Velo = velo;
	m_Dir = dir;

	Unit::SetBBox(m_BBox);

	return true;
}

bool Magic::Move(float dTime)
{
	if (m_IsFire && !m_IsCollision)
	{
		if (!Unit::Move(dTime))
		{
			Init();
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Magic::Draw(float dTime, float alpha)
{
	if (m_IsFire && !m_IsCollision)
	{
		DrawAnimation(dTime, 30, (int)m_Pos.x, (int)m_Pos.y, m_FireSprite);
	}

	if (m_IsCollision)
	{
		long currTime = GetTickCount64();
		float alpha = 1;
		if ((currTime - m_CollTime) / 1000 > 1)
			alpha = 0;
		else alpha = 1 * (1 - ((currTime - m_CollTime) / 1000.0f));

		if (Explosion(dTime, alpha))
		{
			Init();
		}
	}

	if (System::GetInstance()->m_ShowBBox)
	{
		DrawBBox();
	}
	return true;
}

void Magic::Release()
{
	ReleaseSprite(&m_FireSprite);
	m_FireSprite = nullptr;
	ReleaseSprite(&m_ExploAniSprite);
	m_ExploAniSprite = nullptr;

	Unit::Release();
}

bool Magic::CheckCollision()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < ENEMY_MAX_ / 4; j++)
		{
			Monster* mon = UnitManager::GetInstance()->m_Monsters[i][j];

			if (mon->IsDead()) continue;

			if (UnitManager::GetInstance()->CheckCollision(GetBBox(), mon->GetBBox()))
			{
				m_CollTime = GetTickCount64();
				if (mon->Hit(m_Type) && StageInfo == Stage::stage)
				{
					g_Score += UnitManager::m_Score[i][j / 10];
					UnitManager::GetInstance()->CheckStage();
				}
				return true;
			}
		}
	}
	if (StageInfo == Stage::boss)
	{
		BossMonster* boss = UnitManager::GetInstance()->m_BossMonster;

		if (UnitManager::GetInstance()->CheckCollision(GetBBox(), boss->GetBBox()))
		{
			m_CollTime = GetTickCount64();
			boss->Hit(m_Type);

			return true;
		}
	}
	return false;
}