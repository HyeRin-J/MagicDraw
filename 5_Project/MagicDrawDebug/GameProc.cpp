#include "../RRYEngine/framework.h"
#include "ynMMedia.h"			   //예나 미디어 헤더.★
#include "UnitManager.h"
#include "System.h"
#include <vector>
#include "MagicManager.h"
#include "Input.h"
#include "SoundManager.h"
#include "GameProc.h"
#include "resource.h"

Stage StageInfo = Stage::stage;
int WaveCount = 2;
int g_Score = 0;

HWND g_ReplayBtn, b2, b3;
HANDLE buttonImage[3] = { nullptr, };

void CreateEngine()
{
	::CreateEngine(g_hWnd, System::GetInstance()->m_ScreenSize.cx, System::GetInstance()->m_ScreenSize.cy);
	srand(GetTickCount64());
}

void DataLoading()
{
	System::GetInstance()->DataLoading();
	UnitManager::GetInstance()->CreateInitEnemy();
	UnitManager::GetInstance()->CreateHero();
	UnitManager::GetInstance()->CreateBoss();
	MagicManager::GetInstance()->Initialize();
	SoundManager::GetInstance()->LoadData();
}

void Title()
{
	g_Score = 0;
	WaveCount = 0;

	if (b2 != nullptr)
	{
		DestroyWindow(b2);
		b2 = nullptr;
	}
	if (b3 != nullptr)
	{
		DestroyWindow(b3);
		b3 = nullptr;
	}

	System::GetInstance()->SystemUpdate();
	System::GetInstance()->RenderAll();

	if (g_ReplayBtn == nullptr)
	{
		g_ReplayBtn = CreateWindow(TEXT("button"), TEXT("CLICK TO START"), WS_CHILD | WS_VISIBLE | BS_BITMAP,
			403, 793, 443, 107, g_hWnd, (HMENU)0, hInst, NULL);
		buttonImage[0] = LoadImage(hInst, MAKEINTRESOURCE(IDB_TITLEBUTTON), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		SendMessage(g_ReplayBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)buttonImage[0]);
	}
}

void Loading()
{
	if (g_ReplayBtn != nullptr)
	{
		DestroyWindow(g_ReplayBtn);
		g_ReplayBtn = nullptr;
	}
	if (b2 != nullptr)
	{
		DestroyWindow(b2);
		b2 = nullptr;
	}
	if (b3 != nullptr)
	{
		DestroyWindow(b3);
		b3 = nullptr;
	}

	UnitManager::GetInstance()->Init();
	MagicManager::GetInstance()->Init();

	System::GetInstance()->SystemUpdate();

	static int CutCount = 0;
	static long StartTime = GetTickCount64();

	long currTime = GetTickCount64();

	HBRUSH old = (HBRUSH)SelectObject(backBufferDC, GetStockObject(BLACK_BRUSH));

	Rectangle(backBufferDC, 0, 0, 1440, 900);
	BeginRendering();
	EndRendering();
	Flip();

	//4컷 만화 출력하기
	while (CutCount < 4)
	{
		currTime = GetTickCount64();

		if (currTime - StartTime > 1000)
		{
			StartTime = GetTickCount64();
			BitBlt(backBufferDC, 720 * (CutCount % 2), 450 * (CutCount / 2), 720, 450,
				System::GetInstance()->m_BackgroundSprites[(int)BK_SPR_NUM::LOADING].dc, 720 * (CutCount % 2), 450 * (CutCount / 2), SRCCOPY);
			CutCount++;

			EndRendering();

			Flip();
		}
	}

	SelectObject(backBufferDC, old);

	while (currTime - StartTime <= 1000)
		currTime = GetTickCount64();

	CutCount = 0;

	StageInfo = Stage::stage;
}

void Tutorial()
{
	StageInfo = Stage::stage;
}

void NormalStage()
{
	System::GetInstance()->SystemUpdate();

	MagicManager::GetInstance()->CreateMagic();
	MagicManager::GetInstance()->MoveMagic();

	UnitManager::GetInstance()->CheckCreateTime(System::GetInstance()->m_DTime);
	UnitManager::GetInstance()->MoveMonster();

	System::GetInstance()->RenderAll();

	UnitManager::GetInstance()->CheckHeroCollision();
	MagicManager::GetInstance()->CheckCollision();
}

void IntroBossStage()
{
	System::GetInstance()->SystemUpdate();
	MagicManager::GetInstance()->Init();

	static float StartTime = 0;

	StartTime += System::GetInstance()->m_DTime;

	if (StartTime > 1)
		UnitManager::GetInstance()->MoveBoss();

	System::GetInstance()->RenderAll();

	if (StartTime > 2)
	{
		StageInfo = Stage::boss;
		StartTime = 0;
	}
}

void BossStage()
{
	System::GetInstance()->SystemUpdate();

	MagicManager::GetInstance()->CreateMagic();
	MagicManager::GetInstance()->MoveMagic();

	UnitManager::GetInstance()->MoveBoss();

	UnitManager::GetInstance()->MoveMonster();

	System::GetInstance()->RenderAll();

	UnitManager::GetInstance()->CheckHeroCollision();
	MagicManager::GetInstance()->CheckCollision();
}

void StageClear()
{
	if (g_ReplayBtn != nullptr)
	{
		DestroyWindow(g_ReplayBtn);
		g_ReplayBtn = nullptr;
	}
	if (b3 != nullptr)
	{
		DestroyWindow(b3);
		b3 = nullptr;
	}

	System::GetInstance()->SystemUpdate();

	UnitManager::GetInstance()->CheckCreateTime(System::GetInstance()->m_DTime);

	System::GetInstance()->RenderAll();

	MagicManager::GetInstance()->CreateMagic();
	MagicManager::GetInstance()->MoveMagic();

	MagicManager::GetInstance()->CheckCollision();

	MagicManager::GetInstance()->Init();

	if (b2 == nullptr)
	{
		b2 = CreateWindow(TEXT("button"), TEXT("REPLAY"), WS_CHILD | WS_VISIBLE | BS_BITMAP,
			583, 578, 273, 113, g_hWnd, (HMENU)1, hInst, NULL);
		buttonImage[1] = LoadImage(hInst, MAKEINTRESOURCE(IDB_REPLAYBTN), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		SendMessage(b2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)buttonImage[1]);
	}
}

void GameOver()
{
	if (g_ReplayBtn != nullptr)
	{
		DestroyWindow(g_ReplayBtn);
		g_ReplayBtn = nullptr;
	}
	if (b2 != nullptr)
	{
		DestroyWindow(b2);
		b2 = nullptr;
	}
	System::GetInstance()->SystemUpdate();

	System::GetInstance()->RenderAll();

	if (b3 == nullptr)
	{
		b3 = CreateWindow(TEXT("button"), TEXT("REPLAY"), WS_CHILD | WS_VISIBLE | BS_BITMAP,
			583, 578, 273, 113, g_hWnd, (HMENU)1, hInst, NULL);
		buttonImage[2] = LoadImage(hInst, MAKEINTRESOURCE(IDB_REPLAYBTN), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		SendMessage(b3, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)buttonImage[2]);
	}
}

void Release()
{
	UnitManager::GetInstance()->Release();
	System::GetInstance()->DataRelease();
	MagicManager::GetInstance()->Release();

	DestroyWindow(g_ReplayBtn);
	g_ReplayBtn = nullptr;
	DestroyWindow(b2);
	b2 = nullptr;
	DestroyWindow(b3);
	b3 = nullptr;

	for (int i = 0; i < 3; i++)
	{
		DeleteObject(buttonImage[i]);
		buttonImage[i] = nullptr;
	}

	ReleaseEngine();
}