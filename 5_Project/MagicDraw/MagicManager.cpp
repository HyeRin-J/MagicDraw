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
	//������Ʈ Ǯ��
	for (int i = 0; i < MAGIC_MAX_; i++)
	{
		m_Magics[i] = new Magic();
		m_Magics[i]->Init();
	}

	//��������Ʈ �ʱⰪ ����
	for (int i = 0; i < 4; i++)
		m_MagicSprites[i] = ANISPRITE();

	m_MagicExploSprite = ANISPRITE();
	m_MagicExploSprite.loopCount = 1;

	m_MagicStarEffect = SPRITE();
}

void MagicManager::Init()
{
	m_DrawPoints.clear();

	for (int i = 0; i < MAGIC_MAX_; i++)
	{
		m_Magics[i]->Init();
	}
}

void MagicManager::Initialize()
{
	//������Ʈ Ǯ��
	for (int i = 0; i < MAGIC_MAX_; i++)
	{
		int type = i;
		//�ʱ� ������ ����
		m_Magics[i]->Init(1, { -200, 500 }, 1300, { 1, 0 }, type);
		m_Magics[i]->SetSprites(m_MagicSprites[type], m_MagicExploSprite);
		m_Magics[i]->Init();
		m_Magics[i]->SetBBox(340, 100, 40, 110);
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

	//������ �� �ưų�, ���� 1�������� ���� ���� �ﰢ�� �簢�� ���� ��� �Ұ�
	if (magicType == -1 ||  magicType > 4) return;

	if (inputTime > 0.1f)
	{
		inputTime = 0;

		//������Ʈ Ǯ�� �����
		for (int i = 0; i < MAGIC_MAX_; i++)
		{
			if (m_Magics[i] == nullptr ||
				m_Magics[i]->GetType() != magicType - 1 ||
				m_Magics[i]->m_State & IS_FIRE)
				continue;

			m_Magics[i]->m_State |= IS_FIRE;
			System::GetInstance()->m_ShapeNum = -1;
			SoundManager::GetInstance()->PlayOnce(SoundList::MAGIC_LAUNCH);
			(UnitManager::GetInstance()->m_Hero)->m_IsAttacking = true;
			break;
		}
	}
}

//�浹 üũ
void MagicManager::CheckCollision()
{
	for (int i = 0; i < MAGIC_MAX_; i++)
	{
		if (!m_Magics[i]->IsFire() || m_Magics[i]->IsCollision()) continue;

		if (m_Magics[i]->CheckCollision())
		{
			m_Magics[i]->m_State |= IS_COLLISION;
			SoundManager::GetInstance()->PlayOnce(SoundList::MAGIC_HIT);
		}
	}
}

//���� �̵�
void MagicManager::MoveMagic()
{
	for (int i = 0; i < MAGIC_MAX_; i++)
	{
		if (m_Magics[i] == nullptr || !(m_Magics[i]->m_State & IS_FIRE)) continue;

		m_Magics[i]->Move(System::GetInstance()->m_DTime);
	}
}

//���� �׸���
void MagicManager::DrawMagic()
{
	for (int i = 0; i < MAGIC_MAX_; i++)
	{
		if (m_Magics[i] == nullptr || !(m_Magics[i]->m_State & IS_FIRE)) continue;


		m_Magics[i]->Draw(System::GetInstance()->m_DTime);
	}
}

//�޸� �Ҵ� ����
void MagicManager::Release()
{
	for (int i = 0; i < MAGIC_MAX_; i++)
	{
		m_Magics[i]->Release();
		SAFE_DELETE(m_Magics[i]);
	}

	ReleaseSprite(&m_MagicStarEffect);
	ReleaseSprite(&m_MagicExploSprite);

	for (int i = 0; i < 4; i++)
	{
		ReleaseSprite(&m_MagicSprites[i]);
	}

	SAFE_DELETE(m_Instance);
}

int MagicManager::CheckShape()
{
	//����� ������ �Ǵ��� �� �Ǽ� ����
	if (m_DrawPoints.size() < 3) return m_DrawPoints.size() - 1;

	//���� ������ ����
	std::vector<Vector2> vectorList;

	//��� ����
	POINT firstPoint = m_DrawPoints[0];
	POINT curr = m_DrawPoints[1];
	POINT next = m_DrawPoints[2];

	//ó�� ����
	Vector2 firstVector = { (double)curr.x - firstPoint.x, (double)curr.y - firstPoint.y };
	firstVector.Normalize();

	//���� ����
	Vector2 v2 = { (double)next.x - curr.x, (double)next.y - curr.y };
	v2.Normalize();

	//������ ����������
	for (unsigned int i = 3; i < m_DrawPoints.size(); i++)
	{
		//���� ���
		double angle = Vector2::Angle(firstVector, v2);

		//������ ������ ����..
		if (angle < 28)
		{
			if (curr.x != firstPoint.x && curr.y != firstPoint.y)
			{
				firstVector = { (double)curr.x - firstPoint.x, (double)curr.y - firstPoint.y };
				firstVector.Normalize();
			}			
			do 
			{
				curr = next;
				next = m_DrawPoints[i];
				v2 = { (double)next.x - curr.x, (double)next.y - curr.y };
				i++;
				if (i >= m_DrawPoints.size()) break;
			} while (v2.Length() < 20);
			v2.Normalize();
		}
		//������ ũ�� ���� ���͸� �־���� ���ο� ���͸� ����
		else
		{
			if (curr.x != firstPoint.x && curr.y != firstPoint.y)
			{
				firstVector = { (double)curr.x - firstPoint.x, (double)curr.y - firstPoint.y };
				firstVector.Normalize();
				vectorList.push_back(firstVector);
			}

			POINT prev = curr;

			//�� ���� �����ϴ� �κ�
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
			} while (angle > 28);	//������ ū ���� ��� �ݺ��Ѵ�

			//�� ���͸� �����ϱ�
			firstPoint = prev;
			if (curr.x != firstPoint.x && curr.y != firstPoint.y)
			{
				firstVector = { (double)curr.x - firstPoint.x, (double)curr.y - firstPoint.y };
				firstVector.Normalize();
			}
		}
	}

	//������ ������ + ������ ����(+1)
	System::GetInstance()->m_ShapeNum = vectorList.size() + 1;
	return vectorList.size() + 1;
}
