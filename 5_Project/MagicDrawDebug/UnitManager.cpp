#include "../RRYEngine/framework.h"
#include "../RRYEngine/Vector.h"
#include "Unit.h"
#include "Hero.h"
#include "Monster.h"
#include "BossMonster.h"
#include "SoundManager.h"
#include "UnitManager.h"
#include "System.h"
#include "GameProc.h"

UnitManager* UnitManager::m_Instance;
int UnitManager::KillCount = 0;
int UnitManager::m_Score[4][2] = { {20, 0}, {30, 30}, {40, 40}, {50, 50} };

UnitManager::UnitManager()
{
	m_HeroAniSprite = ANISPRITE();
}

void UnitManager::Init()
{
	KillCount = 0;

	m_Hero->Init();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < ENEMY_MAX_ / 4; j++)
		{
			//생성이 안 되었을 경우를 대비
			if (m_Monsters[i][j] != nullptr)
			{
				m_Monsters[i][j]->Init();
			}
		}
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				m_CurrentMonsterCount[i][j][k] = m_CreateCount[i][j][k];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_createTimeInFunction[i][j] = 0;
		}
	}
	m_TutorialCount = 0;
	m_BossMonster->Init();
}

//적 초기 데이터들 설정
void UnitManager::CreateInitEnemy()
{
	//40개...
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < ENEMY_MAX_ / 4; j++)
		{
			int type = i;
			m_Monsters[i][j] = new Monster();

			m_Monsters[i][j]->Init(10, { 1500, 300 }, m_MonsterSpeed[i][j / 10], { -1, 0 }, m_MonsterAniSprites[type], m_MonsterDeadAniSprite[type], type);

			// 충돌 영역 정보
			BBOX bBox;

			bBox.Left = 20;
			bBox.Top = 0;
			bBox.Right = bBox.Left + 150;
			bBox.Bottom = bBox.Top + 150;
			bBox.Hx = 150 / 2;
			bBox.Hy = 150 / 2;
			bBox.Cx = bBox.Left + bBox.Hx;		//박스의 중점  = 충돌박스 left + 박스 가로길이/2;	
			bBox.Cy = bBox.Top + bBox.Hy;		//박스의 중점  = 충돌박스 top + 박스 세로길이/2;

			m_Monsters[i][j]->SetBBox(bBox);
		}
	}
}

//히어로만 출력
void UnitManager::DrawHero()
{
	m_Hero->Draw(System::GetInstance()->m_DTime);

	int x = 20, y = 20;

	for (int i = 0; i < m_Hero->GetHP(); i++)
	{
		DrawAlphaSprite(backBufferDC, x, y, m_HpSprite.Width, m_HpSprite.Height, &m_HpSprite);
		x += 60;
	}
}

//적 이동
void UnitManager::MoveMonster()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < ENEMY_MAX_ / 4; j++)
		{
			//생성이 안 되었을 경우를 대비
			if (m_Monsters[i][j] != nullptr)
			{
				if (m_Monsters[i][j]->IsDead() || !m_Monsters[i][j]->IsCreated()) continue;
				if (!m_Monsters[i][j]->Move(System::GetInstance()->m_DTime))
				{
					m_Monsters[i][j]->Init();
				}
			}
		}
	}
}

//적 그리기
void UnitManager::DrawMonster()
{
	float minPos = 1440;
	int x = 0, y = 0;

	//맨 앞의 몬스터를 찾는다.
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < ENEMY_MAX_ / 4; j++)
		{
			m_Monsters[i][j]->SetFront(false);

			if (m_Monsters[i][j]->IsDead())
			{
				continue;
			}

			if (m_Monsters[i][j]->GetPos().x < minPos)
			{
				minPos = m_Monsters[i][j]->GetPos().x;
				x = i, y = j;
			}
		}
	}
	//표시한다
	m_Monsters[x][y]->SetFront(true);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < ENEMY_MAX_ / 4; j++)
		{
			if (m_Monsters[i][j] == nullptr || !m_Monsters[i][j]->IsCreated()) continue;

			m_Monsters[i][j]->Draw(System::GetInstance()->m_DTime);
		}
	}
}

//보스 몬스터 이동
void UnitManager::MoveBoss()
{
	if (m_BossMonster == nullptr || m_BossMonster->IsDead() || !m_BossMonster->IsCreated()) return;
	m_BossMonster->Move(System::GetInstance()->m_DTime);
}

//보스 몬스터 출력
void UnitManager::DrawBoss()
{
	if (m_BossMonster == nullptr || !m_BossMonster->IsCreated()) return;
	if (!m_BossMonster->Draw(System::GetInstance()->m_DTime))
	{
		StageInfo = Stage::stageClear;
	}

	int x = 500;
	int y = 800;

	for (int i = 1; i <= m_BossMonster->GetHP(); i++)
	{
		DrawFillRect(backBufferDC, x, y, 50, 50, RGB(150, 0, 0));
		x += 50;
	}
}

//히어로 만들기
int UnitManager::CreateHero()
{
	m_Hero = new Hero();
	m_Hero->Init();

	//애니메이션 대응용
	memcpy(m_Hero->m_IdleSprite, &m_HeroAniSprite, sizeof(ANISPRITE));
	m_Hero->SetSprites(m_HeroAttackSprite, m_HeroNormalHitSprite, m_HeroBossHitSprite);

	//충돌 박스
	BBOX bBox;

	bBox.Left = 150;
	bBox.Top = 50;
	bBox.Right = bBox.Left + 200;
	bBox.Bottom = bBox.Top + 400;
	bBox.Hx = 200 / 2;
	bBox.Hy = 400 / 2;
	bBox.Cx = bBox.Left + bBox.Hx;		//박스의 중점  = 충돌박스 top + 박스 가로길이/2;	
	bBox.Cy = bBox.Top + bBox.Hy;

	m_Hero->SetBBox(bBox);

	return TRUE;
}

void UnitManager::MakeEnemy(int type, int number, float x, float y)
{
	bool isCreate = false;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < ENEMY_MAX_ / 4; j++)
		{
			if (m_Monsters[i][j] != nullptr && m_Monsters[i][j]->GetType() == type && j / 10 == number)
			{
				if (m_Monsters[i][j]->IsCreated()) continue;			//생성된 상태면 
				if (m_Monsters[i][j]->IsDead()) continue;			//죽는중이면
				m_Monsters[i][j]->Init();

				int speed = m_MonsterSpeed[i][j / 10];
				//튜토리얼 스테이지
				if (WaveCount == 0)  speed = 50;

				m_Monsters[i][j]->Init(1, { x, y }, speed, { -1, 0 }, m_MonsterAniSprites[type], m_MonsterDeadAniSprite[type], type);

				m_Monsters[i][j]->SetCreated(true);
				isCreate = true;
				break;
			}
		}
		if (isCreate) break;
	}
}

int UnitManager::CheckCreateTime(float dTime)
{
	if (WaveCount == 0)
	{
		if (m_TutorialCount < 4)
		{
			MakeEnemy(m_TutorialCount, 0, 1000 + (100 * m_TutorialCount), (float)380 + (60 * m_TutorialCount));
			m_TutorialCount++;
		}
	}
	else
	{
		//적 생성 시간 체크. 
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (m_CurrentMonsterCount[WaveCount][i][j] <= 0) continue;

				m_createTimeInFunction[i][j] += dTime;

				if (m_CreateTimer[i][j] != -1 && (int)(m_createTimeInFunction[i][j]) >= m_CreateTimer[i][j])
				{
					m_createTimeInFunction[i][j] = 0;
					MakeEnemy(i, j, 1500, (float)380 + (60 * i));
					m_CurrentMonsterCount[WaveCount][i][j]--;
				}
			}
		}
	}
	return TRUE;
}

void UnitManager::CheckStage()
{
	KillCount++;
	if (KillCount >= STAGE_COUNT(WaveCount))
	{
		WaveCount++;
		KillCount = 0;

		if (WaveCount > 2)
		{
			Init();
			StageInfo = Stage::introBoss;
			SoundManager::GetInstance()->PlayOnce(SoundList::BOSS_COMMING);
		}
	}
}

//보스 몬스터 생성
int UnitManager::CreateBoss()
{
	if (m_BossMonster != nullptr)
	{
		if (m_BossMonster->IsCreated()) return FALSE;			//생성된 상태면
		if (m_BossMonster->IsDead()) return FALSE;			//죽는중이면
	}

	m_BossMonster = new BossMonster();
	m_BossMonster->Init(10, { 1400, 150 }, 100, { -1, 0 });
	m_BossMonster->SetSprites(m_MonsterAniSprites[4], m_BossAttackSprite, m_BossSummonSprite, m_BossHitSprite, m_MonsterDeadAniSprite[4]);

	for (int i = 0; i < 6; i++)
		m_BossMonster->SetEyes(m_BossEyeSprite[i], i);
	// 충돌 영역 정보
	BBOX bBox;

	bBox.Left = 275;
	bBox.Top = 48;
	bBox.Right = bBox.Left + 50;
	bBox.Bottom = bBox.Top + 500;
	bBox.Hx = 50 / 2;
	bBox.Hy = 500 / 2;
	bBox.Cx = bBox.Left + bBox.Hx;
	bBox.Cy = bBox.Top + bBox.Hy;

	m_BossMonster->SetBBox(bBox);

	m_BossMonster->SetCreated(true);


	return TRUE;
}

//충돌 체크
bool UnitManager::CheckCollision(BBOX left, BBOX right)
{
	int dX = abs(left.Cx - right.Cx);	//둘 사이의 가로값	
	int dY = abs(left.Cy - right.Cy);	//둘 사이의 세로값
	int HX = left.Hx + right.Hx;		//가로 길이 더하기
	int HY = left.Hy + right.Hy;		//세로 길이 더하기

	//충돌 체크
	if (dX < HX && dY < HY)
	{
		return true;
	}

	return false;
}

//히어로 충돌체크
bool UnitManager::CheckHeroCollision()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < ENEMY_MAX_ / 4; j++)
		{
			if (m_Monsters[i] == nullptr || m_Monsters[i][j]->IsDead() || !m_Monsters[i][j]->IsCreated())	continue;

			if (CheckCollision(m_Hero->GetBBox(), m_Monsters[i][j]->GetBBox()))
			{
				/*if (m_Hero->Hit())
				{
					StageInfo = Stage::gameover;
				}*/
				m_Monsters[i][j]->Hit(m_Monsters[i][j]->GetType());
				KillCount++;
			}
		}
	}

	if (StageInfo == Stage::boss)
	{
		if (CheckCollision(m_Hero->GetBBox(), m_BossMonster->GetBBox()))
		{
			StageInfo = Stage::gameover;
		}
	}
	return false;
}

//스프라이트 및 데이터 제거
void UnitManager::Release()
{
	m_Hero->Release();
	SAFE_DELETE(m_Hero);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < ENEMY_MAX_ / 4; j++)
		{
			m_Monsters[i][j]->Release();
			SAFE_DELETE(m_Monsters[i][j]);
		}
	}

	m_BossMonster->Release();
	SAFE_DELETE(m_BossMonster);
	SAFE_DELETE(m_Instance);
}

//싱글턴
UnitManager* UnitManager::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new UnitManager();
	}

	return m_Instance;
}

