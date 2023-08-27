#include "../RRYEngine/Vector.h"
#include "../RRYEngine/framework.h"
#include "System.h"
#include "Unit.h"

Unit::Unit()
{
	m_BBox = BBOX();
	m_Spr = new SPRITE();
}

POSITION Unit::GetPos()
{
	return m_Pos;
}

bool Unit::SetSprite(SPRITE spr)
{
	return memcpy(m_Spr, &spr, sizeof(SPRITE));
}

SPRITE Unit::GetSprite()
{
	return *m_Spr;
}

void Unit::SetBBox(int Left, int Width, int Top, int Height)
{
	m_BBox.Left = Left;
	m_BBox.Top = Top;
	m_BBox.Right = m_BBox.Left + Width;
	m_BBox.Bottom = m_BBox.Top + Height;
	m_BBox.Hx = Width / 2;
	m_BBox.Hy = Height / 2;
	m_BBox.Cx = m_BBox.Left + m_BBox.Hx;		//�ڽ��� ����  = �浹�ڽ� left + �ڽ� ���α���/2;	
	m_BBox.Cy = m_BBox.Top + m_BBox.Hy;			//�ڽ��� ����  = �浹�ڽ� top + �ڽ� ���α���/2;

	SetBBox(m_BBox);
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
	//������ǥ pos �� �������� w, h ��ŭ �׸���. ���� ��������.
	float w = m_BBox.Hx * 2;
	float h = m_BBox.Hy * 2;
	DrawHollowRect(backBufferDC, GetPos().x + m_BBox.Left, GetPos().y + m_BBox.Top, w, h, RGB(0, 255, 0));

	/*//������ǥ pos �� �������� w, h ��ŭ �׸���. ���� ��������, 45 ȸ��.
	float rot = ynToRadian(45);*/

	//ynTextDraw( pos.x, pos.y, COLOR(0, 1, 0, 1), L"%.2f %.2f", m_UD.dir.x, m_UD.dir.y);

	return TRUE;
}



bool Unit::Hit(int pattern)
{
	return false;
}

//�ʱⰪ
bool Unit::Init(int hp, POSITION pos, float velo, Vector2 dir)
{
	m_Hp = hp;
	m_Pos = pos;
	m_Velo = velo;
	m_Dir = dir;
	return false;
}

bool Unit::Move(float dTime)
{
	if (IsDead()) return FALSE;

	//�̵�...
	m_Pos.y += m_Dir.y * dTime * m_Velo;
	m_Pos.x += m_Dir.x * dTime * m_Velo;


	// �̵� ���� �ִ�ġ ��ġ.  
	float w = m_Spr->Width;			// ��������Ʈ�� '����' ��ũ��
	float h = m_Spr->Height;

	// ȭ�� ������ �����
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
	//����� ���
	//�浹�ڽ� ���
	if (System::GetInstance()->IsShowBBox())
	{
		DrawBBox();
		PrintText(m_BBox.Cx, m_BBox.Cy, RGB(0, 255, 0), IsCollision() ? "OK" : "");
		PrintText(m_Pos.x, m_Pos.y, RGB(0, 255, 0), "%.2f, %.2f", m_Pos.x, m_Pos.y);
	}

	return TRUE;
}

void Unit::Release()
{
	//��������Ʈ ����
	ReleaseSprite(&m_Spr);
	delete m_Spr;
	m_Spr = nullptr;
}
