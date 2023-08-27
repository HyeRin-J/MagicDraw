#include "../RRYEngine/framework.h"
#include "../RRYEngine/Vector.h"
#include "stdio.h"
#include "Unit.h"
#include "Hero.h"
#include "UnitManager.h"
#include "Monster.h"
#include "BossMonster.h"
#include "Input.h"
#include "Magic.h"
#include <vector>
#include "MagicManager.h"
#include "System.h"
#include "GameProc.h"

#pragma warning(disable:4996)

System* System::m_Instance = nullptr;

System::System()
{
	m_ShowHelp = true;
	m_ShowDot = false;
	m_ShowFrm = false;
	m_GridOn = false;
	m_ShowBBox = true;

	m_DTime = 0.0f;
	m_DTimeSys = 0.0f;
	m_DTimeScale = 1.0f;
	m_DTimePlay = 0.0f;
}

System* System::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new System();
	}

	return m_Instance;
}

//데이터 로딩... 대부분은 스프라이트 로딩을 말함
void System::DataLoading()
{
	//백그라운드 로딩
	char filePath[250];

	for (int i = 0; i < (int)BK_SPR_NUM::BK_SPR_END; i++)
	{
		sprintf(filePath, "%s/%s", RESOURCE_PATH, m_BackgroundPath[i]);
		LoadSprite(g_hWnd, filePath, &m_BackgroundSprites[i]);

	};

	//혹시 모를 임시 스프라이트들
	SPRITE temp;
	ANISPRITE aniTemp;

	//주인공 스프라이트 로딩
	sprintf(filePath, "%s/character_ani/walk.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_HeroAniSprite, &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/character_ani/damage.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 3, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_HeroNormalHitSprite, &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/character_ani/bossdamage.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 16, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_HeroBossHitSprite, &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/character_ani/attack.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 3, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_HeroAttackSprite, &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/character_ani/Hp.bmp", RESOURCE_PATH);
	LoadSprite(g_hWnd, filePath, &temp);
	memcpy(&UnitManager::GetInstance()->m_HpSprite, &temp, sizeof(SPRITE));

	//일자 애벌레 애니용
	sprintf(filePath, "%s/monster/bug_ani_glow.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 6, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterAniSprites[0], &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/monster/bug_ouch.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterDeadAniSprite[0], &aniTemp, sizeof(ANISPRITE));

	//v자 나뭇잎 몬스터 애니용
	sprintf(filePath, "%s/monster/leaf_ani_glow.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 6, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterAniSprites[1], &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/monster/leaf_ani_ouch.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterDeadAniSprite[1], &aniTemp, sizeof(ANISPRITE));

	//삼각형 버섯 스프라이트 로딩. 애니용
	sprintf(filePath, "%s/monster/m_mushroom_ani_glow.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 14, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterAniSprites[2], &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/monster/mushroom_ouch.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterDeadAniSprite[2], &aniTemp, sizeof(ANISPRITE));

	//사각형 골렘 스프라이트 로딩. 애니용
	sprintf(filePath, "%s/monster/m_golem_ani_glow.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 9, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterAniSprites[3], &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/monster/golem_ouch.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterDeadAniSprite[3], &aniTemp, sizeof(ANISPRITE));
	//보스 몹
	sprintf(filePath, "%s/boss/boss_walk.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 4, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterAniSprites[4], &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/boss/boss_attack.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 6, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_BossAttackSprite, &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/boss/boss_ouch.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_BossHitSprite, &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/boss/boss_mouth.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 4, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_BossSummonSprite, &aniTemp, sizeof(ANISPRITE));

	sprintf(filePath, "%s/boss/boss_dead.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 9, 1, &aniTemp);
	memcpy(&UnitManager::GetInstance()->m_MonsterDeadAniSprite[4], &aniTemp, sizeof(ANISPRITE));

	for (int i = 0; i < 6; i++)
	{
		sprintf(filePath, "%s/boss/eye_%d.bmp", RESOURCE_PATH, i + 1);
		LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
		memcpy(&UnitManager::GetInstance()->m_BossEyeSprite[i], &aniTemp, sizeof(ANISPRITE));
	}

	//이펙트 테스트
	for (int i = 0; i < 4; i++)
	{
		sprintf(filePath, "%s/Effect/effect_%d.bmp", RESOURCE_PATH, i + 1);
		LoadAniSprite(g_hWnd, filePath, 4, 1, &aniTemp);
		memcpy(&MagicManager::GetInstance()->m_MagicSprites[i], &aniTemp, sizeof(ANISPRITE));

	}

	sprintf(filePath, "%s/Effect/haha.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
	memcpy(&MagicManager::GetInstance()->m_MagicExploSprite, &aniTemp, sizeof(ANISPRITE));

	//도형 그릴 때 화면에 표시해줄 이펙트
	sprintf(filePath, "%s/Effect/magicEffect.bmp", RESOURCE_PATH);
	LoadSprite(g_hWnd, filePath, &temp);
	memcpy(&MagicManager::GetInstance()->m_MagicStarEffect, &temp, sizeof(SPRITE));

	CreateCustomFont();
}

void System::DataRelease()
{
	DeleteObject(m_HShapeBufferBmp);
	//SAFE_DELETE(m_HShapeBufferBmp);
	m_HShapeBufferBmp = nullptr;
	ReleaseDC(g_hWnd, m_ShapeBufferDC);
	DeleteObject(m_ShapeBufferDC);
	//SAFE_DELETE(m_ShapeBufferDC);
	m_ShapeBufferDC = nullptr;

	DeleteObject(m_SystemFont);
	m_SystemFont = nullptr;

	SAFE_DELETE(m_Instance);
}

void System::SystemUpdate()
{
	if (IsKeyUp(VK_F1)) { UnitManager::GetInstance()->DecSpeed(0, 0); }
	if (IsKeyUp(VK_F2)) { UnitManager::GetInstance()->IncSpeed(0, 0); }
	if (IsKeyUp(VK_F3)) { UnitManager::GetInstance()->DecTime(0, 0); }
	if (IsKeyUp(VK_F4)) { UnitManager::GetInstance()->IncTime(0, 0); }

	if (IsKeyUp(VK_F5)) { UnitManager::GetInstance()->DecSpeed(0, 1); }
	if (IsKeyUp(VK_F6)) { UnitManager::GetInstance()->IncSpeed(0, 1); }
	if (IsKeyUp(VK_F7)) { UnitManager::GetInstance()->DecTime(0, 1); }
	if (IsKeyUp(VK_F8)) { UnitManager::GetInstance()->IncTime(0, 1); }

	if (IsKeyUp(VK_F9)) { UnitManager::GetInstance()->DecCount(0, 0); }
	if (IsKeyUp(VK_F10)) { UnitManager::GetInstance()->IncCount(0, 0); }
	if (IsKeyUp(VK_F11)) { UnitManager::GetInstance()->DecCount(0, 1); }
	if (IsKeyUp(VK_F12)) { UnitManager::GetInstance()->IncCount(0, 1); }

	if (IsKeyUp(0x51)) { UnitManager::GetInstance()->DecSpeed(1, 0); }
	if (IsKeyUp(0x57)) { UnitManager::GetInstance()->IncSpeed(1, 0); }
	if (IsKeyUp(0x45)) { UnitManager::GetInstance()->DecTime(1, 0); }
	if (IsKeyUp(0x52)) { UnitManager::GetInstance()->IncTime(1, 0); }

	if (IsKeyUp(0x54)) { UnitManager::GetInstance()->DecSpeed(1, 1); }
	if (IsKeyUp(0x59)) { UnitManager::GetInstance()->IncSpeed(1, 1); }
	if (IsKeyUp(0x55)) { UnitManager::GetInstance()->DecTime(1, 1); }
	if (IsKeyUp(0x49)) { UnitManager::GetInstance()->IncTime(1, 1); }
	
	if (IsKeyUp(0x4F)) { UnitManager::GetInstance()->DecCount(1, 0); }
	if (IsKeyUp(0x50)) { UnitManager::GetInstance()->IncCount(1, 0); }
	if (IsKeyUp(VK_OEM_4)) { UnitManager::GetInstance()->DecCount(1, 1); }
	if (IsKeyUp(VK_OEM_6)) { UnitManager::GetInstance()->IncCount(1, 1); }

	if (IsKeyUp(0x41)) { UnitManager::GetInstance()->DecSpeed(2, 0); }
	if (IsKeyUp(0x53)) { UnitManager::GetInstance()->IncSpeed(2, 0); }
	if (IsKeyUp(0x44)) { UnitManager::GetInstance()->DecTime(2, 0); }
	if (IsKeyUp(0x46)) { UnitManager::GetInstance()->IncTime(2, 0); }

	if (IsKeyUp(0x47)) { UnitManager::GetInstance()->DecSpeed(2, 1); }
	if (IsKeyUp(0x48)) { UnitManager::GetInstance()->IncSpeed(2, 1); }
	if (IsKeyUp(0x4A)) { UnitManager::GetInstance()->DecTime(2, 1); }
	if (IsKeyUp(0x4B)) { UnitManager::GetInstance()->IncTime(2, 1); }

	if (IsKeyUp(0x4C)) { UnitManager::GetInstance()->DecCount(2, 0); }
	if (IsKeyUp(VK_OEM_1)) { UnitManager::GetInstance()->IncCount(2, 0); }
	if (IsKeyUp(VK_OEM_7)) { UnitManager::GetInstance()->DecCount(2, 1); }
	if (IsKeyUp(VK_RETURN)) { UnitManager::GetInstance()->IncCount(2, 1); }

	if (IsKeyUp(0x5A)) { UnitManager::GetInstance()->DecSpeed(3, 0); }
	if (IsKeyUp(0x58)) { UnitManager::GetInstance()->IncSpeed(3, 0); }
	if (IsKeyUp(0x43)) { UnitManager::GetInstance()->DecTime(3, 0); }
	if (IsKeyUp(0x56)) { UnitManager::GetInstance()->IncTime(3, 0); }

	if (IsKeyUp(0x42)) { UnitManager::GetInstance()->DecSpeed(3, 1); }
	if (IsKeyUp(0x4E)) { UnitManager::GetInstance()->IncSpeed(3, 1); }
	if (IsKeyUp(0x4D)) { UnitManager::GetInstance()->DecTime(3, 1); }
	if (IsKeyUp(VK_OEM_COMMA)) { UnitManager::GetInstance()->IncTime(3, 1); }

	if (IsKeyUp(VK_OEM_PERIOD)) { UnitManager::GetInstance()->DecCount(3, 0); }
	if (IsKeyUp(VK_OEM_2)) { UnitManager::GetInstance()->IncCount(3, 0); }
	if (IsKeyUp(VK_LSHIFT)) { UnitManager::GetInstance()->DecCount(3, 1); }
	if (IsKeyUp(VK_RSHIFT)) { UnitManager::GetInstance()->IncCount(3, 1); }


	if (IsKeyUp(VK_SPACE)) { UnitManager::GetInstance()->Init(); }
	//엔진 시스템 타임
	m_DTimeSys = GetEngineTimer();

	//플레이 시간 측정..
	m_DTimePlay += m_DTimeSys;

	// 타이머 #2 : 일반 유닛 타이머 
	m_DTime = m_DTimeSys * m_DTimeScale;


	// 최저 제한...
	if (m_DTime < 0.0001f)	m_DTime = 0.0001f;
}

void System::ShowHelp()
{
	//프레임수 출력.
	if (m_ShowFrm)
		PrintFPS(0, 0);

	if (!m_ShowHelp)
	{
		return;
	}

	int x = 700, y = 50;
	COLORREF black = RGB(0, 0, 0);
	PrintText(x, y, black, "Stage : %d", WaveCount + 1);

	//----------------------- 
	// 기타 도움말.
	//----------------------- 
	x = 0, y = 100;
	y += 14 * 2;
	COLORREF col = RGB(255, 255, 255);
	PrintText(x, y += 14, col, "Shape: %d", m_ShapeNum);	//검출한 도형

	y += 14 * 2;
	PrintText(x, y += 14, col, "[Monster 1]");	//검출한 도형
	PrintText(x, y += 14, col, "1 : Speed : %d (pixel) / s \t[- F1 / + F2]", UnitManager::GetInstance()->GetSpeed(0, 0));
	PrintText(x, y += 14, col, "1 : Time : %.2f s \t[- F3 / + F4]", UnitManager::GetInstance()->GetTimer(0, 0));
	PrintText(x, y += 14, col, "1 : Count : %d \t[- F9 / + F10]", UnitManager::GetInstance()->GetCount(0, 0));
	PrintText(x, y += 14, col, "2 : Speed : %d (pixel) / s \t[- F5 / + F6]", UnitManager::GetInstance()->GetSpeed(0, 1));
	PrintText(x, y += 14, col, "2 : Time : %.2f s \t[- F7 / + F8]", UnitManager::GetInstance()->GetTimer(0, 1));
	PrintText(x, y += 14, col, "2 : Count : %d \t[- F11 / + F12]", UnitManager::GetInstance()->GetCount(0, 1));

	y += 14 * 2;
	PrintText(x, y += 14, col, "[Monster 2]");	//검출한 도형
	PrintText(x, y += 14, col, "1 : Speed : %d (pixel) / s \t[- Q / + W]", UnitManager::GetInstance()->GetSpeed(1, 0));
	PrintText(x, y += 14, col, "1 : Time : %.2f s \t[- E / + R]", UnitManager::GetInstance()->GetTimer(1, 0));
	PrintText(x, y += 14, col, "1 : Count : %d \t[- O / + P]", UnitManager::GetInstance()->GetCount(1, 0));
	PrintText(x, y += 14, col, "2 : Speed : %d (pixel) / s \t[- T / + Y]", UnitManager::GetInstance()->GetSpeed(1, 1));
	PrintText(x, y += 14, col, "2 : Time : %.2f s \t[- U / + I]", UnitManager::GetInstance()->GetTimer(1, 1));
	PrintText(x, y += 14, col, "2 : Count : %d \t[- [ / + ] ]", UnitManager::GetInstance()->GetCount(1, 1));

	y += 14 * 2;
	PrintText(x, y += 14, col, "[Monster 3]");	//검출한 도형
	PrintText(x, y += 14, col, "1 : Speed : %d (pixel) / s \t[- A / + S]", UnitManager::GetInstance()->GetSpeed(2, 0));
	PrintText(x, y += 14, col, "1 : Time : %.2f s \t[- D / + F]", UnitManager::GetInstance()->GetTimer(2, 0));
	PrintText(x, y += 14, col, "1 : Count : %d \t[- L / + ;]", UnitManager::GetInstance()->GetCount(2, 0));
	PrintText(x, y += 14, col, "2 : Speed : %d (pixel) / s \t[- G / + H]", UnitManager::GetInstance()->GetSpeed(2, 1));
	PrintText(x, y += 14, col, "2 : Time : %.2f s \t[- J / + K]", UnitManager::GetInstance()->GetTimer(2, 1));
	PrintText(x, y += 14, col, "2 : Count : %d \t[- ' / + Enter ]", UnitManager::GetInstance()->GetCount(2, 1));

	y += 14 * 2;
	PrintText(x, y += 14, col, "[Monster 3]");	//검출한 도형
	PrintText(x, y += 14, col, "1 : Speed : %d (pixel) / s \t[- Z / + X]", UnitManager::GetInstance()->GetSpeed(3, 0));
	PrintText(x, y += 14, col, "1 : Time : %.2f s \t[- C / + V]", UnitManager::GetInstance()->GetTimer(3, 0));
	PrintText(x, y += 14, col, "1 : Count : %d \t[- . / + /]", UnitManager::GetInstance()->GetCount(3, 0));
	PrintText(x, y += 14, col, "2 : Speed : %d (pixel) / s \t[- B / + N]", UnitManager::GetInstance()->GetSpeed(3, 1));
	PrintText(x, y += 14, col, "2 : Time : %.2f s \t[- M / + ,]", UnitManager::GetInstance()->GetTimer(3, 1));
	PrintText(x, y += 14, col, "2 : Count : %d \t[- (l-SHF) / + (r-SHF) ]", UnitManager::GetInstance()->GetCount(3, 1));


	PrintText(x, y += 14, col, "Space : 초기화");	//검출한 도형
}

bool System::ShakeBackGround()
{
	static bool isReachOffset = false;
	static float startTime = 0;
	static int offset = 0;

	if (!isReachOffset)
	{
		if (offset < 10)
		{
			startTime += m_DTime;
			offset += (10 * startTime) / 0.5f;
			DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::BACKGROUND], 0, 10 - offset);
		}
		else
		{
			isReachOffset = true;
			startTime = 0;
			offset = 0;
		}
		return false;
	}
	else
	{
		if (offset < 10)
		{
			startTime += m_DTime;
			offset += (10 * startTime) / 0.5f;
			DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::BACKGROUND], 0, offset);
		}
		else
		{
			isReachOffset = false;
			startTime = 0;
			offset = 0;
			return true;
		}
		return false;
	}
}

void System::RenderAll()
{
	BeginRendering();	//렌더링 시작

	//백그라운드 출력
	DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::BACKGROUND], 10, 10);

	//일반 스테이지와 보스 스테이지 판단
	switch (StageInfo)
	{
	case Stage::title:

		DrawSprite(backBufferDC, 0, 0, &System::GetInstance()->m_BackgroundSprites[(int)BK_SPR_NUM::TITLE]);

		break;

	case Stage::stage:
		UnitManager::GetInstance()->DrawMonster();

		PrintText(1100, 50, RGB(0, 255, 255), "Score : %d", g_Score);

		break;
	case Stage::stageClear:
	{
		DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::ENDING], 0, 0);

		PrintText(600, 200, RGB(0, 0, 0), "CLEAR!");
		PrintText(530, 400, RGB(100, 100, 100), "Total Score : %d", g_Score);
	}
	break;
	case Stage::introBoss:
		UnitManager::GetInstance()->DrawBoss();
		DrawAlphaSprite(backBufferDC, 0, 0, m_ScreenSize.cx, m_ScreenSize.cy, &m_BackgroundSprites[(int)BK_SPR_NUM::INTROBOSS]);
		break;
	case Stage::boss:
		if (UnitManager::GetInstance()->m_BossMonster->m_IsAttacking)
		{
			if (UnitManager::GetInstance()->m_BossMonster->m_AttackSprite->aniFrame >= 5)
			{
				ShakeBackGround();
				/*
				if (UnitManager::GetInstance()->m_Hero->BossHit())
				{
					StageInfo = Stage::gameover;
				}*/
			}
		}

		UnitManager::GetInstance()->DrawBoss();
		UnitManager::GetInstance()->DrawMonster();

		PrintText(1100, 50, RGB(0, 255, 255), "Score : %d", g_Score);

		break;
	case Stage::gameover:
		DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::ENDING], 0, 0);

		PrintText(600, 200, RGB(0, 0, 0), "GAME OVER");
		PrintText(530, 400, RGB(255, 255, 255), "Total Score : %d", g_Score);
		break;
	default:
		break;
	}

	if (StageInfo == Stage::stage || StageInfo == Stage::introBoss || StageInfo == Stage::boss)
	{
		//주인공 그리기
		UnitManager::GetInstance()->DrawHero();

		//만들어진 마법을 그린다
		MagicManager::GetInstance()->DrawMagic();
	}

	//도움말 출력
	ShowHelp();

	//배경과 OR 연산
	GdiTransparentBlt(backBufferDC, 0, 0, 1440, 900, m_ShapeBufferDC, 0, 0, 1440, 900, RGB(0, 0, 0));

	//렌더 끝
	EndRendering();

	//메인 DC에 출력
	Flip();
}

void System::CreateCustomFont()
{
	char fontPath[256];
	sprintf(fontPath, "%s/Font/123RF.ttf", RESOURCE_PATH);
	AddFontResource(fontPath);

	m_SystemFont = CreateFont(12, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH, "123RF");
	SelectObject(backBufferDC, m_SystemFont);
}