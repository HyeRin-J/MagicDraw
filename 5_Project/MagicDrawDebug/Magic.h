#pragma once
class Magic : public Unit
{
	int m_Type = 1;	//마법 타입

	bool m_IsFire = false;

	long m_CollTime = 0;
public:
	PANISPRITE m_FireSprite;
	PANISPRITE m_ExploAniSprite;

	int GetType() { return m_Type; }

	void SetFire(bool fire) { m_IsFire = fire; }
	bool IsFire() { return m_IsFire; }

	virtual int DrawBBox();

	virtual bool Hit(BBOX colBox);

	bool Explosion(float dTime, float alpha);

	void Init();
	virtual bool Init(int hp, POSITION pos, float velo, Vector2 dir, ANISPRITE spr, ANISPRITE aniSpr, int type = 0);
	virtual bool Move(float dTime);
	virtual bool Draw(float dTime, float alpha = 1.0f);

	virtual void Release();

	bool CheckCollision();

};

