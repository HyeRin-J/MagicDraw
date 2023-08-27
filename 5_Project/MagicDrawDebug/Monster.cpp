#include "../RRYEngine/framework.h"
#include "../RRYEngine/Vector.h"
#include "Monster.h"
#include "System.h"

Monster::Monster()
{

}

void Monster::Init()
{
	m_Pos = { 1400, m_Pos.y };
	m_IsDead = false;
	m_IsCreated = false;
	m_IsCollision = false;
	SetBBox(m_BBox);
}

//애니메이션 스프라이트 기준 초기화
bool Monster::Init(int hp, POSITION pos, float velo, Vector2 dir, ANISPRITE anispr, ANISPRITE deadSpr, int type)
{
	m_Type = type;
	//애니 스프라이트 Unit.h로 옮길 예정..
	m_AniSprite = new ANISPRITE();
	memcpy(m_AniSprite, &anispr, sizeof(ANISPRITE));

	m_DeadAniSprite = new ANISPRITE();
	memcpy(m_DeadAniSprite, &deadSpr, sizeof(ANISPRITE));
	m_DeadAniSprite->loopCount = 1;

	return Unit::Init(hp, pos, velo, dir, anispr.spr);
}

bool Monster::DeadAnimation(float dTime, float alpha)
{
	return DrawAnimation(dTime, 4, m_Pos.x, m_Pos.y, m_DeadAniSprite, alpha);
}

bool Monster::Hit(int pattern)
{
	if (m_Type == pattern)
	{
		m_IsCollision = true;
		m_IsDead = true;
		m_DeadTime = GetTickCount64();
		return true;
	}
	else return false;
}

//몬스터 출력
bool Monster::Draw(float dTime, float alpha)
{
	if (!m_IsDead)
	{
		DrawAnimation(dTime, m_AniSprite->cutX * m_AniSprite->cutY, (int)m_Pos.x, (int)m_Pos.y, m_AniSprite);
	}
	else
	{
		long currTime = GetTickCount64();
		float alpha = 1;
		if ((currTime - m_DeadTime) / 1000 > 1)
			alpha = 0;
		else alpha = 1 * (1 - ((currTime - m_DeadTime) / 1000.0f));

		if (DeadAnimation(dTime, alpha))
		{
			Init();
		}
	}

	//디버그 모드
	if (System::GetInstance()->m_ShowBBox)
	{
		DrawBBox();
		PrintText((int)m_Pos.x, (int)m_Pos.y + 200, RGB(0, 255, 0), "%d", m_Type);
	}

	if (m_IsFront)
	{
		PrintText((int)m_Pos.x + 20, (int)m_Pos.y, RGB(0, 255, 0), "↓");
	}

	return true;
}

//몬스터 이동
bool Monster::Move(float dTime)
{
	//일반 유닛의 이동

	int w = 0;
	int h = 0;

	//애니메이션 스프라이트의 경우, width와 height가 다르므로 보정이 필요함
	if (m_AniSprite != nullptr)
	{
		w = m_AniSprite->spr.Width;
		h = m_AniSprite->spr.Height;
	}
	else
	{
		w = m_Spr->Width;
		h = m_Spr->Height;
	}

	if ((m_Pos.y >= System::GetInstance()->m_ScreenSize.cy + h || m_Pos.y < -h) ||
		(m_Pos.x >= System::GetInstance()->m_ScreenSize.cx + w || m_Pos.x < -w))
	{
		Init();
		return false;
	}
	SetBBox(m_BBox);

	m_Pos.y += m_Dir.y * dTime * m_Velo;
	m_Pos.x += m_Dir.x * dTime * m_Velo;


	return true;
}

void Monster::Release()
{
	ReleaseSprite(&m_AniSprite);
	ReleaseSprite(&m_DeadAniSprite);
	Unit::Release();
}
