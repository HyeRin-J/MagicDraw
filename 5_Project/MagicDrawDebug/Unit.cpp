#include "../RRYEngine/Vector.h"
#include "../RRYEngine/framework.h"
#include "System.h"
#include "Unit.h"

Unit::Unit()
{
	m_BBox = BBOX();
}

POSITION Unit::GetPos()
{
	return m_Pos;
}

void Unit::SetSprite(SPRITE spr)
{
	m_Spr = new SPRITE();
	memcpy(m_Spr, &spr, sizeof(SPRITE));
}

SPRITE Unit::GetSprite()
{
	return *m_Spr;
}

void Unit::SetBBox(BBOX bBox)
{
	m_BBox = bBox;

	m_BBox.Cx = GetBBoxCenter().x;
	m_BBox.Cy = GetBBoxCenter().y;
}

POSITION Unit::GetBBoxCenter()
{
	POSITION pos = GetPos();
	POSITION pt = POSITION();
	pt.x = pos.x + m_BBox.Left + m_BBox.Hx;
	pt.y = pos.y + m_BBox.Top + m_BBox.Hy;
	return pt;
}

POSITION Unit::GetCenter()
{
	POSITION pos;
	pos.x = m_Pos.x + m_Spr->Width / 2 * m_ScaleX;
	pos.y = m_Pos.y + m_Spr->Height / 2 * m_ScaleY;
	return pos;
}

int Unit::DrawBBox()
{
	//지정좌표 pos 를 기준으로 w, h 만큼 그린다. 색상도 지정가능.
	float w = m_BBox.Hx * 2;
	float h = m_BBox.Hy * 2;
	DrawHollowRect(backBufferDC, GetPos().x + m_BBox.Left, GetPos().y + m_BBox.Top, w, h, RGB(0, 255, 0));

	/*//지정좌표 pos 를 기준으로 w, h 만큼 그린다. 색상도 지정가능, 45 회전.
	float rot = ynToRadian(45);*/

	//ynTextDraw( pos.x, pos.y, COLOR(0, 1, 0, 1), L"%.2f %.2f", m_UD.dir.x, m_UD.dir.y);

	return TRUE;
}



bool Unit::Hit(int pattern)
{
	return false;
}

//초기값
bool Unit::Init(int hp, POSITION pos, float velo, Vector2 dir, SPRITE spr)
{
	m_Spr = new SPRITE();
	memcpy(m_Spr, &spr, sizeof(SPRITE));
	m_Hp = hp;
	m_Pos = pos;
	m_Velo = velo;
	m_Dir = dir;
	return false;
}

bool Unit::Move(float dTime)
{
	if (m_IsDead) return FALSE;

	//이동...
	m_Pos.y += m_Dir.y * dTime * m_Velo;
	m_Pos.x += m_Dir.x * dTime * m_Velo;


	// 이동 영역 최대치 설치.  
	float w = m_Spr->Width;			// 스프라이트의 '한컷' 의크기
	float h = m_Spr->Height;

	// 화면 밖으로 벗어나면
	if ((m_Pos.y >= System::GetInstance()->m_ScreenSize.cy + h || m_Pos.y < -h) ||
		(m_Pos.x >= System::GetInstance()->m_ScreenSize.cx + w || m_Pos.x < -w))
	{
		return FALSE;
	}

	SetBBox(m_BBox);

	return TRUE;
}


bool Unit::Draw(float dTime, float alpha)
{
	//스프라이트 출력 
	DrawTransSprite(backBufferDC, m_Pos.x, m_Pos.y, m_Spr, m_ScaleX, m_ScaleY);

	//디버그 모드
	//충돌박스 출력
	if (System::GetInstance()->m_ShowBBox)
	{
		DrawBBox();
		PrintText(m_BBox.Cx, m_BBox.Cy, RGB(0, 255, 0), m_IsCollision ? "OK" : "");
		PrintText(m_Pos.x, m_Pos.y, RGB(0, 255, 0), "%.2f, %.2f", m_Pos.x, m_Pos.y);
	}

	return TRUE;
}

void Unit::Release()
{
	//스프라이트 제거
	ReleaseSprite(&m_Spr);
}
