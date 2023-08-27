#pragma once

struct BBOX
{
	int	Left,				//�浹����. : �̹��� ������� ������ǥ.
		Right,
		Top,
		Bottom;

	int Hx, Hy;			//�浹����, �� ���� 1/2 ����.
	int Cx, Cy;			//�߽��� : �̹��� ������� ������ǥ.
};

struct POSITION
{
	float x;
	float y;
};

class Unit
{
protected:
	int m_Hp = 1;

	POSITION m_Pos = POSITION();		//�����ǥ

	float m_Velo = 10;	//�̵� �ӵ�
	float m_ScaleX = 1, m_ScaleY = 1; //��������Ʈ ������

	Vector2 m_Dir = { -1, 0 };	//�̵��� ����
	PSPRITE m_Spr = NULL;	//����� ��������Ʈ

	BBOX m_BBox;

	bool m_IsDead = false;	//�׾��� ��
	bool m_IsCollision = false;

public:

	Unit();

	int GetHP() { return m_Hp; }

	POSITION GetPos();

	void SetSprite(SPRITE spr);
	SPRITE GetSprite();

	void SetScale(float scaleX, float scaleY) { m_ScaleX = scaleX; m_ScaleY = scaleY; }

	void SetBBox(BBOX bBox);

	BBOX GetBBox() { return m_BBox; }
	POSITION GetBBoxCenter();
	POSITION GetCenter();

	bool IsDead() { return m_IsDead; }

	void SetCollision(bool col) { m_IsCollision = col; }
	bool IsCollision() { return m_IsCollision; }

	virtual int DrawBBox();

	virtual bool Hit(int pattern);

	virtual bool Init(int hp, POSITION pos, float velo, Vector2 dir, SPRITE spr);
	virtual bool Move(float dTime) = 0;
	virtual bool Draw(float dTime, float alpha = 1.0f) = 0;
	virtual void Release();
};

