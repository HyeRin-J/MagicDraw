#pragma once
#include "Unit.h"
class Monster :
    public Unit
{
protected:
    int m_Type = 1;

    bool m_IsCreated = false;
    bool m_IsFront = false;
    long m_DeadTime = 0;
public:
    PANISPRITE m_AniSprite = nullptr;
    PANISPRITE m_DeadAniSprite = nullptr;

    Monster();

    int GetType() { return m_Type; }
    void SetFront(bool front) { m_IsFront = front; }

    bool IsCreated() { return m_IsCreated; }
    void SetCreated(bool isCreate) { m_IsCreated = isCreate; }

    void SetDeadTime(long time) { m_DeadTime = time; }

    void Init();
    bool Init(int hp, POSITION pos, float velo, Vector2 dir, ANISPRITE spr, ANISPRITE deadSpr, int type);
   // bool Init(int hp, POSITION pos, float velo, Vector2 dir, SPRITE spr, int type);

    bool DeadAnimation(float dTime, float alpha);

    virtual bool Hit(int pattern);         //맞았는가
    virtual bool Draw(float dTime, float alpha = 1.0f);		//유닛 그리기
    virtual bool Move(float dTime);	    //유닛 움직이기
    
    virtual void Release();
};

