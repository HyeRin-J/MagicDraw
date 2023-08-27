#include "../RRYEngine/Vector.h"
#include "../RRYEngine/framework.h"
#include "Input.h"
#include "Unit.h"
#include "Magic.h"
#include "System.h"
#include <vector>
#include "GameProc.h"
#include "MagicManager.h"
#include "Hero.h"
#include "UnitManager.h"
#include "SoundManager.h"

MagicManager* MagicManager::m_Instance = nullptr;

MagicManager::MagicManager()
{
	//오브젝트 풀링
	for (int i = 0; i < 12; i++)
	{
		m_Magics[i] = new Magic();
		m_Magics[i]->Init();
	}

	//스프라이트 초기값 설정
	for (int i = 0; i < 4; i++)
		m_MagicSprites[i] = ANISPRITE();

	m_MagicExploSprite = ANISPRITE();
	m_MagicExploSprite.loopCount = 1;

	m_MagicStarEffect = SPRITE();
}

void MagicManager::Init()
{
	m_DrawPoints.clear();

	for (int i = 0; i < 12; i++)
	{
		m_Magics[i]->Init();
	}
}

void MagicManager::Initialize()
{
	//오브젝트 풀링
	for (int i = 0; i < 12; i++)
	{
		int type = i / 3;
		//초기 데이터 설정
		m_Magics[i]->Init(1, { -200, 450 }, 1300, { 1, 0 }, m_MagicSprites[type], m_MagicExploSprite, type);
		m_Magics[i]->Init();

		BBOX bBox;

		bBox.Left = 340;
		bBox.Top = 40;
		bBox.Right = bBox.Left + 100;
		bBox.Bottom = bBox.Top + 100;
		bBox.Hx = 100 / 2;
		bBox.Hy = 100 / 2;
		bBox.Cx = bBox.Left + bBox.Hx;		//박스의 중점  = 충돌박스 top + 박스 가로길이/2;	
		bBox.Cy = bBox.Top + bBox.Hy;

		m_Magics[i]->SetBBox(bBox);
	}
}

MagicManager* MagicManager::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new MagicManager();
	}
	return m_Instance;
}

void MagicManager::CreateMagic()
{
	static float inputTime = 0;
	int magicType = System::GetInstance()->m_ShapeNum;

	inputTime += System::GetInstance()->m_DTime;

	//판정이 안 됐거나, 현재 1스테이지 중일 때는 삼각형 사각형 마법 사용 불가
	if (magicType == -1 ||  magicType > 4) return;

	if (inputTime > 0.1f)
	{
		inputTime = 0;

		//오브젝트 풀링 사용중
		for (int i = 0; i < 12; i++)
		{
			if (m_Magics[i] == nullptr ||
				m_Magics[i]->GetType() != magicType - 1 ||
				m_Magics[i]->IsFire())
				continue;

			m_Magics[i]->SetFire(true);
			System::GetInstance()->m_ShapeNum = -1;
			SoundManager::GetInstance()->PlayOnce(SoundList::MAGIC_LAUNCH);
			(UnitManager::GetInstance()->m_Hero)->m_IsAttacking = true;
			break;
		}
	}
}

//충돌 체크
void MagicManager::CheckCollision()
{
	for (int i = 0; i < 12; i++)
	{
		if (!m_Magics[i]->IsFire() || m_Magics[i]->IsCollision()) continue;

		if (m_Magics[i]->CheckCollision())
		{
			m_Magics[i]->SetCollision(true);
			SoundManager::GetInstance()->PlayOnce(SoundList::MAGIC_HIT);
		}
	}
}

//마법 이동
void MagicManager::MoveMagic()
{
	for (int i = 0; i < 12; i++)
	{
		if (m_Magics[i] == nullptr || !m_Magics[i]->IsFire()) continue;

		m_Magics[i]->Move(System::GetInstance()->m_DTime);
	}
}

//마법 그리기
void MagicManager::DrawMagic()
{
	for (int i = 0; i < 12; i++)
	{
		if (m_Magics[i] == nullptr || !m_Magics[i]->IsFire()) continue;

		m_Magics[i]->Draw(System::GetInstance()->m_DTime);
	}
}

//메모리 할당 해제
void MagicManager::Release()
{
	for (int i = 0; i < 12; i++)
	{
		m_Magics[i]->Release();
		SAFE_DELETE(m_Magics[i]);
	}

	SAFE_DELETE(m_Instance);
}

int MagicManager::CheckShape()
{
	//사이즈가 적으면 판단이 안 되서 리턴
	if (m_DrawPoints.size() < 3) return m_DrawPoints.size() - 1;

	//생긴 벡터의 개수
	std::vector<Vector2> vectorList;

	//노드 지정
	POINT firstPoint = m_DrawPoints[0];
	POINT curr = m_DrawPoints[1];
	POINT next = m_DrawPoints[2];

	//처음 벡터
	Vector2 firstVector = { (double)curr.x - firstPoint.x, (double)curr.y - firstPoint.y };
	firstVector.Normalize();

	//다음 벡터
	Vector2 v2 = { (double)next.x - curr.x, (double)next.y - curr.y };
	v2.Normalize();

	//벡터의 끝지점까지
	for (unsigned int i = 3; i < m_DrawPoints.size(); i++)
	{
		//각도 계산
		double angle = Vector2::Angle(firstVector, v2);

		//각도가 적으면 무시..
		if (angle < 28)
		{
			if (curr.x != firstPoint.x && curr.y != firstPoint.y)
			{
				firstVector = { (double)curr.x - firstPoint.x, (double)curr.y - firstPoint.y };
				firstVector.Normalize();
			}
			curr = next;
			next = m_DrawPoints[i];

			v2 = { (double)next.x - curr.x, (double)next.y - curr.y };
			v2.Normalize();
		}
		//각도가 크면 이전 벡터를 넣어놓고 새로운 벡터를 생성
		else
		{
			if (curr.x != firstPoint.x && curr.y != firstPoint.y)
			{
				firstVector = { (double)curr.x - firstPoint.x, (double)curr.y - firstPoint.y };
				firstVector.Normalize();
				vectorList.push_back(firstVector);
			}

			POINT prev = curr;

			//새 벡터 생성하는 부분
			do
			{
				prev = curr;
				curr = next;
				next = m_DrawPoints[i];

				Vector2 v1 = { (double)curr.x - prev.x, (double)curr.y - prev.y };
				v1.Normalize();

				Vector2 v2 = { (double)next.x - curr.x, (double)next.y - curr.y };
				v2.Normalize();

				angle = Vector2::Angle(v1, v2);
			} while (angle > 28);	//각도가 큰 동안 계속 반복한다

			//새 벡터를 생성하기
			firstPoint = prev;
			if (curr.x != firstPoint.x && curr.y != firstPoint.y)
			{
				firstVector = { (double)curr.x - firstPoint.x, (double)curr.y - firstPoint.y };
				firstVector.Normalize();
			}
		}
	}

	//벡터의 사이즈 + 마지막 벡터(+1)
	System::GetInstance()->m_ShapeNum = vectorList.size() + 1;
	return vectorList.size() + 1;
}
